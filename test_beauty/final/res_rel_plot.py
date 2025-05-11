from os import replace
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


def read_colors_bin(filename, int_t, sample_fraction):

    # filename      :   name of the file to open
    # int_t        :   'i' int or 'I' unsigned int
    # sample_fraction      :   sample_fraction of points to plot ( 0 < sample_fraction <= 1 )

    record_size = 4

    match int_t:
        case "i":
            record_type = "i"
        case "I":
            record_type = "I"
        case _:
            print("Invalid record type in read_colors_bin function. Default type int.")
            record_type = "i"

    n_colors_vals = []

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
                n_colors = struct.unpack(record_type, record)
                n_colors_vals.append(n_colors)
                i = i + 1
            elif i == reset_count:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                n_colors = struct.unpack(record_type, record)
                n_colors_vals.append(n_colors)
                i = 0
            else:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                i = i + 1

    return n_colors_vals


def main():

    sample_fraction = 1
    arguments = sys.argv[1:]

    path = "./partitions/"

    # ---------------------- Input Check ------------------------
    if len(arguments) == 0:

        n = int(input("Insert the size. (Width x Height) : \t"))
        filename = "partitions_of_" + str(n)

        res_vals, rel_vals = read_resrel_bin(
            path + filename + "_resrel.bin", "d", sample_fraction
        )

        n_colors_vals = read_colors_bin(
            path + filename + "_colors.bin", "I", sample_fraction
        )

    elif len(arguments) == 1:

        n = int(arguments[0])
        filename = "partitions_of_" + str(n)

        res_vals, rel_vals = read_resrel_bin(
            path + filename + "_resrel.bin", "d", sample_fraction
        )

        n_colors_vals = read_colors_bin(
            path + filename + "_colors.bin", "I", sample_fraction
        )

    elif len(arguments) == 3:

        n = int(arguments[0])

        res_left_bound = float(arguments[1])
        res_right_bound = float(arguments[2])

        filename = (
            "["
            + f"{res_left_bound:g}"
            + ","
            + f"{res_right_bound:g}"
            + "]_"
            + "partitions_of_"
            + str(n)
        )

        # filename_resrel = (
        #     path
        #     + "["
        #     + f"{res_left_bound:g}"
        #     + ","
        #     + f"{res_right_bound:g}"
        #     + "]_"
        #     + filename
        #     + "_resrel.bin"
        # )
        #
        # filename_colors = (
        #     path
        #     + "["
        #     + f"{res_left_bound:g}"
        #     + ","
        #     + f"{res_right_bound:g}"
        #     + "]_"
        #     + filename
        #     + "_colors.bin"
        # )

        res_vals, rel_vals = read_resrel_bin(
            path + filename + "_resrel.bin", "d", sample_fraction
        )
        n_colors_vals = read_colors_bin(
            path + filename + "_colors.bin", "I", sample_fraction
        )

    else:
        print("Error: Too many arguments")
        sys.exit(1)

    # ---------------------- Print plot ------------------------
    plt.figure(figsize=(12, 10))

    res_vals = np.array(res_vals)
    rel_vals = np.array(rel_vals)
    n_colors_vals = np.array(n_colors_vals)

    sample_size = min(10, len(res_vals))
    indices_sample = np.random.choice(len(res_vals), size=sample_size, replace=False)

    res_vals_sample = res_vals[indices_sample]
    rel_vals_sample = rel_vals[indices_sample]
    n_colors_vals_sample = n_colors_vals[indices_sample]

    # scatter_colors = plt.scatter(
    #     res_vals, rel_vals, c=n_colors_vals, cmap="viridis", alpha=0.7, marker=".", s=2
    # )

    scatter_colors = plt.scatter(
        res_vals_sample,
        rel_vals_sample,
        c=n_colors_vals_sample,
        cmap="viridis",
        alpha=0.7,
        marker=".",
        s=4,
    )

    # Add a colorbar to show the mapping from z-value to color:
    cbar = plt.colorbar(scatter_colors)
    cbar.set_label("Number of colors")

    plt.title(
        "n = "
        + str(n)
        + "    N. Partitions ~ "
        + str(int(len(res_vals) / sample_fraction))
        + "    Points shown : "
        + str(sample_size)
    )
    plt.xlabel("Resolution")
    plt.ylabel("Relevance")
    plt.xticks(rotation=60, ha="right")

    plt.grid(True)

    plt.tight_layout()
    plt.savefig(path + filename + "_resrel.svg", format="svg", dpi="figure")

    plt.show()

    # Plot the histogram

    plt.hist(n_colors_vals, bins=n, edgecolor="black")  # 'bins' controls number of bars
    plt.title("Histogram Number of Colors")
    plt.xlabel("Colors")
    plt.ylabel("Frequency")
    plt.grid(True)

    # Show the plot
    plt.show()


if __name__ == "__main__":
    main()
