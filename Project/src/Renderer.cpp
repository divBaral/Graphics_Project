#include "Renderer.hpp"
#include "Zbuffer.hpp"
#include "Rasterize.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

Renderer::Renderer(sf::RenderWindow *window)
{
    m_window = window;
}

void Renderer::DrawTriangle(std::pair<float, float> p0, std::pair<float, float> p1, std::pair<float, float> p2, sf::Image &image)
{
    sf::ConvexShape convex;
    // lambda function captures this vertices
    std::vector<sf::Vertex> vertices;

    using SlopeData = std::pair<float, float>;
    RasterizeTriangle(
        p0, p1, p2,
        [&](const auto &p)
        { return p; },
        [&](const auto &p0, const auto &p1, float step)
        {
            int begin = p0.first;
            int end = p1.first;

            return SlopeData(begin, (end - begin) / step);
        },
        [&](int y, SlopeData &left, SlopeData &right, auto &&Plot)
        {
            int x = left.first;
            int endx = right.first;

            for (; x < endx; ++x)
            {
                Plot(x, y);
            }
            left.first += left.second;
            right.first += right.second;
        },
        [&](int x, int y)
        {
            sf::Color color = sf::Color(0, 255, 0);
            if (image.getSize().x != 0 || image.getSize().y != 0)
                color = image.getPixel((x / m_window->getSize().x) * image.getSize().x, (y / m_window->getSize().y) * image.getSize().y);
            sf::Vertex v(sf::Vector2f(x, y), color);
            vertices.push_back(v);
        });

    m_window->draw(&vertices[0], vertices.size(), sf::Points);
}
