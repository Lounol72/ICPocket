
#include "../include/interDuel.h"

/**
 * @fn void healTeam(t_Team * t)
 * @brief cette fonction permet de soigner une équipe.
 * 
 * @param t Pointeur vers la structure de l'équipe à soigner.
 */
void healTeam(t_Team * t){
    t->effect=noEffect;
    for(int i=0;i<t->nb_poke;i++){
        t->team[i].current_pv=t->team[i].initial_pv;
        t->team[i].main_effect=noEffect;
        for(int j=0;j<t->team[0].nb_move;j++){
            t->team[i].moveList[j].current_pp=t->team[i].moveList[j].max_pp;
        }
    }
}

/**
 * @fn void getPokeFromTeam(t_Team * player, int index1, t_Team * adv, int index2)
 * @brief cette fonction permet de récupérer un pokémon d'une équipe.
 * 
 * @param player Pointeur vers la structure de l'équipe du joueur.
 * @param index1 Index du pokémon à récupérer dans l'équipe du joueur.
 * @param adv Pointeur vers la structure de l'équipe adverse.
 * @param index2 Index du pokémon à remplacer dans l'équipe adverse.
 */
void getPokeFromTeam(t_Team * player, int index1, t_Team * adv, int index2){
    /*if(player->nb_poke<=index1) player->nb_poke++;
    t_Poke src=adv->team[index2];
    player->team[index1]=src;
    */
    
    t_Poke * dest=&player->team[index1];
    t_Poke * src=&adv->team[index2];

    if(player->nb_poke<=index1) player->nb_poke++;
    dest->id=src->id;
    strcpy(dest->name,src->name);
    dest->gender=src->gender;
    dest->lvl=src->lvl;
    dest->exp=src->exp;
    dest->nature=src->nature;
    dest->current_pv=src->current_pv;
    dest->initial_pv=src->initial_pv;
    dest->nb_move=src->nb_move;
    dest->type[0]=src->type[0];
    dest->type[1]=src->type[1];
    for(int i=0;i<6;i++){
        dest->baseStats[i]=src->baseStats[i];
        dest->iv[i]=src->iv[i];
    }
    for(int i=0;i<src->nb_move;i++){
       dest->moveList[i]=src->moveList[i];
    }
    dest->main_effect=noEffect;
    dest->img=NULL;
    
}