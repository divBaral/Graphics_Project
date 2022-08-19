#pragma once

#include "Zbuffer.hpp"
#include "DepthBuffer.hpp"
#include "glalib.hpp"
#include "material.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector> 

class Renderer
{
public:
    Renderer(sf::RenderWindow *window);
    void DrawTriangle( Point&, Point&, Point&, Matrix4f&, sf::Image&, material& material, DepthBuffer& depthbuffer, Point& camepos);
    void viewport( Point2d&, Point2d&, Point2d& );
    void clear();

    std::vector<sf::Vertex> m_vertices;
    sf::Vertex *m_pixels;
private:
    sf::RenderWindow *m_window;
    Zbuffer *m_zBuffer;
};