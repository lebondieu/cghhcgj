#ifndef SETTINGS_LED_CONTROLLER_H
#define SETTINGS_LED_CONTROLLER_H

#include "generic_sub_controller.h"

namespace Settings {

class LEDController : public GenericSubController {
public:
  LEDController(Responder * parentResponder);
  bool handleEvent(Ion::Events::Event event) override;
  HighlightCell * reusableCell(int index, int type) override;
  int reusableCellCount(int type) override;
  void willDisplayCellForIndex(HighlightCell * cell, int index) override;
private:
  constexpr static int k_totalNumberOfCells = 3;
  MessageTableCellWithGauge m_gaugeCells[k_totalNumberOfCells];
};

}

#endif
