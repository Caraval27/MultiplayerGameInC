#include "../include/main.h"

#define nrOfKeybinds 3

// These functions will use a .txt file to either read or write the keybinds from the user

void saveToFile(FILE *fp, int keybinds[]){
    fp = fopen("keybinds.txt", "w");
    for (int i = 0; i < nrOfKeybinds; i++)
    {
        fprintf(fp, "%d\n", keybinds[i]);
    }
    fclose(fp);
}

void readFromFile(FILE *fp, int keybinds[]){
    fp = fopen("keybinds.txt", "r");
    if (fp != NULL)
    {
        for (int i = 0; i < nrOfKeybinds; i++)
        {
            fscanf(fp, "%d", &keybinds[i]);
        }
        fclose(fp);
    } else{                 // Mostly occurs if the file was manually removed by the user, in which case the default keybinds will be sent instead
        keybinds[0] = 7;    // D key; move right
        keybinds[1] = 4;    // A key; move left
        keybinds[2] = 44;   // Spacebar; jump
    }
}
