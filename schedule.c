#include "basketball_sim.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

// Helper: create a new schedule node
static ScheduleList* createScheduleNode(Team *teamA, Team *teamB, int stage, int day) {
    ScheduleList *node = (ScheduleList *)malloc(sizeof(ScheduleList));
    if (!node) {
        printf(COLOR_ERROR "Error allocating memory for schedule node!\n" COLOR_RESET);
        exit(1);
    }
    initializeMatch(&node->m, teamA, teamB, stage);
    node->day = day;
    node->next = NULL;
    return node;
}

// Generate a realistic regular season schedule - each team plays exactly 82 games
void generateRegularSeasonSchedule(Simulation *sim) {
    ScheduleList *head = NULL;
    ScheduleList *tail = NULL;

    int gamesPerTeam[TEAMS_COUNT] = {0};
    int lastGameDay[TEAMS_COUNT] = {0};
    int targetGames = 82;

    // Phase 1: Fill most of the schedule with rest day preferences
    for (int day = 1; day <= REGULAR_SEASON_DAYS - 20; day++) {
        int gamesPerDay = getRandomNumber(6, 10);
        int used[TEAMS_COUNT] = {0};
        int gamesToday = 0;
        int attempts = 0;

        while (gamesToday < gamesPerDay && attempts < 500) {
            attempts++;
            int a = getRandomNumber(0, TEAMS_COUNT - 1);
            int b = getRandomNumber(0, TEAMS_COUNT - 1);

            if (a == b || used[a] || used[b]) continue;
            if (gamesPerTeam[a] >= targetGames || gamesPerTeam[b] >= targetGames) continue;

            int daysSinceA = day - lastGameDay[a];
            int daysSinceB = day - lastGameDay[b];

            if (daysSinceA == 1 && getRandomNumber(1, 100) > 20) continue;
            if (daysSinceB == 1 && getRandomNumber(1, 100) > 20) continue;

            ScheduleList *node = createScheduleNode(&sim->teams[a], &sim->teams[b], 0, day);
            if (!head) head = node;
            else tail->next = node;
            tail = node;

            used[a] = used[b] = 1;
            lastGameDay[a] = day;
            lastGameDay[b] = day;
            gamesPerTeam[a]++;
            gamesPerTeam[b]++;
            gamesToday++;
        }
    }

    // Phase 2: Fill remaining games without rest restrictions
    for (int day = REGULAR_SEASON_DAYS - 19; day <= REGULAR_SEASON_DAYS; day++) {
        int used[TEAMS_COUNT] = {0};

        // Try to schedule teams that need more games
        for (int i = 0; i < TEAMS_COUNT; i++) {
            if (used[i] || gamesPerTeam[i] >= targetGames) continue;

            for (int j = i + 1; j < TEAMS_COUNT; j++) {
                if (used[j] || gamesPerTeam[j] >= targetGames) continue;

                ScheduleList *node = createScheduleNode(&sim->teams[i], &sim->teams[j], 0, day);
                if (!head) head = node;
                else tail->next = node;
                tail = node;

                used[i] = used[j] = 1;
                lastGameDay[i] = day;
                lastGameDay[j] = day;
                gamesPerTeam[i]++;
                gamesPerTeam[j]++;
                break;
            }
        }
    }

    sim->matchSchedule = head;
    sim->totalDays = REGULAR_SEASON_DAYS;
}

// Wrapper to generate complete season schedule
void generateSchedule(Simulation *sim) {
    generateRegularSeasonSchedule(sim);
}

// Count and return games for current day
int countTodaysGames(const Simulation *sim) {
    int count = 0;
    ScheduleList *curr = sim->matchSchedule;

    while (curr) {
        if (curr->day == sim->currentDay) {
            count++;
        }
        curr = curr->next;
    }

    return count;
}

// Print games for current day
void printTodaysGames(const Simulation *sim) {
    printf("\n");
    printf(COLOR_HEADER "=== Day %d of %d ===\n" COLOR_RESET, sim->currentDay, sim->totalDays);
    printf("Today's Games:\n\n");

    int gamesPrinted = 0;
    ScheduleList *curr = sim->matchSchedule;

    while (curr) {
        if (curr->day == sim->currentDay) {
            Match *m = &curr->m;
            printf("  %s vs %s\n", m->teamA->name, m->teamB->name);
            gamesPrinted++;
        }
        curr = curr->next;
    }

    if (gamesPrinted == 0) {
        printf("  No games scheduled today.\n");
    }
    printf("\n");
}