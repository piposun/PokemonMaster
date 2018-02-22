#ifndef HEADER_USECASES
#define HEADER_USECASES

#include "dataBase.h"

/*! \file usescases.h
    \brief Genere toutes les requetes SQL et affiche les donnees en retour.
    \version 0.1

    Permet d'executer les differentes commandes SQL, de guider l'utilisateur vers les donnees souhaitees et d'afficher les donnees.
*/

/*! \def sizeName
    \brief Definition du taille colonne "nom"
*/
#define sizeName  20

/*! \def sizeNum
    \brief Definition de la taille colonne "num" + "hp" + "atk" + "def" + autres chiffres
*/
#define sizeNum   8

/*! \def sizeDesc
    \brief Definition de la taille colonne "desc"
*/
#define sizeDesc   200

/*! \fn int choixPokemon(int *pokeId, char *pokeName);
    \brief Cette fonction traite la saisie d'un pokemon par son nom ou son numero et renvoi l'id correspondant.

    \param pokeId pointeur sur variable contenant l'id du pokemon.
    \return La fonction renvoie un entier de valeur 0 si la saisie s'est deroulee sans erreur et 1 si erreur de saisie
*/
int choicePokemon(int *pokeId, DataBase *dataBase);

/*! \fn void pokemonList(int pokeId);
    \brief Cette fonction commande et affiche la liste de tous les pokemons.

    \param pokeId pointeur sur variable contenant l'id du pokemon.
*/
void pokemonList(int pokeId, DataBase *dataBase);

/*! \fn void myPokemonList(void);
    \brief Cette fonction commande et affiche la liste des pokemons possedes par l'utilisateur.

*/
void myPokemonList(DataBase *dataBase);

/*! \fn void pokemonProfil(void);
    \brief Cette fonction commande et affiche le profil detaille d'un pokemon demande par l'utilisateur.

*/
void pokemonProfil(DataBase *dataBase);

/*! \fn void updatePokemonList(void);
    \brief Cette fonction commande l'ajout d'un pokemon a la liste des pokemons possedes par l'utilisateur.

*/
void updatePokemonList(DataBase *dataBase);

/*! \fn void allCouplingPossibilitiesPokemonList();
    \brief Cette fonction commande et affiche la liste des compatibilitees d'accouplement d'un pokemon saisi par l'utilisateur.

*/
void allCouplingPossibilitiesPokemonList(DataBase *dataBase);

/*! \fn myCouplingPossibilitiesPokemonList();
    \brief Cette fonction commande et affiche la liste des pokemon possedes et compatibles en accouplement d'un pokemon saisi par l'utilisateur.

*/
void myCouplingPossibilitiesPokemonList(DataBase *dataBase);

/*! \fn void updatePokemon();
    \brief Cette fonction commande et modifie une caracteristique pokemon saisie par l'administrateur.

*/
void updatePokemon(DataBase *dataBase);

/*! \fn void addPokemon();
    \brief Cette fonction commande l'ajout d'un pokemon dans la base de donnees par l'administrateur.
*/
void addPokemon(DataBase *dataBase);

/*! \fn void deletePokemon();
    \brief Cette fonction commande la suppression d'un pokemon dans la base de donnees par l'administrateur, placement du "#" dans le fichier.
*/
void deletePokemon(DataBase *dataBase);


/*! \fn void administrator();
    \brief Cette fonction TODO.

*/
void administrator(DataBase *dataBase); /*prototype fonction: Passage en mode administration*/

#endif /* HEADER_USECASES */
