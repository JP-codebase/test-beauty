import matplotlib.pyplot as plt
import numpy as np
import sys


def plot_grid_high_contrast(grid):

    grid_array = np.array(grid)

    # Finds unique values and assign each a random color
    unique_vals = np.unique(grid_array)

    cmap = plt.get_cmap('tab20', len(unique_vals))

    # Mapping from grid values to indices in the colormap
    val_to_idx = {val: idx for idx, val in enumerate(unique_vals)}  # ?
    grid_indices = np.vectorize(val_to_idx.get)(grid_array)

    fig, ax = plt.subplots()

    mesh = ax.pcolormesh(
        grid_indices, cmap=cmap, edgecolors="k", linewidth=0.7, snap=True
    )

    ax.invert_yaxis()

    ax.set_xticks([])
    ax.set_yticks([])

    ax.set_frame_on(False)

    plt.tight_layout()
    plt.show()


def main():

    arguments = sys.argv[1:]

    # ---------------------- Input Check ------------------------

    if len(arguments) == 6:

        width = int(arguments[0])
        height = int(arguments[1])

        res_left_bound = float(arguments[2])
        res_right_bound = float(arguments[3])

        n_colors = int(arguments[4])
        n_partitions = int(arguments[5])

        n = width * height

        path_partitions = "./partitions/"
        path_images = "./images/"

        filename = (
            "c"
            + f"{n_colors}"
            + "_["
            + f"{res_left_bound:g}"
            + ","
            + f"{res_right_bound:g}"
            + "]_"
            + "partitions_of_"
            + str(n)
            + ".txt"
        )

    else:
        print(f"Error: Expected 6 arguments, founded {len(arguments)}")
        print("Arguments : width height res_min res_max n_colors n_partitions")
        sys.exit(1)


    for k in range(0, n_partitions):

        # Read grid from file
        file = open(path_partitions + f"{k + 1}_" + filename, "r")

        while(True):
            arr = []
            try:
                line = file.readline()
                arr = line.split(sep=" ")

                arr.pop()

                grid = np.array(arr, np.int16)
                grid = grid.reshape(width, height)

                plot_grid_high_contrast(grid)
                
            except  ValueError:
                # print("Skipping invalid line")
                break

        file.close()

    # width, height = 10, 6
    # random_grid = np.random.randint(0, 12, size=(height, width))
    # plot_grid_high_contrast(random_grid)

if __name__ == "__main__":
    main()
