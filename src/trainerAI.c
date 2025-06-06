#include "../include/trainerAI.h"


t_AI iaTest={10,damageOnly,&bleu};
t_AI iaTest2={10,damageOnly,&rouge};

/**
 * @fn void insertionSort(int tabDegats[], int tabMove[], int n)
 * @brief Trie les attaques par ordre décroissant de dégâts
 * 
 * Cette fonction trie les attaques d'un Pokémon par ordre décroissant de dégâts
 * 
 * @param tabDegats Tableau contenant les dégâts des attaques
 * @param tabMove Tableau contenant les indices des attaques
 * @param n Taille des tableaux
 */
void insertionSort(int tabDegats[], int tabMove[], int n) {
    int i, keyDeg, keyMove, j;
    for (i = 1; i < n; i++) {
        keyDeg = tabDegats[i];
		keyMove = tabMove[i];
        j = i - 1;

        // Move elements of arr[0..i-1], that are greater than key, to one position ahead of their current position
        while (j >= 0 && tabDegats[j] < keyDeg) {
            tabDegats[j + 1] = tabDegats[j];
			tabMove[j + 1] = tabMove[j];
            j = j - 1;
        }
        tabDegats[j + 1] = keyDeg;
		tabMove[j + 1] = keyMove;
    }
}

/**
 * @fn int AI_move_choice(t_AI * ai_team, t_Team * player_team)
 * @brief Choisit le mouvement de l'IA
 *
 * Cette fonction choisit le mouvement de l'IA en fonction de son type et de l'équipe adverse.
 * 
 * @param ai_team Pointeur vers la structure de l'IA
 * @param player_team Pointeur vers la structure de l'équipe du joueur
 * @return int Indice du mouvement choisi
 */
int AI_move_choice(t_AI *ai, t_Team *opponent) {
    int tab_damage[4] = { -1, -1, -1, -1 };
    int tab_move[4]   = { -1, -1, -1, -1 };  // Initialisés à -1 pour éviter des choix invalides
    int nb_valid_moves = 0;

    // Pour le débogage, affichez le nombre d'attaques du Pokémon actif
    int nb_moves = ai->AI_t_Team->team[0].nb_move;
    //printf("Nombre d'attaques disponibles (nb_move) : %d\n", nb_moves);

    // Remplissage des tableaux avec les attaques dont il reste des PP
    for (int i = 0; i < nb_moves; i++) {
        if (ai->AI_t_Team->team[0].moveList[i].current_pp > 0) {
            tab_damage[nb_valid_moves] = calcDamage(ai->AI_t_Team, opponent, &(ai->AI_t_Team->team[0].moveList[i]));
            tab_move[nb_valid_moves] = i;
            nb_valid_moves++;
        }
    }

    if (nb_valid_moves == 0) {
        //printf("Aucune attaque valide\n");
        return 0;  // Optionnel : retourner STRUGGLE ou un autre code d'action si nécessaire
    }

    // Tri par ordre décroissant des dégâts estimés
    insertionSort(tab_damage, tab_move, nb_valid_moves);

    int chosenIndex = 0;
    // Choix de l'attaque selon le type d'IA
    if (ai->type % 2 == 0) { // status_first AI
        int has_move_with_effects=0;
        for(int i=0;i<nb_moves;i++){
            /*try to do an main effect applier attack*/
            if (SecEffectTab[ai->AI_t_Team->team[0].moveList[tab_move[i]].ind_secEffect]==applyEffect 
                && ai->AI_t_Team->team[0].moveList[tab_move[i]].power==0 
                && opponent->team[0].main_effect==noEffect 
                && ai->AI_t_Team->team[0].moveList[tab_move[i]].value_effect<4){
                if (ai->AI_lvl >= rand() % 21){
                    chosenIndex=i;
                    has_move_with_effects=1;
                    break;
                }
            }
            /*try to do a second effect applier attack*/
            if (SecEffectTab[ai->AI_t_Team->team[0].moveList[tab_move[i]].ind_secEffect]==applyEffect 
                && ai->AI_t_Team->team[0].moveList[tab_move[i]].power==0 
                && opponent->effect==noEffect 
                && ai->AI_t_Team->team[0].moveList[tab_move[i]].value_effect>3){
                if (ai->AI_lvl >= rand() % 21){
                    chosenIndex=i;
                    has_move_with_effects=1;
                    break;
                }
            }
        }
        if(!has_move_with_effects && ai->type!=boss)
            chosenIndex = (rand() % ai->AI_lvl) % nb_valid_moves;
    }
    else if (ai->type % 3 == 0) { // damage_first AI
        while (chosenIndex < nb_valid_moves - 1 && (rand() % 20) > ai->AI_lvl)
            chosenIndex++;
    }
    else { // none type AI
        chosenIndex = (rand() % ai->AI_lvl) % nb_valid_moves;
    }

    // Affichage pour le débogage (en 0-indexé)
    /*printf("Tab damage après tri : ");
    for (int i = 0; i < nb_valid_moves; i++) {
        printf("%d ", tab_damage[i]);
    }
    printf("\n");

    printf("Tab move après tri : ");
    for (int i = 0; i < nb_valid_moves; i++) {
        printf("%d ", tab_move[i]);
    }
    printf("\n");

    printf("Move choisi (index 0-based) : %d\n", tab_move[chosenIndex]);
    */

    return tab_move[chosenIndex];
}