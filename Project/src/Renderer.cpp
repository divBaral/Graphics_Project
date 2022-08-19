#include "Renderer.hpp"
#include "Rasterize.hpp"
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <algorithm>

Renderer::Renderer(sf::RenderWindow *window)
{
    m_window = window;
   
}
void Renderer::clear()
{
  
}
Vector2d perspectiveProject(Vector P, int width, int height){
	Vector2d Q(-P.x/P.z,-P.y/P.z);
	const float aspect = float(height)/width;

	float fieldOfViewX = PI/2;
	const float s = -2.0*tan(fieldOfViewX*0.5f);

	Q.x = width*(-Q.x/s + 0.5f);
	Q.y = height*(Q.y/(s*aspect) + 0.5f);

	return Q;

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
//distance of point Q from line containing points A & B
float lineDistance2D(const Point2d& A,const Point2d& B, const Point2d& Q){

	Vector2d n(A.y - B.y, B.x - A.x);
	const float d = A.x*B.y - B.x*A.y;
	return (n.dot(Q)+d)/n.length();
}

float bary2D(const Point2d& A, const Point2d& B, const Point2d& C, const Point2d& Q){
	return ((lineDistance2D(B,C,Q))/(lineDistance2D(B,C,A)));
}
void Renderer::DrawTriangle(Point &point0, Point &point1, Point &point2, Matrix4f &viewspace, sf::Image &image, material& material,DepthBuffer& depthbuffer, Point& camepos)
{
    // converting to 2d coordinates for viewport transform
	// viewspace = Matrix4f();
    auto q0 = viewspace * point0;
    auto q1 = viewspace * point1;
    auto q2 = viewspace * point2;

	float width = m_window->getSize().x;
	float height = m_window->getSize().y;
    Point2d p0 = perspectiveProject(q0, width, height);
    Point2d p1 = perspectiveProject(q1, width, height);
    Point2d p2 = perspectiveProject(q2, width, height);

	//viewport(p0, p1, p2);
    //compute bb
    auto x0 = std::min({p0.x, p1.x, p2.x});
    auto y0 = std::min({p0.y, p1.y, p2.y});
    auto x1 = std::max({p0.x, p1.x, p2.x});
    auto y1 = std::max({p0.y, p1.y, p2.y});

	float w = m_window->getSize().x;
    float h = m_window->getSize().y;

	if(x0<0){x0=0;}
	if(y0<0){y0=0;}
	if(x1>w){x1=w;}
	if(y1>h){y1=h;}	

	Vector vertexPw[3] = {-q0/q0.z, -q1/q1.z, -q2/q2.z};
	float vertexW[3] = {-1/q0.z, -1/q1.z, -1/q2.z};
	Vector n = {1,1,1};//normals to imported.
	Vector vertexNw[3] = {n,n, n};
   
    for ( int y=y0; y<y1; ++y) {
        for ( int x = x0; x<x1; ++x) {
            Point2d Q(x+0.5f, y+0.5f);
            float alpha = bary2D(p0, p1, p2, Q);
            float beta = bary2D(p1, p0, p2, Q);
            float gamma = bary2D(p2, p1, p0, Q);
            float weight2D[] = {alpha, beta, gamma};

			//if points lies in triangle
            if(weight2D[0]>0 && weight2D[1]>0 && weight2D[2]>0){
					//interpolate depth
					float w = 0.0f;
					for(int k = 0; k<3; ++k){
						w += weight2D[k] * vertexW[k];
					}
					Point pW = Point();
					Vector nW = Vector();
					//interpolate projective attributes
					for(int k = 0;k<3;++k){
						pW += vertexPw[k]*weight2D[k];
						//nW += vertexNw[k]*weight2D[k];
					}

					//recover interpolated attributes
					 Point P = pW/w;
					const float depth = -P.z*10;//P.length();

					//depth test
					if(  depth<depthbuffer.get(x,y)){
						depthbuffer.set(x,y, depth);
						auto color = sf::Color::White;
						if (image.getSize().x != 0 || image.getSize().y != 0)
                        	color = image.getPixel((x / m_window->getSize().x) * image.getSize().x, (y / m_window->getSize().y) * image.getSize().y);
						
						sf::Vertex v(sf::Vector2f(x, y), color);
                    	m_window->draw(&v, 1, sf::Points);
						
					}
				}

        }
    }
   
}
