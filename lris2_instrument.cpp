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


  /***** Camera::LRIS2::load_firmware *****************************************/
  /**
   * @brief      load the ACF, then select the SINGLE exposure mode
   * @param[in]  args       ACF file name, or ? for help
   * @param[out] retstring  return string
   * @return     ERROR|NO_ERROR|HELP
   *
   */
  long LRIS2::load_firmware(const std::string &args, std::string &retstring) {
    const long error = ArchonInterface::load_firmware(args, retstring);
    if (error != NO_ERROR) return error;
    return this->set_exposure_mode(ArchonExposureMode::SINGLE, {});
  }
  /***** Camera::LRIS2::load_firmware *****************************************/

}
