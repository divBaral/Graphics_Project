#include <Texture.hpp>

Texture::Texture(sf::Texture *texture)
{
    m_texture = texture;
}

void Texture::setTexture(std::string material)
{
    std::string dir = "/media/roshan/SSD/Projects/Graphics_Project/Project/res/textures/";
    if (material == "wood_balls_brown") // roof
    {
        if (!m_texture->loadFromFile(dir + "roof.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "metal_dark_brown") // chimneys
    {
        if (!m_texture->loadFromFile(dir + "chimney.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "plaster_light_brown") // walls
    {
        if (!m_texture->loadFromFile(dir + "wall.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "Wood_brown") // roof side structure
    {
        if (!m_texture->loadFromFile(dir + "frame.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "foundation_brown_brick") // foundation small wall
    {
        if (!m_texture->loadFromFile(dir + "foundation.jpeg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "metal_grey") // gutter i.e. pipes
    {
        if (!m_texture->loadFromFile(dir + "foundation.jpeg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "plaster_sand") // window cover
    {
        if (!m_texture->loadFromFile(dir + "cover_window.png"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "glass_window") // window glass
    {
        if (!m_texture->loadFromFile(dir + "cover_window.png"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "plate_grey") // terrace , parking area?
    {
        if (!m_texture->loadFromFile(dir + "foundation.jpeg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
}