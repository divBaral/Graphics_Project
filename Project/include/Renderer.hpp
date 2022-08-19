#pragma once
#include "DepthBuffer.hpp"
#include "Triangle.hpp"
#include "glalib.hpp"
#include "material.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector> 

class Renderer
{
public:
    Renderer(sf::RenderWindow *window);
    void DrawTriangle( const Triangle& t,  Matrix4f&, DepthBuffer& depthbuffer,const Point& camepos, std::vector<sf::Vertex>& drawablescene);
    void viewport( Point2d&, Point2d&, Point2d& );
    void clear();

    std::vector<sf::Vertex> m_vertices;
    sf::Vertex *m_pixels;
private:
    sf::RenderWindow *m_window;
};