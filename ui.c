#include "basketball_sim.h"
#include "colors.h"
#include <stdlib.h> // For free()

// Forward declaration for the new menu
void HandleLoadedSeasonMenu(Simulation *sim);

// Print the main menu
void PrintMainMenu() {
    printf("%s\n=== NBA Simulation Main Menu ===%s\n", COLOR_HEADER, COLOR_RESET);
    printf("%s1.%s Start Next Season\n", COLOR_NUMBER, COLOR_RESET);
    printf("%s2.%s Load a Past Season\n", COLOR_NUMBER, COLOR_RESET);
    printf("%s3.%s Factory Reset (Erase All Data)\n", COLOR_NUMBER, COLOR_RESET);
    printf("%s0.%s Exit\n", COLOR_NUMBER, COLOR_RESET);
    printf("%s================================%s\n", COLOR_HEADER, COLOR_RESET);
}


void PrintSimulationMenu(Simulation *sim) {
    printf("%s\n=== Simulation Menu (%s%d-%d%s | Day %s%d%s/%s%d%s) ===%s\n",
           COLOR_HEADER,
           COLOR_NUMBER, sim->year, sim->year + 1, COLOR_HEADER,
           COLOR_NUMBER, sim->currentDay, COLOR_HEADER,
           COLOR_NUMBER, sim->totalDays, COLOR_HEADER,
           COLOR_RESET);
    printf("%s1.%s Continue Simulation\n", COLOR_NUMBER, COLOR_RESET);
    printf("%s2.%s View Current Standings\n", COLOR_NUMBER, COLOR_RESET);
    printf("%s3.%s View Team Roster\n", COLOR_NUMBER, COLOR_RESET);
    printf("%s0.%s End Simulation & Return to Main Menu\n", COLOR_NUMBER, COLOR_RESET);
    printf("%s=============================================%s\n", COLOR_HEADER, COLOR_RESET);
}

// Ask the user how long they want to run
int askSimulationDuration(Simulation *sim) {
    int remaining = sim->totalDays - sim->currentDay + 1;
    if (remaining < 0) remaining = 0;

    printf("\nHow long do you want to run?\n");
    printf("  1) 1 day\n");
    printf("  2) 1 week (7 days)\n");
    printf("  3) 2 weeks (14 days)\n");
    printf("  4) 1 month (30 days)\n");
    printf("  5) Until end of season (%d days remaining)\n", remaining);

    int choice = getUserChoice(1, 5);

    switch (choice) {
        case 1: return 1;
        case 2: return 7;
        case 3: return 14;
        case 4: return 30;
        case 5: return remaining;
        default: return 1;
    }
}

// Handle team selection and display roster


// Print playoffs qualifiers
void printPlayoffsQualifiers(Simulation *sim) {
    printf("\n");
    printf("%s========================================================%s\n", COLOR_SUCCESS, COLOR_RESET);
    printf("%s           REGULAR SEASON COMPLETE!%s\n", COLOR_SUCCESS, COLOR_RESET);
    printf("%s========================================================%s\n", COLOR_SUCCESS, COLOR_RESET);
    printf("\n");

    // Get top 8 from each conference
    Team eastTeams[TEAMS_COUNT];
    Team westTeams[TEAMS_COUNT];
    int eastCount = 0, westCount = 0;

    for (int i = 0; i < TEAMS_COUNT; i++) {
        if (sim->teams[i].conference == 0) {
            eastTeams[eastCount++] = sim->teams[i];
        } else {
            westTeams[westCount++] = sim->teams[i];
        }
    }

    sortTeamsByRecord(eastTeams, eastCount);
    sortTeamsByRecord(westTeams, westCount);

    printf("%s%sEASTERN CONFERENCE PLAYOFFS:%s\n", COLOR_BOLD, COLOR_EAST, COLOR_RESET);
    for (int i = 0; i < 8 && i < eastCount; i++) {
        printf("  %s%d.%s %s%-30s%s %s(%d-%d)%s\n",
               COLOR_NUMBER, i+1, COLOR_RESET,
               COLOR_TEAM_NAME, eastTeams[i].name, COLOR_RESET,
               COLOR_STAT_VALUE, eastTeams[i].wins, eastTeams[i].losses, COLOR_RESET);
    }

    printf("\n");
    printf("%s%sWESTERN CONFERENCE PLAYOFFS:%s\n", COLOR_BOLD, COLOR_WEST, COLOR_RESET);
    for (int i = 0; i < 8 && i < westCount; i++) {
        printf("  %s%d.%s %s%-30s%s %s(%d-%d)%s\n",
               COLOR_NUMBER, i+1, COLOR_RESET,
               COLOR_TEAM_NAME, westTeams[i].name, COLOR_RESET,
               COLOR_STAT_VALUE, westTeams[i].wins, westTeams[i].losses, COLOR_RESET);
    }
    printf("\n");
}


// Season end menu
void HandleSeasonEndMenu(Simulation *sim, int *simRunning) {
    int endMenuRunning = 1;

    while (endMenuRunning) {
        printf("%s\n=== %s%d-%d%s Season Complete Menu ===%s\n",
               COLOR_HEADER,
               COLOR_NUMBER, sim->year, sim->year + 1, COLOR_HEADER,
               COLOR_RESET);
        printf("%s1.%s View Final Standings\n", COLOR_NUMBER, COLOR_RESET);
        printf("%s2.%s Start Playoffs\n", COLOR_NUMBER, COLOR_RESET);
        printf("%s3.%s View Season Summary\n", COLOR_NUMBER, COLOR_RESET);
        printf("%s0.%s Save and Return to Main Menu\n", COLOR_NUMBER, COLOR_RESET);
        printf("%s==================================%s\n", COLOR_HEADER, COLOR_RESET);

        int choice = getUserChoice(0, 3);

        switch (choice) {
            case 1:
                printStandings(sim);
                pressEnterToContinue();
                break;
            case 2:
                if (sim->playoffsComplete) {
                    printf(COLOR_WARNING "\nPlayoffs already completed!\n" COLOR_RESET);
                } else {
                    simulatePlayoffs(sim);
                    sim->playoffsComplete = 1;
                }
                pressEnterToContinue();
                break;
            case 3:
                if (sim->playoffsComplete) {
                    printSeasonSummary(sim);
                } else {
                    printf(COLOR_WARNING "\nComplete playoffs first to view season summary!\n" COLOR_RESET);
                }
                pressEnterToContinue();
                break;
            case 0:
                if (sim->playoffsComplete) {
                    saveSimulation(sim);
                    pressEnterToContinue();
                } else {
                     printf(COLOR_WARNING "\nPlayoffs are not complete. Results will not be saved.\n" COLOR_RESET);
                     pressEnterToContinue();
                }
                endMenuRunning = 0;
                *simRunning = 0;
                break;
        }
    }
}

void HandleSimulationMenu(Simulation *sim, int *simRunning) {
    PrintSimulationMenu(sim);
    int choice = getUserChoice(0, 3);

    switch (choice) {
        case 1: { // Continue simulation
            if (sim->seasonComplete) {
                HandleSeasonEndMenu(sim, simRunning);
                break;
            }

            int duration = askSimulationDuration(sim);
            runSimulation(sim, duration);
            clearConsole();
            printStandings(sim);

            if (sim->seasonComplete) {
                printPlayoffsQualifiers(sim);
                pressEnterToContinue();
                HandleSeasonEndMenu(sim, simRunning);
            }
            break;
        }
        case 2:
            printStandings(sim);
            pressEnterToContinue();
            break;

        case 3:
            viewTeamRoster(sim);
            break;

        case 0:
            *simRunning = 0;
            if (!sim->playoffsComplete) {
                printf(COLOR_WARNING "\nPlayoffs are not complete. Results will not be saved.\n" COLOR_RESET);
                pressEnterToContinue();
            }
            printf(COLOR_WARNING "Ending current simulation...\n" COLOR_RESET);
            break;
    }
}

// Handle simulation menu
void HandleLoadedSeasonMenu(Simulation *sim) {
    int loadedMenuRunning = 1;
    while(loadedMenuRunning) {
        printf("%s\n=== Loaded %s%d-%d%s Season Menu ===%s\n",
               COLOR_HEADER,
               COLOR_NUMBER, sim->year, sim->year + 1, COLOR_HEADER,
               COLOR_RESET);
        printf("%s1.%s View Final Standings\n", COLOR_NUMBER, COLOR_RESET);
        printf("%s2.%s View Team Roster\n", COLOR_NUMBER, COLOR_RESET);
        printf("%s3.%s View Season Summary\n", COLOR_NUMBER, COLOR_RESET);
        printf("%s0.%s Back to Main Menu\n", COLOR_NUMBER, COLOR_RESET);
        printf("%s=================================%s\n", COLOR_HEADER, COLOR_RESET);

        int choice = getUserChoice(0, 3);
        switch(choice) {
            case 1:
                printStandings(sim);
                pressEnterToContinue();
                break;
            case 2:
                viewTeamRoster(sim);
                break;
            case 3:
                printSeasonSummary(sim);
                pressEnterToContinue();
                break;
            case 0:
                loadedMenuRunning = 0;
                break;
        }
    }
}

// Handle team selection and display roster
void viewTeamRoster(Simulation *sim) {
    printf("%s\n=== Select a Team ===%s\n", COLOR_HEADER, COLOR_RESET);

    for (int i = 0; i < TEAMS_COUNT; i++) {
        const char *confColor = sim->teams[i].conference == 0 ? COLOR_EAST : COLOR_WEST;
        printf("%s%2d.%s %s%s%s\n",
               COLOR_NUMBER, i + 1, COLOR_RESET,
               confColor, sim->teams[i].name, COLOR_RESET);
    }
    printf(" %s0.%s Back\n", COLOR_NUMBER, COLOR_RESET);

    int choice = getUserChoice(0, TEAMS_COUNT);

    if (choice == 0) return;

    printTeamInfo(&sim->teams[choice - 1]);
    pressEnterToContinue();
}

// Handle main menu
void HandleMainMenu(Simulation *sim, int *running) {
    while (*running) {
        clearConsole();
        PrintMainMenu();
        int choice = getUserChoice(0, 3);

        switch (choice) {
            case 1: { // Start new simulation
                int year = getNextSeasonYear();
                printf(COLOR_SUCCESS "Starting %d-%d simulation...\n" COLOR_RESET, year, year + 1);

                char names[TOTAL_PLAYER_COUNT][MAX_NAME_LENGTH];
                 int playerCount = loadPlayerNames(names, "nba_players.txt");
                if (playerCount == 0) {
                     printf(COLOR_ERROR "Could not load player names. Make sure 'nba_players.txt' is in the same directory. Exiting.\n" COLOR_RESET);
                     *running = 0;
                     break;
                }
                shuffleNameArray(names, playerCount);

                initializeSimulation(sim);
                sim->year = year;
                initializeAllTeams(sim, names, &playerCount);
                generateSchedule(sim);

                printf(COLOR_SUCCESS "Season initialized!\n" COLOR_RESET);
                clearConsole();
                printStandings(sim);
                pressEnterToContinue();

                int simRunning = 1;
                while (simRunning) {
                    HandleSimulationMenu(sim, &simRunning);
                }

                cleanupSimulation(sim);
                break;
            }
            case 2: { // Load a Past Season
                char *filename = selectSaveFile();
                if (filename) {
                    if (loadSimulation(sim, filename)) {
                        clearConsole();
                        HandleLoadedSeasonMenu(sim);
                    }
                    free(filename);
                }
                pressEnterToContinue();
                clearConsole();
                break;
            }
            case 3: { // Factory Reset
                clearConsole();
                factoryReset();
                clearConsole();
                break;
            }
            case 0:
                *running = 0;
                break;
        }
    }
}


// New menu specifically for interacting with loaded data
