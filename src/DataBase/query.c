#include <stdlib.h>
#include <string.h>

#include "dataBase.h"
#include "query.h"
#include "logger.h"

DATA_FIELD getTypeQueryById(Query * query, int index) {
  if (index < 0 || index >= query->descriptor.nbField) {
    return DATA_FIELD_UNKNOWN;
  }
  return query->descriptor.dataField[index].type;
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
  free(query);

  return DATA_BASE_SUCCESS;
}

DATA_BASE commandInsert(HeaderTable *header, FILE *file, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField, char listValue[][DATA_FIELD_MAX_CHARACTER]) {
  char *buff = NULL;
  int sizeRecord = header->lengthRecord/header->nbRecord;
  int nbElement = 0;

  buff = (char*)malloc(sizeof(char) * sizeRecord);
  memset(buff, 0 , sizeof(char) * sizeRecord);
  buff[0] = 'A';

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

    if (nbElement != 1) {
      return DATA_BASE_FAILURE;
    }

    header->nbRecord++;
    header->lengthRecord += sizeRecord;

    fseek(file, 0, SEEK_SET);
    nbElement = fwrite(header, sizeof(HeaderTable), 1, file);

    if (nbElement != 1) {
      return DATA_BASE_FAILURE;
    }
  }

  return DATA_BASE_SUCCESS;
}

Query * commandSelect(HeaderTable *header, FILE *file, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField) {
  Query *query;
  query = (Query*)malloc(sizeof(Query));

  if (query != NULL) {
    if (createDescriptorResult(header, query, listField, nbField) == DATA_BASE_SUCCESS) {
      query->data = (char*)malloc(query->descriptor.sizeRecord * header->nbRecord);

      query->nbRecord = header->nbRecord;
      for(int i = 0; i < header->nbRecord; i++) {
        char *buffRecord = NULL;
        char *cursorQuery = NULL;
        char *cursorRecord = NULL;

        buffRecord = getRecord(header, file, i);

        for(int k = 0; k < query->descriptor.nbField; k++) {
          cursorQuery = query->data + (i * query->descriptor.sizeRecord );
          cursorRecord = buffRecord;
          for(int j = 0; j < header->nbField; j++) {
            if(strcmp(query->descriptor.dataField[k].name, header->descriptor[j].name) == 0) {
              cursorRecord += header->descriptor[j].offset + 1;
              cursorQuery  += query->descriptor.dataField[k].offset;
              memcpy(cursorQuery, cursorRecord, query->descriptor.dataField[k].size);
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

    // TODO code Adrien
    /*char listField[2][DATA_FIELD_MAX_CHARACTER] = {"id", "name"};
    int nbField = 2;
    char listValue[2][DATA_FIELD_MAX_CHARACTER] = {"100", "TEST"};*/

    char listField[1][DATA_FIELD_MAX_CHARACTER] = {"*"};
    int nbField = 1;

    //commandInsert(header, file, listField, nbField, listValue);
    //fflush(file);
    query = commandSelect(header, file, listField, nbField);
  }
  else {
    return NULL;
  }

  //destroyHeader(header);

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
  query->descriptor.dataField = (DataField*)malloc(sizeof(DataField)*query->descriptor.nbField);

  if (query->descriptor.dataField == NULL) {
    return DATA_BASE_FAILURE;
  }

  memset(query->descriptor.dataField, 0, sizeof(DataField)*query->descriptor.nbField);

  for(int i = 0; i < query->descriptor.nbField; i++) {
    for(int j = 0; j < header->nbField; j++) {
      if((nbField == 1 && strcmp(listField[0], "*")==0) || (strcmp(listField[i], header->descriptor[j].name) == 0)) {
        memcpy(&query->descriptor.dataField[i], &header->descriptor[j], sizeof(DataField));
      }
    }
    if(strlen(query->descriptor.dataField[i].name) == 0) {
      ERROR("La colonne '%s' n'existe pas.", listField[i]);
      return DATA_BASE_FAILURE;
    }
  }

  for(int i = 0; i < query->descriptor.nbField; i++) {
    query->descriptor.dataField[i].offset = query->descriptor.sizeRecord;
    query->descriptor.sizeRecord += query->descriptor.dataField[i].size;
  }

  return DATA_BASE_SUCCESS;
}