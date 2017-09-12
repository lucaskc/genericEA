#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#define NUCLEOTIDE_NUM 4
#define N_GENES 	   100
#define POP_SIZE	   90
#define TM 			   3

typedef struct population POP;

struct population {
	long **matrix;
	long popSize; //number of individuals
	long numberOfGenes;
};

POP *newPopulation(long popSize, long numberOfGenes) {
	if(numberOfGenes <= 0) return NULL;

	POP *pop = NULL;

	int i, k = 0, j = 0;

	pop = (POP*) malloc(sizeof(POP));
	if(pop == NULL) return NULL;

	pop->numberOfGenes = numberOfGenes;
	pop->popSize = popSize;

    pop->matrix = (long**) malloc (popSize * sizeof(long*));
    for(i = 0; i < popSize; i++) {
        /**(pop->matrix+i)*/pop->matrix[i] = (long*) malloc (numberOfGenes * sizeof(long));
	}

	srand(time(NULL));
    for(k=0; k < popSize; k++) {
        for(j=0; j < numberOfGenes; j++) {
            pop->matrix[k][j] = rand() % NUCLEOTIDE_NUM;
        }
    }

    return pop;
}

void destroyPopulation(POP *pop) {
	//if(pop == NULL) return;
	
	for(int i = 0; i < pop->popSize; i++) {
		free(pop->matrix[i]);
	}
	free(pop->matrix);
	//free(pop->popSize);
	//free(pop->numberOfGenes);
	free(pop);
}

long getBestIndividual(POP *pop, long *idealSolution, long *fitness) {
	//if(pop == NULL || idealSolution == NULL) return NULL;

	long *points = (long *) calloc(pop->popSize, sizeof(long));
	//if(points == NULL) return NULL;

	//printf("popsize = %ld\n ngenes = %ld", pop->popSize, pop->numberOfGenes);
	//printf("%ld\n%ld\n", pop->numberOfGenes, pop->popSize);
	for(int i = 0; i < pop->popSize; i++) {

		for(int j = 0; j < pop->numberOfGenes; j++) {
			if(pop->matrix[i][j] == 0) {
				points[i]++;


			}
		}

	}
	//ver qual pop->matrix é o melhor individuo, e guardar em bestIndividual

	//*fitness = points[0];
	int bestIndividual = 0;
	for(int i = 1; i < pop->popSize; i++) {
		if(points[i] > points[bestIndividual])
			bestIndividual = i;
	}
	*fitness = points[bestIndividual]; //secondary return value

	return bestIndividual;
}

void crossOverAndMutation(POP *pop, long bestIndividualIndex) {
	//if(pop == NULL) return NULL;

	srand(time(NULL));

	for(int i = 0; i < pop->popSize; i++) {
		if(i == bestIndividualIndex) continue;
        for(int j = 0; j < pop->numberOfGenes; j++) {
        	//"crossover"
        	if(rand()%2) {
        		pop->matrix[i][j] = pop->matrix[bestIndividualIndex][j];
        	}
        	//mutation
        	if(rand()%100 < TM) {
        		pop->matrix[i][j] = rand()%NUCLEOTIDE_NUM;
        	}
        }
    }
}

void printBestIndividual(long *bestIndividual, long *fitness) {
	//printf("Fitness: %ld\nGene Sequence: ", *fitness);

	for(int i = 0; i < N_GENES; i++)
		printf("%lu", bestIndividual[i]);
	//printf("\n----------------------------------\n");
}

int main(int argc, char const *argv[]) {

	long fitness = 0;
	long *idealSolution = (long *) calloc(N_GENES, sizeof(long));
	POP *pop = newPopulation(POP_SIZE, N_GENES);
	long bestIndividualIndex;
    int gen = 0;
    //FILE *out;
    //out = fopen("results2.xls", "w");
	while(fitness != N_GENES) {
        gen++;

		bestIndividualIndex = getBestIndividual(pop, idealSolution, &fitness);
        long *bInd = pop->matrix[bestIndividualIndex];
        printf("\n%d: ", gen);
        printBestIndividual(bInd, &fitness);
        //fprintf(out, "%d\t%ld\n", gen, fitness);
        //printf("%d: %lu\n", gen, fitness);
		crossOverAndMutation(pop, bestIndividualIndex);
	}
	destroyPopulation(pop);
    //fclose(out);
	return 0;
}
