#include <iostream>
#include "quantifying_information.h"

int main() {

  /*unsigned int n{9};*/
  /*unsigned int shape[]{3, 3}; // shape = (number_of_rows, number_of_columns)*/
  /*unsigned int *partition = new unsigned int[n]{4, 3, 1, 1, 0, 0, 0, 0, 0};*/

  unsigned int n{10};
  unsigned int shape[]{2, 5}; // shape = (number_of_rows, number_of_columns)
  unsigned int *partition = new unsigned int[n]{4, 3, 1, 1, 1, 0, 0, 0, 0, 0};

  unsigned int *lattice{new unsigned int[n]};

  unsigned int *lattice_index{lattice};
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < partition[i]; j++) {
      *lattice_index = i;
      lattice_index++;
    }
  }

  lattice_index = nullptr;

  for (int i = 0; i < shape[0]; i++) {
    for (int j = 0; j < shape[1]; j++) {
      std::cout << "\033[3" << (lattice[i * shape[1] + j] + 1) << "m" << "■"
                << ' ';
      /*std::cout << lattice[i * shape[1] + j] << ' ';*/
    }
    std::cout << std::endl;
  }
  /*std::cout << "\033[37m";     // White text*/
  std::cout << "\033[30m"; // Black text

  std::cout << std::endl;
  std::cout << "Energy : " << energy_lattice(lattice, shape, 'p') << std::endl;
  /*energy_lattice(lattice, shape, 'p');*/

  delete[] partition;
  return 0;
}
