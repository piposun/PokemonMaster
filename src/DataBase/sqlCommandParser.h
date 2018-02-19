#define maxArraySize (10)
#define maxStringSize (30)
#define nbMaxClauses (8)
#define nbMaxCommand (4)
/*! \enum sqlType
    \ brief Renseigne le type de commande.
*/
typedef enum{
  SELECT = 0,
  INSERT_INTO,
  UPDATE,
  DELETE
}sqlType;

/* TODO
*/

typedef struct {
  char targetValue[maxStringSize];
  char operatorField[maxStringSize];
  char sourceValue[maxStringSize];
}whereClause;
/*! \enum operatorEnum
    \brief Renseigne le type d'opérateur utilisé

typedef enum {

}operatorEnum;
*/

/*! \struct sqlRequest
    \brief Structure représentant une requete à la base de données.
*/
typedef struct{
  sqlType sqlType;
  char* nameTable;
  int nbArgs;
  int nbValues;
  char listArgs[maxArraySize][maxStringSize];
  char listValues[maxArraySize][maxStringSize];
  whereClause where;
}sqlRequest;

/*! \struct sqlCommand
    \brief Structure représentant une commande SQL.
*/
typedef struct{
  char* commandName;
  int nbMandatoryClauses;
  char* mandatoryClauseNames[nbMaxClauses];
  int nbOptionnalClauses;
  char* optionnalClauseNames[nbMaxClauses];
}sqlCommand;
/*! \fn mapStringArray (char targetArray[][maxStringSize],char sourceArray[][maxStringSize],int size)
    \brief Cette fonction tranfere index par index les chaines de caractères du tableau sourceArray vers le tableau targetArray
    \param targetArray tableau cible
    \param sourceArray tableau sourcechar
    \return Pas de retour
*/
void mapStringArray (char targetArray[][maxStringSize],char sourceArray[][maxStringSize],int size);
/*! \fn  splitString(char* sqlQuerry,char stringArray[][maxStringSize],int *arraySize)
    \brief Cette fonction sépare une chaine de characteres par le séparateur " " en tableau de chaine de caractère.
    \param sqlQuerry chaine de characteres à séparer.
    \param stringArray tableau de chaine de characteres recevant les valeurs de la chaine séparé.
    \param *arraySize pointeur d'entier permettant de renseigner la taille du tableau.
    \param splitCharacter caractère de séparation.
    \return Cette fonction ne renvoie rien.
*/
void splitString(char* sqlQuerry,char stringArray[][maxStringSize],int *arraySize,const char *splitCharacter);
/*! \fn  printQuerryStruct(sqlRequest *sqlQuerry)
    \brief Cette fonction affiche le contenu de la structure sqlRequest passée en paramètre.
    \param *sqlQuerry instance de la structure à afficher.
    \return Cette fonction ne renvoie rien.
*/
void printQuerryStruct (sqlRequest *sqlQuerry);
/*! \fn  printStringArray(char stringArray[][maxStringSize],int arraySize)
    \brief Cette fonction affiche le contenu du tableau de chaine de char passé en paramètre.
    \param stringArray Tableau à afficher.
    \param arraySize taille du tableau à afficher.
    \return Cette fonction ne renvoie rien.
*/
void printStringArray(char stringArray[][maxStringSize],int arraySize);
/*! \fn  printSqlCommand(sqlCommand *command);
    \brief Cette fonction affiche le contenu de la structure sqlCommand passée en paramètre.
    \param command reference de la structure sqlCommand à afficher.
    \return Cette fonction ne renvoie rien.
*/
void printSqlCommand(sqlCommand *command);
/*! \fn  isMandatoryClause(char* string,sqlCommand *command);
    \brief Cette fonction vérifie si la chaine de char passée en paramètre correspond à une des clauses obligatoire de la commande SQL passée en paramètre
    \param string Clause à analyser.
    \param command Reference à la commande SQL.
    \return Cette fonction renvoie un entier de valeur 0 si string est une clause obligatoire de command. Autrement, renvoie 1.
*/
int isMandatoryClause(char* string,sqlCommand *command);
/*! \fn isOptionnalClause(char* string,sqlCommand *command)
    \brief Cette fonction vérifie si la chaine de char passée en paramètre correspond à une des clauses Optionnelle de la commande SQL passée en paramètre.
    \param string Clause à analyser.
    \param command Reference à la commande SQL.
    \return Cette fonction renvoie un entier de valeur 0 si string est une clause Optionnelle de command. Autrement, renvoie 1.
*/
int isOptionnalClause(char* string,sqlCommand *command);
/*! \fn isValidSqlQuerry (char *sqlQuerry,char stringArray[][maxStringSize],int arraySize, sqlCommand *command)
    \brief Cette fonction vérifie la validité d'une requète SQL.
    \param sqlQuerry requete SQL qui sera analysée par la fonction.
    \param stringArray tableau de chaine contenant la requete à analyser.
    \param arraySize taille du tableau de chaine.
    \param command Commande SQL correspondant à la requète.
*/
int isValidSqlQuerry (char *sqlQuerry,char stringArray[][maxStringSize],int arraySize, sqlCommand *command);
/*! \fn identifyCommand (char stringArray[][maxStringSize], sqlCommand commands[nbMaxCommand])
    \brief Cette fonction identifie la correspondance entre une requete brut et une commande SQL disponible.
    \param stringArray tableau de chaine contenant la requète SQL brut.
    \param commands tableau de commandes SQL contenant toutes les commandes gérées par le parseur.
    \return Retourne la commande SQL correspondant à la requète.
*/
sqlCommand identifyCommand (char stringArray[][maxStringSize], sqlCommand commands[nbMaxCommand]);
/*! \fn getSqlRequest (char *sqlQuerry)
    \brief Cette fonction va parser la requète en entrée et renvoyer cette requète sous forme strcuturé.
    \param sqlQuerry Requète SQL à Parser.
    \return Renvoie la structure correspondant à la chaine en entrée. Si la chaine d'entrée est invalide, les champs de la structure seront initialisés à 0.
*/
sqlRequest getSqlRequest (char *sqlQuerry);
/*TODO
*/
whereClause getWhereClause (char stringArray[][maxStringSize],int arraySize);
/*TODO
*/
void removeChar (char string[], char removedChar);
