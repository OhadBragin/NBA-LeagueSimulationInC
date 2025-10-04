#include "basketball_sim.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main() {
    // initialize random seed
    srand(time(NULL));

    // enable ANSI colors
    enableColors();

    printf(COLOR_HEADER "Welcome to the NBA Simulation!\n" COLOR_RESET);

    Simulation sim;
    memset(&sim, 0, sizeof(Simulation));
    int running = 1;

    // Main menu loop
    while (running) {
        HandleMainMenu(&sim, &running);
    }

    printf(COLOR_INFO "Thanks for playing! Exiting program...\n" COLOR_RESET);
    return 0;
}
