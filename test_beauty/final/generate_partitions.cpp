#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "headers/nexpar_functions.h"
#include "headers/quantifying_information.h"

#ifdef REAL_IS_DOUBLE
using real_t = double;
#else
using real_t = float;
#endif

const char RED[] = "\033[31m";
const char RESET_STYLE[] = "\e[m";

// Number of partition of the integer n
long double approximate_number_of_partitions(unsigned int n);

// Input check
void input_check(unsigned int& partition_size);

// Print to file, buffered plain text
void print_partition_to_file_buffered(unsigned int* partition,
                                      unsigned int partition_size,
                                      std::ofstream& file, std::string& buffer,
                                      unsigned int buffer_limit = 1024);


int main(int argc, char* argv[]) {

    unsigned int partition_size;

    // Input check
    {
        if (argc == 2) {
            try {
                int value = std::stoi(argv[1]);

                if (value < 0) {
                    throw std::out_of_range("Value must be a positive integer");
                }

                partition_size = std::stoul(argv[1]);

            } catch (const std::invalid_argument& e) {
                std::cerr << RED
                          << "Error: Invalid argument. Input is "
                             "not a valid integer.\n"
                          << RESET_STYLE;
                return 1;

            } catch (const std::out_of_range& e) {
                std::cerr << "\033[31m"
                             "Error: "
                          << e.what() << "\n"
                          << RESET_STYLE;
                return 1;
            }
        } else {
            std::cout << RED << "Error: Incorrect number of arguments."
                      << RESET_STYLE << std::endl;
            return 1;
        }
    }

    const long double n_of_partitions { approximate_number_of_partitions(
        partition_size) };


    {
        std::cout << "Number of partitions to generate :  " << n_of_partitions
                  << "\nDo you want to continue? [Y/n] ";
        std::string input {};
        std::cin >> input;

        if (input != "Y" && input != "y") {
            std::cout << "Execution terminated." << std::endl;
            return 1;
        }
    }

    std::string filename =
        "./partitions/partitions_of_" + std::to_string(partition_size);

    // txt outpupt to file
    std::ofstream out_file_partitions_txt(filename + ".txt");

    if (!out_file_partitions_txt) {
        std::cerr << RED << "Error: Could not open " << filename << ".txt"
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }

    // Binary output to file
    std::ofstream out_file_resrel_bin(filename + "_resrel.bin",
                                      std::ios::binary);

    if (!out_file_resrel_bin) {
        std::cerr << RED << "Error: Could not open file " << filename
                  << "_resrel.bin"
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }

    std::string buffer;

    // Partition declaration
    unsigned int* partition { new unsigned int[partition_size] { 0 } };
    unsigned int* dg_profile { new unsigned int[partition_size] };
    real_t resolution;
    real_t relevance;

    // Initialize first partition: [partition_size, 0, ..., 0]

    partition[0] = partition_size;
    degeneracy_profile(partition, dg_profile, partition_size);
    resolution = resolution_degeneracy(dg_profile, partition_size);
    relevance = relevance_degeneracy(dg_profile, partition_size);


    // Starting the loop. It ends when reaches the partition [ 1, 1, ..., 1 ]

    bool execution_completed = false;
    do {

        // Output
        print_partition_to_file_buffered(
            partition, partition_size, out_file_partitions_txt, buffer);
        out_file_resrel_bin << resolution << ' ' << relevance;

        // Generate the next partition
        execution_completed = nexpar_ptr(partition, partition_size);
    } while (!execution_completed);

    // Empty the buffer
    if (!buffer.empty()) {
        out_file_partitions_txt << buffer;
        buffer.clear();
    }

    delete[] partition;
    delete[] dg_profile;

    out_file_partitions_txt.close();
    out_file_resrel_bin.close();
    return 0;
}

// Function implementation
long double approximate_number_of_partitions(unsigned int n) {

    const long double pi { std::acos(-1.0L) };
    return std::exp(pi * std::sqrt(2.0L * n / 3.0L)) /
           (4 * n * std::sqrt(3.0L));
}


void print_partition_to_file_buffered(unsigned int* partition,
                                      unsigned int partition_size,
                                      std::ofstream& file, std::string& buffer,
                                      unsigned int buffer_limit) {
    std::ostringstream oss;

    for (unsigned int i { 0 }; i < partition_size; i++) {
        oss << partition[i] << ' ';
    }
    oss << '\n';

    buffer += oss.str();

    if (buffer.size() >= buffer_limit) {
        file << buffer;
        buffer.clear();
    }
}
