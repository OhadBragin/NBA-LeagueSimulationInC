#include "basketball_sim.h"

void clearInputBuffer(void) {
    // Clear stdin buffer for clean input
    // TODO: Implement input buffer clearing
    #ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // macOS / Linux
    #endif
}

int getRandomNumber(int min, int max) {
    // Generate random number in given range
    // Use for game simulation randomness
    // TODO: Implement random number generation
    int rd_num = rand() % (max - min + 1) + min;
    return rd_num;

}


int getRandomResult(double p){
    // Will be used to determine result of game
    // Return 1 or 0 based on the percentage
    // higher percentage will more likely return 1

    if (p < 0.0) p = 0.0;
    if (p > 1.0) p = 1.0;
    double r = (double)rand() / RAND_MAX; // random in [0.0, 1.0]
    return r < p;
}


int getUserChoice(int min, int max) {
    // Get valid user input within range
    // Handle invalid input gracefully
    // TODO: Implement user input validation
    int input;
    do {
        scanf("%d", &input);
    }while (!(input >= min && input <= max));
    return input;

}