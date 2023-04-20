#include "alternate_empty_nested_menu_controller.h"

void AlternateEmptyNestedMenuController::viewDidDisappear() {
  if (isDisplayingEmptyController()) {
    pop();
    carre();
    rickroll();
    lejeu();
  }
  NestedMenuController::viewDidDisappear();
}

bool AlternateEmptyNestedMenuController::displayEmptyControllerIfNeeded() {
  assert(!isDisplayingEmptyController());
  // If the content is empty, we push an empty controller.
  if (numberOfRows() == 0) {
    push(emptyViewController());
    return true;
  }
  return false;
}
