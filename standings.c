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

    // Determine conference colors
    const char *confColor = (conference == 0) ? COLOR_EAST : COLOR_WEST;
    const char *confName = (conference == 0) ? "EASTERN CONFERENCE" : "WESTERN CONFERENCE";

    // Header
    printf("\n%s============================================================%s\n", COLOR_BORDER, COLOR_RESET);
    printf("%s%s                    %s%s\n", COLOR_BOLD, confColor, confName, COLOR_RESET);
    printf("%s============================================================%s\n", COLOR_BORDER, COLOR_RESET);

    // Print table header
    printf("\n  %sRank %-25s %-10s %-6s %-6s%s\n",
           COLOR_STAT_LABEL, "Team", "Record", "PR", "PPG", COLOR_RESET);
    printf("%s-------------------------------------------------------------%s\n", COLOR_BORDER, COLOR_RESET);

    // Print teams
    for (int i = 0; i < confCount; i++) {
        // Highlight playoff teams (top 8)
        const char *rankColor = (i < 8) ? COLOR_SUCCESS : COLOR_NUMBER;
        const char *teamColor = (i < 8) ? COLOR_TEAM_NAME : COLOR_PLAYER;

        printf("  %s%2d%s   %s%-25s%s %s%2dW - %2dL%s  %s%4d%s   %s%.1f%s\n",
               rankColor, i + 1, COLOR_RESET,
               teamColor, confTeams[i].name, COLOR_RESET,
               COLOR_STAT_VALUE, confTeams[i].wins, confTeams[i].losses, COLOR_RESET,
               COLOR_NUMBER, confTeams[i].PR, COLOR_RESET,
               COLOR_STAT_VALUE,
               (confTeams[i].wins + confTeams[i].losses > 0)
                   ? (double)confTeams[i].seasonPoints / (confTeams[i].wins + confTeams[i].losses)
                   : 0.0,
               COLOR_RESET);
    }

    printf("%s============================================================%s\n", COLOR_BORDER, COLOR_RESET);
}


// Print both conferences
void printStandings(const Simulation *sim) {
    printConferenceStandings(sim, 0); // East
    printConferenceStandings(sim, 1); // West
}
