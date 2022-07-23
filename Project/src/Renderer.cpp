#include "Renderer.hpp"
#include "Zbuffer.hpp"
#include "Rasterize.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

Renderer::Renderer(sf::RenderWindow *window)
{
    m_window = window;
    m_zBuffer = new Zbuffer( m_window->getSize().x, m_window->getSize().y);
    m_pixels = new sf::Vertex[m_window->getSize().x * m_window->getSize().y];
    clear();

}

void Renderer::clear()
{
    int X = m_window->getSize().x;
    int Y = m_window->getSize().y;

    for( auto i=0; i<X*Y; ++i )
    {
        int x = i%X;
        int y = i/X;
        sf::Vertex v;
        v.color = sf::Color(0,0,0); //default color
        v.position = sf::Vector2f(x,y);
        m_pixels[i] = v;
    }

    //also clearing z_buffer
    m_zBuffer->Clear();
}

void Renderer::DrawTriangle( std::pair<float,float> p0, std::pair<float,float> p1, std::pair<float,float> p2, Point &point0, Point &point1, Point &point2, float zdepth, sf::Image &image)
{
    // lambda function captures this variable
    // m_vertices.clear();
    int minx = std::min({p0.first, p1.first, p2.first});
    int maxx = std::max({p0.first, p1.first, p2.first});
    int miny = std::min({p0.second, p1.second, p2.second});
    int maxy = std::max({p0.second, p1.second, p2.second});

    if( maxx<m_window->getSize().x && maxy<m_window->getSize().y && minx>0 && miny>0 )
    {
        using SlopeData = std::pair<float, float>;
        RasterizeTriangle(
            p0, p1, p2,
            point0, point1, point2,
            [&](const auto &p)
            { return p; },
            [&](const auto &p0, const auto &p1, float step)
            {
                int begin = p0.first;
                int end = p1.first;

                return SlopeData(begin, (end - begin) / step);
            },
            [&](int y, float d, const auto& normal, SlopeData &left, SlopeData &right, auto &&Plot)
            {
                int x = left.first;
                int endx = right.first;

                //direction ratios
                float a = normal.x;
                float b = normal.y;
                float c = normal.z;

                for (; x<endx; ++x)
                {
                    float depth = (-a*x-b*y-d)/c;
                    Plot(x, y, depth);
                    // depth -= (a/c);
                }

                left.first += left.second;
                right.first += right.second;
            },
            [&](int x, int y , float depth)
            {  

                if( x<m_window->getSize().x && y<m_window->getSize().y && x>0 && y>0 && m_zBuffer->testAndSet( x, y, depth) )
                {
                    sf::Color color = sf::Color(0, 255, 0);
                    if (image.getSize().x != 0 || image.getSize().y != 0)
                        color = image.getPixel((x / m_window->getSize().x) * image.getSize().x, (y / m_window->getSize().y) * image.getSize().y);
                    sf::Vertex v(sf::Vector2f(x, y), color);
                    // m_window->draw(&v, 1, sf::Points);
                    // m_vertices.push_back(v);
                    m_pixels[(y*m_window->getSize().x)+x]= v;
                }
            });
    }

    // m_window->draw(&m_vertices[0], m_vertices.size(), sf::Points);
    // m_window->draw(m_pixels, 800 ,sf::Points);
}
