#include "../include/trainerAI.h"


t_AI iaTest={10,damageOnly,&bleu};
t_AI iaTest2={10,damageOnly,&rouge};

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

int AI_move_choice(t_AI *ai, t_Team *opponent) {
    int tab_damage[4] = { -1, -1, -1, -1 };
    int tab_move[4]   = { -1, -1, -1, -1 };  // Initialisés à -1 pour éviter des choix invalides
    int nb_valid_moves = 0;

    // Pour le débogage, affichez le nombre d'attaques du Pokémon actif
    int nb_moves = ai->AI_t_Team->team[0].nb_move;
    printf("Nombre d'attaques disponibles (nb_move) : %d\n", nb_moves);

    // Remplissage des tableaux avec les attaques dont il reste des PP
    for (int i = 0; i < nb_moves; i++) {
        if (ai->AI_t_Team->team[0].moveList[i].current_pp > 0) {
            tab_damage[nb_valid_moves] = calcDamage(ai->AI_t_Team, opponent, &(ai->AI_t_Team->team[0].moveList[i]));
            tab_move[nb_valid_moves] = i;
            nb_valid_moves++;
        }
    }

    if (nb_valid_moves == 0) {
        printf("Aucune attaque valide\n");
        return 0;  // Optionnel : retourner STRUGGLE ou un autre code d'action si nécessaire
    }

    // Tri par ordre décroissant des dégâts estimés
    insertionSort(tab_damage, tab_move, nb_valid_moves);

    int chosenIndex = 0;
    // Choix de l'attaque selon le type d'IA
    if (ai->type % 3 == 0) { // status_first AI
        chosenIndex = (rand() % ai->AI_lvl) % nb_valid_moves;
    }
    else if (ai->type % 2 == 0) { // damage_first AI
        while (chosenIndex < nb_valid_moves - 1 && (rand() % 20) > ai->AI_lvl)
            chosenIndex++;
    }
    else { // none type AI
        chosenIndex = (rand() % ai->AI_lvl) % nb_valid_moves;
    }

    // Affichage pour le débogage (en 0-indexé)
    printf("Tab damage après tri : ");
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

    return tab_move[chosenIndex];
}