import matplotlib.pyplot as plt

import numpy as np
import struct
import sys


def read_resrel_bin(filename, real_t, sample_fraction):

    # filename      :   name of the file to open
    # real_t        :   'f' float or 'd' double
    # sample_fraction      :   sample_fraction of points to plot ( 0 < sample_fraction <= 1 )

    match real_t:
        case "f":
            record_type = "ff"
            record_size = 8
        case "d":
            record_type = "dd"
            record_size = 16
        case _:
            print(
                "Invalid record type in read_resrel_bin function. Default type float."
            )
            record_type = "ff"
            record_size = 8

    res_vals, rel_vals = [], []

    # Store data if counter < threshold and discard them if threshold < counter < reset_counter
    threshold = sample_fraction * 10 ** (-np.floor(np.log10(sample_fraction)))
    reset_count = 1 * 10 ** (-np.floor(np.log10(sample_fraction)))

    i = 0

    with open(filename, "rb") as file:
        while True:
            if i < threshold:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                res, rel = struct.unpack(record_type, record)
                # print ("res : ", res, "\t rel : ", rel)
                res_vals.append(res)
                rel_vals.append(rel)
                i = i + 1
            elif i == reset_count:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                res, rel = struct.unpack(record_type, record)
                res_vals.append(res)
                rel_vals.append(rel)
                i = 0
            else:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                i = i + 1

    return res_vals, rel_vals


def main():
    sample_fraction = 1
    arguments = sys.argv[1:]

    if len(arguments) == 0:

        n = int(input("Insert the size. (Width x Height) : \t"))
        res_vals, rel_vals = read_resrel_bin(
            "./partitions/partitions_of_" + str(n) + "_resrel.bin", "d", sample_fraction
        )

    elif len(arguments) == 1:

        n = int(arguments[0])
        res_vals, rel_vals = read_resrel_bin(
            "./partitions/partitions_of_" + str(n) + "_resrel.bin", "d", sample_fraction
        )

    elif len(arguments) == 3:

        n = int(arguments[0])
        res_left_bound = float(arguments[1])
        res_right_bound = float(arguments[2])
        # filename = "partitions/[" + str(res_left_bound) + "," + str(res_right_bound) + "]_partitions_of_" + str(n) + "_resrel.bin"
        # filename = "partitions/[ f'{res_left_bound:g}, f'{res_right_bound:g}]_partitions_of_" + str(n) + "_resrel.bin"
        filename = (
            "partitions/["
            + f"{res_left_bound:g}"
            + ","
            + f"{res_right_bound:g}"
            + "]_partitions_of_"
            + str(n)
            + "_resrel.bin"
        )
        res_vals, rel_vals = read_resrel_bin(filename, "d", sample_fraction)

    else:
        print("Error: Too many arguments")
        sys.exit(1)

    # Print plot

    n_colors = np.int16(np.random.rand(len(res_vals))*n)
    plt.figure(figsize=(6, 5))
    scatter_colors = plt.scatter(res_vals, rel_vals, c=n_colors, cmap="viridis", alpha=0.7, marker=".", s=2)

    # Add a colorbar to show the mapping from z-value to color:
    cbar = plt.colorbar(scatter_colors)
    cbar.set_label("Number of colors")

    plt.title(
        "n = "
        + str(n)
        + "  N. Partitions ~ "
        + str(int(len(res_vals) / sample_fraction))
    )
    plt.xlabel("Resolution")
    plt.ylabel("Relevance")
    plt.xticks(rotation=60, ha="right")

    plt.grid(True)

    plt.tight_layout()
    plt.show()
    # plt.savefig()


if __name__ == "__main__":
    main()
