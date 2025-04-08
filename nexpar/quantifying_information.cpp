#include "quantifying_information.h"
#include <cmath>
#include <functional>
#include <unordered_map>

// Degeneracy profile
void degeneracy_profile(unsigned int *partition, unsigned int *dg_profile,
                        unsigned int partition_size) {
  for (unsigned int i = 0; i < partition_size; i++) {
    dg_profile[i] = 0;
  }

  for (unsigned int i = 0; (i < partition_size) && (partition[i] != 0); i++) {
    dg_profile[partition[i] - 1]++;
  }
}

// Resolution

// Without knowing the degeneracy profile
float resolution(unsigned int *partition, unsigned int partition_size) {
  float res{0};

  unsigned int *dg_profile{new unsigned int[partition_size]};
  degeneracy_profile(partition, dg_profile, partition_size);

  for (unsigned int i = 0; i < partition_size; i++) {
    if (dg_profile[i] == 0)
      continue;
    res -= static_cast<float>((i + 1) * dg_profile[i]) /
           static_cast<float>(partition_size) *
           std::log(static_cast<float>(i + 1) /
                    static_cast<float>(partition_size));
  }

  delete[] dg_profile;

  return res;
}

// Knowing the degeneracy profile
float resolution_degeneracy(unsigned int *dg_profile,
                            unsigned int dg_profile_size) {
  float res{0};

  for (unsigned int i = 0; i < dg_profile_size; i++) {
    if (dg_profile[i] == 0)
      continue;
    res -= static_cast<float>((i + 1) * dg_profile[i]) /
           static_cast<float>(dg_profile_size) *
           std::log(static_cast<float>(i + 1) /
                    static_cast<float>(dg_profile_size));
  }

  return res;
}

// Relevance

// Without knowing the degeneracy profile
float relevance(unsigned int *partition, unsigned int partition_size) {
  float rel{0};

  unsigned int *dg_profile{new unsigned int[partition_size]};
  degeneracy_profile(partition, dg_profile, partition_size);

  for (unsigned int i = 0; i < partition_size; i++) {
    if (dg_profile[i] == 0)
      continue;
    rel -= (i + 1) * dg_profile[i] / static_cast<float>(partition_size) *
           std::log(static_cast<float>((i + 1) * dg_profile[i]) /
                    static_cast<float>(partition_size));
  }

  delete[] dg_profile;

  return rel;
}

// Knowing the degeneracy profile
float relevance_degeneracy(unsigned int *dg_profile,
                           unsigned int dg_profile_size) {
  float rel{0};

  for (unsigned int i = 0; i < dg_profile_size; i++) {
    if (dg_profile[i] == 0)
      continue;
    rel -= (i + 1) * dg_profile[i] / static_cast<float>(dg_profile_size) *
           std::log(static_cast<float>((i + 1) * dg_profile[i]) /
                    static_cast<float>(dg_profile_size));
  }

  return rel;
}

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
                     char boundary_conditions ) {

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
