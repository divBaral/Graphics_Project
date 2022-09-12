#pragma once
#include <vector>
class DepthBuffer{

private: 
	int m_width;
	int m_height;
	std::vector<float> m_depth;	

public:
	DepthBuffer(int width,int height): m_width(width),m_height(height),m_depth(width*height){}

	DepthBuffer(int width,int height, float initialValue): m_width(width),m_height(height),m_depth(width*height,initialValue){}

	int width() const{return m_width;}
	int height() const{return m_height;}

	void set(int x,int y,float value){
		m_depth[x+y*m_width] = value;
	}

	const float& get(int x,int y) const{
		return m_depth[x+y*m_width];
	}

};