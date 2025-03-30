#include "quantifying_information.h"
#include <cmath>

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
