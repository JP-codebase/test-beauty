#include <iostream>

// isSingleton
// Given an element in the `index` array cell, this function
// checks if there exists an other element with index `j` such that
// 0 <= `j` < `index` in the array

bool isSingleton(unsigned int index, unsigned int* partition) {
  for (unsigned int i = 1; i < index + 1; i++) {
    if (partition[index - i] == partition[index]) return false;
  }
  return true;
}

// TODO: template for different type of int 
// Binomial coefficient

unsigned int binomial(unsigned int n, unsigned int k) {
  unsigned int bin{1};

  for (unsigned int i{1}; i <= k; i++) {
    bin = (bin * (n - i + 1)) / i;
  }
  return bin;
}

// TODO: template for different type of int
// nthBell

unsigned int nthBell(unsigned int n) {
  unsigned int* BellList{new unsigned int[n + 1]{0}};

  BellList[0] = 1;
  for (unsigned int i = 1; i <= n; i++) {
    for (unsigned int j = 0; j < i; j++) {

      /*std::cout << "binomial(" << i << ", " << j << "): " << binomial(i,j) << std::endl;*/
      BellList[i] += binomial(i - 1, j) * BellList[j];

      /*std::cout << "i: " << i << " , j: " << j << '\t';*/
      /*for (unsigned int i = 0; i < (n + 1); i++) {*/
      /*  std::cout << BellList[i] << " ";*/
      /*}*/
      /*std::cout << std::endl;*/
    }
  }

  return BellList[n];
}

// nexequ
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

  // 3: Move alle elements ]`active`, `setSize`[ in the first subset class
  if (active != (setSize - 1)) {
    for (unsigned int i{active + 1}; i < setSize; i++) {
      partition[i] = 0;
    }
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

  unsigned int counter{1};
  bool completed{false};
  do {
    completed = nexequ(partition, setSize);

    // Print partition
    std::cout << counter << ": \t";
    for (unsigned int i = 0; i < setSize; i++) {
      std::cout << "\033[3" << (partition[i] + 1) << "m" << "■" << ' ';
      // std::cout << partition[i] << ' ';
    }
    std::cout << "\033[37m" << std::endl;

    counter++;
  } while ((counter < 600) && !completed);

  // Bell number
  unsigned int input;
  std::cout << "nth Bell number: " ;
  std::cin >> input;
  std::cout << nthBell(input) << std::endl;

  return 0;
}
