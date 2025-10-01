#include "basketball_sim.h"

void printStandings(const Simulation *sim) {
    // Display complete league standings
    // Show both conferences
    // TODO: Implement standings display
}

int findHighestPPG(const Player* roster) {
    int highestPPG = 0;
    int currentPPG;
    int index = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        currentPPG = roster[i].points / roster[i].gamesPlayed;
        if (currentPPG > highestPPG) {
            highestPPG = currentPPG;
            index = i;
        }
    }

    return index;
}

void sortTeamsByRecord(Team teams[], int count) {
    // Sort teams by win-loss record
    // Consider tiebreakers (point differential, etc.)
    // TODO: Implement team sorting algorithm
}

void printConferenceStandings(const Simulation *sim, int conference) {
    // Print standings for specific conference
    // Show team records and playoff positioning
    // TODO: Implement conference standings display
}