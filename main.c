#include "basketball_sim.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main() {

    //initialize random seed
    srand(time(NULL));
    //clone players file
    cloneFile("nba_players.txt", "cloned.txt");

    //initializations

    printf("Basketball Simulation Starting...\n");

    // Initialize simulation
    Simulation sim;
    initializeSimulation(&sim);

    // Main game loop
    runSimulation(&sim);

    // Cleanup
    cleanupSimulation(&sim);

    printf("Basketball Simulation Ended.\n");
    //delete the cloned file
    remove("cloned.txt");
    return 0;
}