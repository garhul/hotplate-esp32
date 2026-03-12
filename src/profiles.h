#pragma once
#include <Arduino.h>

namespace Profiles {
  extern uint8_t profileCount;
  extern uint8_t currentProfileIndex;

  class ProfilePhase {
    public:
    String name;
    uint16_t targetTemp;
    uint16_t durationSeconds;
    uint8_t fanSpeed;
    ProfilePhase* prev;
    ProfilePhase* next;

    ProfilePhase(String name, uint16_t targetTemp, uint32_t duration, uint8_t fanSpeed = 0) {
      this->name = name;
      this->targetTemp = targetTemp;
      this->durationSeconds = duration;
      this->fanSpeed = fanSpeed;

      prev = nullptr;
      next = nullptr;
    }

    void setPrev(ProfilePhase* p) {
      prev = p;
      if (p != nullptr) {
        p->next = this;
      }
    }

    void setNext(ProfilePhase* n) {
      next = n;
      if (n != nullptr) {
        n->prev = this;
      }
    }

  };

  struct Profile {
    const char* name;
    ProfilePhase* start = nullptr;
    uint16_t durationSeconds = 0;
  };

  extern Profiles::Profile profiles[10]; // TODO: dynamic allocation or at least a more flexible structure

  void loadProfiles();
  void getProfilesList(Profiles::Profile profiles[], size_t profileCount);

}