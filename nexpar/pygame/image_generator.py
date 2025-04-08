import pygame
import numpy as np

pygame.init()
size = (600, 600)
screen = pygame.display.set_mode(size)
clock = pygame.time.Clock()

grid_size = 20
cell_size = size[0] // grid_size

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Generate grid of RGB colors
    grid = np.random.randint(0, 255, (grid_size, grid_size, 3))

    for i in range (grid_size):
        for j in range(grid_size):
            color = grid[i, j]
            pygame.draw.rect(screen, color, (j * cell_size, i * cell_size, cell_size, cell_size))

    pygame.display.flip()
    clock.tick(10) # 10 FPS


pygame.quit()

