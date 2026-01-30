#include "TP1Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include<stdio.h>
typedef struct coup_poids {
		int a; //Poids de l'objet
		int c;//Cout de l'objet 
		double ratio;
		int index;
}STR;
int read_TP1_instance(FILE*fin,dataSet* dsptr)
{
	int rval = 0;

	//capacite b
	int b;
	//Nombre d'objets
	int n;
	rval = fscanf(fin,"%d,%d\n",&n,&b);
	dsptr->b = b;
	dsptr->n = n;
	dsptr->c = (int*)malloc(sizeof(int)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);


	int i;
	for( i = 0 ; i < n ; i++)
		rval = fscanf(fin,"%d,%d\n",&(dsptr->c[i]),&(dsptr->a[i]));

	fprintf(stderr,"\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",
			b,n);
	for( i = 0 ; i < n ; i++)
		fprintf(stderr,"%d,%d\n",dsptr->c[i],dsptr->a[i]);
	fprintf(stderr,"\n");


	return rval;
}

int KP_greedy(dataSet* dsptr)
{
	int rval = 0;	

	return rval;
}

//Fonction de comparaison qsort

int comparaison(const void *i1, const void *i2){
	STR *item1 = (STR *)i1;
	STR *item2 = (STR *)i2;

	if(item1->ratio < item2->ratio) return 1;
	if(item1->ratio > item2->ratio) return -1;

	return 0;
}

int KP_LP(dataSet* dsptr)
{
	int rval = 0;
	int n = dsptr->n
	
	STR *str = (STR*)malloc(sizeof(STR)*n);

	for(int i = 0; i < n; i++)
		str[i].a = dsptr->a[i];
		str[i].c = dsptr->c[i];
		str[i].ratio = (double) dsptr->c[i]/dsptr->a[i];
		str[i].index = i;

	
	qsort(str, sizeof(str), n, comparaison);

	int x
	for(int i = 0; i < n; i++)
		x[i] = 0
	
	int b = dsptr->b;

	for(int j = 1; i<n; j++){
		if b == 0
		{
			return dsptr->x;
		}
		dsptr->x[j] = min((b/str->a[j], 1));
		b = b - (dsptr->x[j]*str->a[j]);
	}

	return dsptr->x;
}

