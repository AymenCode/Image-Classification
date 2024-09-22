#ifndef APPRENTISSAGE_H_INCLUDED
#define APPRENTISSAGE_H_INCLUDED
#include <math.h>
#define TAILLE 10
#define PI acos(-1.0)

///apprentissage
void init(int n);
    // Cette fonction initialise les neurones de la carte à des valeurs aléatoires ainsi que la matrice cat des catégories des images.
void vainqueur(double *v, int label, int *l, int *c, int test);
    // Cette fonction trouve le neurone vainqueur sur la carte et nous informe sur ses coordonnées.
double sigma(int t);
    // Calcul de la fonction sigma qu'on utilise dans la fonction gamma.
double fn_gamma(int l, int c, int i, int j, int t);
    // Calcul de la fonction gamma qu'on utilise dans l'apprentissage.
double alpha(int t);
    // Calcul de la fonction alpha qu'on utilise dans l'apprentissage.
void apprentissage_neurone(double *v, int l, int c, int t);
    // Cette fonction effectue l’apprentissage à partir d'un descripteur d'image.
void apprentissage_carte(FILE * file);
    // Cette fonction réalise l'apprentissage de la carte à partir d'un fichier contenant les descripteurs des images.

///test
double calcul_precision(FILE* file);

///enregistrement
void affiche_carte_cat();
    //cette fonction enregistre la matrice des categories apres apprentissage dans un fichier texte
void affiche_carte();
    //cette fonction enregistre les poids des neurones dans un fichier texte
void remplir_carte();
    //cette fonction recupere les poids des neurones a partir d'un fichier texte
///debug
void show_max_weights();
void show_cat();

#endif // APPRENTISSAGE_H_INCLUDED
