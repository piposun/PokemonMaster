#include <stdlib.h>
#include <string.h>

#include "dataBase.h"
#include "openFile.h"
#include "logger.h"
#include "query.h"

DataBase * connect(char *pathBase) {
  DataBase *dataBase = NULL;
  dataBase = (DataBase*)malloc(sizeof(DataBase));

  if(dataBase == NULL) {
    return NULL;
  }

  // Ouverture des Tables
  openTables(dataBase);

  return dataBase;
}

DATA_BASE close(DataBase *dataBase) {
  closeTables(dataBase);
  free(dataBase);

  return DATA_BASE_SUCCESS;
}

void openTable(DataBase *dataBase, char *table) {
  char path[50];

  dataBase->nbTable++;

  if (dataBase->tables == NULL) {
    dataBase->tables = (Table*)malloc(sizeof(Table) * dataBase->nbTable);
  }
  else {
    dataBase->tables = (Table*)realloc(dataBase->tables, sizeof(Table) * dataBase->nbTable);
  }

  strcpy(dataBase->tables[dataBase->nbTable-1].name, table);
  sprintf(path, "%s%s.dbase", PATH_BASE, table);

  if (openFile(&dataBase->tables[dataBase->nbTable-1].file, path, "r+") == 1) {
    DEBUG("Ouverture du fichier %s", path);
  }
}

void openTables(DataBase *dataBase) {
  ListFiles *list = NULL;

  dataBase->nbTable = 0;

  list = getListFile(PATH_BASE,  ".dbase");

  if (list != NULL) {
    dataBase->nbTable = list->nbFiles;
    dataBase->tables = NULL;
    dataBase->tables = (Table*)malloc(sizeof(Table) * dataBase->nbTable);

    for(int i = 0; i < dataBase->nbTable; i++) {
      char path[50];

      strcpy(dataBase->tables[i].name, list->nameFiles[i]);
      sprintf(path, "%s%s.dbase", PATH_BASE, dataBase->tables[i].name);

      if (openFile(&dataBase->tables[i].file, path, "r+") == 1) {
        DEBUG("Ouverture du fichier %s", path);
      }
    }

    closeListFile(list);
  }

  DEBUG("Connection a %d table(s).", dataBase->nbTable);
}

void closeTables(DataBase *dataBase) {
  for(int i = 0; i < dataBase->nbTable; i++) {
    fclose(dataBase->tables[i].file);
  }

  dataBase->nbTable = 0;
  free(dataBase->tables);
  dataBase->tables = NULL;
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

DATA_BASE recordIsValid(char *record) {
  if (record[0] == CHARACTER_RECORD_VALID) {
    return DATA_BASE_SUCCESS;
  }

  return DATA_BASE_FAILURE;
}

void destroyHeader(HeaderTable *header) {
  if (header != NULL) {
    free(header->descriptor);
    free(header);
  }
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

  headerTable->descriptor = NULL;
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

void restoreTables(DataBase *dataBase) {
  ListFiles *list = NULL;

  if (dataBase == NULL) {
    return;
  }

  closeTables(dataBase);

  list = getListFile(PATH_BASE,  ".sql");

  if (list != NULL) {
    for(int i = 0; i < list->nbFiles; i++) {
      char path[PATH_MAX_CHARACTER];
      char removeFile[PATH_MAX_CHARACTER];
      char chaine[TABLE_MAX_COLUM_CHARACTER];
      FILE *file = NULL;

      sprintf(path, "%s%s.sql", PATH_BASE, list->nameFiles[i]);

      if (openFile(&file, path, "r+") == 1) {
        DEBUG("Ouverture du fichier %s", path);

        while (fgets(chaine, TABLE_MAX_COLUM_CHARACTER, file) != NULL) { // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
          //sprintf(removeFile, "%s%s.dbase", PATH_BASE, list->nameFiles[i]);
          //remove(removeFile);
          DEBUG("COMMAND %s", chaine);
          excuteQuery(dataBase, chaine);
        }

        fclose(file);
      }
    }

    closeListFile(list);
  }
}
