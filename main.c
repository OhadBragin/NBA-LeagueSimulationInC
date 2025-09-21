#include "basketball_sim.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main() {

    //initialize random seed
    srand(time(NULL));

    printf("Basketball Simulation Starting...\n");

    // Initialize simulation
    Simulation sim;
    initializeSimulation(&sim);

    // Main game loop
    runSimulation(&sim);

    // Cleanup
    cleanupSimulation(&sim);

    printf("Basketball Simulation Ended.\n");
    return 0;
}