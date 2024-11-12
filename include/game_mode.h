#ifndef DREV_GAME_MODE_H
#define DREV_GAME_MODE_H

enum GameMode {
    GameModeMenu = 0,
    GameModePlay,
    GameModePause,
    GameModeCount,
};

const char *GameModeLabels[GameModeCount] = {
    [GameModeMenu]  = "GameModeMenu",
    [GameModePlay]  = "GameModePlay",
    [GameModePause] = "GameModePause",
};

#endif // DREV_GAME_MODE_H
