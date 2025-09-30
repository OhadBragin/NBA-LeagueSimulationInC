#include "basketball_sim.h"

void initializePlayer(Player *player, const char *name) {
    // Initialize player with name
    // Set stats to zero
    // TODO: Implement player initialization

    strcpy(player->name, name);
    player->gamesPlayed = 0;
    player->points = 0;
}

void updatePlayerStats(Player *player, int points) {
    // Update player's season statistics
    // Increment games played
    // Add points to total
    // TODO: Implement player stats update
    player->points += points;
    player->gamesPlayed++;
}

void shuffleNameArray(char names[][MAX_NAME_LENGTH], int count) {
    for (int i = count - 1; i > 0; i--) {
        int j = getRandomNumber(0, i);

        //swap names[i], names[j]
        char temp[MAX_NAME_LENGTH];
        strcpy(temp, names[i]);
        strcpy(names[i], names[j]);
        strcpy(names[j], temp);
    }
}

//load players from file to array
//return the count of players loaded

int loadlayerNames(char names[][MAX_NAME_LENGTH], const char *fileName) {
    //open file
    FILE *fptr = fopen(fileName, "r");
    if (fptr == NULL) {
        printf("Error opening file %s\n", fileName);
        exit(1);
    }

    char buffer[MAX_NAME_LENGTH];
    int count = 0;

    while (fgets(buffer, MAX_NAME_LENGTH, fptr) && count < TOTAL_PLAYER_COUNT) {
        //copy all names to array
        strcpy(names[count], buffer);
        count++;
    }

    fclose(fptr);
    return count;
}

// Create a roster by picking players from the name pool
// The namePool and playerCount are modified (players removed)
void createRoster(Player roster[], char names[][MAX_NAME_LENGTH], int *playerCount) {
    //check if there are enough players
    if (*playerCount < MAX_PLAYERS) {
        printf("Not Enough Players!");
        exit(1);
    }
    //create the roster

    if (roster == NULL) {
        printf("Error allocating memory for roster\n");
        exit(1);
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        (*playerCount)--;
        initializePlayer(&roster[i], names[*playerCount]);
    }
}

void freeRoster(Player *roster) {
    if (roster != NULL) {
        free(roster);
    }
}

void printPlayerStats(const Player *player) {
    // Print player name and statistics
    // Show points per game average
    // TODO: Implement player stats display
}