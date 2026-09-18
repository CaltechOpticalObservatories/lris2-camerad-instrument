/**
 * @file    Instruments/lris2/lris2_instrument.h
 * @brief   contains properties unique to the LRIS2 instrument
 *
 */
#pragma once

#include "archon_interface.h"  /// LRIS2 uses ArchonInterface

namespace Camera {

  /***** Camera::LRIS2 ********************************************************/
  /**
   * @class    LRIS2
   * @brief    derived class inherits from ArchonInterface
   * @details  Describes LRIS2-specific functionality. Exposures use the
   *           standard ArchonInterface modes; this class is the hook point for
   *           LRIS2 specifics as the detector and its ACF become available.
   *
   */
  class LRIS2 : public ArchonInterface {
    public:
      void configure_instrument() override;
  };
  /***** Camera::LRIS2 ********************************************************/
}
