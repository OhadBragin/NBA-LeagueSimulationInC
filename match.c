#include "basketball_sim.h"
#define BASE_TEAM_SCORE 100        // Average team score
#define SCORE_VARIANCE 20          // How much PR affects score (+/-)
#define RANDOMNESS_RANGE 12        // Random variation (+/-)
#define MIN_TEAM_SCORE 75          // Minimum realistic score
#define MAX_TEAM_SCORE 170         // Maximum realistic score

Match* createMatch() {
    Match* match = (Match *)malloc(sizeof(Match));
    if (match == NULL) {
        printf("Failed to allocate memory for match.\n");
        exit(1);
    }
    match->teamA = (Team *)malloc(sizeof(Team));
    match->teamB = (Team *)malloc(sizeof(Team));
    if (match->teamA == NULL || match->teamB == NULL) {
        printf("Failed to allocate memory for team.\n");
        exit(1);
    }
    return match;
}

void initializeMatch(Match *match, Team *teamA, Team *teamB, int stage) {
    // Set up match between two teams
    // Set game stage (regular/playoffs/finals)
    // Initialize scores to zero
    // TODO: Implement match initialization
    match->teamA = teamA;
    match->teamB = teamB;
    match->stage = stage;
    match->scoreA = 0;
    match->scoreB = 0;
    match->day = -1;

}

int getK(Match *match) {
    switch (match->stage) {
        case 0: return 24; //regular season
        case 1: return 32; //playoffs
        case 2: return 50; //finals
        default: return 0; //invalid stage
    }
}

void calculateGameScores(Match *m, int teamAWins) {
    int extraLead;
    int ratingDiff;

    // Calculate base scores (100 + power ranking adjustment + random noise)
    int baseScoreA = BASE_TEAM_SCORE + ((m->teamA->PR - 1500) / 25) + getRandomNumber(-RANDOMNESS_RANGE, RANDOMNESS_RANGE);
    int baseScoreB = BASE_TEAM_SCORE + ((m->teamB->PR - 1500) / 25) + getRandomNumber(-RANDOMNESS_RANGE, RANDOMNESS_RANGE);

    if (teamAWins) {
        // TeamA wins
        if (baseScoreA <= baseScoreB) {
            // If winner's base score isn't higher, make it higher
            baseScoreA = baseScoreB + getRandomNumber(1, 6);
        }
        // If teamA has higher rating than teamB, bigger lead
        if (m->teamA->PR > m->teamB->PR) {
            ratingDiff = m->teamA->PR - m->teamB->PR;
            extraLead = ratingDiff / 50; // Bigger rating difference = bigger lead
            baseScoreA += extraLead;
        }
        m->scoreA = baseScoreA;
        m->scoreB = baseScoreB;
    } else {
        // TeamB wins
        if (baseScoreB <= baseScoreA) {
            // If winner's base score isn't higher, make it higher
            baseScoreB = baseScoreA + getRandomNumber(1, 6);
        }
        // If teamB has higher rating than teamA, bigger lead
        if (m->teamB->PR > m->teamA->PR) {
            ratingDiff = m->teamB->PR - m->teamA->PR;
            extraLead = ratingDiff / 50; // Bigger rating difference = bigger lead
            baseScoreB += extraLead;
        }
        m->scoreA = baseScoreA;
        m->scoreB = baseScoreB;
    }


}

void simulateMatch(Match *match) {
    // Simulate the basketball game
    // Calculate scores based on team strength
    // Update team and player statistics
    // TODO: Implement match simulation logic
    //init vars
    int scoreA;
    int scoreB;

    //get probability
    const double aWinProb = calculateProbability(match->teamA->PR, match->teamB->PR);
    const int aWin = calculateOutcomeByPercentage(aWinProb);
    const int K = getK(match);
    calculatePowerRanking(match->teamA, match->teamB, aWinProb,  K, !aWin);
    calculateGameScores(match, aWin);
    updateTeamStats(match->teamA, match->scoreA, aWin);
    updateTeamStats(match->teamB, match->scoreB, !aWin);
    //update each player's stats
    // --- team A ---
   // TODO: Add player points distribution system
}



void printMatchResult(const Match *match) {
    // Determine stage name and color
    const char *stageName;
    const char *stageColor;
    switch (match->stage) {
        case 0:
            stageName = "REGULAR SEASON";
            stageColor = COLOR_REGULAR;
            break;
        case 1:
            stageName = "PLAYOFFS";
            stageColor = COLOR_PLAYOFFS;
            break;
        case 2:
            stageName = "FINALS";
            stageColor = COLOR_FINALS;
            break;
        default:
            stageName = "GAME";
            stageColor = COLOR_INFO;
            break;
    }

    // Determine winner
    int teamAWon = match->scoreA > match->scoreB;
    int pointDiff = abs(match->scoreA - match->scoreB);

    // Print header
    printf("\n");
    printf("%s==========================================%s\n", COLOR_BORDER, COLOR_RESET);
    printf("%s%s            %s%s\n", COLOR_BOLD, stageColor, stageName, COLOR_RESET);
    printf("%s==========================================%s\n", COLOR_BORDER, COLOR_RESET);

    // Print matchup with scores
    printf("\n");
    if (teamAWon) {
        printf("  %s* %-25s %s%3d%s  %s(W)%s\n",
               COLOR_SUCCESS, match->teamA->name,
               COLOR_SCORE_WIN, match->scoreA, COLOR_RESET,
               COLOR_SUCCESS, COLOR_RESET);
        printf("    %s%-25s %s%3d%s  %s(L)%s\n",
               COLOR_LOSER, match->teamB->name,
               COLOR_SCORE_LOSE, match->scoreB, COLOR_RESET,
               COLOR_LOSER, COLOR_RESET);
    } else {
        printf("    %s%-25s %s%3d%s  %s(L)%s\n",
               COLOR_LOSER, match->teamA->name,
               COLOR_SCORE_LOSE, match->scoreA, COLOR_RESET,
               COLOR_LOSER, COLOR_RESET);
        printf("  %s* %-25s %s%3d%s  %s(W)%s\n",
               COLOR_SUCCESS, match->teamB->name,
               COLOR_SCORE_WIN, match->scoreB, COLOR_RESET,
               COLOR_SUCCESS, COLOR_RESET);
    }

    printf("\n");
    printf("  %sMargin of Victory:%s %s%d points%s\n",
           COLOR_STAT_LABEL, COLOR_RESET,
           COLOR_STAT_VALUE, pointDiff, COLOR_RESET);
    printf("\n");

    // Print updated records with alignment
    printf("  %s%-25s%s %s%d-%d%s (PR: %s%d%s)\n",
           COLOR_TEAM_NAME, match->teamA->name, COLOR_RESET,
           COLOR_STAT_VALUE, match->teamA->wins, match->teamA->losses, COLOR_RESET,
           COLOR_NUMBER, match->teamA->PR, COLOR_RESET);
    printf("  %s%-25s%s %s%d-%d%s (PR: %s%d%s)\n",
           COLOR_TEAM_NAME, match->teamB->name, COLOR_RESET,
           COLOR_STAT_VALUE, match->teamB->wins, match->teamB->losses, COLOR_RESET,
           COLOR_NUMBER, match->teamB->PR, COLOR_RESET);

    printf("%s==========================================%s\n", COLOR_BORDER, COLOR_RESET);
    printf("\n");
}