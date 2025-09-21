#include "basketball_sim.h"

void initializeMatch(Match *match, Team *teamA, Team *teamB, int stage) {
    // Set up match between two teams
    // Set game stage (regular/playoffs/finals)
    // Initialize scores to zero
    // TODO: Implement match initialization
    match->teamA = teamA;
    match->teamB = teamB;
    match->stage = stage;
}

int getK(Match *match) {
    switch (match->stage) {
        case 0: return 24; //regular season
        case 1: return 32; //playoffs
        case 2: return 50; //finals
        default: return 0; //invalid stage
    }
}

void simulateMatch(Match *match) {
    // Simulate the basketball game
    // Calculate scores based on team strength
    // Update team and player statistics
    // TODO: Implement match simulation logic
    //get probability
    const double aWinProb = calculateProbability(match->teamA->PR, match->teamB->PR);
    const double bWinProb = 1 - aWinProb;
    const int aWin = calculateOutcomeByPercentage(aWinProb);
    const int winner = aWin ? 0 : 1;  // 0 = A wins, 1 = B wins
    const int K = getK(match);
    calculatePowerRanking(match->teamA, match->teamB, aWinProb, bWinProb, K, winner);

}

int calculateTeamScore(const Team *team) {
    // Calculate team's score for a game
    // Consider team power ranking and player performance
    // Add randomness for realistic variation
    // TODO: Implement score calculation
    return 0; // Placeholder
}

void printMatchResult(const Match *match) {
    // Display the final score
    // Show team names and result
    // TODO: Implement match result display
}