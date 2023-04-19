#include "../include/main.h"

// These functions will use a .txt file to either read or write the keybinds from the user

void saveToFile(FILE *fp, int keybinds[]){
    fp = fopen("keybinds.txt", "w");
    for (int i = 0; i < NR_OF_KEYBINDS; i++){
        fprintf(fp, "%d\n", keybinds[i]);
    }
    fclose(fp);
}

void readFromFileKey(FILE *fp, int keybinds[]){
    fp = fopen("keybinds.txt", "r");
    if (fp != NULL){
        for (int i = 0; i < NR_OF_KEYBINDS; i++){
            fscanf(fp, "%d", &keybinds[i]);
        }
        fclose(fp);
    } else{                 // Mostly occurs if the file was manually removed by the user, in which case the default keybinds will be sent instead
        keybinds[0] = 7;    // D key; move right
        keybinds[1] = 4;    // A key; move left
        keybinds[2] = 44;   // Spacebar; jump
    }
}

void readFromFileLang(FILE *fp, char language[NR_OF_WORDS][50]){
    char readLang[LANG_LENGTH];
    fp = fopen("settings.txt", "r");
    if (fp != NULL){
        fscanf(fp, "%[^\n]%*c", readLang);
    } else{                 
        fp = fopen("settings.txt", "w");
        fprintf(fp, "english.txt");
    }
    fclose(fp);
    fp = fopen(readLang, "r");
    if (fp != NULL){
        for (int i = 0; i < NR_OF_WORDS; i++){
            fscanf(fp, "%[^\n]%*c", language[i]);
        }
        fclose(fp);
    } else{                 
        //Create new English lang file
    }
}

void changeLanguageInFile(FILE *fp, char language[LANG_LENGTH]){
    fp = fopen("settings.txt", "w");
    fprintf(fp, "%s", language);
    fclose(fp);
}