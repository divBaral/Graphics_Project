#include "Renderer.hpp"

Renderer::Renderer(sf::RenderWindow *window, Texture *textureObj)
{
    m_window = window;
    m_textureObj = textureObj;
}
void Renderer::DrawTriangle(float x1, float y1,
                            float x2, float y2,
                            float x3, float y3, std::string &material)
{
    sf::ConvexShape convex;

    // resize it to 3 points
    convex.setPointCount(3);

    // define the points
    convex.setPoint(0, sf::Vector2f(x1, y1));
    convex.setPoint(1, sf::Vector2f(x2, y2));
    convex.setPoint(2, sf::Vector2f(x3, y3));
    convex.setFillColor(sf::Color::White);
    m_textureObj->setTexture(material);
    convex.setTexture(m_textureObj->m_texture);

    m_window->draw(convex);
}