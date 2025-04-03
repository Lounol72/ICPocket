#include "../include/ministdlib.h"

/**
 * @fn char* iitoa(int value, char* str, int base)
 * @brief Convertit un entier en chaîne de caractères.
 * 
 * @param value L'entier à convertir.
 * @param str Pointeur vers la chaîne de caractères où le résultat sera stocké.
 * @param base La base de conversion (par exemple, 10 pour décimal, 16 pour hexadécimal).
 * @return char* 
 */
char* iitoa(int value, char* str, int base){
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    int i = 0;
    int isNegative = 0;

    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (value < 0 && base == 10) {
        isNegative = 1;
        value = -value;
    }

    while (value != 0) {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / base;
    }

    if (isNegative)
        str[i++] = '-';
    
    str[i] = '\0';
    
    int start = 0, end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
    
    return str;
}