#ifndef SAVE_H
#define SAVE_H

#include "duel.h"

/**
*@file save.h
*@author William
 */

/**
 * @fn void sauvegarder(t_Team * teamJ1,t_Team * dresseur)
 * @brief Sauvegarde l'état du jeu dans un fichier.
 * 
 * Cette fonction enregistre l'état du jeu, y compris les informations sur les équipes et les Pokémon, dans un fichier de sauvegarde.
 * 
 * @param teamJ1 Pointeur vers l'équipe du joueur 1.
 * @param dresseur Pointeur vers l'équipe du dresseur. 
 */
void sauvegarder(t_Team * teamJ1,t_Team * dresseur);

/**
 * @fn int charger(char * name,t_Team * teamJ1,t_Team * dresseur)
 * @brief Charge l'état du jeu à partir d'un fichier de sauvegarde.
 * 
 * Cette fonction lit un fichier de sauvegarde et restaure l'état du jeu, y compris les informations sur les équipes et les Pokémon.
 * 
 * @param name Nom du fichier de sauvegarde à charger.
 * @param teamJ1 Pointeur vers l'équipe du joueur 1.
 * @param dresseur Pointeur vers l'équipe du dresseur. 
 * @return 0 si le chargement a réussi, -1 sinon.
 */
int charger(char * name,t_Team * teamJ1,t_Team * dresseur);

/**
 * @fn void sauver(t_Team * teamJ1,int save,char * nomSave)
 * @brief Sauvegarde l'état du jeu dans un fichier.
 * 
 * Cette fonction enregistre l'état du jeu, y compris les informations sur les équipes et les Pokémon, dans un fichier de sauvegarde.
 * 
 * @param teamJ1 Pointeur vers l'équipe du joueur 1.
 * @param save Indicateur de sauvegarde (0 pour une nouvelle sauvegarde, 1 pour écraser une sauvegarde existante).
 * @param nomSave Nom du fichier de sauvegarde.
 */
void sauver(t_Team * teamJ1,int save,char * nomSave);
void saveDestroy(t_Team * joueur);
#endif // SAVE_H