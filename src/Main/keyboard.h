#ifndef HEADER_KEYBOARD
#define HEADER_KEYBOARD

void ClearBuffer(void); /* fonction de vidage buffer avant saisie clavier */
int KeyboardReading(char *inputDatas, int NbCharMax); /* fonction saisie de caracteres sécurisée */
int KeyboardLong(long *inputDatas, int min, int max);/* fonction saisie d'entiers sécurisée */

#endif /* HEADER_KEYBOARD */
