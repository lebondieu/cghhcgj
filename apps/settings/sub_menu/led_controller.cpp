#include "led_controller.h"
#include "../../global_preferences.h"
#include <assert.h>

using namespace Shared;

namespace Settings {

LEDController::LEDController(Responder * parentResponder) :
  GenericSubController(parentResponder)
{
  for (int i = 0; i < k_totalNumberOfCells; i++) {
    m_gaugeCells[i].setMessageFont(KDFont::LargeFont);
  }
}

bool LEDController::handleEvent(Ion::Events::Event event) {
  GlobalPreferences * globalPreferences = GlobalPreferences::sharedGlobalPreferences();
  KDColor color = globalPreferences->ledColor();
  uint8_t red = color.red();
  uint8_t green = color.green();
  uint8_t blue = color.blue();

  if (event == Ion::Events::Right || event == Ion::Events::Left || event == Ion::Events::Plus || event == Ion::Events::Minus) {
    int direction = (event == Ion::Events::Right || event == Ion::Events::Plus) ? 17 : -17;
    if (m_messageTreeModel->children(selectedRow())->label() == I18n::Message::Red && red + direction <= 255 && red + direction >= 0) {
      red += direction;
    }
    else if (m_messageTreeModel->children(selectedRow())->label() == I18n::Message::Green && green + direction <= 255 && green + direction >= 0) {
      green += direction;
    }
    else if (m_messageTreeModel->children(selectedRow())->label() == I18n::Message::Blue && blue + direction <= 255 && blue + direction >= 0) {
      blue += direction;
    }
    else {
      return GenericSubController::handleEvent(event);
    }
  }
  else {
    return GenericSubController::handleEvent(event);
  }
  globalPreferences->setLedColor(KDColor::RGB888(red, green, blue));
  m_selectableTableView.reloadCellAtLocation(m_selectableTableView.selectedColumn(), m_selectableTableView.selectedRow());
  return true;
}

HighlightCell * LEDController::reusableCell(int index, int type) {
  assert(index >= 0 && index < k_totalNumberOfCells);
  return &m_gaugeCells[index];
}

int LEDController::reusableCellCount(int type) {
  return k_totalNumberOfCells;
}

void LEDController::willDisplayCellForIndex(HighlightCell * cell, int index) {
  GenericSubController::willDisplayCellForIndex(cell, index);
  MessageTableCellWithGauge * myGaugeCell = (MessageTableCellWithGauge *)cell;
  GaugeView * myGauge = (GaugeView *)myGaugeCell->accessoryView();

  float level;
  GlobalPreferences * globalPreferences = GlobalPreferences::sharedGlobalPreferences();
  KDColor color = globalPreferences->ledColor();
  uint8_t red = color.red();
  uint8_t green = color.green();
  uint8_t blue = color.blue();
  if (m_messageTreeModel->children(index)->label() == I18n::Message::Red) {
    level = (float)red;
  }
  else if (m_messageTreeModel->children(index)->label() == I18n::Message::Green) {
    level = (float)green;
  }
  else {
    level = (float)blue;
  }
  myGauge->setLevel(level/255);
}

}
