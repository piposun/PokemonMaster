#ifndef HEADER_USECASES
#define HEADER_USECASES
/*! \file lusescases.h
    \brief Fonction dediees a chaque usescases.
    \version 0.1

    Permet d'executer les differentes commandes SQL et de guider l'operateur dans celle-ci.
*/

#define SizeName  20 // Taille de la colonne nom
#define SizeNum   8 // Taille de la colonne numero

void PokemonList(int pokeId); /*prototype fonction: Afficher la liste de tous les pokemons*/
void MyPokemonList(void); /*prototype fonction: Afficher la liste des pokemons possedes*/
void PokemonProfil(); /*prototype fonction: Afficher le profil detaille d'un pokemon*/
void UpdatePokemonList(); /*prototype fonction: Ajouter un pokemon a ma liste de pokemon posseder*/
void AllCouplingPossibilitiesPokemonList(); /*prototype fonction: Acceder a la liste des compatibilites d'accouplement d'un pokemon*/
void MyCouplingPossibilitiesPokemonList(); /*prototype fonction: Acceder a la liste des pokemons possedes et compatibles en accouplement d'un pokemon*/
void UpdatePokemon(); /*prototype fonction: Modifier une caracteristique d'un pokemon*/
void AddPokemon(); /*prototype fonction: Ajouter un pokemon dans la base*/
void DeletePokemon(); /*prototype fonction: Supprimer un pokemon dans la base*/
void Administrator(); /*prototype fonction: Passage en mode administration*/

#endif /* HEADER_USECASES */
