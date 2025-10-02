#include "basketball_sim.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Initialize a player
void initializePlayer(Player *player, const char *name) {
    strncpy(player->name, name, MAX_NAME_LENGTH);
    player->name[MAX_NAME_LENGTH - 1] = '\0';
    player->points = 0;
    player->gamesPlayed = 0;
}

// Update player stats
void updatePlayerStats(Player *player, int points) {
    player->points += points;
    player->gamesPlayed++;
}

// Print stats for one player
void printPlayerStats(const Player *player) {
    double ppg = (player->gamesPlayed > 0)
                   ? (double)player->points / player->gamesPlayed
                   : 0.0;

    printf("%s%-20s%s | %sPoints:%s %d | %sPPG:%s %.1f\n",
           COLOR_PLAYER, player->name, COLOR_RESET,
           COLOR_STAT_LABEL, COLOR_RESET, player->points,
           COLOR_STAT_LABEL, COLOR_RESET, ppg);
}

// Helper: find the player with the highest PPG
int findHighestPPG(Player roster[MAX_PLAYERS]) {
    int bestIndex = 0;
    double bestPPG = -1.0;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        double ppg = (roster[i].gamesPlayed > 0)
                       ? (double)roster[i].points / roster[i].gamesPlayed
                       : 0.0;
        if (ppg > bestPPG) {
            bestPPG = ppg;
            bestIndex = i;
        }
    }
    return bestIndex;
}

// Print roster
void printRoster(const Player *roster) {
    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);
    printf("%s%s            ROSTER%s\n", COLOR_BOLD, COLOR_HEADER, COLOR_RESET);
    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);

    int topIndex = findHighestPPG((Player *)roster);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        const char *nameColor = (i == topIndex) ? COLOR_SUCCESS : COLOR_PLAYER;
        double ppg = (roster[i].gamesPlayed > 0)
                       ? (double)roster[i].points / roster[i].gamesPlayed
                       : 0.0;

        printf("%s#%d%s %s%-20s%s | %sPoints:%s %s%d%s | %sPPG:%s %.1f\n",
               COLOR_NUMBER, i + 1, COLOR_RESET,
               nameColor, roster[i].name, COLOR_RESET,
               COLOR_STAT_LABEL, COLOR_RESET,
               COLOR_NUMBER, roster[i].points, COLOR_RESET,
               COLOR_STAT_LABEL, COLOR_RESET, ppg);
    }
}

// Shuffle players
void shuffleNameArray(char names[][MAX_NAME_LENGTH], int count) {
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char tmp[MAX_NAME_LENGTH];
        strcpy(tmp, names[i]);
        strcpy(names[i], names[j]);
        strcpy(names[j], tmp);
    }
}

// Load names from file
int loadPlayerNames(char names[][MAX_NAME_LENGTH], const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        perror("Error opening player names file");
        return 0;
    }

    int count = 0;
    while (count < TOTAL_PLAYER_COUNT && fgets(names[count], MAX_NAME_LENGTH, file)) {
        names[count][strcspn(names[count], "\n")] = '\0';
        count++;
    }

    fclose(file);
    return count;
}

// Create a roster for a team
void createRoster(Player *roster, char names[][MAX_NAME_LENGTH], int *playerCount) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (*playerCount <= 0) break;
        initializePlayer(&roster[i], names[--(*playerCount)]);
    }
}

// Free roster (placeholder)
void freeRoster(Player *roster) {
    (void)roster; // no dynamic allocation inside Player
}
