
#include "setOperations.h"

std::vector<uint64_t> setInsert(unsigned u, std::vector<uint64_t> A){

	std::vector<uint64_t> R;
	unsigned i, j;
	
	R = A;
	j = BIT_WIDTH - 1 - (u % BIT_WIDTH);
	i = u >> 6;			// floor(u/BIT_WIDTH) 

	R[i] = R[i] | (1L << j);

	return R;
}

std::vector<uint64_t> setDelete(unsigned u, std::vector<uint64_t> A){

	std::vector<uint64_t> R;
	unsigned i, j;
	
	R = A;
	j = BIT_WIDTH - 1 - (u % BIT_WIDTH);
	i = u >> 6;			// floor(u/BIT_WIDTH) 

	R[i] = R[i] & ~(1L << j);

	return R;
}

bool memberOfSet(unsigned u, std::vector<uint64_t> A){

	unsigned i, j;
	
	j = BIT_WIDTH - 1 - (u % BIT_WIDTH);
	i = u >> 6;			// floor(u/BIT_WIDTH) 

	if( A[i] & (1L << j)){
		return 1;	
	}else{
		return 0;
	}
}

std::vector<uint64_t> setUnion( std::vector<uint64_t> A, std::vector<uint64_t> B){
	
	std::vector<uint64_t> R;
	unsigned i, cell;

	cell = A.size();
	
	R.resize(cell);

	for(i = 0; i < cell; i++){
		R[i] = A[i] | B[i];
	}
	return R;
}

std::vector<uint64_t> setIntersection( std::vector<uint64_t> A, std::vector<uint64_t> B){

	std::vector<uint64_t> R;
	unsigned i, cell;

	cell = A.size();
	
	R.resize(cell);

	for(i = 0; i < cell; i++){
		R[i] = A[i] & B[i];
	}
	return R;
}

std::vector<uint64_t> setXOR( std::vector<uint64_t> A, std::vector<uint64_t> B){

	std::vector<uint64_t> R;
	unsigned i, cell;

	cell = A.size();
	
	R.resize(cell);

	for(i = 0; i < cell; i++){
		R[i] = A[i] ^ B[i];
	}
	return R;
}

/// computes the set bits with Brian Kernighan's method
/// http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
unsigned setOrder( const std::vector<uint64_t> A){

	std::vector<uint64_t> B;
	unsigned i; // iterate through B
	unsigned c; // c accumulates the total bits set in B
	
	B = A;		// copy, will destroy the contents
	
	c = 0;		// init bit count to 0
	for(i = 0; i < B.size(); i++){
		for( ; B[i]; c++){
			B[i] &= B[i] - 1; // clear the least significant bit set
		}
	}
	
	return c;  // return the number of set bits
}

void printBinary(std::vector<uint64_t> A){
	int i, j;

	for(i = 0; i < A.size(); i++){
		for(j = BIT_WIDTH-1; j>=0; j--) {
			if((1L<<j)&A[i]) printf("1");
			else			 printf("0");
		}
	}
}

