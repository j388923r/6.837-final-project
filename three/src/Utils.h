#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>
#include <vecmath.h>
#include <stdio.h>     
#include <iostream> 

using namespace std;

class Utils {

public:
	static float Wpoly6(float r, float h);

	static float Wpoly6Gradient(float r, float h);

	static float Wpoly6Laplacian(float r, float h);

	static float Wspiky(float r, float h);

	static float WspikyGradient(float r, float h);

	static float WspikyLaplacian(float r, float h);

	static float Wviscocity(float r, float h);

	static float WviscocityGradient(float r, float h);

	static float WviscocityLaplacian(float r, float h);
};

class Particle {

public:
	float mass;
	Vector3f velocity;
	float density;
	Vector3f pressure_force;
	Vector3f viscocity_force;
	float color_field_gradient = 1 ;
	float color_field_laplacian = 1;
	Vector3f position;
	float surface_tension_force;
	

	Particle(float mass) {
		mass = mass;
		density = 1;
		// position = pos;
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

	string getGridLoc() {
		return to_string((int)(position[0])) + to_string((int)floor(position[1])) + to_string((int)floor(position[2]));
	}
};


