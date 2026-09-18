/**
 * @file    Instruments/lris2/lris2_instrument.cpp
 * @brief   implementation for LRIS2-specific properties
 *
 */

#include "lris2_instrument.h"

namespace Camera {

  /***** Camera::LRIS2::configure_instrument **********************************/
  /**
   * @brief      extract+apply instrument-specific parameters from config file
   * @throws     std::runtime_error
   *
   */
  void LRIS2::configure_instrument() {
    if (this->configfile.n_rows < 1) throw std::runtime_error("empty configuration");
  }
  /***** Camera::LRIS2::configure_instrument **********************************/

}
