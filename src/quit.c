#include "../include/main.h"

void quitText(Buttons* pButtons, DisplayText* pDisplayText) {
    destroyText(pDisplayText->pYouAreDeadText);
    destroyText(pButtons->pMainMenuButtonText);
    destroyText(pButtons->pResumeButtonText);
    destroyText(pButtons->pJoinLobbyButtonText);
    destroyText(pButtons->pCreateLobbyButtonText);
    destroyText(pButtons->pSwedishButtonText);
    destroyText(pButtons->pEnglishButtonText);
    destroyText(pButtons->pReturnButtonText);
    destroyText(pButtons->pMuteButton2Text);
    destroyText(pButtons->pMuteButton1Text);
    destroyText(pButtons->pMoveLeftButton2Text);
    destroyText(pButtons->pMoveLeftButton1Text);
    destroyText(pButtons->pMoveRightButton2Text);
    destroyText(pButtons->pMoveRightButton1Text);
    destroyText(pButtons->pLanguageButtonText);
    destroyText(pButtons->pQuitButtonText);
    destroyText(pButtons->pSettingsButtonText);
    destroyText(pButtons->pStartButtonText);
}

void quitButton(Buttons* pButtons) {
    destroyButton(pButtons->pMainMenuButton);
    destroyButton(pButtons->pResumeButton);
    destroyButton(pButtons->pJoinLobbyButton);
    destroyButton(pButtons->pCreateLobbyButton);
    destroyButton(pButtons->pSwedishButton);
    destroyButton(pButtons->pEnglishButton);
    destroyButton(pButtons->pReturnButton);
    destroyButton(pButtons->pMoveLeftButton);
    destroyButton(pButtons->pMoveRightButton);
    destroyButton(pButtons->pLanguageButton);
    destroyButton(pButtons->pQuitButton);
    destroyButton(pButtons->pSettingsButton);
    destroyButton(pButtons->pStartButton);
}

void quitTexture(GameDisplay* pGameDisplay, PlayersData* pPlayersData) {
    destroyPlayerTextures(pPlayersData->pPlayerTextures);
    destroyTexture(pGameDisplay->pStartPlatformTexture);
    destroyTexture(pGameDisplay->pPlatformTexture);
    destroyTexture(pGameDisplay->pBackgroundTexture);
    destroyTexture(pGameDisplay->pMenuTexture);
}