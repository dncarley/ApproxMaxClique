
/// Implements the Simple Heuristic based Genetic Algorithm for Max Clique
/// Original Heuristic based on Elena Marchiori's Algorithm
///
/// Note: this algorithm does not guarantee the max clique, only a good approximation
///
/// 

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <inttypes.h>
#include "setOperations.h"
#include "dimacsClq.h"

//// Preprocessor debugging options, uncomment to activate
//#define PRINT_DETAILS		/// print input graph, more verbose...
//#define PRINT_LINES		/// print the line number (at random choices)

//// Globals
graph currentGraph;
unsigned populationType = 0;
unsigned numIterations  = 1;

//// GA parameters
#define FEW				3
#define POPULATION_SIZE 50
#define MUTATION_RATE	0.1
#define CROSSOVER_RATE	0.8

/////////////////////////////////////////////////////////////////////////////
std::vector< std::vector<uint64_t> > createInitPopulation(float density){

	unsigned i, j;
	float r;
	std::vector< std::vector<uint64_t> > initPopulation;

	initPopulation.resize( POPULATION_SIZE );

	for(i = 0; i < initPopulation.size(); i++){
	
		initPopulation[i].resize( (currentGraph.numVert >> 6) +1);

		for(j = 0; j < currentGraph.numVert; j++){
		
			r = (float)rand()/RAND_MAX;	/// generate a random number [0.0, 1.0]
			if( r < density){
				initPopulation[i] = setInsert( j, initPopulation[i]);
			}
		}
	}
	
	return initPopulation;
}

/////////////////////////////////////////////////////////////////////////////
std::vector<uint64_t> randomVector(unsigned n){

	unsigned i;
	std::vector< uint64_t > returnVector;
	
	returnVector.resize((n>>6)+1);

	for(i = 0; i < (n>>6)+1; i++){
	
		returnVector[i] = rand();
		returnVector[i] = returnVector[i] <<31;
		returnVector[i] |= rand();
	}
	
	return returnVector;
}

/////////////////////////////////////////////////////////////////////////////
std::vector<uint64_t> uniformCrossOver( std::vector<uint64_t> p1, std::vector<uint64_t> p2, unsigned n){

	unsigned i, choice;
	std::vector< std::vector<uint64_t> > children;
	
	children.resize(2);
	children[0].resize( p1.size());
	children[1].resize( p1.size());	
	
	for( i = 0; i < n; i++){
		
		choice = rand() %2;
		
		if(choice){
			if( memberOfSet(i, p2))	children[0] = setInsert(i, children[0]);
			if( memberOfSet(i, p1))	children[1] = setInsert(i, children[1]);
		}else{
			if( memberOfSet(i, p1))	children[0] = setInsert(i, children[0]);
			if( memberOfSet(i, p2))	children[1] = setInsert(i, children[1]);
		}
	}
	
	return children[0];
}

/////////////////////////////////////////////////////////////////////////////
std::vector<uint64_t> mutate( std::vector<uint64_t> individual, unsigned n){

	unsigned r1, r2;
	std::vector<uint64_t> returnValue;
	
	returnValue = individual;
	
	r1 = rand() %n;
	r2 = rand() %n;
	
	if( memberOfSet( r1, individual) )	returnValue = setInsert( r2, returnValue);
	else								returnValue = setDelete( r2, returnValue);

	if( memberOfSet( r2, individual) )	returnValue = setInsert( r1, returnValue);
	else								returnValue = setDelete( r1, returnValue);	
	
	return returnValue;
}


/////////////////////////////////////////////////////////////////////////////
std::vector<uint64_t> mutateXOR( std::vector<uint64_t> individual, unsigned n){

	std::vector<uint64_t> returnValue;
	unsigned i;
	
	returnValue.resize( individual.size());
	
	returnValue = setXOR( individual, randomVector(currentGraph.numVert));

	for(i = n; i <= returnValue.size()<<6; i++){
		returnValue = setDelete( i, returnValue);
	}
	
	return returnValue;

}


/////////////////////////////////////////////////////////////////////////////
std::vector<uint64_t> cliqueHeuristicGA(const std::vector<uint64_t> inNodes){

	bool isConnected;
	int i, j, idx, choice, vert;
	std::vector<uint64_t> outNodes;

	outNodes = inNodes;
	
	/// relax (enlarge the subgraph)
	for( i = 0; i < FEW; i++){
	
		vert = rand() % currentGraph.numVert;
		outNodes = setInsert(vert, outNodes);
		
	}
	
	/// repair (extract a clique)
	idx = rand() % currentGraph.numVert;
	
	for(i = idx; i < currentGraph.numVert; i++){
		if( memberOfSet(i, outNodes)){		
			
			#ifdef PRINT_LINES
			printf("\tLINE: %u\n", __LINE__);
			#endif
			
			choice = rand() % 2;
			
			if( choice == 0){
				#ifdef PRINT_LINES
				printf("\tLINE: %u\n", __LINE__);
				#endif
				outNodes = setDelete(i, outNodes);
			}else{
				#ifdef PRINT_LINES
				printf("\tLINE: %u\n", __LINE__);
				#endif
				for(j = i+1; j < currentGraph.numVert; j++){	
					if( memberOfSet(j, outNodes) && !memberOfSet(j, currentGraph.adjMatrix[i])){
						outNodes = setDelete(j, outNodes);					
					}			
				}
				for(j = 0; j < i; j++){
					if( memberOfSet(j, outNodes) && !memberOfSet(j, currentGraph.adjMatrix[i])){
						outNodes = setDelete(j, outNodes);					
					}
				}
			}
		}
	}
	
	for(i = idx-1; i >= 0; i--){	

		choice = rand() % 2;

		if(choice == 0){
			#ifdef PRINT_LINES
			printf("\tLINE: %u\n", __LINE__);
			#endif
			outNodes = setDelete(i, outNodes);
		}else{
			#ifdef PRINT_LINES
			printf("\tLINE: %u\n", __LINE__);
			#endif
			for( j = i-1; j >= 0; j-- ){
				if( memberOfSet(j, outNodes) && !memberOfSet(j, currentGraph.adjMatrix[i])){
					outNodes = setDelete(j, outNodes);					
				}			
			}
		}	
	}
	
	/// extend (enlarge the clique)
	idx = rand() % currentGraph.numVert;

	for( i = idx; i < currentGraph.numVert; i++){
		isConnected = 1;
		for( j = 0; j < currentGraph.numVert; j++){
			if( (i != j) && memberOfSet(j, outNodes) ){
				isConnected &= memberOfSet(i, currentGraph.adjMatrix[j]);
			}
			if(!isConnected) break;		/// exit before checking every node if not connected
		}
		if(isConnected){
			outNodes = setInsert(i, outNodes);		
		}
	}

	for( i = 0; i < idx-1; i++){
		isConnected = 1;
		for( j = 0; j < currentGraph.numVert; j++){
			if( (i != j) && memberOfSet(j, outNodes) ){
				isConnected &= memberOfSet(i, currentGraph.adjMatrix[j]);
			}
			if(!isConnected) break;		/// exit before checking every node if not connected
		}
		if(isConnected){
			outNodes = setInsert(i, outNodes);
		}	
	}

	return outNodes;
}


/////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv){

	unsigned i, j, bestIndex, secondIndex, localMax, secondMax;
	float r;
	
	std::vector< uint64_t > bestIndividual, secondIndividual;
	std::vector< std::vector<uint64_t> > population;
	std::vector< unsigned > evalVector;
	
	#ifdef PRINT_DETAILS	
	printf("Rand MAX: %ull\n", RAND_MAX);
	#endif
	
	srand(1);

	if(argc > 3){
		currentGraph   = readGraph(argv[1]);
		populationType = atoi(argv[2]);
		numIterations  = atoi(argv[3]);
	}else{
		fprintf(stderr, "provide `.clq' input\n");
		exit(1);
	}
	
	/// Allocate graph
	population = createInitPopulation(0.25);		// create population with 0.25 set bits
	evalVector.resize(POPULATION_SIZE);
	
	bestIndividual.resize( (currentGraph.numVert>>6) +1);
	secondIndividual.resize( (currentGraph.numVert>>6) +1);
	
	// Apply heuristic to population
	for(i = 0; i < population.size(); i++){
		population[i] = cliqueHeuristicGA( population[i]);
	}
	
	evalVector[0] = setOrder( population[0]);		// setup initial conditions
	evalVector[1] = setOrder( population[1]);
	if( evalVector[0] > evalVector[1]){
		localMax  	= evalVector[0];
		secondMax 	= evalVector[1];
		bestIndex   = 0;
		secondIndex = 1;
	}else{
		localMax  	= evalVector[1];
		secondMax 	= evalVector[0];
		bestIndex   = 1;
		secondIndex = 0;	
	}

	for(i = 2; i < population.size(); i++){
		evalVector[i] = setOrder( population[i]);		// count bits
		
		if(evalVector[i] > localMax){  
        	secondMax 	= localMax;
        	secondIndex = secondIndex;
        	localMax	= evalVector[i];
        	secondIndex = i;
    	}else if(evalVector[i] > secondMax){
        	secondMax	= evalVector[i];
        	secondIndex = i;
    	}
	}
		
	
	///// Iterate 
	
	for(i = 0; i < numIterations; i++){
	
		bestIndividual 	 = population[bestIndex];
		secondIndividual = population[secondIndex]; 

		/// Diversification procedure
		for(j = 0; j < population.size(); j++){

			if( populationType == 0){
				r = (float)rand()/RAND_MAX;

				if( r <= CROSSOVER_RATE ){
					population[j] = uniformCrossOver( bestIndividual, secondIndividual, currentGraph.numVert);
				}else{
					population[j] = bestIndividual;
				}

				if( r <= MUTATION_RATE ){
					population[j] = mutate( population[j], currentGraph.numVert);
				}
				
			}else if( populationType == 1){
			
				population[j] = bestIndividual;
			
			}else if( populationType == 2){
			
				population[j] = mutateXOR(bestIndividual, currentGraph.numVert);			
			}
		}
		
		// Apply heuristic to population
		for(j = 0; j < population.size(); j++){
			population[j] = cliqueHeuristicGA( population[j]);
		}
	
	
		evalVector[0] = setOrder( population[0]);		// setup initial conditions
		evalVector[1] = setOrder( population[1]);
		if( evalVector[0] > evalVector[1]){
			localMax  	= evalVector[0];
			secondMax 	= evalVector[1];
			bestIndex   = 0;
			secondIndex = 1;
		}else{
			localMax  	= evalVector[1];
			secondMax 	= evalVector[0];
			bestIndex   = 1;
			secondIndex = 0;	
		}
	
		for(i = 2; i < population.size(); i++){
			evalVector[i] = setOrder( population[i]);		// count bits
			
			if(evalVector[i] > localMax){  
				secondMax 	= localMax;
				secondIndex = secondIndex;
				localMax	= evalVector[i];
				secondIndex = i;
			}else if(evalVector[i] > secondMax){
				secondMax	= evalVector[i];
				secondIndex = i;
			}
		}
			

		#ifdef PRINT_DETAILS		
		printf("ITER: %u\n", i);
		#endif
	}

	/// print final output
	j= 0;
	for(i = 0; i < population.size(); i++){
		if(evalVector[i] == localMax){
			#ifdef PRINT_DETAILS	
			printf("%u\t", evalVector[i]);	
			printBinary(population[i]);
			printf("\n");
			#endif	
			j++;
		}	
	}

	printf("\ttype: %u\titer: %u\t# cliques >= %u,\t", populationType, numIterations, localMax);
	printf("percent %f\n", ((float)j/population.size())*100.0);

}
