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

int KP_greedy(dataSet *dsptr) {
  int n = dsptr->n;

  STR *str = (STR *)malloc(sizeof(STR) * n);

  for (int i = 0; i < n; i++) {
    str[i].a = dsptr->a[i];
    str[i].c = dsptr->c[i];
    str[i].ratio = (double)dsptr->c[i] / dsptr->a[i];
    str[i].index = i;
  }

  qsort(str, n, sizeof(STR), comparaison);

  double *x = malloc(n * sizeof(double));
  for (int i = 0; i < n; i++) {
    x[i] = 0;
  }

  double b = dsptr->b;

  for (int j = 0; j < n; j++) {
    if (b == 0) {
      break;
    }

    if (b >= str[j].a) {
      x[j] = 1;
      b = b - str[j].a;
    }
  }

  // --- AFFICHAGE GLOUTON ---
  printf("\n[--- HEURISTIQUE GLOUTONNE ---]\n");
  int cout_final = 0;
  int poids_total = 0;
  
  double *x_orig = calloc(n, sizeof(double));
  for (int j = 0; j < n; j++) {
    if (x[j] > 0) {
      x_orig[str[j].index] = x[j];
      cout_final += str[j].c;
      poids_total += str[j].a;
    }
  }

  printf("> Solution x (indices d'origine) : [ ");
  for (int i = 0; i < n; i++) printf("%.0f ", x_orig[i]);
  printf("]\n");
  
  printf("> Capacite utilisee : %d / %d\n", poids_total, dsptr->b);
  printf("> Profit total      : %d\n", cout_final);
  
  free(x_orig);
  free(str);
  free(x);
  
  return cout_final;
}

double KP_LP(dataSet *dsptr) {
  int n = dsptr->n;

  STR *str = (STR *)malloc(sizeof(STR) * n);

  for (int i = 0; i < n; i++) {
    str[i].a = dsptr->a[i];
    str[i].c = dsptr->c[i];
    str[i].ratio = (double)dsptr->c[i] / (double)dsptr->a[i];
    str[i].index = i;
  }

  qsort(str, n, sizeof(STR), comparaison);

  double *x = malloc(n * sizeof(double));
  for (int i = 0; i < n; i++) {
    x[i] = 0;
  }

  double b = dsptr->b;

  for (int j = 0; j < n; j++) {
    if (b == 0) {
      break;
    }
    x[j] = MIN((b / str[j].a), 1);
    b = b - (x[j] * str[j].a);
  }

  // --- AFFICHAGE RELAXATION LINEAIRE ---
  printf("\n[--- RELAXATION LINEAIRE ---]\n");
  double cout_final = 0.0;
  
  double *x_orig = calloc(n, sizeof(double));
  for (int j = 0; j < n; j++) {
    x_orig[str[j].index] = x[j];
    cout_final += x[j] * str[j].c;
  }

  printf("> Solution x (indices d'origine) : [ ");
  for (int i = 0; i < n; i++) {
    if (x_orig[i] == 1.0 || x_orig[i] == 0.0) {
      printf("%.0f ", x_orig[i]);
    } else {
      printf("\033[1;31m%.2f\033[0m ", x_orig[i]); 
    }
  }
  printf("]\n");
  
  printf("> Profit (Borne Sup): %.2f\n", cout_final);

  free(x_orig);
  free(str);
  free(x);
  
  return cout_final;
}

int KP_dynamic(dataSet *dsptr) {
  int n = dsptr->n;
  int b = dsptr->b;

  STR *str = (STR *)malloc(sizeof(STR) * n);
  for (int i = 0; i < n; i++) {
    str[i].a = dsptr->a[i];
    str[i].c = dsptr->c[i];
    str[i].ratio = (double)dsptr->c[i] / dsptr->a[i];
    str[i].index = i;
  }

  int *D = malloc((b + 1) * sizeof(int));
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
    while (y > 0 && Z[y] == Z[y - 1]) {
      y = y - 1;
    }

    if (y > 0 && D[y] > 0) {
      x[D[y] - 1] = 1;
      y = y - str[D[y] - 1].a;
    } else {
      break;
    }
  }

  int profit_optimal = Z[b];

  // --- AFFICHAGE DP ---
  printf("\n[--- PROGRAMMATION DYNAMIQUE ---]\n");
  printf("> Solution x optimale : [ ");
  for (int i = 0; i < n; i++) {
    printf("%.0f ", x[i]);
  }
  printf("]\n");
  printf("> Profit Optimal Exact : %d\n", profit_optimal);

  free(D);
  free(Z);
  free(Z_prime);
  free(x);
  free(str);

  return profit_optimal;
}

int KP_VP(dataSet *dsptr){
  int greedy_solution = KP_greedy(dsptr);
  double relax_solution = KP_LP(dsptr);
  int n = dsptr->n;
  int b = dsptr->b;

  STR *str = (STR *)malloc(sizeof(STR) * n);
  for (int i = 0; i < n; i++) {
    str[i].a = dsptr->a[i];
    str[i].c = dsptr->c[i];
    str[i].ratio = (double)dsptr->c[i] / (double)dsptr->a[i];
    str[i].index = i;
  }

  qsort(str, n, sizeof(STR), comparaison);

  int p = -1;
  int poids_courrant = 0;

  for(int j = 0; j<n; j++){
    if(poids_courrant + str[j].a > b){
      p = j;
      break;
    }
    poids_courrant += str[j].a;
  }

  if(p == -1){
    printf("\n[--- PREPROCESSING ---]\n");
    printf("Tous les objets rentrent dans le sac. Pas besoin de reduire.\n");
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

  int *x_statut = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
      x_statut[i] = -1;
  }

  for (int j = 0; j < n; j++) {
      double c_barre = fabs(str[j].c - (ratio_p * str[j].a));

      if (c_barre >= ecart) {
          variables_fixees++;
          int index_original = str[j].index;

          if (j < p) {
              x_statut[index_original] = 1;
              b_reduit = b_reduit - str[j].a;
              profit_fixe = profit_fixe + str[j].c;
          } else {
              x_statut[index_original] = 0;
          }
      }
  }

  int n_reduit = n - variables_fixees;
  int resultat_final = profit_fixe;

  // --- AFFICHAGE PREPROCESSING ---
  printf("\n[--- PREPROCESSING & REDUCTION ---]\n");
  printf("> Borne Inf (Greedy)   : %d\n", greedy_solution);
  printf("> Borne Sup (LP)       : %.2f\n", relax_solution);
  printf("> Ecart (Gap)          : %.2f\n", ecart);
  
  printf("\n> Objet critique (p)   : index trie %d (Valeur: %.2f, Poids: %.2f)\n", p, cp, ap);
  printf("> Variables fixees     : %d sur %d\n", variables_fixees, n);
  printf("> Capacite sac reduit  : %d (Initiale : %d)\n", b_reduit, dsptr->b);
  printf("> Objets pour le DP    : %d\n", n_reduit);

  if (n_reduit > 0 && b_reduit > 0) {
      
      dataSet dsptr_reduit;
      dsptr_reduit.n = n_reduit;
      dsptr_reduit.b = b_reduit;
      dsptr_reduit.a = (int *)malloc(sizeof(int) * n_reduit);
      dsptr_reduit.c = (int *)malloc(sizeof(int) * n_reduit);

      int idx_reduit = 0;
      for (int i = 0; i < n; i++) {
          if (x_statut[i] == -1) { 
              dsptr_reduit.a[idx_reduit] = dsptr->a[i];
              dsptr_reduit.c[idx_reduit] = dsptr->c[i];
              idx_reduit++;
          }
      }

      int dp_result = KP_dynamic(&dsptr_reduit);
      resultat_final = profit_fixe + dp_result;

      free(dsptr_reduit.a);
      free(dsptr_reduit.c);
      
  } else {
      printf("\nAucun objet supplementaire a evaluer.\n");
  }

  printf("\n> Profit objets fixes  : %d\n", profit_fixe);
  printf(">> PROFIT TOTAL FINAL  : %d <<\n", resultat_final);
  printf("---------------------------------------------------\n");

  free(x_statut);
  free(str);

  return resultat_final;
}