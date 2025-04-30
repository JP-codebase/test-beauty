import pygame
import numpy as np
import matplotlib.pyplot as plt

# Window
pygame.init()

grid_shape = (8, 8)
# grid_shape = (6, 5)  # (number of rows, number of columns)
cell_size = 150

filename = "metropolis.txt"
file = open(filename, "r")

size = (grid_shape[1] * cell_size, grid_shape[0] * cell_size)  # (length, height)
screen = pygame.display.set_mode(size)
clock = pygame.time.Clock()

# Color map
n = grid_shape[0] * grid_shape[1]
cmap = plt.cm.get_cmap('tab20', n)
colors = cmap(np.arange(n))[:, :3]
colors = (colors * 255).round().astype(np.uint8)
print(len(colors))

# colors_255 = (colors * 255).round().astype(np.uint8)
# colors = plt.cm.tab10(np.linspace(0, 1, n))[:, :3] * 255
# colors = colors.astype(int)

input("Starting?")

counter = 1;
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
            file.close()
            pygame.quit()

    # Read grid from file
    arr = []
    try:
        arr = file.readline().split(sep=" ")
        arr.pop()

        grid = np.array(arr, np.int16)
        grid = grid.reshape(grid_shape[0], grid_shape[1])
        rgb_grid = colors[grid]
    except ValueError:
        print("Skipping invalid line")
        break

    for i in range(grid_shape[0]):
        for j in range(grid_shape[1]):
            color = tuple(rgb_grid[i, j])
            pygame.draw.rect(
                screen, color, (j * cell_size, i * cell_size, cell_size, cell_size)
            )

    # Generate grid of RGB colors
    # grid = np.random.randint(0, 255, (grid_shape[0], grid_shape[1], 3))

    # Prints the grid
    # for i in range(grid_shape[0]):
    #     for j in range(grid_shape[1]):
    #         color = grid[i, j]
    #         pygame.draw.rect(
    #             screen, color, (j * cell_size, i * cell_size, cell_size, cell_size)
    #         )

    pygame.display.flip()
    pygame.image.save(screen, f"output{counter}.png")
    counter = counter + 1;
    clock.tick(60)  # FPS

input("Execution finished")
file.close()

pygame.quit()

# TODO

# import numpy as np
# import pygame
# import os
# import time
# 
# last_loaded_frame = -1
# 
# while running:
#     # Look for the latest available file
#     available_frames = sorted([
#         int(fname.split('_')[1].split('.')[0])
#         for fname in os.listdir('output')
#         if fname.startswith('frame_')
#     ])
# 
#     if available_frames and available_frames[-1] > last_loaded_frame:
#         frame_number = available_frames[-1]
#         grid = np.load(f"output/frame_{frame_number}.npy")
#         last_loaded_frame = frame_number
# 
#     draw_grid(screen, grid)
#     pygame.display.flip()
#     clock.tick(30)

# int frame_counter = 0;
# int save_every_n_frames = 10;
# 
# while (true) {
#     generate_next_grid(grid);
# 
#     if (frame_counter % save_every_n_frames == 0) {
#         save_grid_to_file(grid, frame_counter);
#     }
# 
#     frame_counter++;
# }
