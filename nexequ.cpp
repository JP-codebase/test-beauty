#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

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

unsigned int Bell_number(unsigned int n) {
  unsigned int* BellList{new unsigned int[n + 1]{0}};

  BellList[0] = 1;
  for (unsigned int i = 1; i <= n; i++) {
    for (unsigned int j = 0; j < i; j++) {
      /*std::cout << "binomial(" << i << ", " << j << "): " << binomial(i,j) <<
       * std::endl;*/
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

void frequency_profile(unsigned int* partition, unsigned int* fr_profile,
                       unsigned int partition_size) {
  for (unsigned int i = 0; i < partition_size; i++) {
    fr_profile[i] = 0;
  }

  for (unsigned int i = 0; i < partition_size; i++) {
    fr_profile[partition[i]]++;
  }
}

// Frequency sequence

// Frequency sequence without konwing the frequency profile
void frequency_sequence(unsigned int* partition, unsigned int* fr_sequence,
                        unsigned int partition_size) {
  //
  unsigned int* fr_profile{new unsigned int[partition_size]};
  frequency_profile(partition, fr_profile, partition_size);

  for (unsigned int i = 0; i < partition_size; i++) {
    fr_sequence[i] = fr_profile[partition[i]];
  }

  delete[] fr_profile;
}

// Frquency sequence knowing the frequency profile
void frequency_sequence(unsigned int* partition, unsigned int* fr_profile, unsigned int* fr_sequence,
                        unsigned int partition_size) {
  //

  for (unsigned int i = 0; i < partition_size; i++) {
    fr_sequence[i] = fr_profile[partition[i]];
  }
}

int main() {
  unsigned int setSize{0};
  std::cout << "Insert set size: ";
  std::cin >> setSize;

  std::cout << "You are going to generate " << Bell_number(setSize)
            << " partitions. Do you want to continue [Y/n]?" << std::endl;

  char input{};
  std::cin >> input;

  if (input != 'Y' && input != 'y') return 0;

  // Output to file
  std::string filename =
      "./partitions/" + std::to_string(setSize) + "_set_partitions.txt";

  // Output check
  {
    /*std::ifstream out_file(filename);*/
    /*if (out_file.good()) {*/
    /*  std::cerr << "Error: File already exists!";*/
    /*  return 1;*/
    /*}*/
    /*out_file.close();*/
  }

  std::ofstream out_file(filename, std::ios::app);

  // Initial set composed of `setSize` distiguishable characters

  /*unsigned char* set {new unsigned char [setSize]};*/
  /**/
  /*for (unsigned int i = 0; i < setSize; i++){*/
  /*  set[i] = i + 'a';*/
  /*  std::cout << set[i] << ' ';*/
  /*}*/
  /*std::cout << std::endl;*/

  unsigned int* partition{new unsigned int[setSize]};
  unsigned int* fr_profile{new unsigned int[setSize]};
  unsigned int* fr_sequence{new unsigned int[setSize]};

  for (unsigned int i = 0; i < setSize; i++) {
    partition[i] = 0;
    fr_profile[i] = 0;
    fr_sequence[i] = setSize;
  }
  fr_profile[0] = setSize;

  unsigned int counter{1};
  bool completed{false};
  int spacing = std::log(setSize);  // Used to format output

  do {
    // Print the previous partition

    // Print to console

    // Print partition
    std::cout << std::setw(spacing) << counter << ": \t";
    /*std::cout << std::endl;*/
    for (unsigned int i = 0; i < setSize; i++) {
      std::cout << "\033[3" << (partition[i] + 1) << "m" << "■" << ' ';

      /*if ((i % 3) == 0) std::cout << '\n';*/
      /*std::cout << "\033[3" << (partition[i] + 1) << "m" << "█" << ' ';*/
      // std::cout << partition[i] << ' ';
    }
    std::cout << "\033[37m";

    // Print frequency profile
    /*std::cout << '\t' << "fr. profile: ";*/
    /*for (unsigned int i = 0; i < setSize; i++) {*/
    /*  std::cout << fr_profile[i] << ' ';*/
    /*}*/
    /*std::cout << std::endl;*/

    // Print frequency sequence
    std::cout << '\t' << "fr. sequence: ";
    for (unsigned int i = 0; i < setSize; i++) {
      std::cout << fr_sequence[i] << ' ';
    }
    std::cout << std::endl;

    // Print to file
    out_file << std::setw(spacing) << counter << "    ";
    for (unsigned int i = 0; i < setSize; i++) {
      out_file << partition[i] << ' ';
    }
    out_file << '\n';

    completed = nexequ(partition, setSize);
    frequency_profile(partition, fr_profile, setSize);
    /*frequency_sequence(partition, fr_sequence, setSize);*/
    frequency_sequence(partition, fr_profile, fr_sequence, setSize);
    counter++;

  } while ((counter < Bell_number(setSize) + 1) && !completed);

  if (!completed)
    std::cout << "Execution not completed. There's a bug somewhere."
              << std::endl;

  delete[] partition;
  delete[] fr_profile;
  delete[] fr_sequence;

  out_file.close();

  return 0;
}
