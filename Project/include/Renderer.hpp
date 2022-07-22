#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Renderer
{
public:
    Renderer(sf::RenderWindow *window);
    void DrawTriangle(std::pair<float, float>, std::pair<float, float>, std::pair<float, float>, sf::Image &, std::string);

private:
    sf::RenderWindow *m_window;
};