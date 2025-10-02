#include "basketball_sim.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h>
    #define sleep_ms(x) Sleep(x)
#else
    #include <unistd.h>
    #define sleep_ms(x) usleep((x) * 1000)
#endif

// Initialize simulation state
void initializeSimulation(Simulation *sim) {
    sim->currentDay = 1;
    sim->totalDays = REGULAR_SEASON_DAYS;
    sim->matchSchedule = NULL;
    sim->gamesToday = 0;
    sim->seasonComplete = 0;
}

// Advance simulation by one day
void advanceDay(Simulation *sim) {
    if (sim->currentDay < sim->totalDays) {
        sim->currentDay++;
    } else {
        sim->seasonComplete = 1;
    }
}

// Free memory at end of simulation
void cleanupSimulation(Simulation *sim) {
    ScheduleList *curr = sim->matchSchedule;
    while (curr) {
        ScheduleList *next = curr->next;
        free(curr);
        curr = next;
    }
    sim->matchSchedule = NULL;
}

// Run simulation for a chosen number of days
void runSimulation(Simulation *sim, int duration) {
    int daysToRun = duration;

    // Don't overshoot end of season
    if (sim->currentDay + daysToRun > sim->totalDays) {
        daysToRun = sim->totalDays - sim->currentDay + 1;
    }

    printf(COLOR_INFO "\nSimulating %d day(s)...\n" COLOR_RESET, daysToRun);
    sleep_ms(800);

    for (int d = 0; d < daysToRun; d++) {
        // Count today's games
        sim->gamesToday = countTodaysGames(sim);

        // Print today's schedule
        printTodaysGames(sim);

        // Simulate today's games
        int gamesPlayed = 0;
        ScheduleList *curr = sim->matchSchedule;
        while (curr && gamesPlayed < sim->gamesToday) {
            if (curr->day == sim->currentDay) {
                Match *match = &curr->m;
                simulateMatch(match);
                printMatchResult(match);
                gamesPlayed++;
            }
            curr = curr->next;
        }

        // Small pause between days
        if (d < daysToRun - 1 && sim->gamesToday > 0) {
            sleep_ms(300);
        }

        // Advance day
        advanceDay(sim);

        // Check if season ended
        if (sim->seasonComplete) {
            break;
        }
    }
}