#include "basketball_sim.h"

void initializeTeam(Team *team, const char *name, int conference) {
    // Initialize team with name and conference
    // Set up initial roster
    // Initialize stats to zero
    // TODO: Implement team initialization

}



void updateTeamStats(Team *team, int points, int isWin) {
    // Update team's win/loss record
    // Update season points
    // Update power ranking
    // TODO: Implement stats update
    team->wins += isWin;
    team->losses += !isWin;
    team->seasonPoints += points;
}

void calculatePowerRanking(Team *teamA, Team *teamB, double pA, double pB, int K, int winner) { //0|team A / 1|team B
    // Calculate power ranking based on performance
    // Consider wins, losses, and point differential
    // TODO: Implement power ranking calculation
    double aWin = (winner == 0) ? 1.0 : 0.0;
    double bWin = (winner == 1) ? 1.0 : 0.0;
    //change ranking
    teamA->PR = (int)(teamA->PR + (K * (aWin - pA)));
    teamB->PR = (int)(teamB->PR + (K * (bWin - pB)));
}

void printTeamInfo(const Team *team) {
    // Print team name, record, stats
    // Print roster information
    // TODO: Implement team info display
}

void addPlayerToTeam(Team *team, const char *playerName) {
    // Add a new player to team roster
    // Initialize player stats
    // TODO: Implement player addition
}