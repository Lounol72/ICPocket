#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/save.h"


void sauvegarder_Joueur(t_Team * PokeSJ1, t_Dresseur * Dress){
    FILE *fichier = fopen("../data/dataJoueur/saveJoueur.txt", "w+");

    fprintf(fichier, "Hello world");
}

void main(){
    t_Dresseur * Dress;
    t_Team * PokeSJ1;
    sauvegarder_Joueur(PokeSJ1,Dress);
}