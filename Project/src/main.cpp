#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
// headers
#include "Camera.hpp"
#include "DepthBuffer.hpp"
#include "Material.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"
#include "glalib.hpp"
#include "Parser.hpp"


#define SCRWIDTH 800
#define SCRHEIGHT 600

const std::string mtlfilepath = "/home/baral/Downloads/graphicslearn"
                                "/Project/res/models/house.mtl";

const std::string objfilepath = "/home/baral/Downloads/graphicslearn"
                                "/Project/res/models/house.obj";

namespace Utils 
{

Vector2d perspectiveProject(Vector P, int width, int height) 
{
  Vector2d Q(-P.x / P.z, -P.y / P.z);
  const float aspect = float(height) / width;

  float fieldOfViewX = 2 * PI / 3;
  const float s = -2.0 * tan(fieldOfViewX * 0.5f);

  Q.x = width * (-Q.x / s + 0.5f);
  Q.y = height * (Q.y / (s * aspect) + 0.5f);

  return Q;
}
sf::Color shade(std::vector<Vector> &lightArray, Vector camepos, Point P,
                Vector normal, float ambientIntensity, Material m) {
  float color[] = {0, 0, 0};
  normal = normal.normalize();
  Vector view = (camepos - P).normalize();
  float diffuse = 0.0f;
  float specular = 0.0f;
  float intensity = 0.0f; //[]= {0.5, 0.4, 0.3}; //move this
  for (int i = 0; i < 3; ++i) {
    for (auto &light : lightArray) {

      light = light - P;
      
      float intensity = 1000.f/(1+light.length());
      light = light.normalize();
      Vector reflection = normal * (2 * light.dot(normal)) - light;
      float diff = intensity * std::max(normal.dot(light), 0.f);
      float spec =
          diffuse > 0
              ? intensity * pow(std::max(reflection.dot(view), 0.0f), m.ns)
              : 0;
      specular += spec;
      diffuse += diff;
    }
    color[i] = ambientIntensity * m.ka[i] +
               diffuse  * m.kd[i] + specular * m.ks[i];
    
  }
  float maxi = std::max({color[0], color[1], color[2]});
  if ( maxi<255) return sf::Color(color[0], color[1], color[2]);

  float factor = 255/maxi;
  
  for ( int i =0; i<3; ++i ) {
    color[i] *= factor;
  }

 

  // return sf::Color::White;
  return sf::Color(color[0], color[1], color[2]);
}
// distance of point Q from line containing points A & B
float lineDistance2D(const Point2d &A, const Point2d &B, const Point2d &Q) {

  Vector2d n(A.y - B.y, B.x - A.x);
  const float d = A.x * B.y - B.x * A.y;
  return (n.dot(Q) + d) / n.length();
}

float bary2D(const Point2d &A, const Point2d &B, const Point2d &C,
             const Point2d &Q) {
  return ((lineDistance2D(B, C, Q)) / (lineDistance2D(B, C, A)));
}

} // namespace utils


int main() 
{
        sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "SFML works!");
        window.setFramerateLimit(30);

        // load scene/mesh in vector
        std::vector<Triangle> scene;
        LoadObject(
            scene, "/home/baral/Downloads/graphicslearn/Project/res/models/house.mtl",
            "/home/baral/Downloads/graphicslearn/Project/res/models/house.obj");

        // initiaze camera properties
        Camera camera;
        Matrix4f viewspace = camera.GetViewMatrix();
         //point light sources
        std::vector<Vector> lightArray ={Vector(0,0,-10)};

        // transform vertex attributes ready for drawn
        std::vector<sf::Vertex> drawablescene;
        bool eventoccured = true;

        while (window.isOpen()) 
        {
          sf::Event event;
          static float deltaTime = 0.1;

         

          while (window.pollEvent(event)) 
          {
            if (event.type == sf::Event::Closed)
              window.close();

            if (event.type == sf::Event::KeyPressed) {
              eventoccured = true;

              if (event.key.code == sf::Keyboard::Left) {
              
                  camera.ProcessKeyboard(CLEFT,  deltaTime);
                  viewspace = camera.GetViewMatrix();

              } else if (event.key.code == sf::Keyboard::Right) {
              
                  camera.ProcessKeyboard(CRIGHT,  deltaTime);
                  viewspace = camera.GetViewMatrix();

              } else if (event.key.code == sf::Keyboard::W) {

              
                  camera.ProcessKeyboard(FORWARD,  deltaTime);
                  viewspace = camera.GetViewMatrix();

              } else if (event.key.code == sf::Keyboard::S) {

              
                  camera.ProcessKeyboard(BACKWARD,  deltaTime);
                  viewspace = camera.GetViewMatrix();

              }else if (event.key.code == sf::Keyboard::A) {
                  camera.ProcessKeyboard(LEFT,  deltaTime);
                  viewspace = camera.GetViewMatrix();

              }else if (event.key.code == sf::Keyboard::D) {

              
                  camera.ProcessKeyboard(RIGHT,  deltaTime);
                  viewspace = camera.GetViewMatrix();

              }
            }
          }

          window.clear();

          DepthBuffer depthbuffer(window.getSize().x, window.getSize().y, INFINITY);
          Point camepos = camera.getPosition();

          // draw each triangle and add to drawablescene
          if ( eventoccured ) //update rendering only if there is change in mvp matrix
          {
            drawablescene.resize(0);
            for (const auto &t : scene) {
              auto q0 = viewspace * t.v0;
              auto q1 = viewspace * t.v1;
              auto q2 = viewspace * t.v2;


              //cliping shoudl be done;
              float zNear = -1.f;
              if ( q0.z > zNear || q1.z > zNear || q2.z>zNear ) continue;

              float width = window.getSize().x;
              float height = window.getSize().y;

              // converting to 2d coordinates for viewport transform
              Point2d p0 = Utils::perspectiveProject(q0, width, height);
              Point2d p1 = Utils::perspectiveProject(q1, width, height);
              Point2d p2 = Utils::perspectiveProject(q2, width, height);

              Vector vertexPw[3] = {-q0 / q0.z, -q1 / q1.z, -q2 / q2.z};
              float vertexW[3] = {-1 / q0.z, -1 / q1.z, -1 / q2.z};

              // compute bounding box
              auto x0 = std::min({p0.x, p1.x, p2.x});
              auto y0 = std::min({p0.y, p1.y, p2.y});
              auto x1 = std::max({p0.x, p1.x, p2.x});
              auto y1 = std::max({p0.y, p1.y, p2.y});

              // cliping should be done
              if(x0<0){x0=0;} 
              if(y0<0){y0=0;} 
              if(x1>SCRWIDTH){x1=SCRWIDTH;} 
              if(y1>SCRHEIGHT){y1=SCRHEIGHT;}	

              Vector normal = -(t.v1 - t.v0).cross(t.v2 - t.v0);

              for (int y = y0; y < y1; ++y) {
                for (int x = x0; x < x1; ++x) {

                  Point2d Q(x + 0.5f, y + 0.5f);

                  float weight2D[3];
                  weight2D[0] = Utils::bary2D(p0, p1, p2, Q);
                  weight2D[1] = Utils::bary2D(p1, p0, p2, Q);
                  weight2D[2] = Utils::bary2D(p2, p1, p0, Q);
                 

                  // if points lies in triangle
                  if (weight2D[0] > 0 && weight2D[1] > 0 && weight2D[2] > 0) {
                    // interpolate depth
                    float w = 0.0f;
                    for (int k = 0; k < 3; ++k) {
                      w += weight2D[k] * vertexW[k];
                    }
                    Point pW = Point();
                    // interpolate projective attributes
                    for (int k = 0; k < 3; ++k) {
                      pW += vertexPw[k] * weight2D[k];
                    }

                    // recover interpolated attributes
                    Point P = pW / w;

                    const float depth = -P.z * 10; // P.length();

                    // depth test
                    if (depth < depthbuffer.get(x, y)) {
                      depthbuffer.set(x, y, depth);
                      Point vertexCoordinates = P;
                     
                      if (t.hasnormal)
                        normal = t.n0 * weight2D[0] + t.n1 * weight2D[1] + t.n2 * weight2D[2];
                    
                      auto color = Utils::shade(lightArray, camepos, P, normal, .4f, t.mtl);
                      sf::Vertex v(sf::Vector2f(x, y), color);
                      drawablescene.push_back(v);
                    }
                  }
                }
              }
            }
            eventoccured = false;
          }

          window.draw(&drawablescene[0], drawablescene.size(), sf::Points);

          window.display();
        
        }

        return 0;
}
