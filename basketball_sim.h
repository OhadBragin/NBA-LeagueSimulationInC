#ifndef BASKETBALL_SIM_H
#define BASKETBALL_SIM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Constants
#define MAX_PLAYERS 15
#define TEAMS_COUNT 30
#define MAX_GAMES_PER_DAY 16
#define REGULAR_SEASON_DAYS 82
#define PLAYOFFS_DAYS 28
#define FINALS_DAYS 7
#define BASE_ELO 1500

// ---player---
typedef struct Player {
    char name[30];
    int points; //total points scored in the current season
    int gamesPlayed; //total games played this season
} Player;

// ---team---
typedef struct Team {
    char *name;
    int PR; //power-ranking
    int wins; //total wins
    int losses; //total losses
    int conference; // East/West (0=East, 1=West)
    int seasonPoints; //counter for all points scored in the current season
    Player roster[MAX_PLAYERS];
} Team;

// ---match--- (Note: renamed from Match to avoid conflicts)
typedef struct Match {
    Team *teamA;
    Team *teamB;
    int scoreA;
    int scoreB;
    int stage; // 0 = Regular, 1 = Playoffs, 2 = Finals
} Match;

// ---Simulation---
typedef struct Simulation {
    int currentDay; //current day of the season
    int totalDays; //total days in the season
    Team teams[TEAMS_COUNT];
    Match schedule[MAX_GAMES_PER_DAY]; //schedule of all games in the current day
    int gamesToday;
} Simulation;

// Function declarations from different modules
// simulation.c
void initializeSimulation(Simulation *sim);
void runSimulation(Simulation *sim);
void advanceDay(Simulation *sim);
void cleanupSimulation(Simulation *sim);

// team.c
void initializeTeam(Team *team, const char *name, int conference);
void updateTeamStats(Team *team, int points, int isWin);
void calculatePowerRanking(Team *teamA, Team *teamB, double pA, double pB, int K, int winner);
void printTeamInfo(const Team *team);
void addPlayerToTeam(Team *team, const char *playerName);

// player.c
void initializePlayer(Player *player, const char *name);
void updatePlayerStats(Player *player, int points);
void printPlayerStats(const Player *player);

// match.c
void initializeMatch(Match *match, Team *teamA, Team *teamB, int stage);
void simulateMatch(Match *match);
void calculateGameScores(Team *teamA, Team *teamB, int teamAWins, int *scoreA, int *scoreB);
void printMatchResult(const Match *match);
int getK(Match *match);

// schedule.c
void generateSchedule(Simulation *sim);
void generateRegularSeasonSchedule(Simulation *sim);
void generatePlayoffSchedule(Simulation *sim);
void printTodaysGames(const Simulation *sim);

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

// file_io.c
void saveSimulation(const Simulation *sim, const char *filename);
int loadSimulation(Simulation *sim, const char *filename);
void exportStandings(const Simulation *sim, const char *filename);

#endif // BASKETBALL_SIM_H