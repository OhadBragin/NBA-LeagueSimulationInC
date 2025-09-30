#include "basketball_sim.h"



Team* initializeTeam(const char *name, int conference, char names[][MAX_NAME_LENGTH], int *playerCount) {
    // Allocate memory for new team
    Team *team = (Team *)malloc(sizeof(Team));
    if (team == NULL) {
        printf("Error allocating memory for team\n");
        exit(1);
    }

    // Initialize team with name and conference
    team->name = (char *)malloc(strlen(name) + 1);
    if (team->name == NULL) {
        printf("Error allocating memory for team name\n");
        free(team);
        exit(1);
    }
    strcpy(team->name, name);

    team->conference = conference;
    team->wins = 0;
    team->losses = 0;
    team->PR = BASE_ELO;
    team->seasonPoints = 0;

    // Create roster
    createRoster(team->roster, names, playerCount);

    return team;
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
    printf("\n");
    printf("=====================================\n");
    printf("            %s\n", team->name);
    printf("=====================================\n");
    printf("Conference: %s\n", team->conference == 0 ? "East" : "West");
    printf("Record: %d-%d\n", team->wins, team->losses);
    printf("Power Ranking: %d\n", team->PR);
    printf("Season Points: %d\n", team->seasonPoints);

    int gamesPlayed = team->wins + team->losses;

    if (gamesPlayed > 0) {
        printf("Points Per Game: %.1f\n", (double)team->seasonPoints / gamesPlayed);
    }

    printf("=====================================\n");

    // Print roster information
    printRoster(team->roster);

    printf("\n");
}

void addPlayerToTeam(Team *team, const char *playerName) {
    // Add a new player to team roster
    // Initialize player stats
    // TODO: Implement player addition
}


