#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "./headers/precision.h"
#include "./headers/quantifying_information.h"


/* ------------------------ Functions --------------------------------- */

std::string real_t_without_trailing_zeros(real_t f);

void random_colored_grid_filling(unsigned int* colored_grid,
                                 std::vector<unsigned int>& partition,
                                 unsigned int array_size);

real_t next_state(unsigned int* colored_grid, unsigned int width,
                  unsigned int height, unsigned int colored_grid_size);

unsigned int random_int_range(unsigned int n);


/* ------------------------ Constants --------------------------------- */

const char RED[] = "\033[31m";
const char RESET_STYLE[] = "\033[m";

const real_t b = 0.05;
const char boundary_conditions { 'o' };


/* ------------------- Random Number Generator -------------------------- */

std::random_device seed;              // Random seed
std::minstd_rand generator(seed());   // Linear congruential engine that
                                      // generates random numbers using a seed

std::uniform_int_distribution<unsigned int> int_distribution(0, 100);
std::uniform_real_distribution<real_t> real_distribution(0.0, 1.0);


int main(int argc, char* argv[]) {

    /* ------------------------ Input Check ------------------------------ */

    unsigned int width {};
    unsigned int height {};

    unsigned int n_colors {};

    real_t res_min {};
    real_t res_max {};

    // Command line arguments
    {
        if (argc != 6) {
            std::cerr << RED << "Error: Expected 5 arguments: " << RESET_STYLE
                      << "width height resolution_min resolution_max n_colors. "
                      << RED << "Got : " << RESET_STYLE << (argc - 1)
                      << std::endl;
            return 1;
        }

        try {
            width = std::stoul(argv[1]);
            height = std::stoul(argv[2]);
            res_min = std::stod(argv[3]);
            res_max = std::stod(argv[4]);
            n_colors = std::stoul(argv[5]);

            if (res_min < 0 || res_max < 0 || res_min > res_max)
                throw std::out_of_range("Min and Max must be ≥0 and Min ≤ Max");
        } catch (const std::invalid_argument&) {
            std::cerr << RED << "Error: Arguments must be numeric.\n"
                      << RESET_STYLE;
            return 1;
        } catch (const std::out_of_range& e) {
            std::cerr << RED << "Error: " << e.what() << "\n" << RESET_STYLE;
            return 1;
        }
    }

    unsigned int partition_size { width * height };


    /* --------------------- Open Input Files --------------------------- */

    std::string path { "./partitions/" };
    std::string filename { "c" + std::to_string(n_colors) + "_[" +
                           real_t_without_trailing_zeros(res_min) + "," +
                           real_t_without_trailing_zeros(res_max) + "]_" +
                           "partitions_of_" + std::to_string(partition_size) };


    // Input from txt file
    std::ifstream input_file_partitions_txt(path + filename + ".txt");

    if (!input_file_partitions_txt) {
        std::cerr << RED << "Error: Could not open " << path + filename
                  << ".txt"
                  << " for reading." << RESET_STYLE << std::endl;
        return 1;
    }


    /* --------------- Reading Selected Partitions from File -------------- */


    std::string line;
    std::vector<std::vector<unsigned int>> partition_list;

    while (std::getline(input_file_partitions_txt, line)) {
        std::istringstream iss(line);
        std::vector<unsigned int> partition;
        unsigned int x;

        while (iss >> x) {
            partition.push_back(x);
        }

        if (iss.fail() && !iss.eof()) {
            std::cerr << RED << "Error : Non-integer data found on line "
                      << line << "\n";
            continue;
        }
        partition_list.push_back(std::move(partition));
    }


    /* ------------------------ Main Loop --------------------------- */

    unsigned int* colored_grid { new unsigned int[partition_size] };

    real_t energy;
    real_t energy_min;
    real_t energy_max;
    real_t target_energy;

    for (int p = 0; p < partition_list.size(); p++) {

        std::ofstream output_file_colored_grid_txt(
            path + std::to_string(p + 1) + "_" + filename + ".txt");

        if (!output_file_colored_grid_txt) {
            std::cerr << RED << "Error : Could not open "
                      << (path + std::to_string(p + 1) + "_" + filename +
                          ".txt")
                      << " for writing." << RESET_STYLE << std::endl;
        }

        random_colored_grid_filling(
            colored_grid, partition_list[p], partition_size);

        energy_min = energy_colored_grid(colored_grid, width, height);
        energy_max = energy_min;

        const int iterations { 1000000 };

        std::cout << "Partition " << p + 1 << " : ";
        const int ten_percent { iterations / 10 };

        for (int it = 0; it < iterations; it++) {
            energy = next_state(colored_grid, width, height, partition_size);

            // if (energy > energy_max) {
            //     energy_max = energy;
            // } else if (energy < energy_min) {
            //     energy_min = energy;
            // }

            if ((it % ten_percent) == 0) {
                std::cout << '*' << std::flush;
            }
        }
        std::cout << std::endl;


        // std::cout << std::setw(20) << "Min energy : " << std::setw(10)
        //           << energy_min << std::endl;
        // std::cout << std::setw(20) << "Max energy : " << std::setw(10)
        //           << energy_max << std::endl;

        if (p == 0) {
            // std::cout << "Insert target energy : ";
            // std::cin >> target_energy;
            target_energy = energy;
        }


        for (int it = 0; it < 200; it++) {

            if (target_energy ==
                next_state(colored_grid, width, height, partition_size)) {

                for (int j = 0; j < partition_size; j++) {
                    output_file_colored_grid_txt << colored_grid[j] << " ";
                }
                output_file_colored_grid_txt << "\n";
            }
        }

        std::cout << std::endl;
        output_file_colored_grid_txt.close();
    }


    return 0;
}

/* ---------------------- Function Implementations -------------------------
 */

std::string real_t_without_trailing_zeros(real_t f) {

    std::string real_t_str { std::to_string(f) };

    for (int i = real_t_str.size() - 1; i >= 0; i--) {

        if (real_t_str[i] == '0' || real_t_str[i] == '.') {
            real_t_str.erase(i);

        } else if ((real_t_str[i] == '1') || (real_t_str[i] == '2') ||
                   (real_t_str[i] == '3') || (real_t_str[i] == '4') ||
                   (real_t_str[i] == '5') || (real_t_str[i] == '6') ||
                   (real_t_str[i] == '7') || (real_t_str[i] == '8') ||
                   (real_t_str[i] == '9'))
            break;
    }

    return real_t_str;
}


unsigned int random_int_range(unsigned int n) {

    int_distribution.param(
        typename decltype(int_distribution)::param_type { 0, n });
    return int_distribution(generator);
};


void random_colored_grid_filling(unsigned int* colored_grid,
                                 std::vector<unsigned int>& partition,
                                 unsigned int array_size) {

    // Not-yet colored cells list
    std::vector<unsigned int> list(array_size);
    for (unsigned int i = 0; i < array_size; i++) {
        list[i] = i;
    }

    // A random cell is colored using the color partition[i]
    int temp;
    for (unsigned int i = 0; i < array_size; i++) {
        for (unsigned int j = 0; j < partition[i]; j++) {
            temp = random_int_range(size(list) - 1);
            colored_grid[list[temp]] = i;
            list.erase(list.begin() + temp);
        }
    }
}


// Return the energy of the next state
real_t next_state(unsigned int* colored_grid, unsigned int width,
                  unsigned int height, unsigned int colored_grid_size) {

    real_t energy1 { energy_colored_grid(
        colored_grid, width, height, boundary_conditions) };
    // std::cout << "Energy : " << energy1 << std::endl;

    // Pick a pair of random cells
    unsigned int cell1 { random_int_range(colored_grid_size - 1) };
    unsigned int cell2 { random_int_range(colored_grid_size - 1) };

    // std::cout << "Swapping : (" << (cell1 / width + 1) << ", "
    //           << (cell1  % width + 1) << ") \033[3"
    //           << (colored_grid[cell1] + 1) << "m"
    //           << "■" << ' ';
    //
    // std::cout << "\033[37m";   // White text
    // // std::cout << "\033[30m";   // Black text
    //
    // std::cout << " -> (";
    // std::cout << (cell2  / width + 1) << ", " << (cell2  % width + 1)
    //           << ") \033[3" << (colored_grid[cell2] + 1) << "m"
    //           << "■" << ' ';
    //
    // std::cout << "\033[37m";   // White text
    // // std::cout << "\033[30m";   // Black text
    //
    // std::cout << std::endl;

    if (cell1 == cell2) {
        // return false;
        return energy1;
    }

    // Swap cell1 e cell2 in colored_grid
    unsigned int temp { colored_grid[cell1] };
    colored_grid[cell1] = colored_grid[cell2];
    colored_grid[cell2] = temp;

    real_t energy2 { energy_colored_grid(
        colored_grid, width, height, boundary_conditions) };

    // Probability of Accepting the swap
    real_t delta_energy = energy2 - energy1;
    real_t acceptance =
        (delta_energy > 0) ? std::exp(-1 * b * delta_energy) : 1.0;

    real_t rand_num { real_distribution(generator) };

    // std::cout << "Acceptance : " << acceptance << std::endl;
    // (rand_num < acceptance) ? std::cout << "Accepted"
    //                         : std::cout << "Discarded";
    // std::cout << std::endl;

    if (rand_num <= acceptance) {

        return energy1;
    } else {

        temp = colored_grid[cell1];
        colored_grid[cell1] = colored_grid[cell2];
        colored_grid[cell2] = temp;
        return energy2;
    }

    // for (int i = 0; i < height; i++) {
    //     for (int j = 0; j < width; j++) {
    //         std::cout << "\033[3" << (colored_grid[i * width + j] + 1) << "m"
    //                   << "■" << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << "\033[37m";   // White text
    // // std::cout << "\033[30m";   // Black text
    //
    // std::cout << "----------------------------------------------" <<
    // std::endl; std::cout << std::endl;
}
