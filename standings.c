#include "basketball_sim.h"

void printStandings(const Simulation *sim) {
    // Print both conference standings
    printConferenceStandings(sim, 0);  // East
    printConferenceStandings(sim, 1);  // West

    // Ask user if they want to see rosters
    printf("%sDo you want to see team rosters? (y/n): %s", COLOR_INFO, COLOR_RESET);

    char choice;
    scanf(" %c", &choice);
    clearInputBuffer();

    if (choice == 'y' || choice == 'Y') {
        clearScreen();

        // Print Eastern Conference with rosters
        printf("\n");
        printf("%s============================================================%s\n", COLOR_BORDER, COLOR_RESET);
        printf("%s%s                    EASTERN CONFERENCE%s\n", COLOR_BOLD, COLOR_EAST, COLOR_RESET);
        printf("%s============================================================%s\n", COLOR_BORDER, COLOR_RESET);

        Team eastTeams[TEAMS_COUNT/2];
        for (int i = 0; i < TEAMS_COUNT/2; i++) {
            eastTeams[i] = sim->teams[i];
        }
        sortTeamsByPR(eastTeams, TEAMS_COUNT/2);

        for (int i = 0; i < TEAMS_COUNT/2; i++) {
            int gamesPlayed = eastTeams[i].wins + eastTeams[i].losses;
            double ppg = gamesPlayed > 0 ? (double)eastTeams[i].seasonPoints / gamesPlayed : 0.0;

            const char *rankColor = i < 8 ? COLOR_SUCCESS : COLOR_LOSER;
            const char *marker = i < 8 ? "*" : " ";

            printf("\n%s%s%d. %s%s - %s%d-%d%s (PR: %s%d%s, PPG: %s%.1f%s)\n",
                   rankColor, marker, i + 1, COLOR_RESET,
                   eastTeams[i].name,
                   COLOR_STAT_VALUE, eastTeams[i].wins, eastTeams[i].losses, COLOR_RESET,
                   COLOR_NUMBER, eastTeams[i].PR, COLOR_RESET,
                   COLOR_STAT_VALUE, ppg, COLOR_RESET);

            printRoster(eastTeams[i].roster);
        }

        // Print Western Conference with rosters
        printf("\n\n");
        printf("%s============================================================%s\n", COLOR_BORDER, COLOR_RESET);
        printf("%s%s                    WESTERN CONFERENCE%s\n", COLOR_BOLD, COLOR_WEST, COLOR_RESET);
        printf("%s============================================================%s\n", COLOR_BORDER, COLOR_RESET);

        Team westTeams[TEAMS_COUNT/2];
        for (int i = 0; i < TEAMS_COUNT/2; i++) {
            westTeams[i] = sim->teams[TEAMS_COUNT/2 + i];
        }
        sortTeamsByPR(westTeams, TEAMS_COUNT/2);

        for (int i = 0; i < TEAMS_COUNT/2; i++) {
            int gamesPlayed = westTeams[i].wins + westTeams[i].losses;
            double ppg = gamesPlayed > 0 ? (double)westTeams[i].seasonPoints / gamesPlayed : 0.0;

            const char *rankColor = i < 8 ? COLOR_SUCCESS : COLOR_LOSER;
            const char *marker = i < 8 ? "*" : " ";

            printf("\n%s%s%d. %s%s - %s%d-%d%s (PR: %s%d%s, PPG: %s%.1f%s)\n",
                   rankColor, marker, i + 1, COLOR_RESET,
                   westTeams[i].name,
                   COLOR_STAT_VALUE, westTeams[i].wins, westTeams[i].losses, COLOR_RESET,
                   COLOR_NUMBER, westTeams[i].PR, COLOR_RESET,
                   COLOR_STAT_VALUE, ppg, COLOR_RESET);

            printRoster(westTeams[i].roster);
        }

        printf("\n%s* Playoff Teams%s\n", COLOR_SUCCESS, COLOR_RESET);
    }
}

int findHighestPPG(const Player* roster) {
    double highestPPG = 0;
    int index = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        const double currentPPG = (double)roster[i].points / roster[i].gamesPlayed;
        if (currentPPG > highestPPG) {
            highestPPG = currentPPG;
            index = i;
        }
    }

    return index;
}

void sortTeamsByPR(Team teams[], int count) {
    // Sort teams by PR
    // TODO: Implement team sorting algorithm
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (teams[j].PR > teams[j + 1].PR) {
                // Swap
                Team temp = teams[j];
                teams[j] = teams[j + 1];
                teams[j + 1] = temp;
            }
        }
    }
}

void printConferenceStandings(const Simulation *sim, int conference) {
    // Determine conference info
    const char *confName = conference == 0 ? "EASTERN CONFERENCE" : "WESTERN CONFERENCE";
    const char *confColor = conference == 0 ? COLOR_EAST : COLOR_WEST;

    // Put teams into array
    Team arr[TEAMS_COUNT/2];
    int startIdx = conference == 0 ? 0 : TEAMS_COUNT/2;

    for (int i = 0; i < TEAMS_COUNT/2; i++) {
        arr[i] = sim->teams[startIdx + i];
    }

    // Sort array by highest PR
    sortTeamsByPR(arr, TEAMS_COUNT/2);

    // Print header
    printf("\n");
    printf("%s============================================================%s\n", COLOR_BORDER, COLOR_RESET);
    printf("%s%s                    %s%s\n", COLOR_BOLD, confColor, confName, COLOR_RESET);
    printf("%s============================================================%s\n", COLOR_BORDER, COLOR_RESET);
    printf("\n");

    // Print column headers
    printf("  %s%-4s %-25s %6s %6s  %6s%s\n",
           COLOR_STAT_LABEL, "Rank", "Team", "Record", "PR", "Pts", COLOR_RESET);
    printf("%s-------------------------------------------------------------%s\n", COLOR_BORDER, COLOR_RESET);

    // Print each team
    for (int i = 0; i < TEAMS_COUNT/2; i++) {
        int gamesPlayed = arr[i].wins + arr[i].losses;
        double ppg = gamesPlayed > 0 ? (double)arr[i].seasonPoints / gamesPlayed : 0.0;

        // Highlight playoff teams (top 8)
        const char *rankColor = i < 8 ? COLOR_SUCCESS : COLOR_LOSER;
        const char *marker = i < 8 ? "*" : " ";

        printf("  %s%s%-2d%s  %-25s %s%2dW -%2d%sL  %s%4d%s  %s%5.1f%s\n",
               rankColor, marker, i + 1, COLOR_RESET,
               arr[i].name,
               COLOR_STAT_VALUE, arr[i].wins, arr[i].losses, COLOR_RESET,
               COLOR_NUMBER, arr[i].PR, COLOR_RESET,
               COLOR_STAT_VALUE, ppg, COLOR_RESET);
    }

    printf("%s============================================================%s\n", COLOR_BORDER, COLOR_RESET);
}