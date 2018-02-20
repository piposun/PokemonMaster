#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlCommandParser.h"

SQL_REQUEST getSqlRequest (char *sqlQuerry){

  //TODO : Initialiser les structures de façon statique.
  SQL_COMMAND select = (SQL_COMMAND){
    .commandName="SELECT",
    .nbMandatoryClauses=2,
    .mandatoryClauseNames={"SELECT","FROM"},
    .nbOptionnalClauses=3,
    .optionnalClauseNames={"WHERE","GROUP BY","HAVING"}
  };

  SQL_COMMAND insert = (SQL_COMMAND){
    .commandName="INSERT INTO",
    .nbMandatoryClauses=1,
    .mandatoryClauseNames={"INSERT INTO"},
    .nbOptionnalClauses=1,
    .optionnalClauseNames={"VALUES"}
  };

  SQL_COMMAND update = (SQL_COMMAND){
    .commandName="UPDATE",
    .nbMandatoryClauses=2,
    .mandatoryClauseNames={"UPDATE","SET"},
    .nbOptionnalClauses=1,
    .optionnalClauseNames={"WHERE"}
  };

  SQL_COMMAND delete = (SQL_COMMAND){
    .commandName="DELETE FROM",
    .nbMandatoryClauses=1,
    .mandatoryClauseNames={"DELETE FROM"},
    .nbOptionnalClauses=1,
    .optionnalClauseNames={"WHERE"}
  };
//Initialisation des variables nécéssaires pour la fonction

  //Taille et tableau permettant de gérer la requète.
  int rqSize = 0;
  char splitedRequest[maxArraySize][maxStringSize];

  //Taille et tableau permettant de gérer les arguments de la requète.
  int argsSize = 0;
  char splitedArgs[maxArraySize][maxStringSize];

  //Taille et tableau permettant de gérer les valeurs de la requète.
  int valuesSize = 0;
  char splitedValues[maxArraySize][maxStringSize];

  SQL_COMMAND commands[nbMaxCommand]={select,insert,update,delete}; //Création d'un tableau contenant toutes les commandes gérées par le code.

  splitString(sqlQuerry,splitedRequest,&rqSize," "); //Transformation de la requète en tableau et chaine avec le séparateur " "

  SQL_COMMAND command = identifyCommand(splitedRequest,commands); //Identification de la commande SQL

  if (isValidSqlQuerry(sqlQuerry,splitedRequest,rqSize,&command)==0){ //On vérifie que la requète est correctement formater
    SQL_REQUEST request;

    if (strcmp("SELECT",command.commandName)==0){ //En fonction du type de requète, on renseigne la structure de façon spécifique.

      splitString(splitedRequest[1],splitedArgs,&argsSize,",");
      request = (SQL_REQUEST){
        .sqlType=SELECT,
        .nameTable=splitedRequest[3],
        .nbArgs=argsSize,
        .nbValues=0,
        .listArgs={""},
        .listValues={""},
        .where=getWhereClause(splitedRequest,rqSize)};
      mapStringArray(request.listArgs,splitedArgs,argsSize);

    }else if (strcmp("INSERT INTO",command.commandName)==0){

      splitString(splitedRequest[3],splitedValues,&valuesSize,",");
      removeChar(splitedValues[0],'(');
      removeChar(splitedValues[valuesSize-1],')');
      request = (SQL_REQUEST){
        .sqlType = INSERT_INTO,
        .nameTable = splitedRequest[1],
        .nbArgs = 0,
        .nbValues = valuesSize,
        .listArgs={""},
        .listValues={""},
        .where=getWhereClause(splitedRequest,rqSize)};
        mapStringArray(request.listValues,splitedValues,valuesSize);

    }else if (strcmp("UPDATE",command.commandName)==0){

      splitString(splitedRequest[3],splitedArgs,&argsSize,"=");
      request = (SQL_REQUEST){
        .sqlType = UPDATE,
        .nameTable = splitedRequest[1],
        .nbArgs = argsSize,
        .nbValues = 0,
        .listArgs={""},
        .listValues={""},
        .where=getWhereClause(splitedRequest,rqSize)};
        mapStringArray(request.listArgs,splitedArgs,argsSize);

    }else if (strcmp("DELETE FROM",command.commandName)==0){

      request = (SQL_REQUEST){
        .sqlType = DELETE,
        .nameTable =splitedRequest[1],
        .nbArgs = 0,
        .nbValues = 0,
        .listArgs={""},
        .listValues={""},
        .where=getWhereClause(splitedRequest,rqSize)
      };
    }
    printQuerryStruct(&request);
    return request;
  }
}
void printSqlCommand(SQL_COMMAND *command){

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

int isMandatoryClause(char* string,SQL_COMMAND *command){

  for (size_t i = 0; i < command->nbMandatoryClauses; i++) {
    if (strcmp(string,command->mandatoryClauseNames[i])==0) {
      return 0;
    }else{
      return 1;
    }
  }
}

int isOptionnalClause(char* string,SQL_COMMAND *command){

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

void printQuerryStruct (SQL_REQUEST *sqlQuerry){

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

  printf("Structure Where:\n");
  printf("target value :%s\n",sqlQuerry->where.targetValue );
  printf("Operator :%s\n",sqlQuerry->where.operatorField );
  printf("source value:%s\n",sqlQuerry->where.sourceValue );
}

void splitString(char* sqlQuerry,char stringArray[][maxStringSize],int *arraySize, const char *splitCharacter){

  char *token, *cpySqlQuerry;
  int counter = 0; //création de l'index du tableau
  cpySqlQuerry = strdup(sqlQuerry);  //On réserve la mémoire de la string

  while (token = strsep(&cpySqlQuerry, splitCharacter)){//Tant qu'il est possible de segmenter la chaine, affectation de la valeur de la string dans un token
    if (strcmp(token,"INSERT")==0 || strcmp(token,"INTO")==0){
      strcpy(stringArray[0],"INSERT INTO");//gestion de l'exception de INSERT INTO
      counter = 1;
    }else if (strcmp(token,"DELETE")==0 || strcmp(token,"FROM")==0){
      strcpy(stringArray[0],"DELETE FROM");//gestion de l'exception de DELETE FROM
      counter = 1;
    }else{
    strcpy(stringArray[counter],token);// Copie de la valeur du token dans l'index en cours du tableau.
    counter++; //incrément de l'index du tableau
    }
  }
  *arraySize = counter;
  free(cpySqlQuerry);
}

int isValidSqlQuerry (char *sqlQuerry, char stringArray[][maxStringSize],int arraySize,SQL_COMMAND *command){

  //Le nombre d'éléments du tableau doit toujours être un multiple de 2, sinon la paire clause / argument n'est pas respecté.
  /*if (arraySize % 2 != 0){
    printf("Nombre d'arguments non valide");
    return 1;
  }*/
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
SQL_COMMAND identifyCommand (char stringArray[][maxStringSize],SQL_COMMAND commands[nbMaxCommand]){

  for (size_t i = 0; i < nbMaxCommand; i++) {
    //On identifie la commande par la correspondance de la première clause qui est caractéristique de la commande
    if (strcmp(stringArray[0],commands[i].commandName)==0) {
      return commands[i];
    }
  }
    //Si il est impossible d'indentifier la commande, on renvoie une commande vide.
    SQL_COMMAND emptySqlCommand = {"INVALIDE",0,{},0,{}};
    return emptySqlCommand;
}

void mapStringArray (char targetArray[][maxStringSize],char sourceArray[][maxStringSize],int size){

  for (size_t i = 0; i < size; i++) {
    strcpy(targetArray[i],sourceArray[i]);
  }
}

WHERE_CLAUSE getWhereClause (char stringArray[][maxStringSize],int arraySize){

  WHERE_CLAUSE where = (WHERE_CLAUSE){.targetValue="",.operatorField="",.sourceValue=""};
  for (size_t i = 0; i < arraySize; i++) {
    if (strcmp(stringArray[i],"WHERE")==0) { //On cherche la clause WHERE dans le tableau

      int whereSize = 0;
      char splitedWhere[maxArraySize][maxStringSize];

      //TODO : Gérer les autres opérateurs.
      splitString(stringArray[i+1],splitedWhere,&whereSize,"=");//Découpage de l'argument de la clause WHERE

      //On affecte les aguments découpés dans la struct WHERE_CLAUSE.
      strcpy(where.targetValue,splitedWhere[0]);
      strcpy(where.operatorField,"=");
      strcpy(where.sourceValue,splitedWhere[1]);

    }
  }
  return where;
}

void removeChar (char string[], char removedChar){

  int i,j;
  for (size_t i = 0; string[i] != 0; ++i) {//On itère sur tous les caractères de la chaine.
    while (string[i]==removedChar) {// Tant qu'on trouve le caractère à supprimer.
      j=i;//j devient l'index du caractère à supprimer.
      while(string[j]!=0){//Tant que le caractère à supprimer n'est pas le caractère de fin de chaine.
        string[j]=string[j+1];//Le caractère à supprimer est remplacé par le caractère suivant.
        ++j;

      }
    }
  }
}
