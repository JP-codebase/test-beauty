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


def main():

    sample_fraction = 1
    arguments = sys.argv[1:]

    path = "./partitions/"

    # ---------------------- Input Check ------------------------

    if len(arguments) == 4:

        n = int(arguments[0])

        res_left_bound = float(arguments[1])
        res_right_bound = float(arguments[2])
        n_colors = int(arguments[3])

        filename = (
            "c"
            + f"{n_colors}"
            + "_["
            + f"{res_left_bound:g}"
            + ","
            + f"{res_right_bound:g}"
            + "]_"
            + "partitions_of_"
            + f"{n}"
        )

        res_vals, rel_vals = read_resrel_bin(
            path + filename + "_resrel.bin", "d", sample_fraction
        )

    else:
        print(
            "Error: You need to provide 4 arguments: size res_min res_max number_colors"
        )
        sys.exit(1)

    # ---------------------- Resolution-Relevance Plot ------------------------

    plt.figure(figsize=(6, 5))

    res_vals = np.array(res_vals)
    rel_vals = np.array(rel_vals)

    sample_size = min(50000, len(res_vals))
    indices_sample = np.random.choice(len(res_vals), size=sample_size, replace=False)

    res_vals_sample = res_vals[indices_sample]
    rel_vals_sample = rel_vals[indices_sample]

    plt.scatter(
        res_vals_sample,
        rel_vals_sample,
        # alpha=0.7,
        marker=".",
        # s=4,
    )

    plt.title(
        "n = "
        + str(n)
        + "    N. Partitions ~ "
        + str(int(len(res_vals) / sample_fraction))
        + "    Points shown : "
        + str(sample_size)
    )

    # Edit Axis
    plt.xlabel("Resolution")
    plt.ylabel("Relevance")
    plt.xticks(rotation=60, ha="right")

    plt.grid(True)

    # Show the plot
    plt.tight_layout()
    plt.savefig(path + filename + "_resrel.svg", format="svg", dpi="figure")
    plt.show()


if __name__ == "__main__":
    main()
