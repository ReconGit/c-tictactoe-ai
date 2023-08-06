#include <stdio.h>

#include "../include/ui.h"

int main()
{
    char choice;
    do {
        printf("\nTic-Tac-Toe!\n");
        play_game();
        
        printf("Play again? (Y/N): ");
        int u = scanf(" %c", &choice);
        if (u == EOF) {
            printf("Invalid input!\n");
            return 1;
        }
        while (getchar() != '\n');  // clear stdin
    } while (choice == 'Y' || choice == 'y');

    return 0;
}
