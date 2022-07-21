#pragma once
#include <iostream>
void loadTexture(std::vector<std::string> materials, std::map<std::string, sf::Image> &images)
{
    for (std::string material : materials)
    {
        sf::Image image;
        std::string dir = "/media/roshan/SSD/Projects/Graphics_Project/Project/res/textures/";
        if (material == "wood_balls_brown") // roof
        {
            if (!image.loadFromFile(dir + "roof.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "metal_dark_brown") // chimneys
        {
            if (!image.loadFromFile(dir + "chimney.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "plaster_light_brown") // walls
        {
            if (!image.loadFromFile(dir + "wall.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "Wood_brown") // roof side structure
        {
            if (!image.loadFromFile(dir + "frame.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "foundation_brown_brick") // foundation small wall
        {
            if (!image.loadFromFile(dir + "foundation.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "metal_grey") // gutter i.e. pipes
        {
            if (!image.loadFromFile(dir + "foundation.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "plaster_sand") // window cover
        {
            if (!image.loadFromFile(dir + "cover_window.jpeg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "glass_window") // window glass
        {
            if (!image.loadFromFile(dir + "glass.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "plate_grey") // terrace , parking area?
        {
            if (!image.loadFromFile(dir + "foundation.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "outside_walls")
        {
            if (!image.loadFromFile(dir + "wall.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "poleMaterial")
        {
            if (!image.loadFromFile(dir + "stone.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "floor")
        {
            if (!image.loadFromFile(dir + "floor.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        else if (material == "ground")
        {
            if (!image.loadFromFile(dir + "stone.jpg"))
            {
                std::cout << "ERROR:: Loading Texture Failed" << std::endl;
            }
        }
        // else
        // {
        //     if (!image.loadFromFile(dir + "scene.jpg"))
        //     {
        //         std::cout << "ERROR:: Loading Texture Failed" << std::endl;
        //     }
        // }
        images[material] = image;
    }
}