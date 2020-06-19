#ifndef APPS_CONTAINER_STORAGE_H
#define APPS_CONTAINER_STORAGE_H

#include "apps_container.h"

#ifndef APPS_CONTAINER_SNAPSHOT_DECLARATIONS
#error Missing snapshot declarations
#endif

namespace AppsContainerS {
  constexpr int k_numberOfCommonApps = APPS_CONTAINER_SNAPSHOT_COUNT+1; // Take the Home app into account
  constexpr int k_numberAppsCanBeHide = APPS_CAN_BE_HIDE_COUNT;
}

class AppsContainerStorage : public AppsContainer {
public:
  AppsContainerStorage();
  int numberOfApps() override;
  int numberOfAppsCanBeHide() override;
  int numberOfAppsShow() override;
  App::Snapshot * appSnapshotAtIndex(int index) override;
  App::Snapshot * appSnapshotCanBeHideAtIndex(int index) override;
  App::Snapshot * appSnapshotIsShowAtIndex(int index) override;
  void * currentAppBuffer() override { return &m_apps; };
private:
  union Apps {
  public:
    /* Enforce a trivial constructor and destructor that just leave the memory
     * unmodified. This way, m_apps can be trivially destructed. */
    Apps() {};
    ~Apps() {};
  private:
    APPS_CONTAINER_APPS_DECLARATION
    Home::App m_homeApp;
    OnBoarding::App m_onBoardingApp;
    HardwareTest::App m_hardwareTestApp;
    USB::App m_usbApp;
  };
  Apps m_apps;
  APPS_CONTAINER_SNAPSHOT_DECLARATIONS
};

#endif
