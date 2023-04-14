#ifndef KEYBINDS_H
#define KEYBINDS_H

#define NR_OF_KEYBINDS 3

void saveToFile(FILE *fp, int keybinds[]);
void readFromFile(FILE *fp, int keybinds[]);

#endif
