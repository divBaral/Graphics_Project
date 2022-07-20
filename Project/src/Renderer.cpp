#include "Renderer.hpp"
#include "Zbuffer.hpp"

Renderer::Renderer(sf::RenderWindow *window)
{
    m_window = window;
}
void Renderer::DrawTriangle(float x1, float y1,
                            float x2, float y2,
                            float x3, float y3, std::string &material, sf::Texture &texture)
{

    // z-operation while rasterizing
    /*
     if(zb.TestAndSet(x, y, z))
     {
         we do the rasterizing things bla bla
     }

    */

    sf::ConvexShape convex;

    // resize it to 3 points
    convex.setPointCount(3);

    // define the points
    convex.setPoint(0, sf::Vector2f(x1, y1));
    convex.setPoint(1, sf::Vector2f(x2, y2));
    convex.setPoint(2, sf::Vector2f(x3, y3));
    convex.setFillColor(sf::Color::White);
    convex.setTexture(&texture);

    /*
        sf::Image image = texture.copyToImage();
        size = image.getSize();
        color  = image.getPixelsPtr();

    */

    m_window->draw(convex);
}