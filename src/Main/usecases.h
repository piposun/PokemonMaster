#ifndef HEADER_USECASES
#define HEADER_USECASES
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
    \brief Cette fonction traite le choix et la saisie du nom ou id d'un pokemon par l'utilisateur .

    \param pokeId pointeur sur variable contenant l'id du pokemon.
    \param pokeName pointeur sur variable contenant le nom du pokemon.
    \return La fonction renvoie un entier de valeur 0 si la saisie s'est deroulee sans erreur et 1 si erreur de saisie
*/
int choixPokemon(int *pokeId, char *pokeName);

/*! \fn void pokemonList(int pokeId);
    \brief Cette fonction commande et affiche la liste de tous les pokemons.

    \param pokeId pointeur sur variable contenant l'id du pokemon.
*/
void pokemonList(int pokeId);

/*! \fn void myPokemonList(void);
    \brief Cette fonction commande et affiche la liste des pokemons possedes par l'utilisateur.

*/
void myPokemonList(void);

/*! \fn void pokemonProfil(void);
    \brief Cette fonction commande et affiche le profil detaille d'un pokemon demande par l'utilisateur.

*/
void pokemonProfil();

/*! \fn void updatePokemonList(void);
    \brief Cette fonction commande l'ajout d'un pokemon a la liste des pokemons possedes par l'utilisateur.

*/
void updatePokemonList();

/*! \fn void allCouplingPossibilitiesPokemonList();
    \brief Cette fonction commande et affiche la liste des compatibilitees d'accouplement d'un pokemon saisi par l'utilisateur.

*/
void allCouplingPossibilitiesPokemonList();

/*! \fn myCouplingPossibilitiesPokemonList();
    \brief Cette fonction commande et affiche la liste des pokemon possedes et compatibles en accouplement d'un pokemon saisi par l'utilisateur.

*/
void myCouplingPossibilitiesPokemonList();

/*! \fn void updatePokemon();
    \brief Cette fonction commande et modifie une caracteristique pokemon saisie par l'administrateur.

*/
void updatePokemon();

/*! \fn void addPokemon();
    \brief Cette fonction commande l'ajout d'un pokemon dans la base de donnees par l'administrateur.
*/
void addPokemon();

/*! \fn void deletePokemon();
    \brief Cette fonction commande la suppression d'un pokemon dans la base de donnees par l'administrateur, placement du "#" dans le fichier.
*/
void deletePokemon();


/*! \fn void administrator();
    \brief Cette fonction TODO.

*/
void administrator(); /*prototype fonction: Passage en mode administration*/

#endif /* HEADER_USECASES */
