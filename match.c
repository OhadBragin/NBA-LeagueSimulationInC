#include "basketball_sim.h"
#define BASE_TEAM_SCORE 100        // Average team score
#define SCORE_VARIANCE 20          // How much PR affects score (+/-)
#define RANDOMNESS_RANGE 12        // Random variation (+/-)
#define MIN_TEAM_SCORE 75          // Minimum realistic score
#define MAX_TEAM_SCORE 170         // Maximum realistic score
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

void calculateGameScores(Team *teamA, Team *teamB, int teamAWins, int *scoreA, int *scoreB) {
    int extraLead;
    int ratingDiff;

    // Calculate base scores (100 + power ranking adjustment + random noise)
    int baseScoreA = BASE_TEAM_SCORE + ((teamA->PR - 1500) / 25) + getRandomNumber(-RANDOMNESS_RANGE, RANDOMNESS_RANGE);
    int baseScoreB = BASE_TEAM_SCORE + ((teamB->PR - 1500) / 25) + getRandomNumber(-RANDOMNESS_RANGE, RANDOMNESS_RANGE);

    if (teamAWins) {
        // TeamA wins
        if (baseScoreA <= baseScoreB) {
            // If winner's base score isn't higher, make it higher
            baseScoreA = baseScoreB + getRandomNumber(1, 6);
        }
        // If teamA has higher rating than teamB, bigger lead
        if (teamA->PR > teamB->PR) {
            ratingDiff = teamA->PR - teamB->PR;
            extraLead = ratingDiff / 50; // Bigger rating difference = bigger lead
            baseScoreA += extraLead;
        }
        *scoreA = baseScoreA;
        *scoreB = baseScoreB;
    } else {
        // TeamB wins
        if (baseScoreB <= baseScoreA) {
            // If winner's base score isn't higher, make it higher
            baseScoreB = baseScoreA + getRandomNumber(1, 6);
        }
        // If teamB has higher rating than teamA, bigger lead
        if (teamB->PR > teamA->PR) {
            ratingDiff = teamB->PR - teamA->PR;
            extraLead = ratingDiff / 50; // Bigger rating difference = bigger lead
            baseScoreB += extraLead;
        }
        *scoreA = baseScoreA;
        *scoreB = baseScoreB;
    }

    // Add to team season totals
    teamA->seasonPoints += *scoreA;
    teamB->seasonPoints += *scoreB;
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
    const double bWinProb = 1 - aWinProb;
    const int aWin = calculateOutcomeByPercentage(aWinProb);
    const int winner = aWin ? 0 : 1;  // 0 = A wins, 1 = B wins
    const int K = getK(match);
    calculatePowerRanking(match->teamA, match->teamB, aWinProb, bWinProb, K, winner);
    calculateGameScores(match->teamA, match->teamB, aWin, &scoreA, &scoreB);
}



void printMatchResult(const Match *match) {
    // Display the final score
    // Show team names and result
    // TODO: Implement match result display
}