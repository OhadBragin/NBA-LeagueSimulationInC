#include "basketball_sim.h"

void generateSchedule(Simulation *sim) {
    sim->matchSchedule = NULL;
    sim->currentDay = 0;
    sim->totalDays = 0;

    int lastPlayed[TEAMS_COUNT];
    for (int i = 0; i < TEAMS_COUNT; i++) {
        lastPlayed[i] = -2;  // all teams rested at start
    }

    ScheduleList *tail = NULL;
    int currentDay = 0;
    int gamesToday = 0;

    for (int i = 0; i < TEAMS_COUNT; i++) {
        for (int j = i + 1; j < TEAMS_COUNT; j++) {
            for (int round = 0; round < 2; round++) {
                // find a suitable day
                while ((currentDay - lastPlayed[i] < 2) || (currentDay - lastPlayed[j] < 2) || (gamesToday >= 8)) {
                    // move to next day if team not rested or day is full
                    currentDay++;
                    gamesToday = 0;
                }

                // allocate a new match node
                ScheduleList *node = malloc(sizeof(ScheduleList));
                if (!node) { printf("Error allocating schedule node\n"); exit(1); }

                initializeMatch(&node->m, &sim->teams[i], &sim->teams[j], 0); // regular season
                node->m.day = currentDay; // assign scheduled day
                node->next = NULL;

                // insert into linked list
                if (!sim->matchSchedule) {
                    sim->matchSchedule = node;
                    tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }

                // update trackers
                lastPlayed[i] = currentDay;
                lastPlayed[j] = currentDay;
                gamesToday++;
            }
        }
    }

    sim->totalDays = currentDay + 1; // total season length
}


void generateRegularSeasonSchedule(Simulation *sim) {
    // Create regular season matchups
    // Ensure each team plays appropriate number of games
    // Balance home/away games
    // TODO: Implement regular season scheduling
}

void generatePlayoffSchedule(Simulation *sim) {
    // Generate playoff bracket
    // Set up playoff matchups based on standings
    // TODO: Implement playoff scheduling
}

void printTodaysGames(const Simulation *sim) {
    // Display all games scheduled for current day
    // Show team names and game stage
    // TODO: Implement daily schedule display
}