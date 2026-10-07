# ----------------------------------------------------------------------------
# @file    CMakeLists.txt
# @brief   builds the LRIS2 camera daemon and its Python module
#
# This project owns the instrument; camerad comes in as a dependency. Clone and
# build, nothing else to set up:
#
#   cmake -S . -B build && cmake --build build
#
# Add -DBUILD_PYTHON_MODULE=ON for the camera_interface Python module.
# ----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.24)

project(lris2_camerad CXX)

# Declared before the fetch: it is what makes the core's static libraries
# position-independent and creates the camerad::python target
option(BUILD_PYTHON_MODULE "Build the camera_interface Python module" OFF)

include(FetchContent)

# The core's submodules are the other instruments, two of them private, and
# none of them are needed to build this one
FetchContent_Declare(camera_interface
  GIT_REPOSITORY https://github.com/CaltechOpticalObservatories/camera-interface
  GIT_TAG        main
  GIT_SUBMODULES ""
)

set(CONTROLLER archon CACHE STRING "Controller LRIS2 uses")

FetchContent_MakeAvailable(camera_interface)

set(LRIS2_SOURCES
  lris2_instrument.cpp
  lris2_interface_factory.cpp
)

add_executable(camerad ${LRIS2_SOURCES})

# The daemon library carries no controller, and CONTROLLER_ARCHON rides on the
# controller target's interface, so both are needed
target_link_libraries(camerad PRIVATE camerad::daemon camerad::archon)

install(TARGETS camerad RUNTIME DESTINATION bin COMPONENT daemon)
install(DIRECTORY config/ DESTINATION share/lris2 COMPONENT daemon)

# ----------------------------------------------------------------------------
# Python module, built from the same instrument sources so that it drives the
# camera in-process. It needs no daemon, hence no camerad::daemon here
# ----------------------------------------------------------------------------
if (BUILD_PYTHON_MODULE)
  find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

  # pybind11 ships its CMake config inside the wheel, so ask the interpreter
  # where it is rather than making every caller pass -Dpybind11_DIR
  if (NOT pybind11_DIR)
    execute_process(
      COMMAND ${Python3_EXECUTABLE} -c "import pybind11; print(pybind11.get_cmake_dir())"
      OUTPUT_VARIABLE PYBIND11_CMAKE_DIR
      OUTPUT_STRIP_TRAILING_WHITESPACE
      RESULT_VARIABLE PYBIND11_PROBE_RESULT
      ERROR_QUIET
    )
    if (NOT PYBIND11_PROBE_RESULT EQUAL 0)
      message(FATAL_ERROR
              "BUILD_PYTHON_MODULE=ON needs pybind11 in ${Python3_EXECUTABLE}. "
              "Install it with: ${Python3_EXECUTABLE} -m pip install pybind11")
    endif()
    list(APPEND CMAKE_PREFIX_PATH ${PYBIND11_CMAKE_DIR})
  endif()

  find_package(pybind11 CONFIG REQUIRED)

  pybind11_add_module(camera_interface ${LRIS2_SOURCES})
  target_compile_definitions(camera_interface PRIVATE CAMERAD_INSTRUMENT_NAME="lris2")
  target_link_libraries(camera_interface PRIVATE camerad::python camerad::archon)

  # The wheel root, so that the module imports as camera_interface
  install(TARGETS camera_interface LIBRARY DESTINATION . COMPONENT python)

  # A shared library may leave symbols undefined, so a module missing its
  # interface factory links and fails only when imported
  enable_testing()
  add_test(NAME python_import
    COMMAND ${Python3_EXECUTABLE} -c
      "import camera_interface\nassert camera_interface.instrument_name() == 'lris2', camera_interface.instrument_name()\nassert camera_interface.controller_name() == 'archon', camera_interface.controller_name()"
  )
  set_tests_properties(python_import PROPERTIES
    ENVIRONMENT PYTHONPATH=$<TARGET_FILE_DIR:camera_interface>
  )
endif()
