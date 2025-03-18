#include <iostream>

// Given an element in the `index` array cell, this function
// checks if there exists an other element with index `j` such that
// 0 <= `j` < `index` in the array

bool isSingleton(unsigned int index, unsigned int* partition) {
  for (unsigned int i = 1; i < index + 1; i++) {
    if (partition[index - i] == partition[index]) return false;
  }
  return true;
}

// TODO : genereate only 1 element of the sequence
// The function generates the next partition given the current
// It returns if the program reaches its end (`true` == exeuction finished`)
bool nexequ(unsigned int* partition, unsigned int setSize) {
  // 1: Find highest non singleton element `active`
  unsigned int active;
  for (active = setSize - 1; active >= 1; active--) {
    if (!isSingleton(active, partition)) break;
  }

  if (active == 0) return true;

  // 2: Move `active` to the next subset class
  partition[active]++;

  // Print partition
  for (unsigned int i = 0; i < setSize; i++) {
    std::cout << partition[i] << ' ';
  }
  std::cout << std::endl;

  // 3: Move alle elements ]`active`, `setSize`[ in the first subset class
  if (active != (setSize - 1)) {
    for (unsigned int i{active + 1}; i < setSize; i++) {
      partition[i] = 0;
    }

    // Print partition
    for (unsigned int i = 0; i < setSize; i++) {
      std::cout << partition[i] << ' ';
    }
    std::cout << std::endl;
  }

  return false;
}

int main() {
  unsigned int setSize{0};
  std::cout << "Insert set size: ";
  std::cin >> setSize;

  // Initial set composed of `setSize` distiguishable characters

  /*unsigned char* set {new unsigned char [setSize]};*/
  /**/
  /*for (unsigned int i = 0; i < setSize; i++){*/
  /*  set[i] = i + 'a';*/
  /*  std::cout << set[i] << ' ';*/
  /*}*/
  /*std::cout << std::endl;*/

  unsigned int* partition{new unsigned int[setSize]};

  for (unsigned int i = 0; i < setSize; i++) {
    partition[i] = 0;
  }

  unsigned int counter{0};
  bool completed{false};
  do {
    completed = nexequ(partition, setSize);
    counter++;
  } while ((counter < 60) && !completed);

  // Print

  std::cout << "----------------------------------" << std::endl;
  for (unsigned int i = 0; i < setSize; i++) {
    std::cout << partition[i] << ' ';
  }
  std::cout << std::endl;

  for (unsigned int i = 0; i < setSize; i++) {
    std::cout << isSingleton(i, partition) << ' ';
  }
  std::cout << std::endl;

  return 0;
}
