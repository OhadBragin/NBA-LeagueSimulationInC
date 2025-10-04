#include "basketball_sim.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #define sleep_ms(x) Sleep(x)
#else
    #include <unistd.h>
    #define sleep_ms(x) usleep((x) * 1000)
#endif

// Helper: Get top 8 teams from a conference
static void getConferencePlayoffTeams(Simulation *sim, int conference, Team *playoffTeams[8]) {
    Team confTeams[TEAMS_COUNT];
    int confCount = 0;

    // Collect teams from conference
    for (int i = 0; i < TEAMS_COUNT; i++) {
        if (sim->teams[i].conference == conference) {
            confTeams[confCount++] = sim->teams[i];
        }
    }

    // Sort by record
    sortTeamsByRecord(confTeams, confCount);

    // Get top 8
    for (int i = 0; i < 8; i++) {
        // Find original team pointer in sim->teams
        for (int j = 0; j < TEAMS_COUNT; j++) {
            if (strcmp(sim->teams[j].name, confTeams[i].name) == 0) {
                playoffTeams[i] = &sim->teams[j];
                break;
            }
        }
    }
}

// Simulate a single playoff game with home court advantage
static void simulatePlayoffGame(Match *match, int isHomeGameForA) {
    // Home court gives slight PR boost (50 points)
    int originalPR_A = match->teamA->PR;
    int originalPR_B = match->teamB->PR;

    if (isHomeGameForA) {
        match->teamA->PR += 50;
    } else {
        match->teamB->PR += 50;
    }

    // Simulate the game
    simulateMatch(match);

    // Restore original PR (the match already updated them, so adjust back)
    if (isHomeGameForA) {
        match->teamA->PR -= 50;
    } else {
        match->teamB->PR -= 50;
    }
}

// Simulate best-of-7 series
static Team* simulateSeries(Team *teamA, Team *teamB, int stage, int verbose) {
    int winsA = 0, winsB = 0;
    int gameNum = 1;

    if (verbose) {
        printf("\n");
        printf(COLOR_PLAYOFFS "========================================\n");
        printf("  SERIES: %s vs %s\n", teamA->name, teamB->name);
        printf("========================================\n" COLOR_RESET);
    }

    // 2-2-1-1-1 format: Games 1,2 at higher seed, 3,4 at lower seed, then alternating
    // teamA is higher seed (home court advantage)
    int homeTeam[7] = {1, 1, 0, 0, 1, 0, 1}; // 1 = teamA home, 0 = teamB home

    while (winsA < 4 && winsB < 4) {
        Match match;
        initializeMatch(&match, teamA, teamB, stage);

        int isHomeForA = homeTeam[gameNum - 1];
        simulatePlayoffGame(&match, isHomeForA);

        if (match.scoreA > match.scoreB) {
            winsA++;
        } else {
            winsB++;
        }

        if (verbose) {
            printf("\n  Game %d: ", gameNum);
            if (isHomeForA) {
                printf("@ %s\n", teamA->name);
            } else {
                printf("@ %s\n", teamB->name);
            }
            printf("  %s %d - %d %s\n",
                   teamA->name, match.scoreA, match.scoreB, teamB->name);
            printf("  Series: %s leads %d-%d\n",
                   (winsA > winsB) ? teamA->name : teamB->name,
                   (winsA > winsB) ? winsA : winsB,
                   (winsA > winsB) ? winsB : winsA);
        }

        gameNum++;
        sleep_ms(200);
    }

    Team *winner = (winsA == 4) ? teamA : teamB;

    if (verbose) {
        printf("\n");
        printf(COLOR_SUCCESS "  SERIES WINNER: %s (%d-%d)\n" COLOR_RESET,
               winner->name, (winner == teamA) ? winsA : winsB,
               (winner == teamA) ? winsB : winsA);
        printf(COLOR_PLAYOFFS "========================================\n" COLOR_RESET);
        sleep_ms(500);
    }

    return winner;
}

// Simulate one conference playoff bracket
static Team* simulateConferenceBracket(Team *teams[8], int conference, int verbose) {
    const char *confName = (conference == 0) ? "EASTERN" : "WESTERN";
    const char *confColor = (conference == 0) ? COLOR_EAST : COLOR_WEST;

    if (verbose) {
        printf("\n\n");
        printf("%s================================================\n", confColor);
        printf("       %s CONFERENCE PLAYOFFS - ROUND 1\n", confName);
        printf("================================================\n" COLOR_RESET);
    }

    // First Round (8 teams -> 4 teams)
    Team *round2[4];
    round2[0] = simulateSeries(teams[0], teams[7], 1, verbose); // 1 vs 8
    round2[1] = simulateSeries(teams[3], teams[4], 1, verbose); // 4 vs 5
    round2[2] = simulateSeries(teams[1], teams[6], 1, verbose); // 2 vs 7
    round2[3] = simulateSeries(teams[2], teams[5], 1, verbose); // 3 vs 6

    if (verbose) {
        printf("\n\n");
        printf("%s================================================\n", confColor);
        printf("       %s CONFERENCE SEMIFINALS\n", confName);
        printf("================================================\n" COLOR_RESET);
    }

    // Conference Semifinals (4 teams -> 2 teams)
    Team *round3[2];
    round3[0] = simulateSeries(round2[0], round2[1], 1, verbose);
    round3[1] = simulateSeries(round2[2], round2[3], 1, verbose);

    if (verbose) {
        printf("\n\n");
        printf("%s================================================\n", confColor);
        printf("       %s CONFERENCE FINALS\n", confName);
        printf("================================================\n" COLOR_RESET);
    }

    // Conference Finals (2 teams -> 1 team)
    Team *champion = simulateSeries(round3[0], round3[1], 1, verbose);

    if (verbose) {
        printf("\n\n");
        printf("%s***********************************************\n", COLOR_SUCCESS);
        printf("   %s CONFERENCE CHAMPION: %s\n", confName, champion->name);
        printf("***********************************************\n" COLOR_RESET);
        sleep_ms(1000);
    }

    return champion;
}

// Main playoffs simulation
void simulatePlayoffs(Simulation *sim) {
    printf("\n\n");
    printf(COLOR_HEADER "================================================\n");
    printf("            NBA PLAYOFFS BEGIN!\n");
    printf("================================================\n" COLOR_RESET);
    sleep_ms(1000);

    // Get playoff teams
    Team *eastTeams[8];
    Team *westTeams[8];

    getConferencePlayoffTeams(sim, 0, eastTeams);
    getConferencePlayoffTeams(sim, 1, westTeams);

    // Print playoff brackets
    printf("\n");
    printf(COLOR_EAST "EASTERN CONFERENCE BRACKET:\n" COLOR_RESET);
    for (int i = 0; i < 4; i++) {
        printf("  (%d) %s  vs  (%d) %s\n",
               i+1, eastTeams[i]->name, 8-i, eastTeams[7-i]->name);
    }

    printf("\n");
    printf(COLOR_WEST "WESTERN CONFERENCE BRACKET:\n" COLOR_RESET);
    for (int i = 0; i < 4; i++) {
        printf("  (%d) %s  vs  (%d) %s\n",
               i+1, westTeams[i]->name, 8-i, westTeams[7-i]->name);
    }

    pressEnterToContinue();

    // Simulate both conferences
    Team *eastChamp = simulateConferenceBracket(eastTeams, 0, 1);
    Team *westChamp = simulateConferenceBracket(westTeams, 1, 1);

    // NBA Finals
    printf("\n\n");
    printf(COLOR_FINALS "***********************************************\n");
    printf("                NBA FINALS\n");
    printf("***********************************************\n" COLOR_RESET);
    printf("\n");
    printf("  %s%s%s\n", COLOR_EAST, eastChamp->name, COLOR_RESET);
    printf("           vs\n");
    printf("  %s%s%s\n", COLOR_WEST, westChamp->name, COLOR_RESET);
    printf("\n");

    printf("Press Enter to start Finals...");
    while (getchar() != '\n');

    Team *nbaChampion = simulateSeries(eastChamp, westChamp, 2, 1);

    // Championship celebration
    printf("\n\n");
    printf(COLOR_SUCCESS "***********************************************\n");
    printf("***********************************************\n");
    printf("                                               \n");
    printf("       NBA CHAMPION: %s!\n", nbaChampion->name);
    printf("                                               \n");
    printf("***********************************************\n");
    printf("***********************************************\n" COLOR_RESET);

    sim->champion = nbaChampion;
    sleep_ms(2000);
}

// Print comprehensive season summary
void printSeasonSummary(Simulation *sim) {
    printf("\n\n");
    printf(COLOR_HEADER "========================================================\n");
    printf("                  SEASON SUMMARY\n");
    printf("========================================================\n" COLOR_RESET);

    // Champion
    printf("\n");
    printf(COLOR_SUCCESS "  NBA CHAMPION: %s\n" COLOR_RESET, sim->champion->name);
    printf("  Final Record: %d-%d\n", sim->champion->wins, sim->champion->losses);
    printf("  Power Ranking: %d\n", sim->champion->PR);
    printf("\n");

    // Find MVP (highest PPG among all players)
    Player *mvp = NULL;
    double maxPPG = 0.0;
    Team *mvpTeam = NULL;

    for (int i = 0; i < TEAMS_COUNT; i++) {
        for (int j = 0; j < MAX_PLAYERS; j++) {
            Player *p = &sim->teams[i].roster[j];
            if (p->gamesPlayed > 0) {
                double ppg = (double)p->points / p->gamesPlayed;
                if (ppg > maxPPG) {
                    maxPPG = ppg;
                    mvp = p;
                    mvpTeam = &sim->teams[i];
                }
            }
        }
    }

    if (mvp) {
        printf(COLOR_SUCCESS "  SEASON MVP: %s (%s)\n" COLOR_RESET, mvp->name, mvpTeam->name);
        printf("  PPG: %.1f | Total Points: %d | Games: %d\n",
               maxPPG, mvp->points, mvp->gamesPlayed);
        printf("\n");
    }

    // Best regular season record
    Team bestTeam = sim->teams[0];
    for (int i = 1; i < TEAMS_COUNT; i++) {
        if (sim->teams[i].wins > bestTeam.wins) {
            bestTeam = sim->teams[i];
        }
    }

    printf("  BEST REGULAR SEASON: %s (%d-%d)\n",
           bestTeam.name, bestTeam.wins, bestTeam.losses);
    printf("\n");

    // Top 5 scorers
    typedef struct {
        Player *player;
        Team *team;
        double ppg;
    } ScorerInfo;

    ScorerInfo scorers[TEAMS_COUNT * MAX_PLAYERS];
    int scorerCount = 0;

    for (int i = 0; i < TEAMS_COUNT; i++) {
        for (int j = 0; j < MAX_PLAYERS; j++) {
            Player *p = &sim->teams[i].roster[j];
            if (p->gamesPlayed > 0) {
                scorers[scorerCount].player = p;
                scorers[scorerCount].team = &sim->teams[i];
                scorers[scorerCount].ppg = (double)p->points / p->gamesPlayed;
                scorerCount++;
            }
        }
    }

    // Sort scorers by PPG
    for (int i = 0; i < scorerCount - 1; i++) {
        for (int j = i + 1; j < scorerCount; j++) {
            if (scorers[j].ppg > scorers[i].ppg) {
                ScorerInfo tmp = scorers[i];
                scorers[i] = scorers[j];
                scorers[j] = tmp;
            }
        }
    }

    printf(COLOR_HEADER "  TOP 5 SCORERS:\n" COLOR_RESET);
    for (int i = 0; i < 5 && i < scorerCount; i++) {
        printf("  %d. %s (%s) - %.1f PPG\n",
               i + 1,
               scorers[i].player->name,
               scorers[i].team->name,
               scorers[i].ppg);
    }

    printf("\n");
    printf(COLOR_HEADER "========================================================\n" COLOR_RESET);
}