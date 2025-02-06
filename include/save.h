
/**
*@brief Save the player's data in a file
*@param nomSave : the name of the save
*@param joueur : the player's data
*@param dresseur : the last defeated trainer
 */
void sauvegarder_Joueur(char * name,t_trainer * teamJ1,t_trainer * dresseur);
/**
*@brief Load the player's data from a file
*@param nomSave : the name of the save
*@param joueur : the player's data
 */
void charger_Joueur(char * name,t_trainer * teamJ1,t_trainer * dresseur);