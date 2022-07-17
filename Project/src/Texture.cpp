#include <Texture.hpp>

Texture::Texture(sf::Texture *texture)
{
    m_texture = texture;
}

void Texture::setTexture(std::string material)
{
    if (material == "Roof")
    {
        if (!m_texture->loadFromFile("/media/roshan/SSD/Projects/Graphics_Project/Project/res/textures/roof.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "Walls")
    {
        if (!m_texture->loadFromFile("/media/roshan/SSD/Projects/Graphics_Project/Project/res/textures/wall.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "Floor")
    {
        if (!m_texture->loadFromFile("/media/roshan/SSD/Projects/Graphics_Project/Project/res/textures/floor.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "Glass")
    {
        if (!m_texture->loadFromFile("/media/roshan/SSD/Projects/Graphics_Project/Project/res/textures/glass.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "Window_Frames")
    {
        if (!m_texture->loadFromFile("/media/roshan/SSD/Projects/Graphics_Project/Project/res/textures/window_frame.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "Doors" || material == "Sliding_Glass_Door")
    {
        if (!m_texture->loadFromFile("/media/roshan/SSD/Projects/Graphics_Project/Project/res/textures/door.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
    else if (material == "Door_Handles")
    {
        if (!m_texture->loadFromFile("/media/roshan/SSD/Projects/Graphics_Project/Project/res/textures/door_handle.jpg"))
        {
            std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        }
    }
}