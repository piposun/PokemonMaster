#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlCommandParser.h"

#define maxArraySize (10)
#define maxStringSize (30)
#define nbMaxClauses (8)
#define nbMaxCommand (4)

sqlRequest getSqlRequest (char *sqlQuerry){
  //TODO : Initialiser les structures de façon statique.
  sqlCommand select = (sqlCommand){
    .commandName="SELECT",
    .nbMandatoryClauses=2,
    .mandatoryClauseNames={"SELECT","FROM"},
    .nbOptionnalClauses=3,
    .optionnalClauseNames={"WHERE","GROUP BY","HAVING"}
  };
  sqlCommand insert = (sqlCommand){
    .commandName="INSERT INTO",
    .nbMandatoryClauses=1,
    .mandatoryClauseNames={"INSERT INTO"},
    .nbOptionnalClauses=1,
    .optionnalClauseNames={"VALUES"}
  };
  sqlCommand update = (sqlCommand){
    .commandName="UPDATE",
    .nbMandatoryClauses=2,
    .mandatoryClauseNames={"UPDATE","SET"},
    .nbOptionnalClauses=1,
    .optionnalClauseNames={"WHERE"}
  };
  sqlCommand delete = (sqlCommand){
    .commandName="DELETE",
    .nbMandatoryClauses=1,
    .mandatoryClauseNames={"DELETE"},
    .nbOptionnalClauses=1,
    .optionnalClauseNames={"WHERE"}
  };
  int rqSize = 0;
  char splitedRequest[maxArraySize][maxStringSize];
  sqlCommand commands[nbMaxCommand]={select,insert,update,delete};
  splitString(sqlQuerry,splitedRequest,&rqSize," ");
  printStringArray(splitedRequest,rqSize);
  sqlCommand command = identifyCommand(splitedRequest,commands);
  printSqlCommand(&command);
  if (isValidSqlQuerry(sqlQuerry,splitedRequest,rqSize,&command)==0){
    sqlRequest request;
    if (strcmp("SELECT",command.commandName)==0){
      int argsSize = 0;
      char splitedArgs[maxArraySize][maxStringSize];
      splitString(splitedRequest[1],splitedArgs,&argsSize,",");
      request = (sqlRequest){.sqlType=SELECT,.nameTable=splitedRequest[3],.nbArgs=argsSize,.nbValues=0,.listArgs={""},.listValues={""}};
      mapStringArray(request.listArgs,splitedArgs,argsSize);
      printQuerryStruct(&request);
    }
  }
}
void printSqlCommand(sqlCommand *command){
  printf("Nom de la commande : %s\n",command->commandName);
  printf("Clauses obligatoires :\n");
  for (size_t i = 0; i < command->nbMandatoryClauses; i++) {
    printf("%s\n",command->mandatoryClauseNames[i]);
  }
  printf("Clauses optionnelles:\n");
  for (size_t i = 0; i < command->nbOptionnalClauses; i++) {
    printf("%s\n",command->optionnalClauseNames[i] );
  }
}
int isMandatoryClause(char* string,sqlCommand *command){
  for (size_t i = 0; i < command->nbMandatoryClauses; i++) {
    if (strcmp(string,command->mandatoryClauseNames[i])==0) {
      return 0;
    }else{
      return 1;
    }
  }
}
int isOptionnalClause(char* string,sqlCommand *command){
  for (size_t i = 0; i < command->nbOptionnalClauses; i++) {
    if (strcmp(string,command->optionnalClauseNames[i])==0) {
      return 0;
    }else{
      return 1;
    }
  }
}
void printStringArray(char stringArray[][maxStringSize], int arraySize){
  for (size_t i = 0; i < arraySize; i++) {
    printf("%s\n",stringArray[i]);
  }
}
void printQuerryStruct (sqlRequest *sqlQuerry){
  printf("Type de Requete : %d\n",sqlQuerry->sqlType);
  printf("Nom des tables:\n");
  printf("%s\n",sqlQuerry->nameTable );
  printf("Nombre d'arguments :\n");
  printf("%d\n",sqlQuerry->nbArgs);
  printf("Nombre de valeurs :\n");
  printf("%d\n",sqlQuerry->nbValues );
  printf("Liste des arguments :\n");
  for (size_t i = 0; i < sqlQuerry->nbArgs; i++) {
    printf("%s\n",sqlQuerry->listArgs[i] );
  }
  printf("Liste des valeurs :\n");
  for (size_t i = 0; i < sqlQuerry->nbValues; i++) {
    printf("%s\n",sqlQuerry->listValues[i] );
  }
}
void splitString(char* sqlQuerry,char stringArray[][maxStringSize],int *arraySize, const char *splitCharacter){
  char *token, *cpySqlQuerry;
  int counter = 0; //création de l'index du tableau
  cpySqlQuerry = strdup(sqlQuerry);  //On réserve la mémoire de la string
  while (token = strsep(&cpySqlQuerry, splitCharacter)){//Tant qu'il est possible de segmenter la chaine, affectation de la valeur de la string dans un token
    strcpy(stringArray[counter],token);// Copie de la valeur du token dans l'index en cours du tableau.
    counter++; //incrément de l'index du tableau
  }
  *arraySize = counter;
  free(cpySqlQuerry);
}
int isValidSqlQuerry (char *sqlQuerry, char stringArray[][maxStringSize],int arraySize,sqlCommand *command){
  //Le nombre d'éléments du tableau doit toujours être un multiple de 2, sinon la paire clause / argument n'est pas respecté.
  if (arraySize % 2 != 0){
    printf("Nombre d'arguments non valide");
    return 1;
  }
  for (size_t i = 0,j=0; i <= (command->nbMandatoryClauses),j<command->nbMandatoryClauses; i+=2,j++) {
    //i est l'index du tableau de string, il doit être incrémenté par 2 puisque les arguments dans le tableau sont toujours en position multiple de 2.
    //j représente l'index dans le tableau de clauses.
    if (strcmp(stringArray[i],command->mandatoryClauseNames[j])!=0){
      printf("Clause obligatoire non valide : %s au lieu de %s\n",stringArray[i],command->mandatoryClauseNames[j]);
      return 1;
    }
  }
  //Les clauses optionnelles sont toujours après les clauses obligatoires, le scan des clauses optionnelles commence donc à la fin des clauses obligatoires.
    for (size_t i = (command->nbMandatoryClauses*2); i < arraySize ; i+=2) {
      if (isOptionnalClause(stringArray[i],command)!=0){
        printf("Clause Optionnelle non valide : %s\n",stringArray[i]);
        return 1;
      }
    }
    return 0;
}
sqlCommand identifyCommand (char stringArray[][maxStringSize],sqlCommand commands[nbMaxCommand]){
  for (size_t i = 0; i < nbMaxCommand; i++) {
    //On identifie la commande par la correspondance de la première clause qui est caractéristique de la commande
    if (strcmp(stringArray[0],commands[i].commandName)==0) {
      return commands[i];
    }
  }
    sqlCommand emptySqlCommand = {"INVALIDE",0,{},0,{}};
    return emptySqlCommand;
}
void mapStringArray (char targetArray[][maxStringSize],char sourceArray[][maxStringSize],int size){
  for (size_t i = 0; i < size; i++) {
    strcpy(targetArray[i],sourceArray[i]);
  }
}
