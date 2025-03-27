#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

const double pi{std::acos(-1.0)};

// Number of partition of the integer n
double number_partition_n(unsigned int n) {
  return std::exp(pi * std::sqrt(2.0L * n / 3.0L)) / (4 * n * std::sqrt(3.0L));
}

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

// Print to file, buffered plain text
void print_partition_to_file_buffered(unsigned int *partition,
                                      unsigned int partition_size,
                                      std::ofstream &file, std::string &buffer,
                                      unsigned int buffer_limit = 1024) {

  std::ostringstream oss;

  for (unsigned int i{0}; i < partition_size; i++) {
    oss << partition[i] << ' ';
  }
  oss << '\n';

  buffer += oss.str();

  if (buffer.size() >= buffer_limit) {
    file << buffer;
    buffer.clear();
  }
}

// Print to file, binary
void print_parition_to_file_binary(unsigned int *partition,
                                   unsigned int partition_size,
                                   std::ofstream &file) {}

int main() {

  // Input
  unsigned int n;
  std::cout << "Insert an integer > 0: ";
  std::cin >> n;

  const double number_of_partitions{number_partition_n(n)};
  std::cout << "You are going to generate approximately "
            << number_of_partitions
            << " partitions. Do you want to continue [Y/n]?" << std::endl;

  char input{};
  std::cin >> input;

  if (input != 'Y' && input != 'y')
    return 0;

  // Declaration and initialization of the important variables
  unsigned int *partition{new unsigned int[n]};
  unsigned int *dg_profile{new unsigned int[n]};

  partition[0] = n;
  for (unsigned int i{1}; i < n; i++) {
    partition[i] = 0;
  }

  degeneracy_profile(partition, dg_profile, n);

  // Resolution
  float res{resolution_degeneracy(dg_profile, n)};
  float min_resolution[2]{1, res};
  float max_resolution[2]{1, res};

  // Relevance
  float rel{relevance_degeneracy(dg_profile, n)};
  float min_relevance[2]{1, rel};
  float max_relevance[2]{1, rel};

  // Variables used for formatting the output to the terminal
  int status_bar{0};
  unsigned int ten_percent = number_of_partitions / 10;
  unsigned int width{
      static_cast<unsigned int>(std::ceil(std::log10(static_cast<double>(n)))) +
      1};

  // Output to file
  std::string filename = "./partitions/partitions_of_" + std::to_string(n);
  // Output check
  {
    /*std::ifstream out_file(filename);*/
    /*if (out_file.good()) {*/
    /*  std::cerr << "Error: File already exists!";*/
    /*  return 1;*/
    /*}*/
    /*out_file.close();*/
  }

  std::ofstream out_file_partitions_txt(filename + ".txt");
  std::ofstream out_file_resrel_txt(filename + "_resrel.txt");
  std::ofstream out_file_partitions_bin(filename + ".bin", std::ios::binary);
  std::ofstream out_file_resrel_bin(filename + "_resrel.bin", std::ios::binary);

  if (!out_file_partitions_txt) {
    std::cerr << "Error: Could not open " << filename << ".txt"
              << " for writing." << std::endl;
    return 1;
  }
  if (!out_file_resrel_txt) {
    std::cerr << "Error: Could not open file " << filename << "_resrel.txt"
              << " for writing." << std::endl;
    return 1;
  }
  if (!out_file_partitions_bin) {
    std::cerr << "Error: Could not open file " << filename << ".bin"
              << " for writing." << std::endl;
    return 1;
  }
  if (!out_file_resrel_bin) {
    std::cerr << "Error: Could not open file " << filename << "_resrel.bin"
              << " for writing." << std::endl;
    return 1;
  }

  // Buffer
  std::string buffer;
  unsigned int buffer_limit = 128;

  // Time
  auto start = std::chrono::steady_clock::now();
  auto time_step1 = start;
  auto time_step2 = start;

  // Starting the loop
  bool completed{false};
  unsigned long int counter{1};

  std::cout << std::endl;
  std::cout << "----------------------------" << std::endl;

  do {

    if (counter > (status_bar * ten_percent)) {
      time_step1 = std::chrono::steady_clock::now();
      std::cout << status_bar << "0% : "
                << ((std::chrono::duration_cast<std::chrono::microseconds>(
                         (time_step1 - time_step2)))
                        .count()) /
                       1000000.0
                << " seconds" << std::endl;
      time_step2 = time_step1;
      status_bar++;
    }

    // Print to file
    /*print_partition_to_file_buffered(partition, n, out_file_partitions_txt,*/
    /*                                 buffer, buffer_limit);*/
    /*out_file_resrel_txt << res << ' ' << rel << '\n';*/

    out_file_partitions_bin.write(reinterpret_cast<const char *>(partition),
                       sizeof(unsigned int));
    out_file_resrel_bin.write(reinterpret_cast<const char *>(&res), sizeof(res));
    out_file_resrel_bin.write(reinterpret_cast<const char *>(&rel), sizeof(rel));

    // Print to terminal
    /*std::cout << counter << " :\t";*/
    /*for (unsigned int i{0}; i < n; i++) {*/
    /*  std::cout << std::setw(width) << partition[i] << ' ';*/
    /*}*/

    // Print degeneracy profile
    /*std::cout << '\t' << "dg. : ";*/
    /*for (unsigned int i = 0; i < n; i++) {*/
    /*  std::cout << std::setw(width) << dg_profile[i] << ' ';*/
    /*}*/

    // Print resolution
    /*std::cout << '\t' << "res : " << res;*/

    // Print relevance
    /*std::cout << '\t' << "rel : " << rel;*/

    /*std::cout << std::endl;*/

    completed = nexpar(partition, n);
    degeneracy_profile(partition, dg_profile, n);

    res = resolution_degeneracy(dg_profile, n);
    rel = relevance_degeneracy(dg_profile, n);

    counter++;

    if (res < min_resolution[1]) {
      min_resolution[0] = counter;
      min_resolution[1] = res;
    } else if (res > max_resolution[1]) {
      max_resolution[0] = counter;
      max_resolution[1] = res;
    }

    if (rel < min_relevance[1]) {
      min_relevance[0] = counter;
      min_relevance[1] = rel;
    } else if (rel > max_relevance[1]) {
      max_relevance[0] = counter;
      max_relevance[1] = rel;
    }
  } while (!completed);

  if (!buffer.empty()) {
    out_file_partitions_txt << buffer;
    buffer.clear();
  }

  // Execution time
  time_step1 = std::chrono::steady_clock::now();
  std::cout << "100% : "
            << ((std::chrono::duration_cast<std::chrono::microseconds>(
                     (time_step1 - time_step2)))
                    .count()) /
                   1000000.0
            << " seconds" << std::endl;

  std::cout << "Execution time : "
            << ((std::chrono::duration_cast<std::chrono::microseconds>(
                     (time_step1 - start)))
                    .count()) /
                   1000000.0
            << " seconds" << std::endl;

  std::cout << std::endl;
  std::cout << "----------------------------" << std::endl;

  // Max and Min Relevance and Resolution
  std::cout << std::setw(20) << "Min resolution : " << std::setw(10)
            << min_resolution[1] << std::setw(20) << std::setw(20)
            << "position : " << std::setw(10) << min_resolution[0] << std::endl;
  std::cout << std::setw(20) << "Max resolution : " << std::setw(10)
            << max_resolution[1] << std::setw(20)
            << "position : " << std::setw(10) << max_resolution[0] << std::endl;

  std::cout << "----------------------------" << std::endl;
  std::cout << std::setw(20) << "Min relevance : " << std::setw(10)
            << min_relevance[1] << std::setw(20) << std::setw(20)
            << "position : " << std::setw(10) << min_relevance[0] << std::endl;
  std::cout << std::setw(20) << "Max relevance : " << std::setw(10)
            << max_relevance[1] << std::setw(20)
            << "position : " << std::setw(10) << max_relevance[0] << std::endl;

  out_file_partitions_txt.close();
  out_file_resrel_txt.close();
  out_file_partitions_bin.close();
  out_file_resrel_bin.close();

  delete[] partition;
  delete[] dg_profile;

  return 0;
}
