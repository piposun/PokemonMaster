#ifndef HEADER_SQL_PARSER
#define HEADER_SQL_PARSER

#define maxArraySize (15)
#define maxStringSize (30)
#define nbMaxClauses (8)
#define nbMaxCommand (5)
/*! \enum SQL_TYPE
    \ brief Renseigne le type de commande.
*/
typedef enum{
  SELECT = 0, /**< Indique que le type de commande est SELECT */
  INSERT_INTO, /**< Indique que le type de commande est INSERT INTO */
  UPDATE, /**< Indique que le type de commande est UPDATE */
  DELETE, /**< Indique que le type de commande est DELETE */
  CREATE_TABLE,  /**< Indique que le type de commande est CREATE_TABLE */
  UNDIFIENED /**< Indique que le type de commande n'a pas pu être définie */
}SQL_TYPE;

/*! \struct WHERE_CLAUSE
    \brief Structure représentant une clause WHERE.
*/
typedef struct {
  char targetValue[maxStringSize]; /**< Est la valeur cible de l'opération */
  char operatorField[maxStringSize]; /**< Est l'opérateur de l'opération */
  char sourceValue[maxStringSize]; /**< Est la valeur source de l'opération */
}WHERE_CLAUSE;

/*! \struct SQL_REQUEST
    \brief Structure représentant une requete à la base de données.
*/
typedef struct{
  SQL_TYPE sqlType; /**< Enumération représentant le type de requète. */
  char* nameTable; /**< Nom de la table affectée par la requète. */
  int nbArgs; /**< Nombre d'arguments de la requète. */
  int nbValues; /**< Nombre de valeurs de la requète. */
  char listArgs[maxArraySize][maxStringSize]; /**< Tableau contenant les arguments de la requète */
  char listValues[maxArraySize][maxStringSize]; /**< Tableau contenant les valeurs de la requète */
  WHERE_CLAUSE where; /**< Structure représentant les champs de la clase WHERE. */
}SQL_REQUEST;

/*! \struct SQL_COMMAND
    \brief Structure représentant une commande SQL.
*/
typedef struct{
  char* commandName; /**< Indique que le type de commande est SELECT */
  int nbMandatoryClauses; /**< Indique que le type de commande est SELECT */
  char* mandatoryClauseNames[nbMaxClauses]; /**< Indique que le type de commande est SELECT */
  int nbOptionnalClauses; /**< Indique que le type de commande est SELECT */
  char* optionnalClauseNames[nbMaxClauses]; /**< Indique que le type de commande est SELECT */
}SQL_COMMAND;

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

/*! \fn  printQuerryStruct(SQL_REQUEST *sqlQuerry)
    \brief Cette fonction affiche le contenu de la structure sqlRequest passée en paramètre.
    \param *sqlQuerry instance de la structure à afficher.
    \return Cette fonction ne renvoie rien.
*/
void printQuerryStruct (SQL_REQUEST *sqlQuerry);

/*! \fn  printStringArray(char stringArray[][maxStringSize],int arraySize)
    \brief Cette fonction affiche le contenu du tableau de chaine de char passé en paramètre.
    \param stringArray Tableau à afficher.
    \param arraySize taille du tableau à afficher.
    \return Cette fonction ne renvoie rien.
*/
void printStringArray(char stringArray[][maxStringSize],int arraySize);

/*! \fn  printSqlCommand(SQL_COMMAND *command);
    \brief Cette fonction affiche le contenu de la structure SQL_COMMAND passée en paramètre.
    \param command reference de la structure SQL_COMMAND à afficher.
    \return Cette fonction ne renvoie rien.
*/
void printSqlCommand(SQL_COMMAND *command);

/*! \fn  isMandatoryClause(char* string,SQL_COMMAND *command);
    \brief Cette fonction vérifie si la chaine de char passée en paramètre correspond à une des clauses obligatoire de la commande SQL passée en paramètre
    \param string Clause à analyser.
    \param command Reference à la commande SQL.
    \return Cette fonction renvoie un entier de valeur 0 si string est une clause obligatoire de command. Autrement, renvoie 1.
*/
int isMandatoryClause(char* string,SQL_COMMAND *command);

/*! \fn isOptionnalClause(char* string,SQL_COMMAND *command)
    \brief Cette fonction vérifie si la chaine de char passée en paramètre correspond à une des clauses Optionnelle de la commande SQL passée en paramètre.
    \param string Clause à analyser.
    \param command Reference à la commande SQL.
    \return Cette fonction renvoie un entier de valeur 0 si string est une clause Optionnelle de command. Autrement, renvoie 1.
*/
int isOptionnalClause(char* string,SQL_COMMAND *command);

/*! \fn isValidSqlQuerry (char *sqlQuerry,char stringArray[][maxStringSize],int arraySize, SQL_COMMAND *command)
    \brief Cette fonction vérifie la validité d'une requète SQL.
    \param sqlQuerry requete SQL qui sera analysée par la fonction.
    \param stringArray tableau de chaine contenant la requete à analyser.
    \param arraySize taille du tableau de chaine.
    \param command Commande SQL correspondant à la requète.
*/
int isValidSqlQuerry (char *sqlQuerry,char stringArray[][maxStringSize],int arraySize, SQL_COMMAND *command);

/*! \fn identifyCommand (char stringArray[][maxStringSize], SQL_COMMAND commands[nbMaxCommand])
    \brief Cette fonction identifie la correspondance entre une requete brut et une commande SQL disponible.
    \param stringArray tableau de chaine contenant la requète SQL brut.
    \param commands tableau de commandes SQL contenant toutes les commandes gérées par le parseur.
    \return Retourne la commande SQL correspondant à la requète.
*/
SQL_COMMAND identifyCommand (char stringArray[][maxStringSize], SQL_COMMAND commands[nbMaxCommand]);

/*! \fn getSqlRequest (char *sqlQuerry)
    \brief Cette fonction va parser la requète en entrée et renvoyer cette requète sous forme strcuturé.
    \param sqlQuerry Requète SQL à Parser.
    \return Renvoie la structure correspondant à la chaine en entrée. Si la chaine d'entrée est invalide, les champs de la structure seront initialisés à 0.
*/
SQL_REQUEST getSqlRequest (char *sqlQuerry);

/*! \fn getWhereClause (char stringArray[][maxStringSize],int arraySize)
    \brief Cette fonction va détecter la clause WHERE et la convertir en structure.
    \param stringArray Tableau de chaines de caractère contenant le WHERE.
    \param arraySize taille du tableau de chaines.
    \return Renvoie la structure correspondant au WHERE. Si il n'y a pas de WHERE dans le tableau, les champs de la structure renvoyé sont vides.
*/
WHERE_CLAUSE getWhereClause (char stringArray[][maxStringSize],int arraySize);

/*! \fn removeChar (char string[], char removedChar)
    \brief Cette fonction retire le caractère removeChar de la chaine de caractère string
    \param string Chaine de caractère contenant le caractère à supprimer.
    \param removedChar caractère qui doit être supprimé de la chaine.
*/
void removeChar (char string[], char removedChar);

/*! \fn replaceChar (char string[], char searchChar, char replaceChar)
    \brief Cette fonction remplace le caractère searchChar de la chaine de caractère string par replaceChar
    \param string Chaine de caractère contenant le caractère à modifier.
    \param searchChar caractère qui doit être modifié de la chaine.
    \param replaceChar caractère de remplacement.
*/
void replaceChar (char string[], char searchChar, char replaceChar);

/*! \fn feedCreateTableStruct(char stringArray[][maxStringSize],int arraySize,SQL_REQUEST *request)
    \brief Cette fonction renseigne les champs de la structure passée en paramètre.
    \param stringArray Tableau de chaines de caractères contenant les données de la requète.
    \param arraySize Taille du tableau de caractères.
    \param request Pointeur de la structure de SQL_REQUEST qui sera remplie.
*/
void feedCreateTableStruct(char stringArray[][maxStringSize],int arraySize,SQL_REQUEST *request);

#endif /* HEADER_SQL_PARSER */
