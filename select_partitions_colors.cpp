#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "./headers/precision.h"

const char RED[] = "\033[31m";
const char RESET_STYLE[] = "\033[m";

std::string real_t_without_trailing_zeros(real_t f);

int main(int argc, char* argv[]) {

    /* ------------------- Variable Declarations --------------------------- */

    unsigned int partition_size {};
    unsigned int n_colors {};
    unsigned int n_partitions;
    real_t res_min {};
    real_t res_max {};

    std::vector<real_t> res_list;
    std::vector<real_t> rel_list;
    std::vector<std::string> partition_list;

    /* ------------------------ Input Check --------------------------------- */

    // Command line arguments
    {
        if (argc != 6) {
            std::cerr << RED << "Error: Expected 5 arguments: " << RESET_STYLE
                      << "size resolution_min resolution_max n_colors. " << RED
                      << "Got : " << RESET_STYLE << (argc - 1) << std::endl;
            return 1;
        }

        try {
            partition_size = std::stoul(argv[1]);
            res_min = std::stod(argv[2]);
            res_max = std::stod(argv[3]);
            n_colors = std::stoul(argv[4]);
            n_partitions = std::stoul(argv[5]);


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

    std::string path { "./partitions/" };


    /* ------------------------ Open Input Files --------------------------- */

    std::string filename = "[" + real_t_without_trailing_zeros(res_min) + "," +
                           real_t_without_trailing_zeros(res_max) + "]_" +
                           "partitions_of_" + std::to_string(partition_size);


    // Input from txt file
    std::ifstream input_file_partitions_txt(path + filename + ".txt");

    if (!input_file_partitions_txt) {
        std::cerr << RED << "Error: Could not open " << path + filename
                  << ".txt"
                  << " for reading." << RESET_STYLE << std::endl;
        return 1;
    }

    // Input from binary file
    std::ifstream input_file_resrel_bin(path + filename + "_resrel.bin",
                                        std::ios::binary);

    if (!input_file_resrel_bin) {
        std::cerr << RED << "Error: Could not open file " << path + filename
                  << "_resrel.bin"
                  << " for reading." << RESET_STYLE << std::endl;
        return 1;
    }

    std::ifstream input_file_colors_bin(path + filename + "_colors.bin",
                                        std::ios::binary);

    if (!input_file_colors_bin) {
        std::cerr << RED << "Error: Could not open file " << path + filename
                  << "_colors.bin"
                  << " for reading." << RESET_STYLE << std::endl;
        return 1;
    }


    /* ------------------------ Open Output Files --------------------------- */

    // Output to txt file
    std::ofstream output_file_partitions_txt(
        path + "c" + std::to_string(n_colors) + "_" + filename + ".txt");


    if (!output_file_partitions_txt) {
        std::cerr << RED << "Error: Could not open "
                  << (path + "c" + std::to_string(n_colors) + "_" + filename +
                      ".txt")
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }

    // Output to binary file
    std::ofstream output_file_resrel_txt(path + "c" + std::to_string(n_colors) +
                                         "_" + filename + "_resrel.txt");


    if (!output_file_resrel_txt) {
        std::cerr << RED << "Error: Could not open file "
                  << (path + "c" + "_" + filename + "_resrel.txt")
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }


    /* ----------------- Selecting by Color from File --------------------- */

    real_t res { -1 }, rel { -1 };
    const unsigned int size_real_t { sizeof(res) };

    unsigned int n_colors_read { 0 };
    const unsigned int size_u_int { sizeof(n_colors) };

    std::string partition(2 * partition_size, '\0');

    while (true) {

        input_file_resrel_bin.read(reinterpret_cast<char*>(&res), size_real_t);

        input_file_resrel_bin.read(reinterpret_cast<char*>(&rel), size_real_t);

        input_file_colors_bin.read(reinterpret_cast<char*>(&n_colors_read),
                                   size_u_int);

        getline(input_file_partitions_txt, partition);

        if (input_file_resrel_bin.eof()) {
            break;
        }

        if (n_colors_read == n_colors) {
            res_list.push_back(res);
            rel_list.push_back(rel);
            partition_list.push_back(partition);
        }
    }


    /* ----------------- Ordering Partitions by Relevance ------------------- */

    if (n_partitions > res_list.size()) {
        std::cout << "Error : There are no " << n_partitions
                  << " partitions of " << n_colors << " colors." << "\nUsing "
                  << res_list.size() << " as a fallback number." << std::endl;
        n_partitions = res_list.size();
    }

    real_t max;
    int index_max;

    res_list.shrink_to_fit();
    rel_list.shrink_to_fit();
    partition_list.shrink_to_fit();

    std::vector<real_t> res_list_ordered(res_list.size());
    std::vector<real_t> rel_list_ordered(rel_list.size());
    std::vector<std::string> partition_list_ordered(partition_list.size());

    for (int i = rel_list.size() - 1; i >= 0; --i) {
        max = rel_list[0];
        index_max = 0;
        for (int j = 0; j < i + 1; j++) {
            if (rel_list[j] > max) {
                max = rel_list[j];
                index_max = j;
            }
        }
        // std::cout << i << " : " << "\tMax : " << max << std::endl;
        res_list_ordered.at(i) = res_list.at(index_max);
        res_list.erase(res_list.begin() + index_max);

        rel_list_ordered.at(i) = rel_list.at(index_max);
        rel_list.erase(rel_list.begin() + index_max);

        partition_list_ordered.at(i) = partition_list.at(index_max);
        partition_list.erase(partition_list.begin() + index_max);
    }


    /* --------------------- Output to Files ---------------------------- */

    unsigned int step {};
    step = (rel_list_ordered.size() / n_partitions);

    if (step * n_partitions != rel_list_ordered.size()) {
        step++;
    }


    for (int i = 0; i < rel_list_ordered.size(); i = i + step) {

        output_file_resrel_txt << res_list_ordered[i] << " "
                               << rel_list_ordered[i] << "\n";

        output_file_partitions_txt << (partition_list_ordered[i] + "\n");
    }

    input_file_partitions_txt.close();
    input_file_resrel_bin.close();
    input_file_colors_bin.close();

    output_file_partitions_txt.close();
    output_file_resrel_txt.close();

    return 0;
}

/* ---------------------- Function Implementations ------------------------- */

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
