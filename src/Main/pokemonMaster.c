#include <stdlib.h>

#include "dataBase.h"
#include "query.h"
#include "logger.h"
#include "menu.h"


int main(int argc, char const *argv[]) {
  DataBase *dataBase = NULL;
  Query *query = NULL;

  dataBase = connect(NULL);
  if (dataBase == NULL) {
    ERROR("Probleme dans l'ouverture des tables. Fermeture du programme !");
    return 1;
  } else {
    menu(dataBase);
  }

  close(dataBase);

  return 0;
}
