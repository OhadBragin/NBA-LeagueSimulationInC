#include "basketball_sim.h"
#include "colors.h"


void clearScreen() {
    // Clear stdin buffer for clean input
    // TODO: Implement input buffer clearing
    #ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // macOS / Linux
    #endif
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getRandomNumber(int min, int max) {
    // Generate random number in given range
    // Use for game simulation randomness
    // TODO: Implement random number generation
    int rd_num = rand() % (max - min + 1) + min;
    return rd_num;

}

void cloneFile(const char *src, const char *dest) {
    FILE *fptr = fopen(src, "r");
    FILE *cloned = fopen(dest, "w");
    if (fptr == NULL || cloned == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    char buffer[MAX_NAME_LENGTH];
    while (fgets(buffer, MAX_NAME_LENGTH, fptr)) {
        fputs(buffer, cloned);
    }

    fclose(fptr);
    fclose(cloned);
}
int calculateOutcomeByPercentage(double p){
    // Will be used to determine result of game
    // Return 1 or 0 based on the percentage
    // higher percentage will more likely return 1

    if (p < 0.0) p = 0.0;
    if (p > 1.0) p = 1.0;
    double r = (double)rand() / RAND_MAX; // random in [0.0, 1.0]
    return r < p;
}

double calculateProbability(int prA, int prB) {
    return 1.0 / (1.0 + pow(10, (double)(prB - prA) / 400.0));

}




int getUserChoice(int min, int max) {
    // Get valid user input within range
    // Handle invalid input gracefully
    // TODO: Implement user input validation
    int input;
    //validate input
    while (1) {
        scanf("%d", &input);
        if (input >= min && input <= max) {
            break;
        }
        printf(COLOR_ERROR "Invalid input. Please enter a number between %d and %d.\n", min, max);
        printf(COLOR_RESET);
    }
    return input;

}

void pressEnterToContinue(void) {
    printf("Press Enter to continue...");
    while (getchar() != '\n'); // wait for enter
}