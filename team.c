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

void initializeAllTeams(Simulation *sim, char names[][MAX_NAME_LENGTH], int *playerCount) {
    int conference;
    const char *nbaTeamNames[TEAMS_COUNT] = {
        // East (0-14)
        "Boston Celtics", "Brooklyn Nets", "New York Knicks", "Philadelphia 76ers", "Toronto Raptors",
        "Chicago Bulls", "Cleveland Cavaliers", "Detroit Pistons", "Indiana Pacers", "Milwaukee Bucks",
        "Atlanta Hawks", "Charlotte Hornets", "Miami Heat", "Orlando Magic", "Washington Wizards",

        // West (15 - 29)
        "Denver Nuggets", "Minnesota Timberwolves", "Oklahoma City Thunder", "Portland Trail Blazers", "Utah Jazz",
        "Golden State Warriors", "LA Clippers", "Los Angeles Lakers", "Phoenix Suns", "Sacramento Kings",
        "Dallas Mavericks", "Houston Rockets", "Memphis Grizzlies", "New Orleans Pelicans", "San Antonio Spurs"
    };

    for (int i = 0; i < TEAMS_COUNT; i++) {
        if (i < 15) {
            conference = 0;
        } else conference = 1;

        Team *tmp = initializeTeam(nbaTeamNames[i], conference, names, playerCount);
        sim->teams[i] = *tmp; //enter pointer's value
        free(tmp);
    }
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

void calculatePowerRanking(Team *teamA, Team *teamB, double pA, int K, int winner) { //0|team A / 1|team B
    // Calculate power ranking based on performance
    // Consider wins, losses, and point differential
    // TODO: Implement power ranking calculation
    const double aWin = (winner == 0) ? 1.0 : 0.0;
    const double bWin = (winner == 1) ? 1.0 : 0.0;
    double pB = 1.0 - pA;
    //change ranking
    teamA->PR = (int)(teamA->PR + (K * (aWin - pA)));
    teamB->PR = (int)(teamB->PR + (K * (bWin - pB)));
}

void printTeamInfo(const Team *team) {
    int gamesPlayed = team->wins + team->losses;
    const char *confColor = team->conference == 0 ? COLOR_EAST : COLOR_WEST;
    const char *confName = team->conference == 0 ? "East" : "West";

    // Print header
    printf("\n");
    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);
    printf("%s%s            %-25s%s\n", COLOR_BOLD, COLOR_HEADER, team->name, COLOR_RESET);
    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);

    // Print team stats with alignment
    printf("%sConference:        %s%s%s%s\n",
           COLOR_STAT_LABEL, confColor, confName, COLOR_RESET, COLOR_RESET);
    printf("%sRecord:            %s%dW-%dL%s\n",
           COLOR_STAT_LABEL, COLOR_STAT_VALUE, team->wins, team->losses, COLOR_RESET);
    printf("%sPower Ranking:     %s%d%s\n",
           COLOR_STAT_LABEL, COLOR_NUMBER, team->PR, COLOR_RESET);
    printf("%sSeason Points:     %s%d%s\n",
           COLOR_STAT_LABEL, COLOR_STAT_VALUE, team->seasonPoints, COLOR_RESET);

    if (gamesPlayed > 0) {
        printf("%sPoints Per Game:   %s%.1f%s\n",
               COLOR_STAT_LABEL, COLOR_STAT_VALUE,
               (double)team->seasonPoints / gamesPlayed, COLOR_RESET);
    }

    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);

    // Print roster information
    printRoster(team->roster);

    printf("\n");
}

void addPlayerToTeam(Team *team, const char *playerName) {
    // Add a new player to team roster
    // Initialize player stats
    // TODO: Implement player addition
}


