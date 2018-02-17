#ifndef HEADER_KEYBOARD
#define HEADER_KEYBOARD

void ClearBuffer(void); /*fonction de vidage buffer avant saisie clavier*/
int KeyboardReading(char *inputDatas); /*fonction de saisie sécurisée*/
long KeyboardLong();/*fonction de saisie sécurisée d'entiers*/

#endif /* HEADER_KEYBOARD */
