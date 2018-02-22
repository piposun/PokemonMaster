#ifndef HEADER_QUERY
#define HEADER_QUERY

#include "dataBase.h"
#include "sqlCommandParser.h"

/*! \enum DATA_FIELD
    \brief Gere le type de la donnees.
*/
typedef enum {
  CONDITION_EQUAL = 0, /**< Condition egale a */
  CONDITION_GREAT,     /**< Condition superieur a */
  CONDITION_LOW,       /**< Condition inferieur a */
  CONDITION_UNK        /**< Condition inconnu */
}CONDITION;

/*! \struct DescriptorTable
    \brief Structure de l'entête de description des champs pour la reponse d'un requête.
*/
typedef struct {
  int nbField;          /**< Nombre de champ. */
  int sizeRecord;       /**< Taille d'un élement de réponse. */
  DataField *dataField; /**< Pointer des champs de description. */
} DescriptorTable;

/*! \struct Query
    \brief Structure de la réponse à une requête SELECT.
*/
typedef struct {
  DescriptorTable descriptor; /**< Entête de description des champs. */
  int nbRecord;               /**< Nombre de ligne de réponse. */
  char *data;                 /**< Pointer sur la data. */
} Query;

/*! \fn DATA_BASE createDescriptorResult(HeaderTable *header, Query query, char listField[][DATA_FIELD_MAX_CHARACTER])
    \brief Cette fonction cré et initialise l'entête de description des champs pour la réponse.

    \param header Référence de l'entête de la table.
    \param query Référence de la structure de réponse.
    \param listField Référence à la liste des champs de la réponse.
    \param nbField Nombre de champs.
    \return Renvoie DATA_BASE pour indiquer le déroulement de la fonction.
*/
DATA_BASE createDescriptorResult(HeaderTable *header, Query *query, char listField[][maxStringSize], int nbField);

/*! \fn Query * excuteQuery(DataBase *dataBase, char *sql)
    \brief Cette fonction execute la commande SQL.

    \param dataBase Référence de la DataBase.
    \param sql Requête SQL à executer.
    \return Renvoie une réponse si besoin.
*/
Query * excuteQuery(DataBase *dataBase, char *sql);

/*! \fn DATA_FIELD closeQuery(Query *query)
    \brief Cette fonction détruit la référence de la réponse.

    \param query Référence de la réponse.
    \return Renvoie DATA_BASE pour indiquer le déroulement de la fonction.
*/
DATA_BASE closeQuery(Query *query);

/*! \fn DATA_FIELD getTypeQueryById(Query * query, int index)
    \brief Cette fonction renvoie le type de la donnée d'un champ via son index.

    \param dataBase Reference de la réponse.
    \param index Index du champ.
    \return Renvoie le type du champ.
*/
DATA_FIELD getTypeQueryById(Query * query, int index);

/*! \fn int getSizeQueryById(Query * query, int index)
    \brief Cette fonction renvoie la taille d'un champ via son index.

    \param dataBase Reference de la réponse.
    \param index Index du champ.
    \return Renvoie la taille du champ.
*/
int getSizeQueryById(Query * query, int index);


/*! \fn char * getNameQueryById(Query * query, int index)
    \brief Cette fonction renvoie le nom d'un champ via son index.

    \param dataBase Reference de la réponse.
    \param index Index du champ.
    \return Renvoie le nom du champ.
*/
char * getNameQueryById(Query * query, int index);

/*! \fn char * getDataQueryById(Query * query, int index)
    \brief Cette fonction renvoie la data d'un champ de réponse via les indexs de lignes et de colonnes.

    \param dataBase Reference de la réponse.
    \param row Index de la ligne.
    \param colum Index de la colonne.
    \return Renvoie la data d'un champ.
*/
char * getDataQueryById(Query * query, int row, int colum);

/*! \fn char * getDataQueryByName(Query * query, char * fieldName)
    \brief Cette fonction renvoie la data d'un champ via l'index de la ligne et le nom du champ.

    \param dataBase Reference de la réponse.
    \param row Index de la ligne.
    \param fieldName Nom du champ.
    \return Renvoie la data d'un champ.
*/
char * getDataQueryByName(Query * query, int row, char * fieldName);

/*! \fn Query * commandSelect(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest)
    \brief Cette fonction execute la commande SELECT.

    \param hedader Référence à l'entête de la table.
    \param file Référence au fichier de la table.
    \param sqlRequest Référence à la structure de la commande SQL
    \return Renvoie la référence de la réponse.
*/
Query * commandSelect(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest);

/*! \fn DATA_BASE commandInsert(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest)
    \brief Cette fonction execute la commande INSERT INTO.

    \param hedader Référence à l'entête de la table.
    \param file Référence au fichier de la table.
    \param sqlRequest Référence à la structure de la commande SQL
    \return Renvoie DATA_BASE pour indiquer le déroulement de la fonction.
*/
DATA_BASE commandInsert(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest);

/*! \fn DATA_BASE commandDelete(HeaderTable *header, FILE *file, Condition *condition);
    \brief Cette fonction execute la commande DELETE.

    \param hedader Référence à l'entête de la table.
    \param file Référence au fichier de la table.
    \param sqlRequest Référence à la structure de la commande SQL
    \return Renvoie DATA_BASE pour indiquer le déroulement de la fonction.
*/
DATA_BASE commandDelete(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest);

/*! \fn DATA_BASE commandUpdate(HeaderTable *header, FILE *file, Condition *condition, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField, char listValue[][DATA_FIELD_MAX_CHARACTER])
    \brief Cette fonction execute la commande UPDATE.

    \param hedader Référence à l'entête de la table.
    \param file Référence au fichier de la table.
    \param sqlRequest Référence à la structure de la commande SQL
    \return Renvoie DATA_BASE pour indiquer le déroulement de la fonction.
*/
DATA_BASE commandUpdate(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest);

/*! \fn DATA_BASE checkCondition(HeaderTable *header, Condition *condition, char *record)
    \brief Cette fonction verifie si la clause WHERE est accomplie.

    \param hedader Référence à l'entête de la table.
    \param where Référence à la structure de la clause.
    \param record Référence à la data d'un enregistrement.
    \return Renvoie DATA_BASE_SUCCESS si la clause est accomplie.
*/
DATA_BASE checkCondition(HeaderTable *header, WHERE_CLAUSE *where, char *record);

/*! \fn int convertCharTypeToSize(char *type)
    \brief Cette fonction convertie le chaine de caractère d'un type en sa taille d'occupation mémoire.

    \param Chaine de caractère d'un type.
    \return Renvoie sa taille mémoire.
*/
int convertCharTypeToSize(char *type);

/*! \fn DATA_FIELD convertCharTypeToType(char *cType)
    \brief Cette fonction convertie le chaine de caractère d'un type en sa valeur d'énum.

    \param Chaine de caractère d'un type.
    \return Renvoie sa valeur d'énum.
*/
DATA_FIELD convertCharTypeToType(char *cType);

/*! \fn void updateTimeHeader(HeaderTable *headerTable)
    \brief Cette fonction met à jour la date de l'entête d'une table.

    \param headerTable Référence de l'entête de la table.
*/
void updateTimeHeader(HeaderTable *headerTable);

/*! \fn Query *query executeCommand(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest)
    \brief Cette fonction execute la bonne commande SQL en fonction du type de la commande.

    \param header Référence de l'entête de la table.
    \param file Référence au fichier de la table.
    \param sqlRequest Référence de la structure de commande SQL.
    \return Renvoie si besoin une réponse.
*/
Query * executeCommand(HeaderTable *header, FILE *file, SQL_REQUEST *sqlRequest);


/*! \fn DATA_BASE commandCreateTable(SQL_REQUEST *sqlRequest)
    \brief Cette fonction execute la commende CREATE TABLE.

    \param sqlRequest Référence de la structure de commande SQL.
    \return Renvoie DATA_BASE pour indiquer le déroulement de la fonction.
*/
DATA_BASE commandCreateTable(SQL_REQUEST *sqlRequest);

#endif /* HEADER_QUERY */
