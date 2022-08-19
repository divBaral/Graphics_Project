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

	float fieldOfViewX = 2*PI/3;
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
sf::Color applyLighting2(std::vector<Vector>& lightArray, Vector camepos, Point P, Vector normal, float ambientIntensity, material m) {
	float color[] = {0,0,0};
	normal = normal.normalize();
	Vector view = (camepos-P).normalize();
	float diffuse = 0.0f;
	float specular = 0.0f;
	float intensity=0.0f; //[]= {0.5, 0.4, 0.3}; //move this
	for ( int i=0; i<3; ++i ) {
		for ( auto& light : lightArray ) {
		
		light = light-P;
		auto evaulateintesity = [&] (float x) -> float { 
			
			return x/30;
			return 1/(0.01*x*x+0.01*x) ;
			
		};
		float intensity = evaulateintesity(light.length());
		light = light.normalize();
		Vector reflection =  normal*(2*light.dot(normal))  - light;
		//std::cerr<<"intesity : "<<intensity<<std::endl;
		float diff =  intensity * std::max(normal.dot(light),0.f );
		float spec = diffuse > 0 ? intensity*pow(std::max(reflection.dot(view), 0.0f), m.ns) : 0;
		specular+=spec;
		diffuse += diff;
		}
		color[i] = ambientIntensity*m.ka[i] +  diffuse * m.kd[i] + specular*m.ks[i];
		color[i] = color[i] * 255;
	}
	float maxi = std::max({color[0], color[1], color[2] });
	float factor = 255*1/maxi;
	if ( maxi > 255 ) {
		for ( int i =0; i<3; ++i ) {
			color[i] *= factor;
		}
	}
	
	//return sf::Color::White;
	return sf::Color(color[0], color[1], color[2]);
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
void Renderer::DrawTriangle(const Triangle& t,  Matrix4f &viewspace, DepthBuffer& depthbuffer, const Point& camepos, std::vector<sf::Vertex>& Drawablescene)
{
    // converting to 2d coordinates for viewport transform
	// viewspace = Matrix4f();
    auto q0 = viewspace * t.v0;
    auto q1 = viewspace * t.v1;
    auto q2 = viewspace * t.v2;

	float width = m_window->getSize().x;
	float height = m_window->getSize().y;
    Point2d p0 = perspectiveProject(q0, width, height);
    Point2d p1 = perspectiveProject(q1, width, height);
    Point2d p2 = perspectiveProject(q2, width, height);

	

    //compute bb
    auto x0 = std::min({p0.x, p1.x, p2.x});
    auto y0 = std::min({p0.y, p1.y, p2.y});
    auto x1 = std::max({p0.x, p1.x, p2.x});
    auto y1 = std::max({p0.y, p1.y, p2.y});

	float w = m_window->getSize().x;
    float h = m_window->getSize().y;
	if ( x0<0 || x1>w || y0<0 || y1>h ) return;
	if(x0<0){x0=0;}
	if(y0<0){y0=0;}
	if(x1>w){x1=w;}
	if(y1>h){y1=h;}	

	
	Vector vertexPw[3] = {-q0/q0.z, -q1/q1.z, -q2/q2.z};
	float vertexW[3] = {-1/q0.z, -1/q1.z, -1/q2.z};   
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
					//interpolate projective attributes
					for(int k = 0;k<3;++k){
						pW += vertexPw[k]*weight2D[k];
						
					}

					//recover interpolated attributes
					 Point P = pW/w;
					
					const float depth = -P.z*10;//P.length();

					//depth test
					if(  depth<depthbuffer.get(x,y)){
						depthbuffer.set(x,y, depth);
						Point vertexCoordinates = P;
						Vector normal = (t.v1-t.v0).cross(t.v2-t.v0);
						if ( t.hasnormal ) normal = t.n0*alpha+  t.n1*beta + t.n2*gamma;
						std::vector<Vector > lightArray = {Vector(4,4, -2), Vector(-4,4,2)};
						auto color = applyLighting2(lightArray, camepos, P, normal, .3f, t.mtl  );	
						sf::Vertex v(sf::Vector2f(x, y), color);
						Drawablescene.push_back(v);
                    	
						
					}
				}

        }
    }
   
}
