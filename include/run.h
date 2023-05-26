#pragma once

#include "../include/main.h"

void handleMainMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, SDL_Event event, State* pState, bool* pMute);
void renderMainMenu(GameDisplay* pGameDisplay, Buttons* pButtons);

void handleSettingsMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState);
void renderSettingsMenu(GameDisplay* pGameDisplay, Buttons* pButtons);

void handleLanguageMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event);
void renderLanguageMenu(GameDisplay* pGameDisplay, Buttons* pButtons);
void readKeybindString(Language* pLanguage, int index, GameDisplay* pGameDisplay, Buttons* pButtons);
void handleEnterInput(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, SDL_Event event, State* pState);

void handleLobbyMenu(GameDisplay* pGameDisplay, PlayersData* pPlayersData, NetworkData* pNetworkData, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState, LobbyConnect* pLobbyConnect, Platform** pPlatforms, Platform* pStartPlatform, int* pTime);
void renderLobbyMenu(GameDisplay* pGameDisplay, Buttons* pButtons);
void handleLobby(GameDisplay* pGameDisplay, NetworkData* pNetworkData, GameplayData* pGameplayData, ClientCommand* pClientCommands, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState, LobbyConnect* pLobbyConnect, Platform** pPlatforms);
void fillZero(char inputIP[], int max);

void handleOngoing(GameDisplay* pGameDisplay, PlayersData* pPlayersData, NetworkData* pNetworkData, GameplayData* pGameplayData, ClientCommand* pClientCommands, DisplayText* pDisplayText, Language* pLanguage, Sound* p, SDL_Event event, State* pState, Background* pBackground, Platform** pPlatforms, Platform* pStartPlatform, bool* pIsRunning, int *pTime, bool* pMute);
void handleOngoingInput(PlayersData* pPlayersData, NetworkData* pNetworkData, ClientCommand* pClientCommands, Language* pLanguage, SDL_Event* event, State* pState, bool* pIsRunning, bool* pMute);

void handleGameMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Event event, State* pState, bool* pMute);
void renderGameMenu(GameDisplay* pGameDisplay, Buttons* pButtons);

void handleGameOver(PlayersData* pPlayersData, GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButton, DisplayText* pDisplayText, SDL_Event event, State* pState);
void renderGameOver(GameDisplay* pGameDisplay, Buttons* pButtons, Text* pWhoWonText);

void resetGame(PlayersData* pPlayersData, GameDisplay* pGameDisplay, Platform** pPlatforms, Platform* pStartPlatform, int *pTime);