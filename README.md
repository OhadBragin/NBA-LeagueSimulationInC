/**
 * @file basketball_sim.h
 * @brief Main header file for the C-Sim Basketball League.
 *
 * This file contains all the core data structures, constants,
 * and function prototypes used across the entire simulation project.
 * It serves as the central "glue" that connects all the different
 * .c modules.
 */

#ifndef BASKETBALL_SIM_H
#define BASKETBALL_SIM_H

// --- Standard Library Includes ---
#include <stdio.h>    // For file I/O (printf, FILE, fopen, etc.)
#include <stdlib.h>   // For memory allocation (malloc, free) and random numbers (rand, srand)
#include <string.h>   // For string manipulation (strcpy, strcmp)
#include <time.h>     // For seeding the random number generator (time)
#include <unistd.h>   // For sleep() (optional, for pacing)

// --- Project Constants ---
#define MAX_PLAYERS 15           // Players per team
#define TEAMS_COUNT 30           // Teams in league
#define MAX_GAMES_PER_DAY 16     // Max games in one day (30 teams / 2 = 15, 16 is safe)
#define REGULAR_SEASON_DAYS 82   // Total days in the regular season

// For team conferences
#define CONFERENCE_EAST 0
#define CONFERENCE_WEST 1

// For match stages
#define STAGE_REGULAR 0
#define STAGE_PLAYOFFS 1
#define STAGE_FINALS 2

// --- Data Structures ---

/**
 * @struct Player
 * @brief Represents a single player.
 */
typedef struct {
    char name[30];
    int points;       // Total points scored this season
    int gamesPlayed;  // Total games played this season
} Player;

/**
 * @struct Team
 * @brief Represents a single team.
 */
typedef struct {
    char name[50];
    int PR;           // Power Ranking (higher is better)
    int wins;
    int losses;
    int conference;   // 0 = East, 1 = West
    int seasonPoints; // Total points scored by team
    Player roster[MAX_PLAYERS];
} Team;

/**
 * @struct Match
 * @brief Represents a single game.
 *
 * @note We use team indices (teamA_idx, teamB_idx) instead of
 * pointers (Team* teamA). This makes saving and loading the
 * simulation state to a file *much* simpler and safer.
 */
typedef struct {
    int teamA_idx;    // Index of team A in the main simulation.teams array
    int teamB_idx;    // Index of team B in the main simulation.teams array
    int scoreA;
    int scoreB;
    int stage;        // 0=Regular, 1=Playoffs, 2=Finals
} Match;

/**
 * @struct Simulation
 * @brief Holds the entire state of the league simulation.
 */
typedef struct {
    int currentDay;
    int totalDays;
    Team teams[TEAMS_COUNT];
    Match schedule[MAX_GAMES_PER_DAY]; // Schedule for the *current* day
    int gamesToday;                    // Number of games scheduled today
    int simulationStage;               // 0=Regular, 1=Playoffs, etc.
} Simulation;


// --- Function Prototypes ---

// --- simulation.c ---
void initializeSimulation(Simulation *sim);
void runSimulation(Simulation *sim);
void simulateDay(Simulation *sim);
void advanceDay(Simulation *sim);

// --- team.c ---
void initializeAllTeams(Team teams[TEAMS_COUNT]);
void updateTeamStats(Team *team, int pointsScored, int pointsAllowed);

// --- player.c ---
void initializePlayers(Team *team);
void distributePlayerPoints(Team *team, int teamScore);

// --- match.c ---
void runMatch(Simulation *sim, Match *match);
int calculateScore(int powerRanking);

// --- schedule.c ---
void generateDailySchedule(Simulation *sim);
void initializePlayoffs(Simulation *sim); // Stub for future
void initializeFinals(Simulation *sim);   // Stub for future

// --- standings.c ---
void printStandings(Simulation *sim);
int compareTeams(const void *a, const void *b);

// --- utils.c ---
void clearScreen();
void pauseExecution();

// --- file_io.c ---
int saveSimulation(const Simulation *sim, const char *filename);
int loadSimulation(Simulation *sim, const char *filename);

#endif // BASKETBALL_SIM_H
