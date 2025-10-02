#include "basketball_sim.h"
#include "colors.h"

// Print the main menu
void PrintMainMenu() {
    printf(COLOR_HEADER "\n=== NBA Simulation Main Menu ===\n" COLOR_RESET);
    printf("1. Start New Simulation\n");
    printf("2. View Past Simulations\n");
    printf("0. Exit\n");
    printf(COLOR_HEADER "================================\n" COLOR_RESET);
}

// Print simulation menu
void PrintSimulationMenu(Simulation *sim) {
    printf(COLOR_HEADER "\n=== Simulation Menu (Day %d/%d) ===\n" COLOR_RESET,
           sim->currentDay, sim->totalDays);
    printf("1. Continue Simulation\n");
    printf("2. View Current Standings\n");
    printf("3. View Team Roster\n");
    printf("0. End Simulation\n");
    printf(COLOR_HEADER "===================================\n" COLOR_RESET);
}

// Ask the user how long they want to run
int askSimulationDuration(Simulation *sim) {
    int remaining = sim->totalDays - sim->currentDay + 1;

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
        printf(COLOR_HEADER "\n=== Season Complete Menu ===\n" COLOR_RESET);
        printf("1. View Final Standings\n");
        printf("2. View Team Roster\n");
        printf("3. View Playoff Qualifiers\n");
        printf("4. Start Playoffs (Coming Soon)\n");
        printf("0. Return to Main Menu\n");
        printf(COLOR_HEADER "============================\n" COLOR_RESET);

        int choice = getUserChoice(0, 4);

        switch (choice) {
            case 1:
                printStandings(sim);
                pressEnterToContinue();
                break;
            case 2:
                viewTeamRoster(sim);
                break;
            case 3:
                printPlayoffsQualifiers(sim);
                pressEnterToContinue();
                break;
            case 4:
                printf(COLOR_WARNING "\nPlayoffs feature coming soon!\n" COLOR_RESET);
                pressEnterToContinue();
                break;
            case 0:
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
                printf(COLOR_WARNING "\nRegular season is already complete!\n" COLOR_RESET);
                pressEnterToContinue();
                break;
            }

            int duration = askSimulationDuration(sim);
            runSimulation(sim, duration);

            // Show standings after simulation
            printStandings(sim);

            // Check if season ended
            if (sim->seasonComplete) {
                printPlayoffsQualifiers(sim);
                pressEnterToContinue();
                HandleSeasonEndMenu(sim, simRunning);
            }
            pressEnterToContinue();
            break;
        }

        case 2: // View standings
            printStandings(sim);
            pressEnterToContinue();
            break;

        case 3: // View team roster
            viewTeamRoster(sim);
            break;

        case 0: // End simulation
            *simRunning = 0;
            printf(COLOR_WARNING "Ending simulation...\n" COLOR_RESET);
            break;

        default:
            printf(COLOR_ERROR "Invalid choice. Try again.\n" COLOR_RESET);
            break;
    }
}

// Handle main menu
void HandleMainMenu(Simulation *sim, int *running) {
    PrintMainMenu();
    int choice = getUserChoice(0, 2);

    switch (choice) {
        case 1: { // Start new simulation
            printf(COLOR_SUCCESS "Starting new simulation...\n" COLOR_RESET);

            char names[TOTAL_PLAYER_COUNT][MAX_NAME_LENGTH];
            int playerCount = loadPlayerNames(names, "nba_players.txt");
            shuffleNameArray(names, playerCount);

            initializeSimulation(sim);
            initializeAllTeams(sim, names, &playerCount);
            generateSchedule(sim);

            printf(COLOR_SUCCESS "Season initialized!\n" COLOR_RESET);

            printStandings(sim);
            pressEnterToContinue();

            int simRunning = 1;
            while (simRunning) {
                HandleSimulationMenu(sim, &simRunning);
            }

            cleanupSimulation(sim);
            break;
        }

        case 2:
            printf(COLOR_INFO "Feature not implemented yet: View Past Simulations.\n" COLOR_RESET);
            pressEnterToContinue();
            break;

        case 0:
            *running = 0;
            printf(COLOR_WARNING "Exiting NBA Simulation...\n" COLOR_RESET);
            break;

        default:
            printf(COLOR_ERROR "Invalid choice. Try again.\n" COLOR_RESET);
            break;
    }
}