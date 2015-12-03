#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>

using namespace std;

class Utils {

public:
	float Wpoly6(float r, float h);

	float Wpoly6Gradient(float r, float h);

	float Wpoly6Laplacian(float r, float h);

	float Wspiky(float r, float h);

	float WspikyGradient(float r, float h);

	float WspikyLaplacian(float r, float h);

	float Wviscocity(float r, float h);

	float WviscocityGradient(float r, float h);

	float WviscocityLaplacian(float r, float h);
};

class Particle {

public:
	float mass;
	float velocity;
	float density;
	float pressure_force;
	float viscocity_force;
	float color_field_gradient;
	float color_field_laplacian;

	Particle(float mass) {
		mass = mass;
		density = 0;
	}
	
	void addToDensity(float deltaDensity) {
		if(deltaDensity > 0) {
			density += deltaDensity;
		}
	}
};


