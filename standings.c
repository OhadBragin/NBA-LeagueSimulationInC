#include "basketball_sim.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Compare function for qsort
static int compareTeams(const void *a, const void *b) {
    const Team *teamA = (const Team *)a;
    const Team *teamB = (const Team *)b;

    // Sort by wins first
    if (teamA->wins != teamB->wins) {
        return teamB->wins - teamA->wins; // more wins = higher rank
    }

    // If same wins, use PR
    if (teamA->PR != teamB->PR) {
        return teamB->PR - teamA->PR;
    }

    // If still tied, use season points
    return teamB->seasonPoints - teamA->seasonPoints;
}

// Sort teams by record and PR
void sortTeamsByRecord(Team teams[], int count) {
    qsort(teams, count, sizeof(Team), compareTeams);
}

// Print standings for one conference
void printConferenceStandings(const Simulation *sim, int conference) {
    Team confTeams[TEAMS_COUNT];
    int confCount = 0;

    // Collect teams by conference
    for (int i = 0; i < TEAMS_COUNT; i++) {
        if (sim->teams[i].conference == conference) {
            confTeams[confCount++] = sim->teams[i];
        }
    }

    // Sort them
    sortTeamsByRecord(confTeams, confCount);

    // Header
    printf(COLOR_BORDER "\n============================================================\n" COLOR_RESET);
    if (conference == 0)
        printf(COLOR_EAST "                    EASTERN CONFERENCE\n" COLOR_RESET);
    else
        printf(COLOR_WEST "                    WESTERN CONFERENCE\n" COLOR_RESET);
    printf(COLOR_BORDER "============================================================\n" COLOR_RESET);

    // Print table header
    printf("\n  Rank %-25s %-10s %-6s %-6s\n",
           "Team", "Record", "PR", "PPG");
    printf("-------------------------------------------------------------\n");

    // Print teams
    for (int i = 0; i < confCount; i++) {
        printf("  %2d   %-25s %2dW - %2dL  %4d   %.1f\n",
               i + 1,
               confTeams[i].name,
               confTeams[i].wins,
               confTeams[i].losses,
               confTeams[i].PR,
               (confTeams[i].wins + confTeams[i].losses > 0)
                   ? (double)confTeams[i].seasonPoints / (confTeams[i].wins + confTeams[i].losses)
                   : 0.0);
    }

    printf(COLOR_BORDER "============================================================\n" COLOR_RESET);
}

// Print both conferences
void printStandings(const Simulation *sim) {
    printConferenceStandings(sim, 0); // East
    printConferenceStandings(sim, 1); // West
}
