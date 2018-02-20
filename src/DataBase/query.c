#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dataBase.h"
#include "query.h"
#include "logger.h"

DATA_FIELD getTypeQueryById(Query * query, int index) {
  if (index < 0 || index >= query->descriptor.nbField) {
    return DATA_FIELD_UNKNOWN;
  }
  return query->descriptor.dataField[index].type;
}

int getSizeQueryById(Query * query, int index) {
  if (index < 0 || index >= query->descriptor.nbField) {
    return 0;
  }
  return query->descriptor.dataField[index].size;
}

char * getNameQueryById(Query * query, int index) {
  if (index < 0 || index >= query->descriptor.nbField) {
    return NULL;
  }
  return query->descriptor.dataField[index].name;
}

char * getDataQueryById(Query * query, int row, int colum) {
  if (colum < 0 || colum >= query->descriptor.nbField || row < 0 || row >= query->nbRecord) {
    return NULL;
  }
  return query->data + (row * query->descriptor.sizeRecord) + query->descriptor.dataField[colum].offset;
}

char * getDataQueryByName(Query * query, int row, char * fieldName) {
  if (row < 0 || row >= query->nbRecord) {
    return NULL;
  }
  for(int i = 0; i < query->descriptor.nbField; i++) {
    if(strcmp(query->descriptor.dataField[i].name, fieldName) == 0) {
      return query->data + (row * query->descriptor.sizeRecord) + query->descriptor.dataField[i].offset;
    }
  }

  ERROR("La colonne '%s' n'existe pas.", fieldName);

  return NULL;
}

DATA_BASE closeQuery(Query *query) {
  free(query->data);
  free(query->descriptor.dataField);
  free(query);

  return DATA_BASE_SUCCESS;
}

DATA_BASE commandInsert(HeaderTable *header, FILE *file, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField, char listValue[][DATA_FIELD_MAX_CHARACTER]) {
  char *buff = NULL;
  int sizeRecord = header->lengthRecord/header->nbRecord;
  int nbElement = 0;

  buff = (char*)malloc(sizeof(char) * sizeRecord);
  memset(buff, 0 , sizeof(char) * sizeRecord);
  buff[0] = CHARACTER_RECORD_VALID;

  if (buff != NULL) {
    for(int i = 0; i < nbField; i++) {
      for(int j = 0; j < header->nbField; j++) {
        if(strcmp(listField[i], header->descriptor[j].name) == 0) {
          switch (header->descriptor[j].type) {
            case DATA_FIELD_INT:
            {
              int value = atoi(listValue[i]);
              memcpy(buff+1+header->descriptor[j].offset, &value, header->descriptor[j].size);
            }
            break;
            case DATA_FIELD_CHAR:
            {
              memcpy(buff+1+header->descriptor[j].offset, listValue[i], header->descriptor[j].size);
            }
            break;
            default:
            break;
          }
        }
      }
    }


    fseek(file, 0, SEEK_END);
    nbElement = fwrite(buff, sizeof(char) * sizeRecord, 1, file);
    free(buff);

    if (nbElement != 1) {
      return DATA_BASE_FAILURE;
    }

    header->nbRecord++;
    header->lengthRecord += sizeRecord;

    updateTimeHeader(header);

    fseek(file, 0, SEEK_SET);
    nbElement = fwrite(header, sizeof(HeaderTable), 1, file);



    if (nbElement != 1) {
      return DATA_BASE_FAILURE;
    }
  }

  return DATA_BASE_SUCCESS;
}

DATA_BASE checkCondition(HeaderTable *header, Condition *condition, char *record) {
  if (condition == NULL) {
    return DATA_BASE_SUCCESS;
  }

  for(int i = 0; i < header->nbField; i++) {
    if(strcmp(condition->field, header->descriptor[i].name) == 0) {
      switch (condition->type) {
        case CONDITION_EQUAL:
        {
          switch (header->descriptor[i].type) {
            case DATA_FIELD_INT:
            {
              int value = atoi(condition->value);
              int data;
              char *pData = record + 1 + header->descriptor[i].offset;

              memcpy(&data, pData, header->descriptor[i].size);

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
              char *data = record + 1 + header->descriptor[i].offset;
              if(strcmp(condition->value, data) == 0) {
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

  timestamp = time(NULL);
  t = localtime(&timestamp);

  headerTable->lastUpdate.day   = t->tm_mday;
  headerTable->lastUpdate.month = t->tm_mon;
  headerTable->lastUpdate.year  = 1900 + t->tm_year;
}

DATA_BASE commandCreateTable() {
  FILE *file;

  file = fopen("../../ressources/bases/testBase.dbase", "w+");

  if (file != NULL) {
    HeaderTable headerTable;
    DataField *dataField = NULL;

    int nbField = 2;
    int sizeRecord = 0;

    dataField = (DataField *)malloc(sizeof(DataField)*nbField);

    if (dataField == NULL) {
      fclose(file);
      return DATA_BASE_FAILURE;
    }

    for(int i = 0; i < nbField; i++) {
      int size = convertCharTypeToSize("INT");

      dataField[i].offset = sizeRecord;
      dataField[i].type = convertCharTypeToType("INT");
      strcpy(dataField[i].name, "id");
      dataField[i].size = size;

      sizeRecord += size;
    }


    headerTable.valid = 1;
    headerTable.nbRecord = 0;
    headerTable.nbField  = nbField;
    headerTable.lengthHeader = sizeof(HeaderTable);
    headerTable.lengthRecord = headerTable.nbRecord * sizeRecord;
    headerTable.lengthField  = headerTable.nbField * sizeof(DataField);
    updateTimeHeader(&headerTable);

    fseek(file, 0 , SEEK_SET);
    fwrite(&headerTable, sizeof(HeaderTable), 1, file);
    fwrite(dataField, sizeof(DataField), nbField, file);

    free(dataField);
  }

  fclose(file);

  return DATA_BASE_SUCCESS;
}

DATA_BASE commandDelete(HeaderTable *header, FILE *file, Condition *condition) {
  int sizeRecord = header->lengthRecord/header->nbRecord;
  int nbElement = 0;

  for(int i = 0; i < header->nbRecord; i++) {
    char *record = NULL;

    record = getRecord(header, file, i);

    if (checkCondition(header, condition, record) == DATA_BASE_SUCCESS) {
      record[0] = CHARACTER_RECORD_INVALID;

      fseek(file, -(sizeof(char) * sizeRecord) , SEEK_CUR);
      nbElement = fwrite(record, sizeof(char) * sizeRecord, 1, file);

      if (nbElement != 1) {
        free(record);
        return DATA_BASE_FAILURE;
      }
    }

    if (record != NULL) {
      free(record);
    }
  }

  updateTimeHeader(header);

  fseek(file, 0 , SEEK_SET);
  nbElement = fwrite(header, sizeof(HeaderTable), 1, file);

  if (nbElement != 1) {
    return DATA_BASE_FAILURE;
  }

  return DATA_BASE_SUCCESS;
}

DATA_BASE commandUpdate(HeaderTable *header, FILE *file, Condition *condition, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField, char listValue[][DATA_FIELD_MAX_CHARACTER]) {
  int sizeRecord = header->lengthRecord/header->nbRecord;
  int nbElement = 0;

  for(int i = 0; i < header->nbRecord; i++) {
    char *record = NULL;

    record = getRecord(header, file, i);

    if (checkCondition(header, condition, record) == DATA_BASE_SUCCESS) {
      for(int k = 0; k < nbField; k++) {
        for(int l = 0; l < header->nbField; l++) {
          if(strcmp(listField[k], header->descriptor[l].name) == 0) {
            switch (header->descriptor[l].type) {
              case DATA_FIELD_INT:
              {
                int value = atoi(listValue[k]);
                memcpy(record+1+header->descriptor[l].offset, &value, header->descriptor[l].size);
              }
              break;
              case DATA_FIELD_CHAR:
              {
                memcpy(record+1+header->descriptor[l].offset, listValue[k], header->descriptor[l].size);
              }
              break;
              default:
              break;
            }

            fseek(file, -(sizeof(char) * sizeRecord) , SEEK_CUR);
            nbElement = fwrite(record, sizeof(char) * sizeRecord, 1, file);

            if (nbElement != 1) {
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
      free(record);
    }
  }

  updateTimeHeader(header);

  fseek(file, 0 , SEEK_SET);
  nbElement = fwrite(header, sizeof(HeaderTable), 1, file);

  if (nbElement != 1) {
    return DATA_BASE_FAILURE;
  }

  return DATA_BASE_SUCCESS;
}

Query * commandSelect(HeaderTable *header, FILE *file, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField) {
  Query *query = NULL;
  query = (Query*)malloc(sizeof(Query));

  if (query != NULL) {
    if (createDescriptorResult(header, query, listField, nbField) == DATA_BASE_SUCCESS) {
      query->nbRecord = 0;
      query->data = NULL;

      for(int i = 0; i < header->nbRecord; i++) {
        char *buffRecord = NULL;
        char *cursorQuery = NULL;
        char *cursorRecord = NULL;
        //TODO
        Condition condition;
        strcpy(condition.field, "name");
        strcpy(condition.value, "Pikachu");
        condition.type = CONDITION_EQUAL;

        buffRecord = getRecord(header, file, i);

        if (recordIsValid(buffRecord) == DATA_BASE_SUCCESS) {
          if (checkCondition(header, &condition, buffRecord) == DATA_BASE_SUCCESS) {

            query->nbRecord++;
            if (query->data == NULL) {
              query->data = (char*)malloc(query->descriptor.sizeRecord);
            }
            else {
              query->data = (char*)realloc(query->data, query->descriptor.sizeRecord * query->nbRecord);
            }

            if (query->data == NULL) {
              if (buffRecord != NULL) {
                free(buffRecord);
              }
              return NULL;
            }

            for(int k = 0; k < query->descriptor.nbField; k++) {
              cursorQuery = query->data + ((query->nbRecord-1) * query->descriptor.sizeRecord);
              cursorRecord = buffRecord;
              for(int j = 0; j < header->nbField; j++) {
                if(strcmp(query->descriptor.dataField[k].name, header->descriptor[j].name) == 0) {
                  cursorRecord += header->descriptor[j].offset + 1;
                  cursorQuery  += query->descriptor.dataField[k].offset;
                  memcpy(cursorQuery, cursorRecord, query->descriptor.dataField[k].size);
                }
              }
            }
          }
        }
        if (buffRecord != NULL) {
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
  Condition condition;

  if (dataBase == NULL) {
    ERROR("La dataBase est NULL !");
    return NULL;
  }

  file = searchTable(dataBase, "testBase");
  if (file != NULL) {
    HeaderTable *header;

    header = getHeaderTable(file);
    if (header == NULL) {
      return NULL;
    }


    strcpy(condition.field, "name");
    strcpy(condition.value, "Pikachu");
    condition.type = CONDITION_EQUAL;

    char listUpdateField[1][DATA_FIELD_MAX_CHARACTER] = {"id"};
    char listUpdateValue[1][DATA_FIELD_MAX_CHARACTER] = {"1"};
    int nbUpdateField = 1;

    commandUpdate(header, file, &condition, listUpdateField, nbUpdateField, listUpdateValue);

    // TODO code Adrien
    /*char listField[2][DATA_FIELD_MAX_CHARACTER] = {"id", "name"};
    int nbField = 2;
    char listValue[2][DATA_FIELD_MAX_CHARACTER] = {"100", "TEST"};*/

    char listField[1][DATA_FIELD_MAX_CHARACTER] = {"id"};
    int nbField = 1;

    //commandInsert(header, file, listField, nbField, listValue);
    //fflush(file);
    query = commandSelect(header, file, listField, nbField);

    destroyHeader(header);
  }
  else {
    return NULL;
  }



  return query;
}

DATA_BASE createDescriptorResult(HeaderTable *header, Query *query, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField) {

  if (nbField == 1 && strcmp(listField[0], "*")==0) {
    query->descriptor.nbField = header->nbField;
  }
  else {
    query->descriptor.nbField = nbField;
  }

  query->descriptor.sizeRecord = 0;
  query->descriptor.dataField = NULL;
  query->descriptor.dataField = (DataField*)malloc(sizeof(DataField)*query->descriptor.nbField);

  if (query->descriptor.dataField == NULL) {
    return DATA_BASE_FAILURE;
  }

  memset(query->descriptor.dataField, 0, sizeof(DataField)*query->descriptor.nbField);

  if (nbField == 1 && strcmp(listField[0], "*")==0) {
    for(int i = 0; i < header->nbField; i++) {
      memcpy(&query->descriptor.dataField[i], &header->descriptor[i], sizeof(DataField));
    }
  }
  else {
    for(int i = 0; i < query->descriptor.nbField; i++) {
      for(int j = 0; j < header->nbField; j++) {
        if(strcmp(listField[i], header->descriptor[j].name) == 0) {
          memcpy(&query->descriptor.dataField[i], &header->descriptor[j], sizeof(DataField));
        }
      }
      if(strlen(query->descriptor.dataField[i].name) == 0) {
        ERROR("La colonne '%s' n'existe pas.", listField[i]);
        return DATA_BASE_FAILURE;
      }
    }
  }

  for(int i = 0; i < query->descriptor.nbField; i++) {
    query->descriptor.dataField[i].offset = query->descriptor.sizeRecord;
    query->descriptor.sizeRecord += query->descriptor.dataField[i].size;
  }

  return DATA_BASE_SUCCESS;
}
