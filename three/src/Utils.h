#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>
#include <vecmath.h>
#include <stdio.h>     


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
	Vector3f velocity;
	float density;
	Vector3f pressure_force;
	Vector3f viscocity_force;
	float color_field_gradient;
	float color_field_laplacian;
	Vector3f position;
	

	Particle(float mass, Vector3f pos) {
		mass = mass;
		density = 0;\
		position = pos;
	}
	
	void addToDensity(float deltaDensity) {
		if(deltaDensity > 0) {
			density += deltaDensity;
		}
	}

	Vector3f getPosition() {
		return position;
	}

	Vector3f move(Vector3f delta) {
		position = position + delta;
	}
};


