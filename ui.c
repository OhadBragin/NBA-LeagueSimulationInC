#include "basketball_sim.h"
#include "colors.h"
#include <stdlib.h> // For free()

// Forward declaration for the new menu
void HandleLoadedSeasonMenu(Simulation *sim);

// Print the main menu
void PrintMainMenu() {
    printf(COLOR_HEADER "\n=== NBA Simulation Main Menu ===\n" COLOR_RESET);
    printf("1. Start Next Season\n");
    printf("2. Load a Past Season\n");
    printf("3. Factory Reset (Erase All Data)\n");
    printf("0. Exit\n");
    printf(COLOR_HEADER "================================\n" COLOR_RESET);
}

// Print simulation menu
void PrintSimulationMenu(Simulation *sim) {
    printf(COLOR_HEADER "\n=== Simulation Menu (%d-%d | Day %d/%d) ===\n" COLOR_RESET,
           sim->year, sim->year + 1, sim->currentDay, sim->totalDays);
    printf("1. Continue Simulation\n");
    printf("2. View Current Standings\n");
    printf("3. View Team Roster\n");
    printf("0. End Simulation & Return to Main Menu\n");
    printf(COLOR_HEADER "=============================================\n" COLOR_RESET);
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
void viewTeamRoster(Simulation *sim) {
    printf(COLOR_HEADER "\n=== Select a Team ===\n" COLOR_RESET);

    for (int i = 0; i < TEAMS_COUNT; i++) {
        printf("%2d. %s\n", i + 1, sim->teams[i].name);
    }
    printf(" 0. Back\n");

    int choice = getUserChoice(0, TEAMS_COUNT);

    if (choice == 0) return;

    printTeamInfo(&sim->teams[choice - 1]);
    pressEnterToContinue();
}

// Print playoffs qualifiers
void printPlayoffsQualifiers(Simulation *sim) {
    printf("\n");
    printf(COLOR_SUCCESS "========================================================\n");
    printf("           REGULAR SEASON COMPLETE!                     \n");
    printf("========================================================\n" COLOR_RESET);
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

    printf(COLOR_EAST "EASTERN CONFERENCE PLAYOFFS:\n" COLOR_RESET);
    for (int i = 0; i < 8 && i < eastCount; i++) {
        printf("  %d. %s (%d-%d)\n", i+1, eastTeams[i].name,
               eastTeams[i].wins, eastTeams[i].losses);
    }

    printf("\n");
    printf(COLOR_WEST "WESTERN CONFERENCE PLAYOFFS:\n" COLOR_RESET);
    for (int i = 0; i < 8 && i < westCount; i++) {
        printf("  %d. %s (%d-%d)\n", i+1, westTeams[i].name,
               westTeams[i].wins, westTeams[i].losses);
    }
    printf("\n");
}


// Season end menu
void HandleSeasonEndMenu(Simulation *sim, int *simRunning) {
    int endMenuRunning = 1;

    while (endMenuRunning) {
        printf(COLOR_HEADER "\n=== %d-%d Season Complete Menu ===\n" COLOR_RESET, sim->year, sim->year + 1);
        printf("1. View Final Standings\n");
        printf("2. View Team Roster\n");
        printf("3. Start Playoffs\n");
        printf("4. View Season Summary\n");
        printf("0. Save and Return to Main Menu\n");
        printf(COLOR_HEADER "==================================\n" COLOR_RESET);

        int choice = getUserChoice(0, 3);

        switch (choice) {
            case 1:
                printStandings(sim);
                pressEnterToContinue();
                break;
            case 2:
                viewTeamRoster(sim);
                break;
            case 3:
                if (sim->playoffsComplete) {
                    printf(COLOR_WARNING "\nPlayoffs already completed!\n" COLOR_RESET);
                } else {
                    simulatePlayoffs(sim);
                    sim->playoffsComplete = 1;
                }
                pressEnterToContinue();
                break;
            case 4:
                if (sim->playoffsComplete) {
                    printSeasonSummary(sim);
                } else {
                    printf(COLOR_WARNING "\nComplete playoffs first to view season summary!\n" COLOR_RESET);
                }
                pressEnterToContinue();
                break;
            case 0:
                if (sim->playoffsComplete) {
                    saveSimulation(sim); // Automatically save on exit
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

// Handle simulation menu
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
                pressEnterToContinue();
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
void HandleLoadedSeasonMenu(Simulation *sim) {
    int loadedMenuRunning = 1;
    while(loadedMenuRunning) {
        printf(COLOR_HEADER "\n=== Loaded %d-%d Season Menu ===\n" COLOR_RESET, sim->year, sim->year + 1);
        printf("1. View Final Standings\n");
        printf("2. View Team Roster\n");
        printf("3. View Season Summary\n");
        printf("0. Back to Main Menu\n");
        printf(COLOR_HEADER "=================================\n" COLOR_RESET);

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