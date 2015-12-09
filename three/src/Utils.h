#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <vecmath.h>
#include <stdio.h>     
#include <iostream>
#include <sstream>

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
	float color_field_gradient = 0;
	float color_field_laplacian = 0;
	Vector3f position;
	string neighborLocs[27];
	static const float stretcher;
	Vector3f surface_tension_force = 0;
	Vector3f collision_force;
	

	Particle(float m) {
		mass = m;
		density = 1;
		// position = pos;
	}
	
	void addToDensity(float deltaDensity) {
		if(deltaDensity > 0) {
			density += deltaDensity;
		}
	}

	void setPosition(Vector3f pos) {
		position = pos;
		int count = 0;
		for ( int i = -1; i < 2; ++i) {
			for ( int j = -1; j < 2; ++j) {
				for ( int k = -1; k < 2; ++k) {
					neighborLocs[count++] = to_string((int)(pos[0] * stretcher + i)) + to_string((int)floor(pos[1] * stretcher + j)) + to_string((int)floor(pos[2] * stretcher + k));
				}
			}
		}
	}

	Vector3f getPosition() {
		return position;
	}

	Vector3f move(Vector3f delta) {
		position = position + delta;
	}

	string getGridLoc() {
		return to_string((short)(position[0] * stretcher)) + to_string((short)floor(position[1] * stretcher)) + to_string((short)floor(position[2] * stretcher));
	}

	void clearForces() {
		pressure_force = Vector3f(0, 0, 0);
		viscocity_force = Vector3f(0, 0, 0);
		surface_tension_force = Vector3f(0, 0, 0);
		collision_force = Vector3f(0, 0, 0);
		color_field_gradient = color_field_laplacian = density =0;
	}
};


