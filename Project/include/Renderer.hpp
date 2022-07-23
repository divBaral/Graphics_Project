#pragma once

#include "Zbuffer.hpp"
#include "glalib.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Renderer
{
public:
    Renderer(sf::RenderWindow *window);
    void DrawTriangle(std::pair<float,float>, std::pair<float,float>, std::pair<float,float>, Point&, Point&, Point&, float ,sf::Image &);
    void clear();

    std::vector<sf::Vertex> m_vertices;
    sf::Vertex *m_pixels;

private:
    sf::RenderWindow *m_window;
    Zbuffer *m_zBuffer;
    // std::vector<sf::Vertex> m_vertices;

    // sf::Vertex *m_pixels;
    // std::vector<sf::Vertex> m_pixels[800*600];
    // sf::Vertex m_pixels[SCRHEIGHT*SCRWIDTH];
};