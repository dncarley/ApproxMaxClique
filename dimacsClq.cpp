
#include "dimacsClq.h"

graph readGraph(const char * filename){

	unsigned i, v1, v2, temp;
	std::string line;
	char * cString;
	char * token;
	graph returnGraph;
	
	std::ifstream myfile(filename);
	
	if (myfile.is_open()){
    	while ( myfile.good()){
			getline(myfile, line);
			
			switch(line[0]){
				case 'c':
					break;
				case 'p':
					cString = new char[line.size()+1];
					strcpy (cString, line.c_str());		/// convert to cstring to
					temp = 0;							/// count the split occurrences
					token = strtok(cString," ");
					while(token != NULL){
						if(temp == 2) returnGraph.numVert = (unsigned)atoi(token);
						if(temp == 3) returnGraph.numEdge = (unsigned)atoi(token);
						token = strtok (NULL, " ");
						temp++;
					}
					
					returnGraph.adjMatrix.resize(returnGraph.numVert);	// allocate return graph
					for(i = 0; i < returnGraph.numVert; i++){
						returnGraph.adjMatrix[i].resize((returnGraph.numVert>>6)+1);
					}
	
					delete[] cString;  
					break;
				case 'e':
					cString = new char[line.size()+1];
					strcpy(cString, line.c_str());		/// convert to cstring to
					temp = 0;							/// count the split occurances

					token = strtok(cString," ");
					while (token != NULL){
						if(temp == 1) v1  = (unsigned)atoi(token);
						if(temp == 2) v2  = (unsigned)atoi(token);
						token = strtok(NULL, " ");
						temp++;
					}
					v1--;		// index vertices from zero to numVert-1
					v2--;
					
					returnGraph.adjMatrix[v1] = setInsert(v2, returnGraph.adjMatrix[v1]);
					returnGraph.adjMatrix[v2] = setInsert(v1, returnGraph.adjMatrix[v2]);
						
					delete[] cString;  
					
				default:
					break;
			}			
		}
		myfile.close();
	}
	else{
		fprintf(stderr, "Unable to open file"); 
	}

	return returnGraph;
}
