#ifndef BASKETBALL_SIM_H
#define BASKETBALL_SIM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// ==========================
// Constants
// ==========================
#define MAX_PLAYERS 5
#define TEAMS_COUNT 30
#define REGULAR_SEASON_DAYS 150
#define PLAYOFFS_DAYS 28
#define FINALS_DAYS 7
#define BASE_ELO 1500
#define TOTAL_PLAYER_COUNT 570
#define MAX_NAME_LENGTH 30

// ==========================
// Struct Declarations
// ==========================

// Forward declare Team so Match can use it
struct Team;

// --- match ---
typedef struct Match {
    struct Team *teamA;
    struct Team *teamB;
    int scoreA;
    int scoreB;
    int stage; // 0 = Regular, 1 = Playoffs, 2 = Finals
} Match;

// --- schedule linked list ---
typedef struct ScheduleList {
    Match m;
    int day; // which day this game is scheduled for
    struct ScheduleList *next;
} ScheduleList;

// --- player ---
typedef struct Player {
    char name[MAX_NAME_LENGTH];
    int points;       // total points scored in the current season
    int gamesPlayed;  // total games played this season
} Player;

// --- team ---
typedef struct Team {
    char *name;
    int PR;            // power-ranking (ELO)
    int wins;
    int losses;
    int conference;    // 0 = East, 1 = West
    int seasonPoints;  // counter for all points scored in the current season
    Player roster[MAX_PLAYERS];
} Team;

// --- simulation ---
typedef struct Simulation {
    int currentDay;          // current day of the season
    int totalDays;           // total days in the season
    Team teams[TEAMS_COUNT]; // teams array
    ScheduleList *matchSchedule; // schedule of all games in the season
    int gamesToday;          // games that will be played in the current day
    int seasonComplete;
} Simulation;

// ==========================
// Function Declarations
// ==========================

// simulation.c
void initializeSimulation(Simulation *sim);
void runSimulation(Simulation *sim, int duration);
void advanceDay(Simulation *sim);
void cleanupSimulation(Simulation *sim);

// team.c
Team* initializeTeam(const char *name, int conference,
                     char names[][MAX_NAME_LENGTH], int *playerCount);
void initializeAllTeams(Simulation *sim, char names[][MAX_NAME_LENGTH], int *playerCount);
void updateTeamStats(Team *team, int points, int isWin);
void calculatePowerRanking(Team *teamA, Team *teamB, double pA, double pB, int K, int winner);
void printTeamInfo(const Team *team);
void addPlayerToTeam(Team *team, const char *playerName);

// player.c
void initializePlayer(Player *player, const char *name);
void updatePlayerStats(Player *player, int points);
void printPlayerStats(const Player *player);
void shuffleNameArray(char names[][MAX_NAME_LENGTH], int count);
int loadPlayerNames(char names[][MAX_NAME_LENGTH], const char *fileName);
void createRoster(Player *roster, char names[][MAX_NAME_LENGTH], int *playerCount);
void freeRoster(Player *roster);
void printRoster(const Player *roster);

// match.c
void initializeMatch(Match *match, Team *teamA, Team *teamB, int stage);
void simulateMatch(Match *match);
void calculateGameScores(Match *m, int teamAWins);
void printMatchResult(const Match *match);
int getK(Match *match);

// schedule.c
void generateSchedule(Simulation *sim);
void generateRegularSeasonSchedule(Simulation *sim);
void printTodaysGames(const Simulation *sim);
int countTodaysGames(const Simulation *sim);

// standings.c
void printStandings(const Simulation *sim);
void sortTeamsByRecord(Team teams[], int count);
void printConferenceStandings(const Simulation *sim, int conference);

// utils.c
void clearInputBuffer(void);
int getRandomNumber(int min, int max);
void pressEnterToContinue(void);
int getUserChoice(int min, int max);
int calculateOutcomeByPercentage(double p);
double calculateProbability(int prA, int prB);
void cloneFile(const char *src, const char *dest);

// file_io.c
void saveSimulation(const Simulation *sim, const char *filename);
int loadSimulation(Simulation *sim, const char *filename);
void exportStandings(const Simulation *sim, const char *filename);

// ui.c
void PrintMainMenu();
void HandleMainMenu(Simulation *sim, int *running);
void PrintSimulationMenu();
void HandleSimulationMenu(Simulation *sim, int *simRunning);
int askSimulationDuration();
void viewTeamRoster(Simulation *sim);

#endif // BASKETBALL_SIM_H