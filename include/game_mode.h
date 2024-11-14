#ifndef DREV_GAME_MODE_H
#define DREV_GAME_MODE_H

enum GameMode {
    GameModeMenu = 0,
    GameModePlay,
    GameModePause,
    GameModeGameOver,
    GameModeCount,
};

const char *GameModeLabels[GameModeCount] = {
    [GameModeMenu]     = "GameModeMenu",
    [GameModePlay]     = "GameModePlay",
    [GameModePause]    = "GameModePause",
    [GameModeGameOver] = "GameModeGameOver",
};

#endif // DREV_GAME_MODE_H
