#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "apprentissage.h"

double *W[TAILLE][TAILLE];
int cat[TAILLE][TAILLE];
int taille_desc;

void init(int n){                                       // n est la taille du vecteur d'entree qui designe le descripteur
    srand(time(NULL));
    int i,j,k;
    for(i=0;i<TAILLE;i++){
        for(j=0;j<TAILLE;j++){
            W[i][j]=(double *)(malloc(n * sizeof(double)));
            for(k=0;k<n;k++)
                W[i][j][k] = rand()/(double)RAND_MAX;                // initialisation des neurones de la carte à des valeurs aléatoires.
            cat[i][j] = 0;                                             // initialisation de la matrice des catégories.
        }
    }
}

void vainqueur(double *v, int label, int *l, int *c, int test){      // v est le descripteur, l et c désignent la ligne et la colonne où se trouve le neurone vainqueur.
    int i, j, k;
    double mind = 0;        //distance minimale (activation minimale).
    for(i = 0; i < taille_desc; i++)
        mind += (v[i] - W[0][0][i]) * (v[i] - W[0][0][i]);      // On initialise la valeur de l'activation minimale sur l'activation de la première cellule de la carte et on comparera avec les autres cellules.
    //mind = sqrt(mind);
    *l = 0; *c = 0;
    for(i = 0; i < TAILLE; i++){
        for(j = 0; j < TAILLE; j++){
            double curd = 0;
            for(k = 0; k < taille_desc; k++)
                curd += (v[k] - W[i][j][k]) * (v[k] - W[i][j][k]);  // On calcule l'activation de la cellule actuelle.
            //curd = sqrt(curd);
            if(curd < mind){
                *l = i;
                *c = j;
                mind = curd;    // Si on trouve une plus petite activation, on associe ses coordonnées et sa valeur à celles du neurone vainqueur jusqu'à trouver la plus petite activation.
            }
        }
    }
    if(!test)
        cat[*l][*c] = label;        // modification de la categorie du neurone vainqueur
}

double sigma(int t){   // t désigne le temps, à chaque itération de t on traite une nouvelle image.
    double res = 1 / log(log(t+15)) - 2;
    return res;
}

double fn_gamma(int l, int c, int i, int j, int t){            // l et c designent la position du neurone vainqueur
                                                               // i et j designent la position du neurone ij du reseau
                                                               // t désigne le temps
    double sig = sigma(t);                                     // appel à la fonction sigma
    int d_2= (j - c) * (j - c) + (i - l) * (i - l);         // d_2 est le carré de la distance entre le neurone vainqueur et le neurone ij
    double res = (1 / (sig * sqrt(2 * PI))) * exp(-d_2 / (2 * sig * sig));
    return res;
}

double alpha(int t){   // t désigne le temps
    double res = 1 / log(log(t+15)) - 2;
    return res;
}

void apprentissage_neurone(double *v, int l, int c, int t){
    // v est le descripteur d'entrée
    // l et c désignent la position du neurone vainqueur
    // t désigne le temps
    int i,j,k;
    for(i=0;i<TAILLE;i++){
        for(j=0;j<TAILLE;j++){
            for(k=0;k<taille_desc;k++)
                W[i][j][k] += alpha(t)*fn_gamma(l,c,i,j,t)*(v[k]-W[i][j][k]);
        }
    }
}

void apprentissage_carte(FILE *file){               // file est un fichier qui contient les descripteurs des images et la categorie de chaque image.
                                                    // il s'agit d'un même descripteur qui sera utilisé pour toutes les images.
    int nb_lignes;
    int i, t;
    int label;
    fscanf(file, "%d", &nb_lignes);                 // nombre de descripteurs des images.
    fscanf(file, "%d", &taille_desc);               // taille d'un descripteur.
    double* descripteur;
    descripteur = (double*)malloc(taille_desc * sizeof(double));
    init(taille_desc);                              // appel à la fonction init pour initialiser la carte et la matrice des catégories.
    for(t = 1; t <= nb_lignes; t++){
        //printf("apprentissage en cours, t = %d\n", t);
        fscanf(file, "%d", &label);                 // label indique la catégorie de l'image.
        for(i = 0; i < taille_desc; i++)
            fscanf(file, "%lf", &descripteur[i]);   // On extrait chaque descripteur à partir du fichier passé en argument.
        int l = 0, c = 0;
        vainqueur(descripteur,label,&l,&c,0);         // On cherche le neurone vainqueur à l'aide du descripteur qu'on a extrait.
        apprentissage_neurone(descripteur,l,c,t);   // Connaissant le neurone vainqueur, on effectue l'apprentissage avec ce descripteur qu'on a extrait.
    }
}

///test
double calcul_precision(FILE* file){           // Calcul de la precision de la carte
    int i, j, l, c, nb_test, label, nb_correct = 0;
    int taille;
    double res = 0.0;
    fscanf(file, "%d", &nb_test);
    fscanf(file, "%d", &taille);
    double* desc = (double*)malloc(sizeof(double) * taille);
    FILE* guess = fopen("guess.txt", "w");
    FILE* tests = fopen("tests.txt", "r");
    fprintf(guess, "%d\n", nb_test);
    for(i = 0; i < nb_test; i++){
        fscanf(file, "%d", &label);
        for(j = 0; j < taille; j++)
            fscanf(file, "%lf", &desc[j]);
        vainqueur(desc, label, &l, &c, 1);
        if(cat[l][c] == label)
            nb_correct++;
        double accur=0.0;
        double* mindis = (double*)malloc(sizeof(double) * 4);

        int i,j,k;
        for(i=0;i<4;i++)
            mindis[i] = 0;

        for(i = 0; i < TAILLE; i++){
            for(j = 0; j < TAILLE; j++){
                double curd = 0;
                for(k = 0; k < taille_desc; k++)
                    curd += (desc[k] - W[i][j][k]) * (desc[k] - W[i][j][k]);  // On calcule l'activation de la cellule actuelle.
                //curd = sqrt(curd);
                if(curd < mindis[cat[i][j]] || mindis[cat[i][j]] == 0){
                    mindis[cat[i][j]] = curd;    // Si on trouve une plus petite activation, on associe ses coordonnées et sa valeur à celles du neurone vainqueur jusqu'à trouver la plus petite activation.
                }
            }
        }
        accur=100*(1/mindis[cat[l][c]] / (double)(1/mindis[1] + 1/mindis[2] + 1/mindis[3]));

        char name[50];
        fscanf(tests,"%s", name);
        fprintf(guess, "%s %d %d\n", name, cat[l][c], (int)floor(accur+0.5));
    }
    res = nb_correct / (double)nb_test;
    return res * 100.0;
}

///enregistrement
void affiche_carte() {        //Affichage de la carte.
    FILE* file = fopen("carte.txt","w");
    int i,k,j;
    for(i=0; i<TAILLE; i++) {
        for(j=0; j<TAILLE; j++) {
            for(k=0; k<taille_desc; k++)
                fprintf(file, "%lf ", W[i][j][k]);
            fprintf(file,"\n");
        }
    }
    fclose(file);
}

void affiche_carte_cat() {              // Affichage de la matrice de catégories.
    FILE* file = fopen("carte_cat.txt","w");
    int i,j;
    fprintf(file, "%d\n", TAILLE);
    for(i=0; i<TAILLE; i++){
        for(j=0; j<TAILLE; j++)
            fprintf(file, "%d ", cat[i][j]);
        fprintf(file, "\n");
    }
    fclose(file);
}

void remplir_carte() {
    int i,j,k;
    FILE* file = fopen("carte.txt","r");
    if(file == NULL) exit(-1);
    for(i=0; i<TAILLE; i++){
        for(j=0; j<TAILLE; j++){
            fscanf(file, "%d", &cat[i][j]);
            for(k=0; k<taille_desc; k++)
                fscanf(file, "%lf", &W[i][j][k]);
        }
    }
    fclose(file);
}

///debug
void show_max_weights(){
    printf("size is %d\n", taille_desc);
    int i,j,k;
    for(i = 0;i<TAILLE;i++){
        for(j = 0; j<TAILLE; j++){
            double m = 0.0;
            for(k = 0;k<taille_desc;k++)
                if(abs(W[i][j][k]) > m) m = W[i][j][k];
            printf("%lf ", m);
        }
        printf("\n");
    }
}

void show_cat(){            // Affichage de la matrice cat des catégories.
    int i,j;
    for(i=0;i<TAILLE;i++){
        for(j=0;j<TAILLE;j++)
            printf("%d ", cat[i][j]);
        printf("\n");
    }
}
