#include <iostream>

// Given an element in the `index` array cell, this function
// checks if there exists an other element with index `j` such that
// 0 <= `j` < `index` in the array

bool isSingleton(unsigned int index, unsigned int* partition) {
  for (int i = index - 1; i >= 0; i--) {
    if (partition[i] == partition[index]) return false;
  }

  return true;
}

int main() {
  unsigned int setSize{0};
  std::cout << "Insert set size: ";
  std::cin >> setSize;

  // Initial set composed of `setSize` distiguishable characters

  /*unsigned char* set {new unsigned char [setSize]};*/
  /**/
  /*for (int i = 0; i < setSize; i++){*/
  /*  set[i] = i + 'a';*/
  /*  std::cout << set[i] << ' ';*/
  /*}*/
  /*std::cout << std::endl;*/

  unsigned int* partition{new unsigned int[setSize]};

  for (int i = 0; i < setSize; i++) {
    partition[i] = 0;
  }

  std::cout << isSingleton(setSize - 1, partition) << std::endl;

  // Print

  std::cout << "----------------------------------" << std::endl;
  for (int i = 0; i < setSize; i++) {
    std::cout << partition[i] << ' ';
  }
  std::cout << std::endl;

  return 0;
}
