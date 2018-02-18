#include <stdlib.h>
#include <string.h>

#include "dataBase.h"
#include "openFile.h"
#include "logger.h"

DataBase * connect(char *pathBase) {
  // Parcours des Tables

  DataBase *dataBase;
  dataBase = (DataBase*)malloc(sizeof(DataBase));

  if(dataBase == NULL) {
    return NULL;
  }

  dataBase->nbTable = 1;
  dataBase->tables = (Table*)malloc(sizeof(Table) * dataBase->nbTable);

  for(int i = 0; i < dataBase->nbTable; i++) {
    char path[50];

    strcpy(dataBase->tables[i].name, "testBase");
    sprintf(path, "../ressources/bases/%s.dbase", dataBase->tables[i].name);

    if (openFile(&dataBase->tables[i].file, path, "r+") == 1) {
      DEBUG("Ouverture du fichier %s", path);
    }
    else {
      return NULL;
    }
  }

  DEBUG("Connection a %d table(s).", dataBase->nbTable);

  return dataBase;
}

DATA_BASE close(DataBase *dataBase) {
  free(dataBase->tables);

  return DATA_BASE_SUCCESS;
}

FILE* searchTable(DataBase *dataBase, char* nameTable) {
  for(int i = 0; i < dataBase->nbTable; i++) {
    if (strcmp(dataBase->tables[i].name, nameTable) == 0) {
      return dataBase->tables[i].file;
    }
  }

  WARN("Table %s non trouvee.", nameTable);

  return NULL;
}

char * getRecord(HeaderTable *header, FILE* file, int indexRecord) {
  char *buff = NULL;
  int sizeRecord = header->lengthRecord / header->nbRecord;
  buff = (char*)malloc(sizeof(char) * sizeRecord);
  if (buff != NULL) {
    int position = header->lengthHeader + header->lengthField + (indexRecord * sizeRecord);
    fseek(file, position, SEEK_SET);
    fread(buff, sizeof(char) * sizeRecord, 1, file);
  }

  return buff;
}

HeaderTable * getHeaderTable(FILE *tableFile) {
  int nbElement = 0;
  HeaderTable *headerTable = NULL;

  if (tableFile == NULL) {
    return NULL;
  }

  headerTable = (HeaderTable*)malloc(sizeof(HeaderTable));

  if(headerTable == NULL) {
    return NULL;
  }

  memset(headerTable, 0, sizeof(HeaderTable));

  fseek(tableFile, 0 , SEEK_SET);
  nbElement = fread(headerTable, sizeof(HeaderTable), 1, tableFile);

  if (nbElement == 0) {
    ERROR("Impossible de lire l'entete.");
    return NULL;
  }

  DEBUG("******* HEADER *******");
  DEBUG("valid        : %hd", headerTable->valid);
  DEBUG("lastUpdate   : %hd/%hd/20%hd", headerTable->lastUpdate.day, headerTable->lastUpdate.month, headerTable->lastUpdate.year);
  DEBUG("nbRecord     : %d", headerTable->nbRecord);
  DEBUG("nbField      : %d", headerTable->nbField);
  DEBUG("lengthHeader : %d", headerTable->lengthHeader);
  DEBUG("lengthRecord : %d", headerTable->lengthRecord);
  DEBUG("lengthField  : %d", headerTable->lengthField);
  DEBUG("**********************");

  headerTable->descriptor = (DataField*)malloc(headerTable->lengthField);

  if (headerTable->descriptor != NULL) {
    fseek(tableFile, headerTable->lengthHeader , SEEK_SET);
    fread(headerTable->descriptor, sizeof(DataField), headerTable->nbField, tableFile);

    DEBUG("******* FIELD *******");
    for(int i = 0; i < headerTable->nbField; i++) {
      DEBUG("******* %d *******", i);
      DEBUG("offset : %d", headerTable->descriptor[i].offset);
      DEBUG("type   : %d", headerTable->descriptor[i].type);
      DEBUG("name   : %s", headerTable->descriptor[i].name);
      DEBUG("size   : %d", headerTable->descriptor[i].size);
    }
    DEBUG("*********************");
  }

  return headerTable;
}
