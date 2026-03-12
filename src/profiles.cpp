#include "profiles.h"

namespace Profiles {
  uint8_t profileCount = 0;
  uint8_t currentProfileIndex = 0;

  Profile profiles[10]; // TODO: dynamic allocation or at least a more flexible structure
  ProfilePhase* current = nullptr;
  void loadProfiles() {
    profileCount = 0;
    // For now hardcoded profiles, eventually some file loading would be nice

    /** Standard profile
     * Ramp: 90s to 150
     * Soak: 90s at 150
     * Reflow: 60s to 240
     * Reflow: 10s to 240
     * Cooldown: 50s to min - Fan On - Heater off
    */

    ProfilePhase* ramp = new ProfilePhase("Ramp", 150, 90);
    ProfilePhase* soak = new ProfilePhase("Soak", 150, 90);
    ProfilePhase* reflow1 = new ProfilePhase("Reflow 1", 240, 60);
    ProfilePhase* reflow2 = new ProfilePhase("Reflow 2", 240, 10);
    ProfilePhase* cooldown = new ProfilePhase("Cooldown", 25, 50, 255);
    cooldown->setPrev(reflow2);
    reflow2->setPrev(reflow1);
    reflow1->setPrev(soak);
    soak->setPrev(ramp);


    Profiles::profiles[profileCount].name = "Standard";
    Profiles::profiles[profileCount].start = ramp;

    current = Profiles::profiles[profileCount].start;
    while (current != nullptr) {
      Profiles::profiles[profileCount].durationSeconds += Profiles::profiles[profileCount].start->durationSeconds;
      current = current->next;
    }


    profileCount++;
    ProfilePhase* q_ramp = new ProfilePhase("Ramp", 150, 60);
    ProfilePhase* q_soak = new ProfilePhase("Soak", 180, 60);
    ProfilePhase* q_reflow1 = new ProfilePhase("Reflow 1", 240, 60);
    ProfilePhase* q_reflow2 = new ProfilePhase("Reflow 2", 240, 5);
    ProfilePhase* q_cooldown = new ProfilePhase("Cooldown", 25, 50, 255);
    q_cooldown->setPrev(q_reflow2);
    q_reflow2->setPrev(q_reflow1);
    q_reflow1->setPrev(q_soak);
    q_soak->setPrev(q_ramp);

    Profiles::profiles[profileCount].name = "Quick";
    Profiles::profiles[profileCount].start = q_ramp;

    current = Profiles::profiles[profileCount].start;
    while (current != nullptr) {
      Profiles::profiles[profileCount].durationSeconds += Profiles::profiles[profileCount].start->durationSeconds;
      current = current->next;
    }
  }

  void getProfilesList(Profiles::Profile profiles[], size_t profileCount) {
    for (size_t i = 0; i < profileCount && i < Profiles::profileCount; i++) {
      profiles[i] = Profiles::profiles[i];
    }
  };

}