#pragma once
#include <SFML/Graphics.hpp>
// #include <Renderer.hpp>
#include <iostream>

class Texture
{
public:
    Texture(sf::Texture *texture);
    void setTexture(std::string material);
    friend class Renderer;

private:
    sf::Texture *m_texture;
};