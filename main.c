#include "basketball_sim.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main() {

    //initialize random seed
    srand(time(NULL));

    //initializations
    char names[TOTAL_PLAYER_COUNT][MAX_NAME_LENGTH];
    int playerCount;

    printf("Basketball Simulation Starting...\n");

    playerCount = loadPlayerNames(names, "nba_players.txt");
    shuffleNameArray(names, TOTAL_PLAYER_COUNT);
    Team *team = initializeTeam("The Bragins", 0, names, &playerCount );
    printTeamInfo(team);
    // Initialize simulation
    //Simulation sim;
    //initializeSimulation(&sim);

    // Main game loop
    //runSimulation(&sim);

    // Cleanup
    //cleanupSimulation(&sim);

    printf("Basketball Simulation Ended.\n");
    //delete the cloned file
    return 0;
}