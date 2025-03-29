#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <iostream>

// Structure representing one point (x, y)
struct Point {
    float x;
    float y;
};

int main() {
    // Open the binary file in read mode at the end to get file size
    std::ifstream infile("data.bin", std::ios::binary | std::ios::ate);
    if (!infile) {
        std::cerr << "Error: Cannot open file 'data.bin'!" << std::endl;
        return -1;
    }

    // Get file size and calculate the number of points
    std::streamsize size = infile.tellg();
    infile.seekg(0, std::ios::beg);
    size_t numPoints = size / (2 * sizeof(float));
    std::cout << "Loading " << numPoints << " points..." << std::endl;

    // Read all points into a vector
    std::vector<Point> points(numPoints);
    if (!infile.read(reinterpret_cast<char*>(points.data()), size)) {
        std::cerr << "Error reading file!" << std::endl;
        return -1;
    }
    infile.close();

    // Create a vertex array with one vertex per point
    sf::VertexArray vertices(sf::Points, numPoints);
    for (size_t i = 0; i < numPoints; i++) {
        // Set the position (you may need to adjust scaling/translation)
        vertices[i].position = sf::Vector2f(points[i].x, points[i].y);
        vertices[i].color = sf::Color::White;
    }

    // Create an SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Scatter Plot");
    
    // Main loop: handle events and render
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(vertices);
        window.display();
    }

    return 0;
}

