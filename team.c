#include "basketball_sim.h"



void initializeTeam(Team *team, const char *name, int conference, char names[][MAX_NAME_LENGTH], int *playerCount) {
    // Initialize team with name and conference
    // Set up initial roster
    // Initialize stats to zero
    strcpy(team->name, name);
    team->conference = conference;
    team->wins = 0;
    team->losses = 0;
    team->PR = BASE_ELO;
    team->seasonPoints = 0;
    // TODO: Implement roster creation
    createRoster(team->roster, names, playerCount);
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
    const double aWin = (winner == 0) ? 1.0 : 0.0;
    const double bWin = (winner == 1) ? 1.0 : 0.0;
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