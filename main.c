#include "basketball_sim.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
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