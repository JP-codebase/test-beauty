#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

// nexpar
// It returns true if the program reaches its end (`true` == exeuction
// finished`)

bool nexpar(unsigned int *partition, unsigned int partition_size) {

  // Find active position
  if (partition[0] == 1)
    return true;

  unsigned int active_position;
  unsigned int sum{0};

  for (active_position = 0; active_position < partition_size - 1;
       active_position++) {
    if ((partition[active_position + 1] == 1) ||
        (partition[active_position + 1] == 0))
      break;
    sum += partition[active_position];
  }

  partition[active_position]--;
  sum += partition[active_position];

  unsigned int quotient{(partition_size - sum) / partition[active_position]};
  unsigned int remainder{(partition_size - sum) -
                         quotient * partition[active_position]};

  for (unsigned int i = active_position + 1;
       i < (active_position + 1 + quotient); i++) {
    partition[i] = partition[active_position];
  }

  if (remainder != 0) {
    partition[active_position + 1 + quotient] = remainder;

    for (unsigned int i = active_position + 2 + quotient; i < partition_size;
         i++) {
      partition[i] = 0;
    }
  } else {
    for (unsigned int i = active_position + 1 + quotient; i < partition_size;
         i++) {
      partition[i] = 0;
    }
  }

  return false;
}

// Degeneracy profile without knowing the frequency profile
void degeneracy_profile(unsigned int *partition, unsigned int *dg_profile,
                        unsigned int partition_size) {
  for (unsigned int i = 0; i < partition_size; i++) {
    dg_profile[i] = 0;
  }

  unsigned int *fr_profile{new unsigned int[partition_size]};

  for (unsigned int i = 0; (i < partition_size) && (fr_profile[i] != 0); i++) {
    dg_profile[fr_profile[i] - 1]++;
  }

  delete[] fr_profile;
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

int main() {

  unsigned int n;
  std::cout << "Insert an integer > 0: ";
  std::cin >> n;

  /*std::cout << "You are going to generate " << Bell_number(setSize)*/
  /*          << " partitions. Do you want to continue [Y/n]?" << std::endl;*/
  /**/

  /*char input{};*/
  /*std::cin >> input;*/
  /**/
  /*if (input != 'Y' && input != 'y')*/
  /*  return 0;*/

  unsigned int *partition{new unsigned int[n]};
  unsigned int *dg_profile{new unsigned int[n]};

  partition[0] = n;
  for (unsigned int i{1}; i < n; i++) {
    partition[i] = 0;
  }

  /*for (unsigned int i{0}; i < n; i++) {*/
  /*  partition[i] = 1;*/
  /*}*/

  /*degeneracy_profile(partition, dg_profile, n);*/

  // Resolution
  /*float res{resolution_degeneracy(dg_profile, n)};*/
  /*float min_resolution[2]{1, res};*/
  /*float max_resolution[2]{1, res};*/

  // Relevance
  /*float rel{relevance_degeneracy(dg_profile, n)};*/
  /*float min_relevance[2]{1, rel};*/
  /*float max_relevance[2]{1, rel};*/

  bool completed{false};
  unsigned int counter{1};
  unsigned int width{
      static_cast<unsigned int>(std::ceil(std::log10(static_cast<double>(n)))) +
      1};
  do {

    std::cout << counter << " :\t";
    for (unsigned int i{0}; i < n; i++) {
      std::cout << std::setw(width) << partition[i] << ' ';
    }

    // Print degeneracy profile
    /*std::cout << '\t' << "dg. profile : ";*/
    /*for (unsigned int i = 0; i < n; i++) {*/
    /*  std::cout << dg_profile[i] << ' ';*/
    /*}*/

    // Print resolution
    /*std::cout << '\t' << "resolution : " << res;*/

    // Print relevance
    /*std::cout << '\t' << "relevance : " << rel;*/

    std::cout << std::endl;

    completed = nexpar(partition, n);
    /*degeneracy_profile(partition, dg_profile, n);*/
    /*degeneracy_profile(partition, fr_profile, dg_profile, n);*/

    /*res = resolution_degeneracy(dg_profile, n);*/
    /*rel = relevance_degeneracy(dg_profile, n);*/

    counter++;
  } while (!completed);

  std::cout << "----------------------------" << std::endl;

  delete[] partition;
  delete[] dg_profile;

  return 0;
}
