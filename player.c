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

        if (i != j) {
            //swap names[i], names[j]
            char temp[MAX_NAME_LENGTH];
            strcpy(temp, names[i]);
            strcpy(names[i], names[j]);
            strcpy(names[j], temp);
        }

    }
}

//load players from file to array
//return the count of players loaded

int loadPlayerNames(char names[][MAX_NAME_LENGTH], const char *filename) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error opening %s file!\n", filename);
        exit(1);
    }

    char buffer[MAX_NAME_LENGTH];
    int count = 0;

    while (fgets(buffer, MAX_NAME_LENGTH, fptr) && count < TOTAL_PLAYER_COUNT) {
        // Remove newline character if present
        buffer[strcspn(buffer, "\n")] = '\0';
        buffer[strcspn(buffer, "\r")] = '\0';  // Also remove carriage return

        // Only add non-empty names
        if (strlen(buffer) > 0) {
            strcpy(names[count], buffer);
            names[count][MAX_NAME_LENGTH - 1] = '\0';  // Ensure null termination
            count++;
        }
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

void printRoster(const Player *roster) {
    printf("\n");
    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);
    printf("%s%s            ROSTER%s\n", COLOR_BOLD, COLOR_HEADER, COLOR_RESET);
    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);

    // Find top scorer (by PPG)
    int topIndex = findHighestPPG(roster);

    // Print each player
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (roster[i].name[0] == '\0') continue;

        double ppg = roster[i].gamesPlayed > 0
                     ? (double)roster[i].points / roster[i].gamesPlayed
                     : 0.0;

        // Top scorer in green, others in white
        const char *nameColor = (i == topIndex) ? COLOR_SUCCESS : COLOR_PLAYER;

        printf(" %s%2d.%s %s%-20s%s  %sPoints:%s %s%4d%s  %sGames:%s %s%3d%s  %sPPG:%s %s%4.1f%s\n",
               COLOR_NUMBER, i + 1, COLOR_RESET,       // player index
               nameColor, roster[i].name, COLOR_RESET, // player name
               COLOR_STAT_LABEL, COLOR_RESET, COLOR_NUMBER, roster[i].points, COLOR_RESET,  // points
               COLOR_STAT_LABEL, COLOR_RESET, COLOR_NUMBER, roster[i].gamesPlayed, COLOR_RESET, // games
               COLOR_STAT_LABEL, COLOR_RESET, COLOR_NUMBER, ppg, COLOR_RESET); // ppg
    }

    printf("%s=====================================%s\n", COLOR_BORDER, COLOR_RESET);
    printf("\n");
}



void printPlayerStats(const Player *player) {
    // Print player name and statistics
    // Show points per game average
    // TODO: Implement player stats display
}