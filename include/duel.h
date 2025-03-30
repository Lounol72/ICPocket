#ifndef DUEL_H
#define DUEL_H

#define NEUTRAL_STAT_CHANGE 6
#define POKE_IS_ABSCENT -1
#define STRUGGLE -10
#define CONFUSED_MOVE -20

#define ATTAQUE_1 0
#define ATTAQUE_2 1
#define ATTAQUE_3 2
#define ATTAQUE_4 3
#include "structPoke.h"
#include "structures.h"

/**
 * @file duel.h
 * @author Natp24109
 * @brief Gestion des duels dans le style de "ICmon" : système de tour par tour avec 2 équipes opposées.
 */

 /**
  * @brief variable globale pour les variations de statistiques.
  */
extern float statVariations[13];

/**
 * @brief variable globale pour les effets de statut.
 */
extern t_Move struggle;

/**
 * @brief variable globale pour les effets de confusion.
 */
extern t_Move confusedMove;

extern float moveEffectivenessFlag;
extern short int secondaryEffectHappenedFlag;
extern short int criticalHitFlag;


#include "trainerAI.h"
#include "save.h"

/**
 * @brief structure de buffer pour le niveau supérieur.
 * 
 */
typedef struct{
	int moveId;
	t_Poke * target;
} Lvl_Up_Buffer;

/**
 * @brief tableau de fonctions pour les effets secondaires.
 * 
 */
extern int (*SecEffectTab[3])(t_Team *,int,int,int);

/*This buffer purpose is to allow leveling up learning moves to work without blocking the SDL window*/
/**
 * @brief tableau de buffer pour le niveau supérieur.
 * 
 */
extern Lvl_Up_Buffer lvl_up_buffer[12];

/**
 * @brief taille du buffer pour le niveau supérieur.
 * 
 */
extern int lvl_up_buffer_size;

/**
 * @brief team rouge et team bleu
 * 
 */
extern t_Team rouge;
extern t_Team bleu;

/**
 * @fn isAttacking(int);
 * @brief fonction pour lancer une attaque
 * 
 * @return int 
 */
int isAttacking(int);

/**
 * @fn int isSwitching(int);
 * @brief fonction pour vérifier si un ICmon switch
 * 
 * @return int 
 */
int isSwitching(int);

/**
 * @fn int ppCheck(t_Move *);
 * @brief fonction pour verifier les pp restants
 *
 * @param move Un pointeur sur la structure t_Move, pointant sur les pp d'une capacité à vérifier.
 * @return int
 */
int ppCheck(t_Move *);

/**
 * @fn int isExisting(t_Poke *);
 * @brief verifie si un pokemon a des pv
 * 
 * @param p Un pointeur sur la structure t_Poke, pointant sur le ICmon à vérifier.
 * @return int 
 */
int isExisting(t_Poke *);

/**
 * @fn int isAlive(t_Poke *);
 * @brief verifie si un pokemon est vivant ou non
 * 
 * @param p Un pointeur sur la structure t_Poke, pointant sur le ICmon à vérifier.
 * @return int 
 */
int isAlive(t_Poke * p);

/**
 * @fn int isTeamAlive(t_Team *);
 * @brief verifie si une équipe est vivante ou non
 * 
 * @param t Un pointeur sur la structure t_Team, pointant sur l'équipe à vérifier.
 * @return int 
 */
int isTeamAlive(t_Team *);

/**
 * @fn int testActionValidity(int, t_Team *);
 * @brief verifie si une attaque est valide ou non
 * 
 * @param action Un entier représentant l'action à vérifier.
 * @param t Un pointeur sur la structure t_Team, pointant sur l'équipe à vérifier.
 * @return int 
 */
int testActionValidity(int action, t_Team * t);

/**
 * @fn int accuracyCheck(int);
 * @brief verifie le taux de précision de l'attaque
 * 
 * @param accuracy valeur de précision de l'attaque
 * @return int 
 */
int accuracyCheck(int accuracy);

/**
 * @fn int hasMoveLeft(t_Poke *);
 * @brief verifie si le pokemon a encore des attaques
 * 
 * @param p Un pointeur sur la structure t_Poke, pointant sur le ICmon à vérifier.
 * @return int 
 */
int hasMoveLeft(t_Poke *);

/**
 * @fn int isStruggling(int);
 * @brief verifie si le pokemon utilise lutte
 * 
 * @param int permet de savoir si le pokemon utilise lutte ou non
 * @return int 
 */
int isStruggling(int);

/**
 * @fn int statVarChange(t_Team * target, int probability, int modifier, int targetedStat);
 * @brief permet de modifier les stats d'un pokemon grâce à un effet secondaire d'une attaque
 * 
 * @param target Un pointeur sur la structure t_Team, pointant sur l'équipe cible.
 * @param probability probabilite d'appliquer l'effet secondaire
 * @param modifier modification des stats de la cible
 * @param targetedStat stats cible 
 * @return int 
 */
int statVarChange(t_Team * target, int probability, int modifier, int targetedStat);

/**
 * @fn int applyEffect(t_Team * target, int probability, int effect, int bool_main_effect);
 * @brief applique un effet secondaire sur une cible
 * 
 * @param target pointeur sur la team cible
 * @param probability probabilité d'appliquer l'effet secondaire
 * @param effect nature de l'effet
 * @param bool_main_effect permet de savoir si l'effet est permanent ou pas
 * @return int 
 */
int applyEffect(t_Team * target, int probability, int effect, int bool_main_effect);

/**
 * @fn int recoilDamage(t_Team * target, int probability, int percentage_of_val, int indexPoke);
 * @brief applique des dégats de recul à la cible
 * 
 * @param target pointe sur la team cible
 * @param probability probabilité d'appliquer l'effet secondaire
 * @param percentage_of_val pourcentage de vie à retirer
 * @param indexPoke position du ICmon qui subit l'effet
 * @return int 
 */
int recoilDamage(t_Team * target, int probability, int percentage_of_val, int indexPoke);

/**
 * @fn void launchSecEffect(t_Team * offender, t_Team * defender, t_Move * action);
 * @brief lance un effet secondaire sur une cible
 * 
 * @param offender Un pointeur sur la structure t_Team, pointant sur l'équipe attaquante.
 * @param defender Un pointeur sur la structure t_Team, pointant sur l'équipe défensive.
 * @param action Un pointeur sur la structure t_Move, pointant sur l'attaque à appliquer.
 */
void launchSecEffect(t_Team * offender, t_Team * defender, t_Move * action);

/**
 * @brief fonction de debug
 * 
 * @param t 
 */
void printTeam(t_Team * t);

/**
 * @fn int calcStatFrom(t_Poke * p,int stat);
 * @brief modifie la statistique d'un ICmon suivant l'effet appliquer
 * 
 * @param p trouve l' ICmon 
 * @param stat prend la statistique à modifier
 * @return int 
 */
int calcStatFrom(t_Poke * p,int stat);

/**
 * @fn void initTeam(t_Team * t,int);
 * @brief initialise une équipe de ICmon
 * 
 * @param t pointeur sur la structure t_Team, pointant sur l'équipe à initialiser.
 * @param int nombre de ICmon à initialiser
 */
void initTeam(t_Team * t,int);

/**
 * @fn void initBlueTeam(t_Team * t,t_Team * joueur);
 * @brief initialise une équipe adverse en fonction du nombre d'adversaire battu par le joueur
 * 
 * @param t Un pointeur sur la structure t_Team, pointant sur l'équipe à initialiser.
 * @param joueur Un pointeur sur la structure t_Team, pointant sur l'équipe du joueur.
 */
void initBlueTeam(t_Team *t,t_Team *joueur);

/**
 * @fn int calcDamage(t_Team * offender,t_Team * defender, t_Move *);
 *
 * @brief calcule les dégats infligés par une attaque
 * 
 * @param offender Un pointeur sur la structure t_Team, pointant sur l'équipe attaquante.
 * @param defender Un pointeur sur la structure t_Team, pointant sur l'équipe défensive.
 * @param move Un pointeur sur la structure t_Move, pointant sur l'attaque à appliquer.
 * @return int 
 */
int calcDamage(t_Team * offender,t_Team * defender, t_Move *);

/**
 * @fn void setDefaultStatChanges(t_Team * p);
 * @brief remet les variations de stats par défault
 * 
 * @param p team a heal
 */
void setDefaultStatChanges(t_Team * p);

/**
 * @fn int resolveSpeedDuel(int speed1, int speed2);
 * @brief connaitre qui attaque en premier
 * 
 * @param speed1 vitesse de l'ICmon du joueur
 * @param speed2 vitesse de l'ICmon adverse
 * @return int 
 */
int resolveSpeedDuel(int speed1, int speed2);

/**
 * @fn int PriorityForFirstPoke(t_Team * p1, t_Team * p2, t_Move * move1, t_Move * move2, int index1, int index2);
 * @brief utilise l'attaque qui a la priorité la plus haute
 * 
 * @param p1 ICmon du joueur
 * @param p2 ICmon adverse
 * @param move1 attaque du joueur
 * @param move2 attaque adverse
 * @param index1 id de l'action du joueur
 * @param index2 id de l'action adverse
 * @return int 
 */
int PriorityForFirstPoke(t_Team * p1, t_Team * p2, t_Move * move1, t_Move * move2, int index1, int index2);

/**
 * @fn void affectDamage(t_Team * offender, t_Team * defender int indexMove);
 * @brief La fonction affectDamage applique la quantité de dégats infligées par offender ultilisant l'attaque positionnée à l'indice indexMove sur defender. La précision des attaques est également prise en compte ici, pouvant résulter sur une absence d'effet après l'appel. Cette fonction est automatiquement appelée par playATurn si une attaque est passé en paramètre de cette dernière.
 * @param offender Un pointeur sur structure t_Team, pointant sur l'équipe attaquante suposée initialisée (voir initTeam).
 * @param offender Un pointeur sur structure t_Team, pointant sur l'équipe subissant l'attaque suposée initialisée (voir initTeam).
 * @param indexMove L'indice entier de l'attaque à réaliser 
 		-si l'indice est entre 0 et 3, l'attaque est celle connu par le ICmon offender->team[0]
		-si il est négatif, l'attaque représente LUTTE si il vaut -1 ou CONFUSION si il vaut -2.
		PlayATurn appelle automatiquement cette fonction avec l'indice adapté.
 * @param return if the damage is affected of not
*/
int  affectDamage(t_Team * offender, t_Team * defender, int indexMove);
/**
 * @fn void swapActualAttacker(t_Team * target, int swapIndex);
 * @brief La fonction swapActualAttacker échange le ICmon présent à l'indice 0 avec celui présent à l'indice swapIndex donné en paramètre. La fonction est sans effet si l'indice spécifié est invalide (hors liste, ICmon ko).
 * @param target Un pointeur sur structure t_Team, pointant sur une équipe suposée initialisée (voir initTeam).
 * @param swapIndex L'indice entier du ICmon cible de l'échange, généralement compris entre 1 et 5.
*/
void swapActualAttacker(t_Team * target, int swapIndex);

/**
 * @fn int playATurn(t_Team * t1, int move1, t_Team * t2, int move2);
 * @brief La fonction playATurn permet de jouer un tour d'un duel ICmon classique, pour deux équipes au préalable générées et initialisées.
 * La fonction modifie l'état des équipes en paramètre et gère tout les cas particuliers (actions invalides, gestion des afflictions, cas de lutte).
 * @param t1,t2 Deux pointeurs sur structures t_Team, pointant sur deux équipes supposé initialisées (voir initTeam).
 * @param move1 Un entier repésentant l'action de l'équipe n°1, comprise entre 0 et 3. Il est recommandé d'utiliser les macros ATTAQUE_X pour renseigner cette valeur.
 * @param move1 Un entier repésentant l'action de l'équipe n°2, comprise entre 0 et 3. Il est recommandé d'utiliser les macros ATTAQUE_X pour renseigner cette valeur.
 * @return un booléen informant si un tour à été joué ou n'était pas valide pour l'être.
 */
int playATurn(t_Team * t1, int move1, t_Team * t2, int move2);

/**
 * @fn int expCurve(int lvl);
 * @brief La fonction expCurve renvoie le nombre d'expérience nécessaire pour passer au niveau supérieur.
 * @param lvl Le niveau du ICmon.
 * @return Le nombre d'expérience nécessaire pour passer au niveau supérieur.
 */
extern unsigned expCurve(int lvl);

/**
 * @fn int reachedNextLvl(t_Poke * p);
 * @brief La fonction reachedNextLvl renvoie un booléen indiquant si le ICmon a atteint le niveau supérieur.
 * @param p Un pointeur sur la structure t_Poke, pointant sur le ICmon à vérifier.
 * @return Un booléen indiquant si le ICmon a atteint le niveau supérieur.
 */
int reachedNextLvl(t_Poke * p);

/**
 * @fn void gainExp(t_Team * target, t_Poke * source);
 * @brief La fonction gainExp permet de faire gagner de l'expérience à un ICmon.
 * @param target Un pointeur sur la structure t_Team, pointant sur l'équipe cible.
 * @param source Un pointeur sur la structure t_Poke, pointant sur le ICmon source de l'expérience.
 */
void gainExp(t_Team * target, t_Poke * source);

/**
 * @fn void checkLearningMove(t_Poke * p);
 * @brief La fonction checkLearningMove vérifie si le ICmon a appris un nouveau mouvement.
 * @param p Un pointeur sur la structure t_Poke, pointant sur le ICmon à vérifier.
 */
void checkLearningMove(t_Poke * p);

/**
 * @fn void testBattle(t_Team * rouge, t_Team * bleu);
 * @brief La fonction testBattle permet de tester le combat entre deux équipes.
 * @param rouge Un pointeur sur la structure t_Team, pointant sur l'équipe rouge.
 * @param bleu Un pointeur sur la structure t_Team, pointant sur l'équipe bleue.
 */
void testBattle(t_Team * rouge, t_Team * bleu);

/**
 * @fn void testSwitch(t_Team * rouge, t_Team * bleu);
 * @brief La fonction testSwitch permet de tester le changement de ICmon entre deux équipes.
 * @param rouge Un pointeur sur la structure t_Team, pointant sur l'équipe rouge.
 * @param bleu Un pointeur sur la structure t_Team, pointant sur l'équipe bleue.
 */
void testSwitch(t_Team * rouge, t_Team * bleu);

/**
 * @fn void testPP(t_Team * rouge, t_Team * bleu);
 * @brief La fonction testPP permet de tester les points de pouvoir (PP) d'une équipe.
 * @param rouge Un pointeur sur la structure t_Team, pointant sur l'équipe rouge.
 * @param bleu Un pointeur sur la structure t_Team, pointant sur l'équipe bleue.
 */
void testPP(t_Team * rouge, t_Team * bleu);

/**
 * @fn void testStruggle(t_Team * rouge, t_Team * bleu);
 * @brief La fonction testStruggle permet de tester l'attaque Lutte entre deux équipes.
 * @param rouge Un pointeur sur la structure t_Team, pointant sur l'équipe rouge.
 * @param bleu Un pointeur sur la structure t_Team, pointant sur l'équipe bleue.
 */
void testStruggle(t_Team * rouge, t_Team * bleu);

/**
 * @fn void teamTest(t_Team * t, int);
 * @brief permet de tester une équipe de ICmon
 * 
 * @param t un pointeur sur la structure t_Team, pointant sur l'équipe à tester.
 * @param int nombre de ICmon à tester
 */
void teamTest(t_Team * t, int);

#include "interDuel.h"

#endif
