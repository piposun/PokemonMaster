#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dataBase.h"
#include "query.h"


int main(int argc, char const *argv[]) {
  FILE *file;

  file = fopen("../../ressources/bases/testBase.dbase", "w+");
  int sizeRecord = sizeof(char) + sizeof(int) + sizeof(char) * DATA_FIELD_MAX_CHARACTER;

  if (file != NULL) {
    HeaderTable headerTable;
    DataField dataField;
    int champ1 = 30;
    char champ2[DATA_FIELD_MAX_CHARACTER] = "Pikachu";
    char actif = CHARACTER_RECORD_VALID;

    headerTable.valid = 1;
    headerTable.lastUpdate.day   = 28;
    headerTable.lastUpdate.month = 12;
    headerTable.lastUpdate.year  = 17;
    headerTable.nbRecord = 1;
    headerTable.nbField  = 2;
    headerTable.lengthHeader = sizeof(HeaderTable);
    headerTable.lengthRecord = headerTable.nbRecord * sizeRecord;
    headerTable.lengthField  = headerTable.nbField * sizeof(DataField);
    fwrite(&headerTable, sizeof(HeaderTable), 1, file);

    dataField.offset = 0;
    dataField.type = DATA_FIELD_INT;
    strcpy(dataField.name, "id");
    dataField.size = sizeof(int);
    fwrite(&dataField, sizeof(DataField), 1, file);

    dataField.offset = sizeof(int);
    dataField.type = DATA_FIELD_CHAR;
    strcpy(dataField.name, "name");
    dataField.size = sizeof(char) * DATA_FIELD_MAX_CHARACTER;
    fwrite(&dataField, sizeof(DataField), 1, file);

    fwrite(&actif, sizeof(char), 1, file);
    fwrite(&champ1, sizeof(int), 1, file);
    fwrite(champ2, sizeof(char) * DATA_FIELD_MAX_CHARACTER, 1, file);
  }

  fclose(file);

  file = fopen("../../ressources/bases/testBase.dbase", "r+");

  if (file != NULL) {
    HeaderTable headerTable;
    DataField *dataField = NULL;

    fread(&headerTable, sizeof(HeaderTable), 1, file);
    printf("valid        : %hd\n", headerTable.valid);
    printf("lastUpdate   : %hd/%hd/20%hd\n", headerTable.lastUpdate.day, headerTable.lastUpdate.month, headerTable.lastUpdate.year);
    printf("nbRecord     : %d\n", headerTable.nbRecord);
    printf("nbField      : %d\n", headerTable.nbField);
    printf("lengthHeader : %d\n", headerTable.lengthHeader);
    printf("lengthRecord : %d\n", headerTable.lengthRecord);
    printf("lengthField  : %d\n", headerTable.lengthField);

    dataField = (DataField*)malloc(sizeof(DataField) * headerTable.nbField);
    if (dataField != NULL) {
      char *buff;
      fread(dataField, sizeof(DataField), headerTable.nbField, file);

      for(int i = 0; i < headerTable.nbField; i++) {
        printf("offset : %d\n", dataField[i].offset);
        printf("type   : %d\n", dataField[i].type);
        printf("name   : %s\n", dataField[i].name);
        printf("size   : %d\n", dataField[i].size);
      }

      buff = (char*)malloc(sizeRecord*headerTable.nbRecord);
      if (buff != NULL) {
        char *cursorRecord = NULL;
        char *cursorField  = NULL;
        char *cursor       = NULL;
        char *field        = NULL;

        fread(buff, sizeRecord, headerTable.nbRecord, file);
        for(int indexRecord = 0; indexRecord < headerTable.nbRecord; indexRecord++) {
          cursorRecord = buff + (indexRecord*sizeRecord);

          field = (char*)malloc(sizeof(char));
          if (field != NULL) {
            memcpy(field, cursorRecord, sizeof(char));
            printf("Champ Actif: %c\n", *field);

            if (*field == '#')
            continue;

            free(field);
          }
          else
          continue;

          cursorField  = cursorRecord + sizeof(char);

          for(int i = 0; i < headerTable.nbField; i++) {
            cursor = cursorField + dataField[i].offset;
            field = (char*)malloc(dataField[i].size);
            if (field != NULL) {
              memcpy(field, cursor, dataField[i].size);
              switch (dataField[i].type) {
                case DATA_FIELD_INT:
                {
                  printf("Champ %d : %d\n",i+1, (int)*field);
                }
                break;
                case DATA_FIELD_CHAR:
                {
                  printf("Champ %d : %s\n",i+1, field);
                }
                break;
                default:
                break;
              }
              free(field);
            }
          }
        }
      }
    }
  }

  fclose(file);

  return 0;
}
