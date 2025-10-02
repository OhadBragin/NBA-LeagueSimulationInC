#include "basketball_sim.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize a team
Team* initializeTeam(const char *name, int conference, char names[][MAX_NAME_LENGTH], int *playerCount) {
    Team *team = (Team *)malloc(sizeof(Team));
    if (!team) {
        printf("Error allocating memory for team\n");
        exit(1);
    }

    team->name = (char *)malloc(strlen(name) + 1);
    if (!team->name) {
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

void initializeAllTeams(Simulation *sim, char names[][MAX_NAME_LENGTH], int *playerCount) {
    const char *nbaTeamNames[TEAMS_COUNT] = {
        "Boston Celtics", "Brooklyn Nets", "New York Knicks", "Philadelphia 76ers",
        "Toronto Raptors", "Chicago Bulls", "Cleveland Cavaliers", "Detroit Pistons",
        "Indiana Pacers", "Milwaukee Bucks", "Atlanta Hawks", "Charlotte Hornets",
        "Miami Heat", "Orlando Magic", "Washington Wizards", // Eastern (15)

        "Denver Nuggets", "Minnesota Timberwolves", "Oklahoma City Thunder", "Portland Trail Blazers",
        "Utah Jazz", "Golden State Warriors", "LA Clippers", "Los Angeles Lakers",
        "Phoenix Suns", "Sacramento Kings", "Dallas Mavericks", "Houston Rockets",
        "Memphis Grizzlies", "New Orleans Pelicans", "San Antonio Spurs" // Western (15)
    };

    for (int i = 0; i < TEAMS_COUNT; i++) {
        int conference = (i < 15) ? 0 : 1; // 0 = East, 1 = West
        Team *tmp = initializeTeam(nbaTeamNames[i], conference, names, playerCount);
        sim->teams[i] = *tmp;   // copy into Simulation's array
        free(tmp);
    }
}

// Update team stats
void updateTeamStats(Team *team, int points, int isWin) {
    team->wins += isWin;
    team->losses += !isWin;
    team->seasonPoints += points;
}

// Power Ranking (Elo-style)
void calculatePowerRanking(Team *teamA, Team *teamB, double pA, double pB, int K, int winner) {
    const double aWin = (winner == 0) ? 1.0 : 0.0;
    const double bWin = (winner == 1) ? 1.0 : 0.0;

    teamA->PR = (int)(teamA->PR + (K * (aWin - pA)));
    teamB->PR = (int)(teamB->PR + (K * (bWin - pB)));
}

// Print info about a team
void printTeamInfo(const Team *team) {
    const char *confColor = team->conference == 0 ? COLOR_EAST : COLOR_WEST;
    const char *confName  = team->conference == 0 ? "Eastern Conference" : "Western Conference";

    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);
    printf("%s%s            %-25s%s\n", COLOR_BOLD, COLOR_HEADER, team->name, COLOR_RESET);
    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);

    printf("%sConference:%s %s%s%s\n",
           COLOR_STAT_LABEL, COLOR_RESET, confColor, confName, COLOR_RESET);

    printf("%sRecord:%s %s%d-%d%s\n",
           COLOR_STAT_LABEL, COLOR_RESET, COLOR_STAT_VALUE, team->wins, team->losses, COLOR_RESET);

    printf("%sPower Ranking:%s %s%d%s\n",
           COLOR_STAT_LABEL, COLOR_RESET, COLOR_NUMBER, team->PR, COLOR_RESET);

    int gamesPlayed = team->wins + team->losses;
    if (gamesPlayed > 0) {
        printf("%sPoints Per Game:%s %.1f\n",
               COLOR_STAT_LABEL, COLOR_RESET, (double)team->seasonPoints / gamesPlayed);
    }

    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);

    // Print roster
    printRoster(team->roster);

    printf("\n");
}

// Add player (future feature)
void addPlayerToTeam(Team *team, const char *playerName) {
    // TODO: implement if you want dynamic roster additions
}
