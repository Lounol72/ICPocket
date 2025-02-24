#ifndef UTILS_H
#define UTILS_H

typedef enum AppState {
    QUIT = 0, // 0
    SETTINGS, // 1
    MENU, // 2
    GAME,  // 3
    NEWGAME,  // 4
    LOADGAME, // 5
    ICMONS, // 6
    INTER, // 7
    PAUSE, // 8
} AppState;

#endif /* UTILS_H */