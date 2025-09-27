#include "basketball_sim.h"

void initializePlayer(Player *player, const char *name) {
    // Initialize player with name
    // Set stats to zero
    // TODO: Implement player initialization
    strcpy(player->name, name);
    player->points = 0;
    player->gamesPlayed = 0;
}

void updatePlayerStats(Player *player, int points) {
    // Update player's season statistics
    // Increment games played
    // Add points to total
    // TODO: Implement player stats update
    player->points += points;
    player->gamesPlayed++;
}

void printPlayerStats(const Player *player) {
    // Print player name and statistics
    // Show points per game average
    // TODO: Implement player stats display
}