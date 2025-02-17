#include "../include/newgame.h"

void handleNewGameEvent(Window * win, SDL_Event * event){
    handleEvent(win, event);
    if (!game.gameState.initialized) {
        initData();
        initTeam(&game.battleState.rouge, 3);
        initTeam(&game.battleState.bleu, 3);
        game.battleState.ia = (t_AI){10, damageOnly, &game.battleState.bleu};
        for(int i = 0; i < game.battleState.rouge.nb_poke; i++){
            printPoke(&(game.battleState.rouge.team[i]));
        }
        printf("pv rouge : %d\n\n",game.battleState.rouge.team[0].current_pv);
        printf("pv bleu : %d\n\n",game.battleState.bleu.team[0].current_pv);
        updateICButtons(win, &game.battleState.rouge);
        bleu = game.battleState.bleu;
        game.gameState.initialized = 1;
    }
}