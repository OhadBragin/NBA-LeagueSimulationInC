#include "basketball_sim.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

#define BASE_TEAM_SCORE 100        // Average team score
#define SCORE_VARIANCE 20          // How much PR affects score (+/-)
#define RANDOMNESS_RANGE 12        // Random variation (+/-)
#define MIN_TEAM_SCORE 75          // Minimum realistic score
#define MAX_TEAM_SCORE 170         // Maximum realistic score

// Initialize a match between two teams
void initializeMatch(Match *match, Team *teamA, Team *teamB, int stage) {
    match->teamA = teamA;
    match->teamB = teamB;
    match->scoreA = 0;
    match->scoreB = 0;
    match->stage = stage; // 0 = Regular, 1 = Playoffs, 2 = Finals
}

// Dynamic K-factor based on stage
int getK(Match *match) {
    switch (match->stage) {
        case 0: return 24; // Regular season
        case 1: return 32; // Playoffs
        case 2: return 50; // Finals
        default: return 24;
    }
}

// Calculate game scores based on power rankings and winner
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

// Simulate a match between two teams
void simulateMatch(Match *match) {
    // Probabilities
    double probA = calculateProbability(match->teamA->PR, match->teamB->PR);
    int teamAWins = calculateOutcomeByPercentage(probA);

    // Scores
    calculateGameScores(match, teamAWins);

    // Update team stats
    updateTeamStats(match->teamA, match->scoreA, teamAWins);
    updateTeamStats(match->teamB, match->scoreB, !teamAWins);

    // Update power rankings
    int winner = teamAWins ? 0 : 1;
    double probB = 1.0 - probA;
    int K = getK(match);
    calculatePowerRanking(match->teamA, match->teamB, probA, probB, K, winner);

    // Distribute player stats
    int pointsA = match->scoreA;
    int pointsB = match->scoreB;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        int shareA = pointsA / MAX_PLAYERS + getRandomNumber(-2, 2);
        int shareB = pointsB / MAX_PLAYERS + getRandomNumber(-2, 2);
        if (shareA < 0) shareA = 0;
        if (shareB < 0) shareB = 0;

        updatePlayerStats(&match->teamA->roster[i], shareA);
        updatePlayerStats(&match->teamB->roster[i], shareB);
    }
}

// Print match result with detailed formatting
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