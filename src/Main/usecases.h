#ifndef HEADER_USECASES
#define HEADER_USECASES
/*! \file lusescases.h
    \brief Fonction dediees a chaque usescases.
    \version 0.1

    Permet d'executer les differentes commandes SQL et de guider l'operateur dans celle-ci.
*/

#define SizeName  20 // Taille de la colonne nom
#define SizeNum   8 // Taille de la colonne numero

int choixPokemon(int *pokeId, char *pokeName); /*prototype fonction: Choix d'un pokemon par l'operateur*/
void pokemonList(int pokeId); /*prototype fonction: Afficher la liste de tous les pokemons*/
void myPokemonList(void); /*prototype fonction: Afficher la liste des pokemons possedes*/
void pokemonProfil(); /*prototype fonction: Afficher le profil detaille d'un pokemon*/
void updatePokemonList(); /*prototype fonction: Ajouter un pokemon a ma liste de pokemon posseder*/
void allCouplingPossibilitiesPokemonList(); /*prototype fonction: Acceder a la liste des compatibilites d'accouplement d'un pokemon*/
void myCouplingPossibilitiesPokemonList(); /*prototype fonction: Acceder a la liste des pokemons possedes et compatibles en accouplement d'un pokemon*/
void updatePokemon(); /*prototype fonction: Modifier une caracteristique d'un pokemon*/
void addPokemon(); /*prototype fonction: Ajouter un pokemon dans la base*/
void deletePokemon(); /*prototype fonction: Supprimer un pokemon dans la base*/
void administrator(); /*prototype fonction: Passage en mode administration*/

#endif /* HEADER_USECASES */
