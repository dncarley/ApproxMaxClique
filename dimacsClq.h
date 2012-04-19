#ifndef DIMACS_CLQ_H_DEFINED
#define DIMACS_CLQ_H_DEFINED

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "setOperations.h"

typedef struct graph{

	unsigned numVert;
	unsigned numEdge;
	std::vector< std::vector<uint64_t> > adjMatrix;		// 0-1 matrix [0.. numVert-1]	

};

graph readGraph(const char * filename);

#endif
