#pragma once

#define NR_OF_KEYBINDS 3
#define NR_OF_WORDS 10
#define LANG_LENGTH 20

void saveToFile(FILE *fp, int keybinds[NR_OF_KEYBINDS]);
void readFromFileKey(FILE *fp, int keybinds[NR_OF_KEYBINDS]);
void readFromFileLang(FILE *fp, char language[NR_OF_WORDS][50]);
void changeLanguageInFile(FILE *fp, char language[LANG_LENGTH]);
