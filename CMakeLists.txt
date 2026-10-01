# ----------------------------------------------------------------------------
# @file    CMakeLists.txt
# @brief   builds the LRIS2 camera daemon
#
# This project owns the instrument; camerad comes in as a dependency. Clone and
# build, nothing else to set up:
#
#   cmake -S . -B build && cmake --build build
# ----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.24)

project(lris2_camerad CXX)

include(FetchContent)

FetchContent_Declare(camera_interface
  GIT_REPOSITORY https://github.com/CaltechOpticalObservatories/camera-interface
  GIT_TAG        main
)

set(CONTROLLER archon CACHE STRING "Controller LRIS2 uses")

FetchContent_MakeAvailable(camera_interface)

add_executable(camerad
  lris2_instrument.cpp
  lris2_interface_factory.cpp
)

target_link_libraries(camerad PRIVATE camerad_server)

install(TARGETS camerad RUNTIME DESTINATION bin)
install(DIRECTORY config/ DESTINATION share/lris2)
