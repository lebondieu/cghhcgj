#ifndef SETTINGS_APPS_PREFERENCES__CONTROLLER_H
#define SETTINGS_APPS_PREFERENCES__CONTROLLER_H

#include "generic_sub_controller.h"
#include "preferences_controller.h"
#include <escher.h>
#include "../../../macros.h"

#ifndef APPS_CAN_BE_HIDE_COUNT
#error Missing app can be hide count
#endif

namespace Settings {

class AppsSettingsController : public GenericSubController {
public:
  AppsSettingsController(Responder * parentResponder);
  bool handleEvent(Ion::Events::Event event) override;
  HighlightCell * reusableCell(int index, int type) override;
  int reusableCellCount(int type) override;
  void willDisplayCellForIndex(HighlightCell * cell, int index) override;
  int typeAtLocation(int i, int j) override;
private:
  constexpr static int k_totalNumberOfCell = APPS_CAN_BE_HIDE_COUNT;
  MessageTableCellWithMessage m_subTitleCell;
  MessageTableCellWithSwitch m_cells[APPS_CAN_BE_HIDE_COUNT];
};

}

#endif
