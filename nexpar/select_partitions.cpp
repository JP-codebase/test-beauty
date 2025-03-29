#include <fstream>
#include <iostream>
#include <string>

int main() {

  unsigned int n{17};

  std::string in_filename{"./partitions/partitions_of_" + std::to_string(n) +
                          ".bin"};
  std::string out_filename{"reading_test.txt"};

  std::ifstream in_file(in_filename, std::ios::binary);
  /*std::ofstream out_file(out_filename);*/

  unsigned int* partition {new unsigned int[n]};
  while (!in_file.eof()) {
    in_file.read(reinterpret_cast<char *>(partition), sizeof(partition[0]) * n);

    for (unsigned int i {0}; i < n; i++) {
      std::cout << partition[i] << ' ';
    }
    std::cout << std::endl;
  };

  return 0;
}
