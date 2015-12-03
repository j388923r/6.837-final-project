#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>

using namespace std;

class Utils {

public:
	float Utils::Wpoly6(float r, float h);

	float Utils::Wpoly6Gradient(float r, float h);

	float Utils::Wpoly6Laplacian(float r, float h);

	float Utils::Wspiky(float r, float h);

	float Utils::WspikyGradient(float r, float h);

	float Utils::WspikyLaplacian(float r, float h);

	float Utils::Wviscocity(float r, float h);

	float Utils::WviscocityGradient(float r, float h);

	float Utils::WviscocityLaplacian(float r, float h);
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
		this.mass = mass;
		this.density = 0;
	}
	
	void addToDensity(float deltaDensity) {
		if(deltaDensity > 0) {
			this.density += deltaDensity;
		}
	}
};


