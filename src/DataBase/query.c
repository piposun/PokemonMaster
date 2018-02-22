#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "query.h"
#include "logger.h"
#include "openFile.h"

DATA_FIELD getTypeQueryById(Query * query, int index) {
  // Vérification si l'index n'est pas hors borne du tableau
  if (index < 0 || index >= query->descriptor.nbField) {
    return DATA_FIELD_UNKNOWN;
  }
  // Renvoie le type du champ
  return query->descriptor.dataField[index].type;
}

int getSizeQueryById(Query * query, int index) {
  // Vérification si l'index n'est pas hors borne du tableau
  if (index < 0 || index >= query->descriptor.nbField) {
    return 0;
  }
  // Renvoie la taille du champ
  return query->descriptor.dataField[index].size;
}

char * getNameQueryById(Query * query, int index) {
  // Vérification si l'index n'est pas hors borne du tableau
  if (index < 0 || index >= query->descriptor.nbField) {
    return NULL;
  }
  // Renvoie le nom du champ
  return query->descriptor.dataField[index].name;
}

char * getDataQueryById(Query * query, int row, int colum) {
  // Vérification si les indexs ne sont pas hors borne du tableau
  if (colum < 0 || colum >= query->descriptor.nbField || row < 0 || row >= query->nbRecord) {
    return NULL;
  }
  // Renvoie la data du champ
  return query->data + (row * query->descriptor.sizeRecord) + query->descriptor.dataField[colum].offset;
}

char * getDataQueryByName(Query * query, int row, char * fieldName) {
  // Vérification si l'index n'est pas hors borne du tableau
  if (row < 0 || row >= query->nbRecord) {
    return NULL;
  }
  // Recherche du champ qui correspond à fieldName
  for(int i = 0; i < query->descriptor.nbField; i++) {
    if(strcmp(query->descriptor.dataField[i].name, fieldName) == 0) {
      // Renvoie la data du champ
      return query->data + (row * query->descriptor.sizeRecord) + query->descriptor.dataField[i].offset;
    }
  }

  ERROR("La colonne '%s' n'existe pas.", fieldName);

  return NULL;
}

DATA_BASE closeQuery(Query *query) {
  // Libère la data de la réponse
  free(query->data);
  // Libère la description des champs de la réponse
  free(query->descriptor.dataField);
  // Libère la réponse
  free(query);

  return DATA_BASE_SUCCESS;
}

Query * executeCommand(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest) {
  DATA_BASE result;
  Query *query = NULL;

  // Test du type de la requête SQL
  switch (sqlRequest->sqlType) {
    case SELECT:
    {
      // Execute la commande SELECT
      query = commandSelect(header, file, sqlRequest);

      // Si la réponse est initialsé c'est que tout c'est bien passé
      if (query == NULL) {
        result = DATA_BASE_FAILURE;
      }
    }
    break;

    case INSERT_INTO:
    {
      // Execute la commande INSERT INTO
      result = commandInsert(header, file, sqlRequest);
    }
    break;

    case UPDATE:
    {
      // Execute la commande UPDATE
      result = commandUpdate(header, file, sqlRequest);
    }
    break;

    case DELETE:
    {
      // Execute la commande DELTE
      result = commandDelete(header, file, sqlRequest);
    }
    break;

    case CREATE_TABLE: //Déjà traité
    default:
    break;
  }
  return query;
}

DATA_BASE commandInsert(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest) {
  char *buff = NULL;
  int sizeRecord = 0;
  int nbElement = 0;

  // Calcul de la taille d'un enregistrement
  for(int i = 0; i < header->nbField; i++) {
    sizeRecord += header->descriptor[i].size;
  }
  // Ajout le caractère de validité de l'enregistrement
  sizeRecord += sizeof(char);

  // Initialisation de l'enregistrement
  buff = (char*)malloc(sizeof(char) * sizeRecord);
  memset(buff, 0 , sizeof(char) * sizeRecord);

  // Set la validité de l'enregistrement
  buff[0] = CHARACTER_RECORD_VALID;

  if (buff != NULL) {
    // Parcours des champs
    for(int i = 0; i < header->nbField; i++) {
      /* Initialisation du curseur d'écriture
          => buff = adresse de base
          => +1 = décalage pour le champ de validité
          => +offset = décalage d'écriture du champ
      */
      char *cursor = buff+1+header->descriptor[i].offset;

      // Choix du comportement en fonction du type du champ
      switch (header->descriptor[i].type) {
        case DATA_FIELD_PK:
        {
          // Clé primaire est un entier et est autoincrémental
          int value = header->nbRecord + 1;
          // Copie de la data
          memcpy(cursor, &value, header->descriptor[i].size);
        }
        break;
        case DATA_FIELD_INT:
        {
          // Conversion d'une chaine de caractère en entier
          int value = atoi(sqlRequest->listValues[i-1]);
          // Copie de la data
          memcpy(cursor, &value, header->descriptor[i].size);
        }
        break;
        case DATA_FIELD_CHAR:
        {
          // Copie de la data
          memcpy(cursor, sqlRequest->listValues[i-1], strlen(sqlRequest->listValues[i-1]));
        }
        break;
        default:
        break;
      }
    }

    // Place le curseur du fichier à la fin du fichier
    fseek(file, 0, SEEK_END);
    // Ecriture dans le fichier
    nbElement = fwrite(buff, sizeof(char) * sizeRecord, 1, file);

    // Libère le buffeur de l'enregistrement
    free(buff);

    if (nbElement != 1) {
      return DATA_BASE_FAILURE;
    }

    // Mise à jour de l'entête
    header->nbRecord++;
    header->lengthRecord += sizeRecord;

    // Mise à jour de la date
    updateTimeHeader(header);

    // Place le curseur en début du fichier
    fseek(file, 0, SEEK_SET);
    // Ecriture dans le fichier
    nbElement = fwrite(header, sizeof(HeaderTable), 1, file);

    if (nbElement != 1) {
      return DATA_BASE_FAILURE;
    }
  }

  return DATA_BASE_SUCCESS;
}

CONDITION convTypeWhere(char *cOperator) {
  CONDITION operator = CONDITION_UNK;

  // Recherche de correspondance
  if(strcmp(cOperator, "=") == 0) {
    operator = CONDITION_EQUAL;
  }
  else if(strcmp(cOperator, ">") == 0) {
    operator = CONDITION_GREAT;
  }
  else if(strcmp(cOperator, "<") == 0) {
    operator = CONDITION_LOW;
  }

  return operator;
}

DATA_BASE checkCondition(HeaderTable *header, WHERE_CLAUSE *where, char *record) {
  /* Si la structure est NULL ou si il n'y a pas de correspondance,
  cela signifie qu'il n'y a pas de clause where dans la requête SQL, on valide */
  if (where == NULL || convTypeWhere(where->operatorField) == CONDITION_UNK) {
    return DATA_BASE_SUCCESS;
  }

  // Parcours tous les champs
  for(int i = 0; i < header->nbField; i++) {
    // Recherche la correspondance entre le champ de la clause where et les champs de la table
    if(strcmp(where->targetValue, header->descriptor[i].name) == 0) {
      // Conversion du type de l'opérateur
      switch (convTypeWhere(where->operatorField)) {
        case CONDITION_EQUAL:
        {
          switch (header->descriptor[i].type) {
            case DATA_FIELD_PK:
            case DATA_FIELD_INT:
            {
              // Conversion en entier pour les champs de type INT ou PRIMARY_KEY
              int value = atoi(where->sourceValue);
              int data;
              // Initialisation du curseur de lecture
              char *pData = record + 1 + header->descriptor[i].offset;

              // Copie de la data
              memcpy(&data, pData, header->descriptor[i].size);

              // Comparaison des valeurs
              if(value == data) {
                return DATA_BASE_SUCCESS;
              }
              else
              {
                return DATA_BASE_FAILURE;
              }
            }
            break;
            case DATA_FIELD_CHAR:
            {
              // Initialisation du curseur de lecture
              char *data = record + 1 + header->descriptor[i].offset;
              if(strcmp(where->sourceValue, data) == 0) {
                return DATA_BASE_SUCCESS;
              }
              else
              {
                return DATA_BASE_FAILURE;
              }
            }
            break;
            default:
            break;
          }

        }
        break;
        case CONDITION_GREAT:
        case CONDITION_LOW:
        default:
        break;
      }
    }
  }
  return DATA_BASE_FAILURE;
}

DATA_FIELD convertCharTypeToType(char *cType) {
  DATA_FIELD type = DATA_FIELD_UNKNOWN;

  // Recherche de correspondance
  if (strcmp(cType, "INT") == 0) {
    type = DATA_FIELD_INT;
  }
  else if(strcmp(cType, "PRIMARY_KEY") == 0) {
    type = DATA_FIELD_PK;
  }
  else if (strstr(cType, "VARCHAR") != NULL) {
    type = DATA_FIELD_CHAR;
  }

  return type;
}

int convertCharTypeToSize(char *type) {
  int size = 0;

  // Recherche de correspondance
  if (strcmp(type, "INT") == 0 || strcmp(type, "PRIMARY_KEY") == 0) {
    size = sizeof(int);
  }
  else if (strstr(type, "VARCHAR") != NULL) {
    sscanf(type, "VARCHAR[%d]", &size);
    size = size * sizeof(char);
  }

  return size;
}

void updateTimeHeader(HeaderTable *headerTable) {
  time_t timestamp;
  struct tm * t;

  // Initialisation de la structure time
  timestamp = time(NULL);
  t = localtime(&timestamp);

  // Mise à jour de l'entête
  headerTable->lastUpdate.day   = t->tm_mday;
  headerTable->lastUpdate.month = t->tm_mon;
  headerTable->lastUpdate.year  = 1900 + t->tm_year;
}

DATA_BASE commandCreateTable(SQL_REQUEST *sqlRequest) {
  FILE *file = NULL;
  char path[PATH_MAX_CHARACTER];

  // Initialisation du path
  sprintf(path, "%s%s.dbase", PATH_BASE, sqlRequest->nameTable);

  // Ouverture du fichier en w+ pour créer ou écraser le fichier
  if (openFile(&file, path, "w+") == 1) {
    DEBUG("Ouverture du fichier %s", path);

    HeaderTable headerTable;
    DataField *dataField = NULL;

    int nbField = sqlRequest->nbArgs;
    int sizeRecord = 0;

    // Allocation de la structure de description des champs
    dataField = (DataField *)malloc(sizeof(DataField)*nbField);

    // Echec, on ferme
    if (dataField == NULL) {
      fclose(file);
      return DATA_BASE_FAILURE;
    }

    // Initialisation de la structure de description des champs
    for(int i = 0; i < nbField; i++) {
      int size = convertCharTypeToSize(sqlRequest->listValues[i]);

      dataField[i].offset = sizeRecord;
      dataField[i].type = convertCharTypeToType(sqlRequest->listValues[i]);
      strcpy(dataField[i].name, sqlRequest->listArgs[i]);
      dataField[i].size = size;

      sizeRecord += size;
    }

    // Initialisation de l'entête de la table
    headerTable.valid = 1;
    headerTable.nbRecord = 0;
    headerTable.nbField  = nbField;
    headerTable.lengthHeader = sizeof(HeaderTable);
    headerTable.lengthRecord = headerTable.nbRecord * sizeRecord;
    headerTable.lengthField  = headerTable.nbField * sizeof(DataField);
    updateTimeHeader(&headerTable);

    // Place le curseur en debut de fichier
    fseek(file, 0 , SEEK_SET);
    // Ecriture de l'entête dans le fichier
    fwrite(&headerTable, sizeof(HeaderTable), 1, file);
    // Ecriture des description des champs dans le fichier
    fwrite(dataField, sizeof(DataField), nbField, file);

    // Libère la structure de description des champs
    free(dataField);
  }

  fclose(file);

  return DATA_BASE_SUCCESS;
}

DATA_BASE commandDelete(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest) {
  int sizeRecord = header->lengthRecord/header->nbRecord;
  int nbElement = 0;

  // Parcours tous les enregistrement
  for(int i = 0; i < header->nbRecord; i++) {
    char *record = NULL;

    // Récupération d'un enregistrement
    record = getRecord(header, file, i);

    // Vérification de la clause where avec l'enregistrement
    if (checkCondition(header, &sqlRequest->where, record) == DATA_BASE_SUCCESS) {
      // SET le champ de validité de l'enregistrement à la valeur invalide
      record[0] = CHARACTER_RECORD_INVALID;

      // Place le curseur au debut de l'enregistrement dans le fichier
      fseek(file, -(sizeof(char) * sizeRecord) , SEEK_CUR);
      // Ecrasement de l'enregistrement par le nouveau
      nbElement = fwrite(record, sizeof(char) * sizeRecord, 1, file);

      if (nbElement != 1) {
        free(record);
        return DATA_BASE_FAILURE;
      }
    }

    if (record != NULL) {
      // Libère l'enregistrement
      free(record);
    }
  }

  // Mise à jour de la date de la table
  updateTimeHeader(header);

  // place le curseur au début du fichier
  fseek(file, 0 , SEEK_SET);
  nbElement = fwrite(header, sizeof(HeaderTable), 1, file);

  if (nbElement != 1) {
    return DATA_BASE_FAILURE;
  }

  return DATA_BASE_SUCCESS;
}

DATA_BASE commandUpdate(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest) {
  int sizeRecord = header->lengthRecord/header->nbRecord;
  int nbElement = 0;

  // Parcours de l'ensemble des enregistrements
  for(int i = 0; i < header->nbRecord; i++) {
    char *record = NULL;

    // Récupération d'un enregistrement
    record = getRecord(header, file, i);

    // Vérification de la clause where avec l'enregistrement
    if (checkCondition(header, &sqlRequest->where, record) == DATA_BASE_SUCCESS) {
      // Parcours la liste des arguments de la requête
      for(int k = 0; k < sqlRequest->nbArgs; k++) {
        // Parcours la liste des champs de la table
        for(int l = 0; l < header->nbField; l++) {
          // Recherche de correspondance
          if(strcmp(sqlRequest->listArgs[k], header->descriptor[l].name) == 0) {
            /* Initialisation du curseur d'écriture
                => record = adresse de base
                => +1 = décalage pour le champ de validité
                => +offset = décalage d'écriture du champ
            */
            char *cursor = record+1+header->descriptor[l].offset;
            switch (header->descriptor[l].type) {
              case DATA_FIELD_INT:
              {
                // Conversion d'une chaine de caractère en entier
                int value = atoi(sqlRequest->listValues[k]);
                // Copie de la data
                memcpy(cursor, &value, header->descriptor[l].size);
              }
              break;
              case DATA_FIELD_CHAR:
              {
                // Copie de la data
                memcpy(cursor, sqlRequest->listValues[k], header->descriptor[l].size);
              }
              break;
              default:
              break;
            }

            // Place le curseur devant l'enregistrement dans le fichier
            fseek(file, -(sizeof(char) * sizeRecord) , SEEK_CUR);
            // Ecrase l'enregistrement
            nbElement = fwrite(record, sizeof(char) * sizeRecord, 1, file);

            if (nbElement != 1) {
              // Si echec d'écriture dans le fichier, on libère l'enregistrement
              free(record);
              return DATA_BASE_FAILURE;
            }

            break;
          }
        }

        if (nbElement == 1) {
          break;
        }
      }
    }

    if (record != NULL) {
      // Libère l'enregistrement
      free(record);
    }
  }

  // Mise à jour de la date
  updateTimeHeader(header);

  // Place le curseur au debut du fichier
  fseek(file, 0 , SEEK_SET);
  // Ecriture
  nbElement = fwrite(header, sizeof(HeaderTable), 1, file);

  if (nbElement != 1) {
    return DATA_BASE_FAILURE;
  }

  return DATA_BASE_SUCCESS;
}

Query * commandSelect(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest) {
  Query *query = NULL;
  // Allocation de la réponse
  query = (Query*)malloc(sizeof(Query));

  if (query != NULL) {
    // Creation de la structure de description des champs de la réponse
    if (createDescriptorResult(header, query, sqlRequest->listArgs, sqlRequest->nbArgs) == DATA_BASE_SUCCESS) {
      query->nbRecord = 0;
      query->data = NULL;

      // Parcours l'ensemble des enregistrements de la table
      for(int i = 0; i < header->nbRecord; i++) {
        char *buffRecord = NULL;   // buffeur de l'enregistrement
        char *cursorQuery = NULL;  // Curseur d'écriture dans la réponse
        char *cursorRecord = NULL; // Curseur de lecture de l'enregistrement

        // Récupère l'enregistrement
        buffRecord = getRecord(header, file, i);

        // Vérification si l'enregistrement est valide
        if (recordIsValid(buffRecord) == DATA_BASE_SUCCESS) {
          // Vérification si la clause where est valide
          if (checkCondition(header, &sqlRequest->where, buffRecord) == DATA_BASE_SUCCESS) {

            // Incrémente le nombre d'enregistrement dans la réponse
            query->nbRecord++;

            // Allocation / Reallocation de l'espace data de la réponse
            if (query->data == NULL) {
              query->data = (char*)malloc(query->descriptor.sizeRecord);
            }
            else {
              query->data = (char*)realloc(query->data, query->descriptor.sizeRecord * query->nbRecord);
            }

            // Echec d'allocation, on libère
            if (query->data == NULL) {
              if (buffRecord != NULL) {
                free(buffRecord);
              }
              return NULL;
            }

            // Parcours les champs de description de la réponse
            for(int k = 0; k < query->descriptor.nbField; k++) {
              // Place le curseur d'écriture à l'endroit correspondant à sa position dans le descripteur des champs
              cursorQuery = query->data + ((query->nbRecord-1) * query->descriptor.sizeRecord);
              // PLace le curseur de lecture au debut de l'enregistrement
              cursorRecord = buffRecord;
              // Parcours les champs de la table
              for(int j = 0; j < header->nbField; j++) {
                // Correspondance entre les descripteurs
                if(strcmp(query->descriptor.dataField[k].name, header->descriptor[j].name) == 0) {
                  // Place le curseur de lecture à la position du champ dans l'enregistrement
                  cursorRecord += header->descriptor[j].offset + 1;
                  // Place le curseur d'écriture à la position du champ dans la réponse
                  cursorQuery  += query->descriptor.dataField[k].offset;
                  // Copie de la data
                  memcpy(cursorQuery, cursorRecord, query->descriptor.dataField[k].size);
                }
              }
            }
          }
        }
        if (buffRecord != NULL) {
          // Libère l'enregistrement
          free(buffRecord);
        }
      }
    }
    else
    {
      return NULL;
    }
  }

  return query;
}

Query * excuteQuery(DataBase *dataBase, char *sql) {
  Query *query = NULL;
  FILE  *file;

  SQL_REQUEST sqlRequest;

  if (dataBase == NULL) {
    ERROR("La dataBase est NULL !");
    return NULL;
  }

  // Extraction des informations de la requête SQL
  sqlRequest = getSqlRequest(sql);

  // Cas particulier, si c'est une création
  if (sqlRequest.sqlType == CREATE_TABLE)
  {
    // Crée la nouvelle table
    commandCreateTable(&sqlRequest);
    // Ouvre cette nouvelle table
    openTable(dataBase, sqlRequest.nameTable);
  }
  else {
    // Recherche de la table
    file = searchTable(dataBase, sqlRequest.nameTable);
    if (file != NULL) {
      HeaderTable *header;

      // Récupère le header de la table
      header = getHeaderTable(file);
      if (header == NULL) {
        return NULL;
      }

      // Execute la commande SQL
      query = executeCommand(header, file, &sqlRequest);

      // Libère l'entête de la table
      destroyHeader(header);
    }
    else {
      return NULL;
    }
  }

  // Renvoie la réponse
  return query;
}

DATA_BASE createDescriptorResult(HeaderTable *header, Query *query, char listField[][maxStringSize], int nbField) {

  // Cas particulier, si le champ est * alors on prend tous les champs de la table
  if (nbField == 1 && strcmp(listField[0], "*")==0) {
    query->descriptor.nbField = header->nbField;
  }
  else {
    query->descriptor.nbField = nbField;
  }

  // Allocation de la structure de description de la réponse
  query->descriptor.sizeRecord = 0;
  query->descriptor.dataField = NULL;
  query->descriptor.dataField = (DataField*)malloc(sizeof(DataField)*query->descriptor.nbField);

  if (query->descriptor.dataField == NULL) {
    return DATA_BASE_FAILURE;
  }

  // Tout à 0
  memset(query->descriptor.dataField, 0, sizeof(DataField)*query->descriptor.nbField);

  // Si * => copie directement la description de la table dans la réponse
  if (nbField == 1 && strcmp(listField[0], "*")==0) {
    for(int i = 0; i < header->nbField; i++) {
      memcpy(&query->descriptor.dataField[i], &header->descriptor[i], sizeof(DataField));
    }
  }
  else {
    // Parcours l'ensemble des champs de la réponse
    for(int i = 0; i < query->descriptor.nbField; i++) {
      // Parcours l'ensemble des champs de la table
      for(int j = 0; j < header->nbField; j++) {
        // Recherche de correspondance
        if(strcmp(listField[i], header->descriptor[j].name) == 0) {
          // Copie de la description du champ
          memcpy(&query->descriptor.dataField[i], &header->descriptor[j], sizeof(DataField));
        }
      }
      // Pas de correspondande
      if(strlen(query->descriptor.dataField[i].name) == 0) {
        ERROR("La colonne '%s' n'existe pas.", listField[i]);
        return DATA_BASE_FAILURE;
      }
    }
  }

  // Calcul des positions et de la taille des champs
  for(int i = 0; i < query->descriptor.nbField; i++) {
    query->descriptor.dataField[i].offset = query->descriptor.sizeRecord;
    query->descriptor.sizeRecord += query->descriptor.dataField[i].size;
  }

  return DATA_BASE_SUCCESS;
}
