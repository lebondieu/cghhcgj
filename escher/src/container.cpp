#include <escher/container.h>
#include <assert.h>
#if !defined(DEVICE_N0100) && !defined(DEVICE_N0110)
#include <escher/clipboard.h>
#include <SDL.h>
#endif

Container::Container() :
  RunLoop()
{
}

// Initialize private static member
App * Container::s_activeApp = nullptr;

Container::~Container() {
  if (s_activeApp) {
    s_activeApp->~App();
  }
}

bool Container::switchTo(App::Snapshot * snapshot) {
  if (s_activeApp && snapshot == s_activeApp->snapshot()) {
    return true;
  }
  if (s_activeApp && !s_activeApp->prepareForExit()) {
    /* activeApp()->prepareForExit() returned false, which means that the app
     * needs another event loop to prepare for being switched off. */
    return false;
  }
  if (s_activeApp) {
    s_activeApp->willBecomeInactive();
    s_activeApp->snapshot()->pack(s_activeApp);
    s_activeApp = nullptr;
  }
  if (snapshot) {
    s_activeApp = snapshot->unpack(this);
  }
  if (s_activeApp) {
    s_activeApp->didBecomeActive(window());
    window()->redraw();
  }
  return true;
}

#if !defined(DEVICE_N0100) && !defined(DEVICE_N0110)
void updateClipboard(bool set) {
  Clipboard * escherClipboard = Clipboard::sharedClipboard();
  if(set) {
    int clipboardUpdated = SDL_SetClipboardText(escherClipboard->storedText());
    assert(clipboardUpdated == 0);
    return;
  }
  char * SDLClipboard = SDL_GetClipboardText();
  if(strcmp(escherClipboard->storedText(), SDLClipboard) == 0) {
    SDL_free(SDLClipboard);
    return;
  }
  escherClipboard->store(SDLClipboard);
  SDL_free(SDLClipboard);
}
#endif

bool Container::dispatchEvent(Ion::Events::Event event) {
#if !defined(DEVICE_N0100) && !defined(DEVICE_N0110)
  // When changing SDL clipboard, and then directly pasting in omega, escher clipboard has to be updated
  if(event == Ion::Events::Paste) {
    updateClipboard(false);
  }
#endif
  if (event == Ion::Events::TimerFire ) {
    window()->redraw();
    return true;
  }
  bool eventProcessed = s_activeApp->processEvent(event);
#if !defined(DEVICE_N0100) && !defined(DEVICE_N0110)
  updateClipboard(event == Ion::Events::Copy || event == Ion::Events::Cut);
#endif
  if (eventProcessed) {
    window()->redraw();
    return true;
  }
  return false;
}

void Container::run() {
  window()->redraw();
  RunLoop::run();
}

int Container::numberOfTimers() {
  return s_activeApp->numberOfTimers() + numberOfContainerTimers();
}

Timer * Container::timerAtIndex(int i) {
  if (i < s_activeApp->numberOfTimers()) {
    return s_activeApp->timerAtIndex(i);
  }
  return containerTimerAtIndex(i-s_activeApp->numberOfTimers());
}

int Container::numberOfContainerTimers() {
  return 0;
}

Timer * Container::containerTimerAtIndex(int i) {
  assert(false);
  return nullptr;
}
