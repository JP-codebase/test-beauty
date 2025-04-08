import pygame
import numpy as np
import matplotlib.pyplot as plt

# Window
pygame.init()

grid_shape = (2, 3)  # (number of rows, number of columns)
cell_size = 150

filename = "./partitions/6_set_partitions.txt"
file = open(filename, "r")

size = (grid_shape[1] * cell_size, grid_shape[0] * cell_size)  # (length, height)
screen = pygame.display.set_mode(size)
clock = pygame.time.Clock()

# Color map
n = grid_shape[0] * grid_shape[1]
colors = plt.cm.tab10(np.linspace(0, 1, n))[:, :3] * 255
colors = colors.astype(int)

input("Starting?")

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
    # pygame.image.save(screen, "output.png")
    clock.tick(30)  # FPS

input("Execution finished")
file.close()

pygame.quit()
