#pragma once
#include <SFML/Graphics.hpp>
class Renderer 
{
public:
    Renderer( sf::RenderWindow *window);
    void DrawTriangle(float x1, float y1,
                      float x2, float y2,
                      float x3, float y3);
private:

    sf::RenderWindow* m_window;

};