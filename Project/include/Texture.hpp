#pragma once
#include <iostream>
void loadTexture(std::vector<std::string> materials, std::map<std::string, sf::Texture> &textures)
{
    for (std::string material : materials)
    {
        sf::Texture texture;
        std::string dir = "D:/Graphics_Project/Graphics_Project/Project/res/textures/";
        if (material == "wood_balls_brown") // roof
        {
            if (!texture.loadFromFile(dir + "roof.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "metal_dark_brown") // chimneys
        {
            if (!texture.loadFromFile(dir + "chimney.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "plaster_light_brown") // walls
        {
            if (!texture.loadFromFile(dir + "wall.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "Wood_brown") // roof side structure
        {
            if (!texture.loadFromFile(dir + "frame.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "foundation_brown_brick") // foundation small wall
        {
            if (!texture.loadFromFile(dir + "foundation.jpeg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "metal_grey") // gutter i.e. pipes
        {
            if (!texture.loadFromFile(dir + "foundation.jpeg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "plaster_sand") // window cover
        {
            if (!texture.loadFromFile(dir + "cover_window.png"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "glass_window") // window glass
        {
            if (!texture.loadFromFile(dir + "cover_window.png"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "plate_grey") // terrace , parking area?
        {
            if (!texture.loadFromFile(dir + "foundation.jpeg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        textures[material] = texture;
    }
}