#include <stdlib.h>

#include "dataBase.h"
#include "query.h"
#include "logger.h"


int main(int argc, char const *argv[]) {
  DataBase *dataBase = NULL;
  Query *query = NULL;

  dataBase = connect(NULL);
  if (dataBase == NULL) {
    ERROR("Probleme dans l'ouverture des tables. Fermeture du programme !");
    return 1;
  }

  restoreTables(dataBase);

  query = excuteQuery(dataBase, "SELECT * FROM Pokemon WHERE id=1");

  if (query != NULL) {
    for(int i = 0; i < query->nbRecord; i++) {
      char * field;
      for(int j = 0; j < query->descriptor.nbField; j++) {
        field = getDataQueryById(query, i, j);
        switch (getTypeQueryById(query, j)) {
          case DATA_FIELD_PK:
          case DATA_FIELD_INT:
          {
            DEBUG("Champ %d %s : %d", j+1, getNameQueryById(query, j), (int)*field);
          }
          break;
          case DATA_FIELD_CHAR:
          {
            DEBUG("Champ %d %s : %s",j+1, getNameQueryById(query, j), field);
          }
          break;
          default:
          break;
        }
      }
    }

    closeQuery(query);
  }

  close(dataBase);

  return 0;
}
