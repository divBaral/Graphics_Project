#pragma once
#include <SFML/Graphics.hpp>
#include <Texture.hpp>
#include <iostream>
class Renderer
{
public:
    Renderer(sf::RenderWindow *window, Texture *textureObj);
    void DrawTriangle(float x1, float y1,
                      float x2, float y2,
                      float x3, float y3, std::string &material);

private:
    sf::RenderWindow *m_window;
    Texture *m_textureObj;
};