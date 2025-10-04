#include "basketball_sim.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

#define BASE_TEAM_SCORE 114        // Average team score
#define SCORE_VARIANCE 20          // How much PR affects score (+/-) the lower this number, the more variance
#define RANDOMNESS_RANGE 10        // Random variation (+/-)
#define MIN_TEAM_SCORE 85          // Minimum realistic score
#define MAX_TEAM_SCORE 155         // Maximum realistic score
#define RANDOM_POWRESS_NOISE 5     // How much a player's prowess can vary per game (+/-)
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
        case 0: return 32; // Regular season (was 24)
        case 1: return 48; // Playoffs (was 32)
        case 2: return 64; // Finals (was 50)
        default: return 32;
    }
}

int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

// Calculate game scores based on power rankings and winner
void calculateGameScores(Match *m, int teamAWins) {
    int extraLead;
    int ratingDiff;

    // Calculate base scores (100 + power ranking adjustment + random noise)
    int baseScoreA = BASE_TEAM_SCORE + ((m->teamA->PR - 1500) / SCORE_VARIANCE) + getRandomNumber(-RANDOMNESS_RANGE, RANDOMNESS_RANGE);
    int baseScoreB = BASE_TEAM_SCORE + ((m->teamB->PR - 1500) / SCORE_VARIANCE) + getRandomNumber(-RANDOMNESS_RANGE, RANDOMNESS_RANGE);

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
        m->scoreA = clamp(baseScoreA, MIN_TEAM_SCORE, MAX_TEAM_SCORE);
        m->scoreB = clamp(baseScoreB, MIN_TEAM_SCORE, MAX_TEAM_SCORE);
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
        m->scoreA = clamp(baseScoreA, MIN_TEAM_SCORE, MAX_TEAM_SCORE);
        m->scoreB = clamp(baseScoreB, MIN_TEAM_SCORE, MAX_TEAM_SCORE);
    }
}

void distributePlayerPoints(Team *team, int teamScore) {
    double adjustedProwess[MAX_PLAYERS];
    int finalPoints[MAX_PLAYERS] = {0}; // array to store final points
    double totalAdjustedProwess = 0.0;
    int highestProwessIdx = 0; // Keep track of the best player

    // Calculate each player's prowess with random noise and find who's having the best game.
    for (int i = 0; i < MAX_PLAYERS; i++) {
        double noise = (double)getRandomNumber(-RANDOM_POWRESS_NOISE, RANDOM_POWRESS_NOISE);
        double gameProwess = team->roster[i].scoringProwess + noise;

        if (gameProwess < 1.0) {
            gameProwess = 1.0;
        }

        adjustedProwess[i] = gameProwess;
        totalAdjustedProwess += gameProwess;

        // Check if this player has the highest prowess
        if (adjustedProwess[i] > adjustedProwess[highestProwessIdx]) {
            highestProwessIdx = i;
        }
    }

    // Calculate each player's initial share of points.
    int totalPointsAssigned = 0;
    if (totalAdjustedProwess > 0) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            double percentage = adjustedProwess[i] / totalAdjustedProwess;
            int points = (int)(teamScore * percentage);
            finalPoints[i] = points;
            totalPointsAssigned += points;
        }
    }

    // Step 3: Give any leftover points to a random player to ensure total matches team score.
    int remainder = teamScore - totalPointsAssigned;
    if (remainder > 0) {
        finalPoints[getRandomNumber(0,4)] += remainder;
    }

    // Step 4: Update all player stats
    for (int i = 0; i < MAX_PLAYERS; i++) {
        updatePlayerStats(&team->roster[i], finalPoints[i]);
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
    distributePlayerPoints(match->teamA, match->scoreA);
    distributePlayerPoints(match->teamB, match->scoreB);
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