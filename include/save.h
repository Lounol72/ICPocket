
/**
*@brief Save the player's data in a file
*@param nomSave : the name of the save
*@param joueur : the player's data
*@param dresseur : the last defeated trainer
 */
void sauvegarder(char * name,t_Team * teamJ1,t_Team * dresseur);
/**
*@brief Load the player's data from a file
*@param nomSave : the name of the save
*@param joueur : the player's data
 */
void charger(char * name,t_Team * teamJ1,t_Team * dresseur);