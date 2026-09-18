/**
 * @file    Instruments/lris2/lris2_interface_factory.cpp
 * @brief   LRIS2 Interface Factory
 *
 */
#include "lris2_instrument.h"
#include "camera_interface.h"

namespace Camera {

  /***** Camera::Interface::create ********************************************/
  /**
   * @brief      factory function to create pointer to LRIS2
   * @return     unique_ptr<LRIS2>
   *
   */
  std::unique_ptr<Interface> Interface::create() {
    return std::make_unique<LRIS2>();
  }
  /***** Camera::Interface::create ********************************************/

}
