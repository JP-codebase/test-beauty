#include <functional>
#include <iostream>
#include <unordered_map>

using number_of_bonds =
    std::function<unsigned int(int, int, unsigned int *, unsigned int[2])>;

number_of_bonds open_boundary = [](int i, int j, unsigned int *lattice,
                                   unsigned int lattice_shape[2]) {
  unsigned int n_bounds{0};

  if (i > 0) {
    if (lattice[i * lattice_shape[1] + j] ==
        lattice[(i - 1) * lattice_shape[1] + j]) {
      n_bounds++;
    }
  }

  if (i < (lattice_shape[0] - 1)) {
    if (lattice[i * lattice_shape[1] + j] ==
        lattice[(i + 1) * lattice_shape[1] + j]) {
      n_bounds++;
    }
  }

  if (j > 0) {
    if (lattice[i * lattice_shape[1] + j] ==
        lattice[i * lattice_shape[1] + (j - 1)]) {
      n_bounds++;
    }
  }

  if (j < (lattice_shape[1] - 1)) {
    if (lattice[i * lattice_shape[1] + j] ==
        lattice[i * lattice_shape[1] + (j + 1)]) {
      n_bounds++;
    }
  }

  return n_bounds;
};

number_of_bonds periodic_boundary = [](int i, int j, unsigned int *lattice,
                                       unsigned int lattice_shape[2]) {
  unsigned int n_bounds{0};

  if (lattice[i * lattice_shape[1] + j] ==
      lattice[((i - 1 + lattice_shape[0]) % lattice_shape[0]) *
                  lattice_shape[1] +
              j]) {
    n_bounds++;
  }

  if (lattice[i * lattice_shape[1] + j] ==
      lattice[((i + 1) % lattice_shape[0]) * lattice_shape[1] + j]) {
    n_bounds++;
  }

  if (lattice[i * lattice_shape[1] + j] ==
      lattice[i * lattice_shape[1] +
              (j - 1 + lattice_shape[1]) % lattice_shape[1]]) {
    n_bounds++;
  }

  if (lattice[i * lattice_shape[1] + j] ==
      lattice[i * lattice_shape[1] + (j + 1) % lattice_shape[1]]) {
    n_bounds++;
  }

  return n_bounds;
};

float energy_lattice(unsigned int *lattice, unsigned int lattice_shape[2],
                     char boundary_conditions = 'o') {

  std::unordered_map<char, number_of_bonds> boundary_functions = {
      {'o', open_boundary},
      {'p', periodic_boundary},
  };
  auto pair = boundary_functions.find(boundary_conditions);

  number_of_bonds n_bonds = pair->second;

  float energy{0};
  const float J{1}; // Coupling constant

  /*for (int i = 0; i < lattice_shape[0]; i++) {*/
  /*  for (int j = (i % 2 == 0) ? 0 : 1; j < lattice_shape[1]; j += 2) {*/
  /*    energy -= n_bonds(i, j, lattice, lattice_shape);*/
  /*  }*/
  /*}*/

  for (int i = 0; i < lattice_shape[0]; i++) {
    for (int j = 0; j < lattice_shape[1]; j++) {
      energy -= n_bonds(i, j, lattice, lattice_shape);
    }
  }

  energy = J * energy / 2.0;
  return energy;
}

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
