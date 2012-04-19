#ifndef SET_OPERATIONS_H_DEFINED
#define SET_OPERATIONS_H_DEFINED

#include <iostream>
#include <inttypes.h>
#include <vector>

#define BIT_WIDTH 64		

std::vector<uint64_t> setInsert(unsigned u, std::vector<uint64_t> A);
std::vector<uint64_t> setDelete(unsigned u, std::vector<uint64_t> A);
bool memberOfSet(unsigned u, std::vector<uint64_t> A);
std::vector<uint64_t> setUnion( std::vector<uint64_t> A, std::vector<uint64_t> B);
std::vector<uint64_t> setIntersection( std::vector<uint64_t> A, std::vector<uint64_t> B);
std::vector<uint64_t> setXOR( std::vector<uint64_t> A, std::vector<uint64_t> B);
unsigned setOrder( const std::vector<uint64_t> A);
void printBinary(std::vector<uint64_t> A);

#endif
