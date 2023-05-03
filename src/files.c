#include "../include/main.h"

// These functions will use a .txt file to either read or write the keybinds from the user

void saveToFile(FILE *fp, int keybinds[]){
    fp = fopen("../assets/textfiles/keybinds.txt", "w");
    for (int i = 0; i < NR_OF_KEYBINDS; i++){
        fprintf(fp, "%d\n", keybinds[i]);
    }
    fclose(fp);
}

void readFromFileKey(FILE *fp, int keybinds[]){
    fp = fopen("../assets/textfiles/keybinds.txt", "r");
    if (fp != NULL){
        for (int i = 0; i < NR_OF_KEYBINDS; i++){
            fscanf(fp, "%d", &keybinds[i]);
        }
        fclose(fp);
    } else{                 // Mostly occurs if the file was manually removed by the user, in which case the default keybinds will be sent instead
        keybinds[0] = 100;    // D key; move right
        keybinds[1] = 97;    // A key; move left
        keybinds[2] = 44;   // Spacebar; jump
    }
}

void readFromFileLang(FILE *fp, char language[NR_OF_WORDS][50]){
    char readLang[LANG_LENGTH];
    char target[30] = "../assets/textfiles/";
    fp = fopen("../assets/textfiles/settings.txt", "r");
    if (fp != NULL){
        fscanf(fp, "%[^\n]%*c", readLang);
        strcat(target, readLang);
    } else{
        fp = fopen("../assets/textfiles/settings.txt", "w");
        fprintf(fp, "english.txt");
    }
    fclose(fp);
    fp = fopen(target, "r");
    if (fp != NULL){
        for (int i = 0; i < NR_OF_WORDS; i++){
            fscanf(fp, "%[^\n]%*c", language[i]);
        }
        fclose(fp);
    } else{
        fp = fopen(target, "w");
        fprintf(fp, "Start game\nSettings\nQuit\nResume game\nMain menu\nLanguage\nReturn\nMove right:\nMove left:\nYou are Dead\nCreate Lobby\nJoin Lobby\nPress key");
        fclose(fp);

        fp = fopen(target, "r");
        for (int i = 0; i < NR_OF_WORDS; i++){
            fscanf(fp, "%[^\n]%*c", language[i]);
        }
        fclose(fp);
    }
}

void readFromFileLangMAC( bool chosenLanguage, char language[NR_OF_WORDS][50]){
    if (chosenLanguage){
        strcpy(language[0], "Start game");
        strcpy(language[1], "Settings");
        strcpy(language[2], "Quit");
        strcpy(language[3], "Resume game");
        strcpy(language[4], "Main menu");
        strcpy(language[5], "Language");
        strcpy(language[6], "Return");
        strcpy(language[7], "Move right:");
        strcpy(language[8], "Move left:");
        strcpy(language[9], "You are dead");
        strcpy(language[10], "Create lobby");
        strcpy(language[11], "Join lobby");
    } else{
        strcpy(language[0], "Starta spelet");
        strcpy(language[1], "Inst�llningar");
        strcpy(language[2], "Avsluta");
        strcpy(language[3], "�teruppta spelet");
        strcpy(language[4], "Huvudmeny");
        strcpy(language[5], "Spr�k");
        strcpy(language[6], "�terv�nda");
        strcpy(language[7], "Flytta h�ger:");
        strcpy(language[8], "Flytta v�nster:");
        strcpy(language[9], "Du �r d�d");
        strcpy(language[10], "Skapa lobby");
        strcpy(language[11], "Ansluta till lobby");
    }
}

void changeLanguageInFile(FILE *fp, char language[LANG_LENGTH]){
    fp = fopen("../assets/textfiles/settings.txt", "w");
    fprintf(fp, "%s", language);
    fclose(fp);
}
