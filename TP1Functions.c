#include "TP1Functions.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct coup_poids {
  int a; // Poids de l'objet
  int c; // Cout de l'objet
  double ratio;
  int index;
} STR;
int read_TP1_instance(FILE *fin, dataSet *dsptr) {
  int rval = 0;

  // capacite b
  int b;
  // Nombre d'objets
  int n;
  rval = fscanf(fin, "%d,%d\n", &n, &b);
  dsptr->b = b;
  dsptr->n = n;
  dsptr->c = (int *)malloc(sizeof(int) * n);
  dsptr->a = (int *)malloc(sizeof(int) * n);

  int i;
  for (i = 0; i < n; i++)
    rval = fscanf(fin, "%d,%d\n", &(dsptr->c[i]), &(dsptr->a[i]));

  fprintf(stderr,
          "\nInstance file read, we have capacity %d and there is %d items of "
          "values/weights:\n",
          b, n);
  for (i = 0; i < n; i++)
    fprintf(stderr, "%d,%d\n", dsptr->c[i], dsptr->a[i]);
  fprintf(stderr, "\n");

  return rval;
}

int KP_greedy(dataSet *dsptr) {
  int n = dsptr->n;

  STR *str = (STR *)malloc(sizeof(STR) * n);

  for (int i = 0; i < n; i++) {
    str[i].a = dsptr->a[i];
    str[i].c = dsptr->c[i];
    str[i].ratio = (double)dsptr->c[i] / dsptr->a[i];
    str[i].index = i;
  }

  double *x = malloc(n * sizeof(double));
  for (int i = 0; i < n; i++) {
    x[i] = 0;
  }

  double b = dsptr->b;

  for (int j = 0; j < n; j++) {
    if (b == 0) {
      printf("Tableau x final  greedy:\n");
      for (int i = 0; i < dsptr->n; i++) {
        printf("x[%d] = %f\n", i, x[i]);
      }

      int cout_final = 0;

      for (int i = 0; i < dsptr->n; i++) {
        if (x[i] != 0) {
          cout_final += x[i] * str[i].c;
        }
      }

      printf("Cout final de l'algorithme greedy : %d \n", cout_final);
      return cout_final;
    }

    if (b >= str[j].a) {
      x[j] = 1;
      b = b - str[j].a;
    }
  }

  printf("Tableau x final  greedy:\n");
  for (int i = 0; i < dsptr->n; i++) {
    printf("x[%d] = %f\n", i, x[i]);
  }

  int cout_final = 0;

  for (int i = 0; i < dsptr->n; i++) {
    if (x[i] != 0) {
      cout_final += x[i] * str[i].c;
    }
  }

  printf("Cout final de l'algorithme greedy : %d \n", cout_final);
  return cout_final;
}

// Fonction de comparaison qsort

int comparaison(const void *i1, const void *i2) {
  STR *item1 = (STR *)i1;
  STR *item2 = (STR *)i2;

  if (item1->ratio < item2->ratio)
    return 1;
  if (item1->ratio > item2->ratio)
    return -1;

  return 0;
}

double KP_LP(dataSet *dsptr) {
  int n = dsptr->n;

  STR *str = (STR *)malloc(sizeof(STR) * n);

  for (int i = 0; i < n; i++) {
    str[i].a = dsptr->a[i];
    str[i].c = dsptr->c[i];
    str[i].ratio = (double)dsptr->c[i] / dsptr->a[i];
    str[i].index = i;
  }

  // for(int i = 0; i < n; i++){
  //	printf("str[%d] = %f\n", str[i].index, str[i].ratio);
  // }

  // qsort(str, n, sizeof(str), comparaison);

  // printf("Ratio apres le tri: \n");
  // for(int i = 0; i < n; i++){
  //	printf("str[%d] = %f\n", str[i].index, str[i].ratio);
  // }

  double *x = malloc(n * sizeof(double));
  for (int i = 0; i < n; i++) {
    x[i] = 0;
  }

  double b = dsptr->b;

  for (int j = 0; j < n; j++) {
    if (b == 0) {
      printf("Tableau x final Linear:\n");
      for (int i = 0; i < dsptr->n; i++) {
        printf("x[%d] = %f\n", i, x[i]);
      }

      double cout_final = 0;

      for (int i = 0; i < dsptr->n; i++) {
        if (x[i] != 0) {
          cout_final += x[i] * str[i].c;
        }
      }

      printf("Cout final de l'algorithme Relaxation lineaire: %f \n",
             cout_final);
      return cout_final;
    }
    x[j] = MIN((b / str[j].a), 1);
    // printf("Valeur de x[%d] : %f \n", j, x[j]);

    // printf("Valeur de b avant la soustraction %f \n", b);
    b = b - (x[j] * str[j].a);

    // printf("Valeur de b apres la soustraction %f \n", b);
  }

  printf("Tableau x final Linear:\n");
  for (int i = 0; i < dsptr->n; i++) {
    printf("x[%d] = %f\n", i, x[i]);
  }

  double cout_final = 0;

  for (int i = 0; i < dsptr->n; i++) {
    if (x[i] != 0) {
      cout_final += x[i] * str[i].c;
    }
  }

  printf("Cout final de l'algorithme Relaxation lineaire: %f \n", cout_final);
  return cout_final;
}

int KP_dynamic(dataSet *dsptr) {

  int n = dsptr->n;
  // Definition de la structure
  STR *str = (STR *)malloc(sizeof(STR) * n);

  for (int i = 0; i < n; i++) {
    str[i].a = dsptr->a[i];
    str[i].c = dsptr->c[i];
    str[i].ratio = (double)dsptr->c[i] / dsptr->a[i];
    str[i].index = i;
  }

  double b = dsptr->b;
  // D(y) = max{j ⩽ k : xj = 1 pour le probleme Pk(y)}
  int *D = malloc((b + 1) * sizeof(int));

  // Z(y) = valeur optimale du probl`eme Pk(y)
  int *Z = malloc((b + 1) * sizeof(int));

  int *Z_prime = malloc((b + 1) * sizeof(int));
  for (int y = 0; y <= b; y++) {
    Z[y] = 0;
    D[y] = 0;
    Z_prime[y] = 0;
  }

  for (int k = 0; k < n; k++) {
    for (int y = 0; y <= b; y++) {
      Z_prime[y] = Z[y];
    }

    for (int y = str[k].a; y <= b; y++) {
      if ((Z_prime[y - str[k].a] + str[k].c) > Z_prime[y]) {
        D[y] = k + 1;
        Z[y] = MAX(Z_prime[y], (str[k].c + Z_prime[y - str[k].a]));
      }
    }
  }

  double *x = malloc(n * sizeof(double));
  for (int j = 0; j < n; j++) {
    x[j] = 0;
  }

  int y = b;

  while (y > 0) {
    while (Z[y] == Z[y - 1]) {
      y = y - 1;
    }

    if (D[y] > 0) {
      x[D[y] - 1] = 1;
    };
    y = y - str[D[y] - 1].a;
  }

  printf("Tableau x final Dynamic Programming:\n");
  for (int i = 0; i < dsptr->n; i++) {
    printf("x[%d] = %f\n", i, x[i]);
  }

  printf("VALEURS DE D:\n");
  for (int y = 0; y <= b; y++) {
    printf("D[%d] = %d\n", y, D[y]);
  }

  printf("VALEURS DE Z:\n");
  for (int y = 0; y <= b; y++) {
    printf("Z[%d] = %d\n", y, Z[y]);
  }
  return *x;
}

int KP_VP(dataSet *dsptr){

	int rval;
	int greedy_solution = KP_greedy(dsptr);
	double relax_solution = KP_LP(dsptr);
	int n = dsptr->n;
  double *x = malloc(n * sizeof(double));

  STR *str = (STR *)malloc(sizeof(STR) * n);

  for (int i = 0; i < n; i++) {
    str[i].a = dsptr->a[i];
    str[i].c = dsptr->c[i];
    str[i].ratio = (double)dsptr->c[i] / dsptr->a[i];
    str[i].index = i;
  }

  qsort(str, n, sizeof(STR), comparaison);

  int p = -1;
  int poids_courrant = 0;
  int b = dsptr->b;

  for(int j = 0; j<n; j++){
    if(poids_courrant + str[j].a > b){
      p = j;
      break;
    }
    poids_courrant += str[j].a;
  }

  if(p == -1){
    printf("Tous les objets rentrent dans le sac. Pas besoin de reduir");
    free(str);
    return greedy_solution;
  }

  double cp = str[p].c;
  double ap = str[p].a;
  double ratio_p = cp / ap;


  double ecart = relax_solution - (double)greedy_solution;
  int variables_fixees = 0;

  int b_reduit = dsptr->b;
  int profit_fixe = 0;

  for (int i = 0; i < n; i++) {
        x[i] = -1;
    }

    for (int j = 0; j < n; j++) {
        // c_j barre : |cj - (cp/ap)*aj|
        double c_barre = fabs(str[j].c - (ratio_p * str[j].a));

        if (c_barre >= ecart) {
            variables_fixees++;
            int index_original = str[j].index;

            if (j < p) {
                // On force l'objet à 1 (on le met dans le sac)
                x[index_original] = 1;
                b_reduit = b_reduit - str[j].a;
                profit_fixe = profit_fixe + str[j].c;
            } else {
                // On force l'objet à 0 (on le jette)
                x[index_original] = 0;
            }
        }
    }

    printf("Nombre de variables fixees : %d sur %d\n", variables_fixees, n);

    // 6. Créer l'instance réduite et lancer la Programmation Dynamique
    int n_reduit = n - variables_fixees;
    int resultat_final = profit_fixe;

    // S'il reste des objets à évaluer et qu'il reste de la place
    if (n_reduit > 0 && b_reduit > 0) {
        
        // Création du nouveau sac à dos réduit
        dataSet dsptr_reduit;
        dsptr_reduit.n = n_reduit;
        dsptr_reduit.b = b_reduit;
        dsptr_reduit.a = (int *)malloc(sizeof(int) * n_reduit);
        dsptr_reduit.c = (int *)malloc(sizeof(int) * n_reduit);

        int idx_reduit = 0;
        // On remplit le sac réduit en parcourant les objets dans leur ordre d'origine
        for (int i = 0; i < n; i++) {
            if (x[i] == -1) { // Si l'objet n'a pas été fixé
                dsptr_reduit.a[idx_reduit] = dsptr->a[i];
                dsptr_reduit.c[idx_reduit] = dsptr->c[i];
                idx_reduit++;
            }
        }

        // 7. Résoudre le problème réduit avec KP_dynamic
        printf("\n--- LANCEMENT DP SUR PROBLEME REDUIT ---\n");
        printf("Objets restants : %d, Capacite restante : %d\n", n_reduit, b_reduit);
        
        int dp_result = KP_dynamic(&dsptr_reduit); // Étape 10 de l'algorithme du PDF
        
        // Le résultat final est la somme de ce qu'on a pré-rempli + ce que le DP a trouvé
        resultat_final = profit_fixe + dp_result;

        // On libère la mémoire de l'instance réduite
        free(dsptr_reduit.a);
        free(dsptr_reduit.c);
        
    } else {
        // Soit le sac est plein (b_reduit <= 0), soit tous les objets ont été fixés (n_reduit == 0)
        printf("Aucun objet supplementaire a evaluer (Capacite restante: %d, Objets restants: %d).\n", b_reduit, n_reduit);
    }

    printf("\nProfit des objets fixes d'avance : %d\n", profit_fixe);
    printf("--- PROFIT TOTAL FINAL (Apres reduction) : %d ---\n\n", resultat_final);

    // 8. Nettoyage de la mémoire allouée
    free(x);
    free(str);

    return resultat_final;
}
