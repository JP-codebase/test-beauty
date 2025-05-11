#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "headers/nexpar_functions.h"
#include "headers/precision.h"
#include "headers/quantifying_information.h"


const char RED[] = "\033[31m";
const char RESET_STYLE[] = "\033[m";

// Number of partition of the integer n
long double approximate_number_of_partitions(unsigned int n);

// Print to file, buffered plain text
void print_partition_to_file_buffered(unsigned int* partition,
                                      unsigned int partition_size,
                                      std::ofstream& file, std::string& buffer,
                                      unsigned int buffer_limit = 1024);


int main(int argc, char* argv[]) {

    unsigned int partition_size;

    /* ------------------------ Input Check --------------------------------- */

    // Command line arguments
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
                std::cerr << RED << "Error: " << e.what() << "\n"
                          << RESET_STYLE;
                return 1;
            }
        } else {
            std::cerr << RED << "Error: Incorrect number of arguments.\n"
                      << RESET_STYLE;
            return 1;
        }
    }

    const long double n_of_partitions { approximate_number_of_partitions(
        partition_size) };


    // Asking for confirmation
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


    /* ------------------------ Open Output Files --------------------------- */


    std::string path = "./partitions/";
    std::string filename = "partitions_of_" + std::to_string(partition_size);

    // Outpupt to txt file
    std::ofstream out_file_partitions_txt(path + filename + ".txt");

    if (!out_file_partitions_txt) {
        std::cerr << RED << "Error: Could not open " << path + filename
                  << ".txt"
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }

    // Output to binary file
    std::ofstream out_file_resrel_bin(path + filename + "_resrel.bin",
                                      std::ios::binary);

    if (!out_file_resrel_bin) {
        std::cerr << RED << "Error: Could not open file " << path + filename
                  << "_resrel.bin"
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }

    std::ofstream out_file_colors_bin(path + filename + "_colors.bin",
                                      std::ios::binary);

    if (!out_file_colors_bin) {
        std::cerr << RED << "Error: Could not open file " << path + filename
                  << "_colors.bin"
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }

    std::string buffer;


    /* ----------------------- Variable Declariation ----------------------- */

    // Partition declaration
    unsigned int* partition { new unsigned int[partition_size] { 0 } };
    unsigned int* dg_profile { new unsigned int[partition_size] };
    real_t res;
    real_t rel;


    // Initialize first partition: [partition_size, 0, ..., 0]
    partition[0] = partition_size;
    degeneracy_profile(partition, dg_profile, partition_size);

    // Resolution
    res = resolution_degeneracy(dg_profile, partition_size);
    real_t min_resolution { res };
    real_t max_resolution { res };

    // Relevance
    rel = relevance_degeneracy(dg_profile, partition_size);
    real_t min_relevance { rel };
    real_t max_relevance { rel };

    // Number of colors
    unsigned int n_colors { number_of_colors_partition(partition,
                                                       partition_size) };


    /* ------------------------ UI ---------------------------------------- */

    // Status bar
    unsigned long int counter { 1 };
    constexpr long double kProgressStepPercent = 10.0L;

    const long double runtime_fraction { n_of_partitions / 100.0L *
                                         kProgressStepPercent };
    unsigned int status_bar { 0 };

    // Precision for floating point output
    long int default_float_precision { std::cout.precision() };   //
    long int custom1_float_precision { 1 };
    long int custom2_float_precision { 2 };

    // Time
    auto start = std::chrono::steady_clock::now();
    auto time_step1 = std::chrono::steady_clock::now();
    auto time_step2 = std::chrono::steady_clock::now();


    /* ------------------------ Main Loop ---------------------------------- */

    // Starting the loop. It ends when reaches the partition [ 1, 1, ..., 1 ]
    bool execution_completed = false;
    do {

        // Chrono
        if (static_cast<long double>(counter) > (runtime_fraction)) {

            status_bar++;
            time_step2 = std::chrono::steady_clock::now();

            std::cout << std::setw(10) << std::right

                      << std::setprecision(custom1_float_precision)
                      << std::fixed << (status_bar * kProgressStepPercent)
                      << "% : "

                      << std::setw(10) << std::right
                      << std::setprecision(custom2_float_precision)
                      << std::scientific
                      << std::chrono::duration<double>(time_step2 - time_step1)
                             .count()
                      << " seconds" << std::endl;


            time_step1 = time_step2;
            counter = 0;
        }


        // Output
        print_partition_to_file_buffered(
            partition, partition_size, out_file_partitions_txt, buffer);

        out_file_resrel_bin.write(reinterpret_cast<const char*>(&res),
                                  sizeof(res));

        out_file_resrel_bin.write(reinterpret_cast<const char*>(&rel),
                                  sizeof(rel));
        
        out_file_colors_bin.write(reinterpret_cast<const char*>(&n_colors),
                                  sizeof(n_colors));

        // Generate the next partition
        execution_completed = nexpar_ptr(partition, partition_size);
        counter++;

        degeneracy_profile(partition, dg_profile, partition_size);

        res = resolution_degeneracy(dg_profile, partition_size);
        if (res < min_resolution) {
            min_resolution = res;
        } else if (res > max_resolution) {
            max_resolution = res;
        }

        rel = relevance_degeneracy(dg_profile, partition_size);
        if (rel < min_relevance) {
            min_relevance = rel;
        } else if (rel > max_relevance) {
            max_relevance = rel;
        }
        
        n_colors = number_of_colors_partition(partition, partition_size);

    } while (!execution_completed);


    // Empty the buffer

    if (!buffer.empty()) {
        out_file_partitions_txt << buffer;
        buffer.clear();
    }

    time_step2 = std::chrono::steady_clock::now();
    std::cout << std::setw(10) << std::right

              << std::setprecision(custom1_float_precision) << std::fixed
              << (100) << "% : "

              << std::setw(10) << std::right
              << std::setprecision(custom2_float_precision) << std::scientific
              << std::chrono::duration<double>(time_step2 - time_step1).count()
              << " seconds" << std::endl;


    /* ---------------------- Ending the Program --------------------------- */

    std::cout << std::endl;
    std::cout << "Execution time : "
              << std::chrono::duration<double>(time_step2 - start).count()
              << " seconds" << std::endl;

    std::cout << std::endl;

    std::cout << "----------------------------" << std::endl;

    // Max and Min Relevance and Resolution

    std::cout << std::endl;

    std::cout << std::setw(20) << "Min resolution : " << std::setw(10)
              << min_resolution << std::setw(20) << std::setw(20)

              << "Min relevance : " << std::setw(10) << min_relevance
              << std::setw(20) << std::setw(20) << std::endl;

    std::cout << std::setw(20) << "Max resolution : " << std::setw(10)
              << max_resolution << std::setw(20)
              << "Max relevance : " << std::setw(10) << max_relevance
              << std::setw(20) << std::endl;

    std::cout << std::endl;
    std::cout << "----------------------------" << std::endl;

    delete[] partition;
    delete[] dg_profile;

    out_file_partitions_txt.close();
    out_file_resrel_bin.close();
    out_file_colors_bin.close();
    return 0;
}


// Function implementations
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
