#ifndef MINISTDLIB_H
#define MINISTDLIB_H

/*Utilisation: itoa(num a convertir , temporaire , base du nb a convertir)*/

/**
 * @fn char* iitoa(int value, char* str, int base)
 * @brief Convertit un entier en chaîne de caractères.
 * 
 * @param value L'entier à convertir.
 * @param str Pointeur vers la chaîne de caractères où le résultat sera stocké.
 * @param base La base de conversion (par exemple, 10 pour décimal, 16 pour hexadécimal).
 * @return char* 
 */
char* iitoa(int value, char* str, int base);

#endif /* MINISTDLIB_H */
