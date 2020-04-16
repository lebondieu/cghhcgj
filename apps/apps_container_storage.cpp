#include "apps_container_storage.h"
#include "../macros.h"

#ifndef APPS_CONTAINER_SNAPSHOT_CONSTRUCTORS
#error Missing snapshot constructors
#endif

#ifndef APPS_CONTAINER_SNAPSHOT_LIST
#error Missing snapshot list
#endif

#ifndef APPS_CONTAINER_SNAPSHOT_COUNT
#error Missing snapshot count
#endif


#ifndef APPS_CAN_BE_HIDE_COUNT
#error Missing app can be hide count
#endif

#ifndef APPS_CAN_BE_HIDE_LIST
#error Missing apps can be hide list
#endif

AppsContainerStorage::AppsContainerStorage() :
  AppsContainer()
  APPS_CONTAINER_SNAPSHOT_CONSTRUCTORS
{
}

int AppsContainerStorage::numberOfApps() {
  return AppsContainerS::k_numberOfCommonApps;
}

int AppsContainerStorage::numberOfAppsCanBeHide() {
  return AppsContainerS::k_numberAppsCanBeHide;
}

int AppsContainerStorage::numberOfAppsShow() {
  bool canBeHideList[] = {
    APPS_CAN_BE_HIDE_LIST
  };

  int count = 0;

  for (int i = 0; i < AppsContainerS::k_numberOfCommonApps - 1; i++) {
    if (!canBeHideList[i] || appSnapshotAtIndex(i+1)->canBeShow()) {
      count++;
    }
  }
  return count;
}

App::Snapshot * AppsContainerStorage::appSnapshotAtIndex(int index) {
  if (index < 0) {
    return nullptr;
  }
  App::Snapshot * snapshots[] = {
    homeAppSnapshot()
    APPS_CONTAINER_SNAPSHOT_LIST
  };
  assert(sizeof(snapshots)/sizeof(snapshots[0]) == AppsContainerS::k_numberOfCommonApps);
  assert(index >= 0 && index < AppsContainerS::k_numberOfCommonApps);
  return snapshots[index];
}

App::Snapshot * AppsContainerStorage::appSnapshotCanBeHideAtIndex(int index) {
  if (index < 0) {
    return nullptr;
  }

  bool canBeHideList[] = {
    APPS_CAN_BE_HIDE_LIST
  };

  assert(index < AppsContainerS::k_numberAppsCanBeHide); // don't forgot home app is in commons apps
  int current_index = 0;

  for (int i = 0; i < AppsContainerS::k_numberOfCommonApps - 1; i++) {
    if (canBeHideList[i]) {
      if (current_index == index) {
        return appSnapshotAtIndex(i+1);
      }
      current_index++;
    }
  }

  return nullptr;
}


App::Snapshot * AppsContainerStorage::appSnapshotIsShowAtIndex(int index) {
  if (index < 0) {
    return nullptr;
  }

  bool canBeHideList[] = {
    APPS_CAN_BE_HIDE_LIST
  };

  assert(index < AppsContainerS::k_numberOfCommonApps-1); // don't forgot home app is in commons apps
  int current_index = 0;

  for (int i = 0; i < AppsContainerS::k_numberOfCommonApps - 1; i++) {
    if (!canBeHideList[i] || appSnapshotAtIndex(i+1)->canBeShow()) {
      if (current_index == index) {
        return appSnapshotAtIndex(i+1);
      }
      current_index++;
    }
  }
  return nullptr; // if index >= k_numberAppsCanBeHide - number of apps hide
}