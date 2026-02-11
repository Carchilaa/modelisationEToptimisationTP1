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

	return rval;
}
