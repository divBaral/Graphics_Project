#include "Renderer.hpp"
#include "Zbuffer.hpp"
#include "Rasterize.hpp"
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <algorithm>

Renderer::Renderer(sf::RenderWindow *window)
{
    m_window = window;
    m_zBuffer = new Zbuffer(m_window->getSize().x, m_window->getSize().y);
}
void Renderer::clear()
{
    // clearing z-buffer
    m_zBuffer->Clear();
}

/****
 * for viewport tranform
 * i.e. tranformation from normal coordinates to the screen coordinates/pixels
 ****/
void Renderer::viewport(Point2d &p1, Point2d &p2, Point2d &p3)
{
    float SCRWIDTH = m_window->getSize().x;
    float SCRHEIGHT = m_window->getSize().y;

    Matrix3f ToPixel = af2::PointsToPoints({-1, 1}, {1, 1}, {-1, -1},
                                           {0, 0}, {SCRWIDTH, 0}, {0, SCRHEIGHT});

    // viewport transform; tranforming NDC to screen coordinates
    p1 = ToPixel * p1;
    p2 = ToPixel * p2;
    p3 = ToPixel * p3;
}
sf::Color applyLighting2(Vector light, Vector normal, Vector view, float ambientIntensity, material m) {
	float mag = light.length() / 40;
	light = light.normalize();
	normal = normal.normalize();
	view = view.normalize();

	uint8_t color[3];

	float final_intensity[3];

	Vector reflection =  normal*(2*light.dot(normal))  - light;
	for (int i = 0; i < 3; i++) {
		float intensity = 0;
		float ambient = m.ka[i] * ambientIntensity;
		float diffuse = std::max(light.dot( normal), 0.0f) * m.kd[i] / mag;
		float specular = diffuse > 0 ? pow(std::max(reflection.dot(view), 0.0f), m.ns) * m.ks[i] / mag : 0;
		//std::cout << specular << std::endl;
		intensity = ambient + diffuse + specular;
		final_intensity[i] = intensity;
	}
	float ma  = std::max({ final_intensity[0], final_intensity[1], final_intensity[2] });
	float scale;
	if (ma <= 1) {
		scale = 1;
	}else{
		scale = 1 / ma;
	}

	for (int i = 0; i < 3; i++) {
		final_intensity[i] *= scale;
		color[i] = final_intensity[i] * 255;
	}


	return(sf::Color(color[0], color[1], color[2]));
}
void Renderer::DrawTriangle(Point &point0, Point &point1, Point &point2, Matrix4f &viewspace, sf::Image &image, material& material)
{
    // converting to 2d coordinates for viewport transform
    auto q0 = viewspace * point0;
    auto q1 = viewspace * point1;
    auto q2 = viewspace * point2;
    Point2d p0 = {q0.x, q0.y};
    Point2d p1 = {q1.x, q1.y};
    Point2d p2 = {q2.x, q2.y};

    // viewport transform
    
    viewport(p0, p1, p2);

    // here p0,p1,p2 are coordinates scaled to screen size
    // and point0,point1,point2 are original coordinates i.e. not scaled to screen size

    // temporary clipping jugaad
    int minx = std::min({p0.x, p1.x, p2.x});
    int maxx = std::max({p0.x, p1.x, p2.x});
    int miny = std::min({p0.y, p1.y, p2.y});
    int maxy = std::max({p0.y, p1.y, p2.y});

    if (maxx < m_window->getSize().x && maxy < m_window->getSize().y && minx >= 0 && miny >= 0)
    {
        using SlopeData = std::pair<float, float>;

        // unpacking the coordinates of the triangle
        auto x0 = p0.x, y0 = p0.y;
        auto x1 = p1.x, y1 = p1.y;
        auto x2 = p2.x, y2 = p2.y;

        // sorting three coordinates, so that
        // the topmost coordinate is assigned to (X0,Y0)
        // the middle corner is assigned to (X1,Y1)
        // the bottom corner is assigned to (X2,Y2)
        // ordering first by Y-coordinate and then by X-coordinate
        if (std::tie(y1, x1) < std::tie(y0, x0))
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(p0, p1);

            // swapping unscaled/original coordinates
            std::swap(point0, point1);
            std::swap(q0, q1);
        }

        if (std::tie(y2, x2) < std::tie(y0, x0))
        {
            std::swap(x0, x2);
            std::swap(y0, y2);
            std::swap(p0, p2);

            // swapping unscaled/original coordinates
            std::swap(point0, point2);
            std::swap(q0, q2);
        }

        if (std::tie(y2, x2) < std::tie(y1, x1))
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
            std::swap(p1, p2);

            // swapping unscaled/original coordinates
            std::swap(point1, point2);
             std::swap(q1, q2);
        }

        // If triangle has no area, return
        if (y0 == y2)
            return;

        // calculating the direction ratios of normal to the surface of the triangle p0,p1,p2
        Vector side1(p2.x - p1.x, p2.y - p1.y, q2.z - q1.z);
        Vector side2(p0.x - p1.x, p0.y - p1.y, q0.z - q1.z);
        // //cross product
        Vector normal = side1.cross(side2);

        // checking if the bend in the triangle lies on either right side or on left side
        bool shortSide = (y1 - y0) * (x2 - x0) < (x1 - x0) * (y2 - y0);
        // false: short side is in left side
        // true: short side is in right side

        // if the bend is in the right side flip the normal
        if (shortSide)
            normal = normal * (-1);
        normal = normal.normalize();

        // calculating direction cosines of the normal to the triangle
        float a = normal.x;
        float b = normal.y;
        float c = normal.z;
        // calculating constant value in the equation of the triangle
        float d = -1 * (a * q0.x + b * q0.y + c * q0.z);

        // filling the triangle
        RasterizeTriangle(
            p0, p1, p2,
            [&](const auto &p)
            {
                std::pair<float, float> q = {p.x, p.y};
                return q;
            },
            [&](const auto &p0, const auto &p1, float step)
            {
                int begin = p0.x;
                int end = p1.x;

                return SlopeData(begin, (end - begin) / step);
            },
            [&](int y, SlopeData &left, SlopeData &right, auto &&Plot)
            {
                int x = left.first;
                int endx = right.first;

                for (; x <= endx; ++x)
                {
                    float depth = (-a *x - b * y - d) / c;
                    Plot(x, y, depth);
                }

                left.first += left.second;
                right.first += right.second;
            },
            [&](int x, int y, float depth)
            {
                if (m_zBuffer->testAndSet(x, y, depth))
                {
                    sf::Color color = sf::Color(0, 255, 0);
                    if (image.getSize().x != 0 || image.getSize().y != 0)
                    color = image.getPixel((x / m_window->getSize().x) * image.getSize().x, (y / m_window->getSize().y) * image.getSize().y);

                    Vector vertexcoordinates = 
                    Vector light = {4, 4, 4};
                    Vector campos = {0,0,0};
                    // normal
                    //
                    //material.print();
                    sf::Color cf = applyLighting2(light-vertexcoordinates , normal, campos-vertexcoordinates,0.4, material );
                    sf::Vertex v(sf::Vector2f(x, y), cf);
                    m_window->draw(&v, 1, sf::Points);
                }
            });
    }
}
