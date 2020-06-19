#include "apps_settings_controller.h"
#include <assert.h>

#include "../../global_preferences.h"
#include "../../apps_container.h"
#include "preferences_controller.h"


using namespace Shared;


namespace Settings {

AppsSettingsController::AppsSettingsController(Responder * parentResponder) :
  GenericSubController(parentResponder)
{
  m_subTitleCell.setMessageFont(KDFont::LargeFont);
  for (int i = 0; i < k_totalNumberOfCell; i++) {
    m_cells[i].setMessageFont(KDFont::LargeFont);
  }
}

bool AppsSettingsController::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::OK || event == Ion::Events::EXE || event == Ion::Events::Right) {

    if (m_selectableTableView.selectedRow() > 0) {
      AppsContainer * container = AppsContainer::sharedAppsContainer();
      container->appSnapshotCanBeHideAtIndex(m_selectableTableView.selectedRow()-1)->switchCanBeShow();

      m_selectableTableView.reloadCellAtLocation(m_selectableTableView.selectedColumn(), m_selectableTableView.selectedRow());
      container->redrawWindow(true);
      return true;
    }
  }
  return GenericSubController::handleEvent(event);
}

HighlightCell * AppsSettingsController::reusableCell(int index, int type) {
  assert(type == 0 || type == 2);
  assert(index >= 0 && index < k_totalNumberOfCell + 1);

  if (type == 0) {
    return &m_subTitleCell;
  } else {
    return &m_cells[index];
  }
}

int AppsSettingsController::reusableCellCount(int type) {
  assert(type == 0 || type == 2);
  if (type == 0) {
    return 1;
  } else {
    return k_totalNumberOfCell;
  }
  return 0;
}

void AppsSettingsController::willDisplayCellForIndex(HighlightCell * cell, int index) {
  GenericSubController::willDisplayCellForIndex(cell, index);

  if (index > 0) {
    
    MessageTableCellWithSwitch * mySwitchCell = (MessageTableCellWithSwitch *)cell;

    SwitchView * mySwitch = (SwitchView *)mySwitchCell->accessoryView();

    AppsContainer * container = AppsContainer::sharedAppsContainer();

    mySwitch->setState(container->appSnapshotCanBeHideAtIndex(index-1)->canBeShow());

  }
}

int AppsSettingsController::typeAtLocation(int i, int j) {
  #include <stdio.h>
  if (j == 0) {
    return 0;
  } else {
    return 2;
  }
}

}
