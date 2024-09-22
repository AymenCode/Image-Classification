#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "apprentissage.h"

char desc[100] = "texture"; // on peut changer le nom du descripteur (color ou bien texture)
char cmd[100] = "";
char test[100];
char file_name[100];
bool lancer_desc = false;
int nb_iterations = 50;

int i;

int main()
{
    printf("Descripteur: %s \n", desc);

    //verifier si on veut lancer la creation des descripteurs
    if(lancer_desc){
        strcat(cmd, desc);
        strcat(cmd, ".py"); // on colle "py" le nom (color ou texture)
        system("cd");
        system(cmd);  // on lance la commande (color.py ou bien texture.py)
    }

    //apprentissage
    strcpy(file_name, desc);
    strcat(file_name, ".txt"); // filename=color.txt ou bien texture.txt
    for(i=0;i<nb_iterations;i++){
        printf("apprentissage en cours, iteration: %d/%d\n", i+1,nb_iterations);
        system("shuffle.py"); // Pour mélanger à chaque fois
        FILE* file = fopen(file_name,"r"); // Ouvrir le fichier en mode read
        apprentissage_carte(file);//cette fonction enregistre les poids des neurones dans un fichier texte
    }

    affiche_carte_cat(); //cette fonction enregistre la matrice des categories apres apprentissage dans un fichier texte
    system("afficher_carte.py");// vette fonction affiche la carte
    show_cat();
    //show_max_weights();
    //remplir_carte_cat();
    //test
  strcpy(test, desc);
   strcat(test, "_test.txt");
   FILE* test_file = fopen(test,"r");
   //calculer la précision
   double precision = calcul_precision(test_file);
    printf("Precision: %lf\n ", precision);

    return 0;
}
