#include "../include/GameEngine.h"

int test_game_1() {
    GameEngine game;
    game.init();
    game.run();
    game.cleanup();
    return 0;
}

int test_game_2() {
    
}
int main() {
    int result = test_game();
    printf("Test game 1 result: %d\n", result);
    return result;
}