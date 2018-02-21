#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "query.h"
#include "logger.h"
#include "openFile.h"

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

Query * executeCommand(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest) {
  DATA_BASE result;
  Query *query = NULL;

  switch (sqlRequest->sqlType) {
    case SELECT:
    {
      query = commandSelect(header, file, sqlRequest);
      if (query == NULL) {
        result = DATA_BASE_FAILURE;
      }
    }
    break;

    case INSERT_INTO:
    {
      result = commandInsert(header, file, sqlRequest);
    }
    break;

    case UPDATE:
    {
      result = commandUpdate(header, file, sqlRequest);
    }
    break;

    case DELETE:
    {
      result = commandDelete(header, file, sqlRequest);
    }
    break;

    case CREATE_TABLE: //Deja traité
    default:
    break;
  }
  return query;
}

DATA_BASE commandInsert(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest) {
  char *buff = NULL;
  int sizeRecord = 0;
  int nbElement = 0;

  for(int i = 0; i < header->nbField; i++) {
    sizeRecord += header->descriptor[i].size;
  }

  buff = (char*)malloc(sizeof(char) * sizeRecord);
  memset(buff, 0 , sizeof(char) * sizeRecord);
  buff[0] = CHARACTER_RECORD_VALID;

  if (buff != NULL) {
    for(int i = 0; i < header->nbField; i++) {
      switch (header->descriptor[i].type) {
        case DATA_FIELD_PK:
        {
          int value = header->nbRecord + 1;
          memcpy(buff+1+header->descriptor[i].offset, &value, header->descriptor[i].size);
        }
        break;
        case DATA_FIELD_INT:
        {
          int value = atoi(sqlRequest->listValues[i-1]);
          memcpy(buff+1+header->descriptor[i].offset, &value, header->descriptor[i].size);
        }
        break;
        case DATA_FIELD_CHAR:
        {
          memcpy(buff+1+header->descriptor[i].offset, sqlRequest->listValues[i-1], strlen(sqlRequest->listValues[i-1]));
        }
        break;
        default:
        break;
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

CONDITION convTypeWhere(char *cOperator) {
  CONDITION operator = CONDITION_UNK;

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
  if (where == NULL || convTypeWhere(where->operatorField) == CONDITION_UNK) {
    return DATA_BASE_SUCCESS;
  }
  
  for(int i = 0; i < header->nbField; i++) {
    if(strcmp(where->targetValue, header->descriptor[i].name) == 0) {
      switch (convTypeWhere(where->operatorField)) {
        case CONDITION_EQUAL:
        {
          switch (header->descriptor[i].type) {
            case DATA_FIELD_PK:
            case DATA_FIELD_INT:
            {
              int value = atoi(where->sourceValue);
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

DATA_BASE commandCreateTable(SQL_REQUEST *sqlRequest) {
  FILE *file = NULL;
  char path[PATH_MAX_CHARACTER];

  sprintf(path, "%s%s.dbase", PATH_BASE, sqlRequest->nameTable);
  DEBUG("PATH %s", path);
  if (openFile(&file, path, "w+") == 1) {
    DEBUG("Ouverture du fichier %s", path);

    HeaderTable headerTable;
    DataField *dataField = NULL;

    int nbField = sqlRequest->nbArgs;
    int sizeRecord = 0;

    dataField = (DataField *)malloc(sizeof(DataField)*nbField);

    if (dataField == NULL) {
      fclose(file);
      return DATA_BASE_FAILURE;
    }

    for(int i = 0; i < nbField; i++) {
      int size = convertCharTypeToSize(sqlRequest->listValues[i]);

      dataField[i].offset = sizeRecord;
      dataField[i].type = convertCharTypeToType(sqlRequest->listValues[i]);
      strcpy(dataField[i].name, sqlRequest->listArgs[i]);
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

DATA_BASE commandDelete(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest) {
  int sizeRecord = header->lengthRecord/header->nbRecord;
  int nbElement = 0;

  for(int i = 0; i < header->nbRecord; i++) {
    char *record = NULL;

    record = getRecord(header, file, i);

    if (checkCondition(header, &sqlRequest->where, record) == DATA_BASE_SUCCESS) {
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

DATA_BASE commandUpdate(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest) {
  int sizeRecord = header->lengthRecord/header->nbRecord;
  int nbElement = 0;

  for(int i = 0; i < header->nbRecord; i++) {
    char *record = NULL;

    record = getRecord(header, file, i);

    if (checkCondition(header, &sqlRequest->where, record) == DATA_BASE_SUCCESS) {
      for(int k = 0; k < sqlRequest->nbArgs; k++) {
        for(int l = 0; l < header->nbField; l++) {
          if(strcmp(sqlRequest->listArgs[k], header->descriptor[l].name) == 0) {
            switch (header->descriptor[l].type) {
              case DATA_FIELD_INT:
              {
                int value = atoi(sqlRequest->listValues[k]);
                memcpy(record+1+header->descriptor[l].offset, &value, header->descriptor[l].size);
              }
              break;
              case DATA_FIELD_CHAR:
              {
                memcpy(record+1+header->descriptor[l].offset, sqlRequest->listValues[k], header->descriptor[l].size);
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

Query * commandSelect(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest) {
  Query *query = NULL;
  query = (Query*)malloc(sizeof(Query));

  if (query != NULL) {
    if (createDescriptorResult(header, query, sqlRequest->listArgs, sqlRequest->nbArgs) == DATA_BASE_SUCCESS) {
      query->nbRecord = 0;
      query->data = NULL;

      for(int i = 0; i < header->nbRecord; i++) {
        char *buffRecord = NULL;
        char *cursorQuery = NULL;
        char *cursorRecord = NULL;

        buffRecord = getRecord(header, file, i);

        if (recordIsValid(buffRecord) == DATA_BASE_SUCCESS) {
          if (checkCondition(header, &sqlRequest->where, buffRecord) == DATA_BASE_SUCCESS) {

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

  SQL_REQUEST sqlRequest;

  if (dataBase == NULL) {
    ERROR("La dataBase est NULL !");
    return NULL;
  }

  sqlRequest = getSqlRequest(sql);

  if (sqlRequest.sqlType == CREATE_TABLE)
  {
    commandCreateTable(&sqlRequest);
    openTable(dataBase, sqlRequest.nameTable);
  }
  else {
    file = searchTable(dataBase, sqlRequest.nameTable);
    if (file != NULL) {
      HeaderTable *header;

      header = getHeaderTable(file);
      if (header == NULL) {
        return NULL;
      }

      query = executeCommand(header, file, &sqlRequest);

      destroyHeader(header);
    }
    else {
      return NULL;
    }
  }

  return query;
}

DATA_BASE createDescriptorResult(HeaderTable *header, Query *query, char listField[][maxStringSize], int nbField) {

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
