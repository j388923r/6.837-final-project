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
	string MCLocs[64];
	static const float stretcher;
	Vector3f surface_tension_force = 0;
	

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
		setMarchingCubeLocs();
	}

	Vector3f getPosition() {
		return position;
	}

	Vector3f move(Vector3f delta) {
		position = position + delta;
	}

	string getGridLoc() {
		return to_string((int)(position[0] * stretcher)) + to_string((int)floor(position[1] * stretcher)) + to_string((int)floor(position[2] * stretcher));
	}

	void setMarchingCubeLocs() {
		Vector3f stretchedPos = position * stretcher;
		MCLocs[0] = to_string((int)(stretchedPos[0])) + to_string((int)floor(stretchedPos[1])) + to_string((int)floor(stretchedPos[2]));
		MCLocs[1] = to_string((int)(stretchedPos[0]) + 1) + to_string((int)floor(stretchedPos[1])) + to_string((int)floor(stretchedPos[2]));
		MCLocs[2] = to_string((int)(stretchedPos[0]) + 1) + to_string((int)floor(stretchedPos[1])) + to_string((int)floor(stretchedPos[2] + 1));
		MCLocs[3] = to_string((int)(stretchedPos[0])) + to_string((int)floor(stretchedPos[1])) + to_string((int)floor(stretchedPos[2] + 1));
		MCLocs[4] = to_string((int)(stretchedPos[0])) + to_string((int)floor(stretchedPos[1] + 1)) + to_string((int)floor(stretchedPos[2]));
		MCLocs[5] = to_string((int)(stretchedPos[0]) + 1) + to_string((int)floor(stretchedPos[1] + 1)) + to_string((int)floor(stretchedPos[2]));
		MCLocs[6] = to_string((int)(stretchedPos[0]) + 1) + to_string((int)floor(stretchedPos[1] + 1)) + to_string((int)floor(stretchedPos[2] + 1));
		MCLocs[7] = to_string((int)(stretchedPos[0])) + to_string((int)floor(stretchedPos[1] + 1)) + to_string((int)floor(stretchedPos[2] + 1));

		Vector3f stretchedPosMinus111 = stretchedPos - Vector3f(1.0f, 1.0f, 1.0f);
		MCLocs[8] = to_string((int)(stretchedPosMinus111[0])) + to_string((int)floor(stretchedPosMinus111[1])) + to_string((int)floor(stretchedPosMinus111[2]));
		MCLocs[9] = to_string((int)(stretchedPosMinus111[0]) + 1) + to_string((int)floor(stretchedPosMinus111[1])) + to_string((int)floor(stretchedPosMinus111[2]));
		MCLocs[10] = to_string((int)(stretchedPosMinus111[0]) + 1) + to_string((int)floor(stretchedPosMinus111[1])) + to_string((int)floor(stretchedPosMinus111[2] + 1));
		MCLocs[11] = to_string((int)(stretchedPosMinus111[0])) + to_string((int)floor(stretchedPosMinus111[1])) + to_string((int)floor(stretchedPosMinus111[2] + 1));
		MCLocs[12] = to_string((int)(stretchedPosMinus111[0])) + to_string((int)floor(stretchedPosMinus111[1] + 1)) + to_string((int)floor(stretchedPosMinus111[2]));
		MCLocs[13] = to_string((int)(stretchedPosMinus111[0]) + 1) + to_string((int)floor(stretchedPosMinus111[1] + 1)) + to_string((int)floor(stretchedPosMinus111[2]));
		MCLocs[14] = to_string((int)(stretchedPosMinus111[0]) + 1) + to_string((int)floor(stretchedPosMinus111[1] + 1)) + to_string((int)floor(stretchedPosMinus111[2] + 1));
		MCLocs[15] = to_string((int)(stretchedPosMinus111[0])) + to_string((int)floor(stretchedPosMinus111[1] + 1)) + to_string((int)floor(stretchedPosMinus111[2] + 1));

		Vector3f stretchedPosMinus110 = stretchedPos - Vector3f(1.0f, 1.0f, 0.0f);
		MCLocs[16] = to_string((int)(stretchedPosMinus110[0])) + to_string((int)floor(stretchedPosMinus110[1])) + to_string((int)floor(stretchedPosMinus110[2]));
		MCLocs[17] = to_string((int)(stretchedPosMinus110[0]) + 1) + to_string((int)floor(stretchedPosMinus110[1])) + to_string((int)floor(stretchedPosMinus110[2]));
		MCLocs[18] = to_string((int)(stretchedPosMinus110[0]) + 1) + to_string((int)floor(stretchedPosMinus110[1])) + to_string((int)floor(stretchedPosMinus110[2] + 1));
		MCLocs[19] = to_string((int)(stretchedPosMinus110[0])) + to_string((int)floor(stretchedPosMinus110[1])) + to_string((int)floor(stretchedPosMinus110[2] + 1));
		MCLocs[20] = to_string((int)(stretchedPosMinus110[0])) + to_string((int)floor(stretchedPosMinus110[1] + 1)) + to_string((int)floor(stretchedPosMinus110[2]));
		MCLocs[21] = to_string((int)(stretchedPosMinus110[0]) + 1) + to_string((int)floor(stretchedPosMinus110[1] + 1)) + to_string((int)floor(stretchedPosMinus110[2]));
		MCLocs[22] = to_string((int)(stretchedPosMinus110[0]) + 1) + to_string((int)floor(stretchedPosMinus110[1] + 1)) + to_string((int)floor(stretchedPosMinus110[2] + 1));
		MCLocs[23] = to_string((int)(stretchedPosMinus110[0])) + to_string((int)floor(stretchedPosMinus110[1] + 1)) + to_string((int)floor(stretchedPosMinus110[2] + 1));

		Vector3f stretchedPosMinus011 = stretchedPos - Vector3f(0.0f, 1.0f, 1.0f);
		MCLocs[24] = to_string((int)(stretchedPosMinus011[0])) + to_string((int)floor(stretchedPosMinus011[1])) + to_string((int)floor(stretchedPosMinus011[2]));
		MCLocs[25] = to_string((int)(stretchedPosMinus011[0]) + 1) + to_string((int)floor(stretchedPosMinus011[1])) + to_string((int)floor(stretchedPosMinus011[2]));
		MCLocs[26] = to_string((int)(stretchedPosMinus011[0]) + 1) + to_string((int)floor(stretchedPosMinus011[1])) + to_string((int)floor(stretchedPosMinus011[2] + 1));
		MCLocs[27] = to_string((int)(stretchedPosMinus011[0])) + to_string((int)floor(stretchedPosMinus011[1])) + to_string((int)floor(stretchedPosMinus011[2] + 1));
		MCLocs[28] = to_string((int)(stretchedPosMinus011[0])) + to_string((int)floor(stretchedPosMinus011[1] + 1)) + to_string((int)floor(stretchedPosMinus011[2]));
		MCLocs[29] = to_string((int)(stretchedPosMinus011[0]) + 1) + to_string((int)floor(stretchedPosMinus011[1] + 1)) + to_string((int)floor(stretchedPosMinus011[2]));
		MCLocs[30] = to_string((int)(stretchedPosMinus011[0]) + 1) + to_string((int)floor(stretchedPosMinus011[1] + 1)) + to_string((int)floor(stretchedPosMinus011[2] + 1));
		MCLocs[31] = to_string((int)(stretchedPosMinus011[0])) + to_string((int)floor(stretchedPosMinus011[1] + 1)) + to_string((int)floor(stretchedPosMinus011[2] + 1));

		Vector3f stretchedPosMinus101 = stretchedPos - Vector3f(1.0f, 0.0f, 1.0f);
		MCLocs[32] = to_string((int)(stretchedPosMinus101[0])) + to_string((int)floor(stretchedPosMinus101[1])) + to_string((int)floor(stretchedPosMinus101[2]));
		MCLocs[33] = to_string((int)(stretchedPosMinus101[0]) + 1) + to_string((int)floor(stretchedPosMinus101[1])) + to_string((int)floor(stretchedPosMinus101[2]));
		MCLocs[34] = to_string((int)(stretchedPosMinus101[0]) + 1) + to_string((int)floor(stretchedPosMinus101[1])) + to_string((int)floor(stretchedPosMinus101[2] + 1));
		MCLocs[35] = to_string((int)(stretchedPosMinus101[0])) + to_string((int)floor(stretchedPosMinus101[1])) + to_string((int)floor(stretchedPosMinus101[2] + 1));
		MCLocs[36] = to_string((int)(stretchedPosMinus101[0])) + to_string((int)floor(stretchedPosMinus101[1] + 1)) + to_string((int)floor(stretchedPosMinus101[2]));
		MCLocs[37] = to_string((int)(stretchedPosMinus101[0]) + 1) + to_string((int)floor(stretchedPosMinus101[1] + 1)) + to_string((int)floor(stretchedPosMinus101[2]));
		MCLocs[38] = to_string((int)(stretchedPosMinus101[0]) + 1) + to_string((int)floor(stretchedPosMinus101[1] + 1)) + to_string((int)floor(stretchedPosMinus101[2] + 1));
		MCLocs[39] = to_string((int)(stretchedPosMinus101[0])) + to_string((int)floor(stretchedPosMinus101[1] + 1)) + to_string((int)floor(stretchedPosMinus101[2] + 1));

		Vector3f stretchedPosMinus010 = stretchedPos - Vector3f(0.0f, 1.0f, 0.0f);
		MCLocs[40] = to_string((int)(stretchedPosMinus010[0])) + to_string((int)floor(stretchedPosMinus010[1])) + to_string((int)floor(stretchedPosMinus010[2]));
		MCLocs[41] = to_string((int)(stretchedPosMinus010[0]) + 1) + to_string((int)floor(stretchedPosMinus010[1])) + to_string((int)floor(stretchedPosMinus010[2]));
		MCLocs[42] = to_string((int)(stretchedPosMinus010[0]) + 1) + to_string((int)floor(stretchedPosMinus010[1])) + to_string((int)floor(stretchedPosMinus010[2] + 1));
		MCLocs[43] = to_string((int)(stretchedPosMinus010[0])) + to_string((int)floor(stretchedPosMinus010[1])) + to_string((int)floor(stretchedPosMinus010[2] + 1));
		MCLocs[44] = to_string((int)(stretchedPosMinus010[0])) + to_string((int)floor(stretchedPosMinus010[1] + 1)) + to_string((int)floor(stretchedPosMinus010[2]));
		MCLocs[45] = to_string((int)(stretchedPosMinus010[0]) + 1) + to_string((int)floor(stretchedPosMinus010[1] + 1)) + to_string((int)floor(stretchedPosMinus010[2]));
		MCLocs[46] = to_string((int)(stretchedPosMinus010[0]) + 1) + to_string((int)floor(stretchedPosMinus010[1] + 1)) + to_string((int)floor(stretchedPosMinus010[2] + 1));
		MCLocs[47] = to_string((int)(stretchedPosMinus010[0])) + to_string((int)floor(stretchedPosMinus010[1] + 1)) + to_string((int)floor(stretchedPosMinus010[2] + 1));

		Vector3f stretchedPosMinus100 = stretchedPos - Vector3f(1.0f, 0.0f, 0.0f);
		MCLocs[48] = to_string((int)(stretchedPosMinus100[0])) + to_string((int)floor(stretchedPosMinus100[1])) + to_string((int)floor(stretchedPosMinus100[2]));
		MCLocs[49] = to_string((int)(stretchedPosMinus100[0]) + 1) + to_string((int)floor(stretchedPosMinus100[1])) + to_string((int)floor(stretchedPosMinus100[2]));
		MCLocs[50] = to_string((int)(stretchedPosMinus100[0]) + 1) + to_string((int)floor(stretchedPosMinus100[1])) + to_string((int)floor(stretchedPosMinus100[2] + 1));
		MCLocs[51] = to_string((int)(stretchedPosMinus100[0])) + to_string((int)floor(stretchedPosMinus100[1])) + to_string((int)floor(stretchedPosMinus100[2] + 1));
		MCLocs[52] = to_string((int)(stretchedPosMinus100[0])) + to_string((int)floor(stretchedPosMinus100[1] + 1)) + to_string((int)floor(stretchedPosMinus100[2]));
		MCLocs[53] = to_string((int)(stretchedPosMinus100[0]) + 1) + to_string((int)floor(stretchedPosMinus100[1] + 1)) + to_string((int)floor(stretchedPosMinus100[2]));
		MCLocs[54] = to_string((int)(stretchedPosMinus100[0]) + 1) + to_string((int)floor(stretchedPosMinus100[1] + 1)) + to_string((int)floor(stretchedPosMinus100[2] + 1));
		MCLocs[55] = to_string((int)(stretchedPosMinus100[0])) + to_string((int)floor(stretchedPosMinus100[1] + 1)) + to_string((int)floor(stretchedPosMinus100[2] + 1));

		Vector3f stretchedPosMinus001 = stretchedPos - Vector3f(0.0f, 0.0f, 1.0f);
		MCLocs[56] = to_string((int)(stretchedPosMinus001[0])) + to_string((int)floor(stretchedPosMinus001[1])) + to_string((int)floor(stretchedPosMinus001[2]));
		MCLocs[57] = to_string((int)(stretchedPosMinus001[0]) + 1) + to_string((int)floor(stretchedPosMinus001[1])) + to_string((int)floor(stretchedPosMinus001[2]));
		MCLocs[58] = to_string((int)(stretchedPosMinus001[0]) + 1) + to_string((int)floor(stretchedPosMinus001[1])) + to_string((int)floor(stretchedPosMinus001[2] + 1));
		MCLocs[59] = to_string((int)(stretchedPosMinus001[0])) + to_string((int)floor(stretchedPosMinus001[1])) + to_string((int)floor(stretchedPosMinus001[2] + 1));
		MCLocs[60] = to_string((int)(stretchedPosMinus001[0])) + to_string((int)floor(stretchedPosMinus001[1] + 1)) + to_string((int)floor(stretchedPosMinus001[2]));
		MCLocs[61] = to_string((int)(stretchedPosMinus001[0]) + 1) + to_string((int)floor(stretchedPosMinus001[1] + 1)) + to_string((int)floor(stretchedPosMinus001[2]));
		MCLocs[62] = to_string((int)(stretchedPosMinus001[0]) + 1) + to_string((int)floor(stretchedPosMinus001[1] + 1)) + to_string((int)floor(stretchedPosMinus001[2] + 1));
		MCLocs[63] = to_string((int)(stretchedPosMinus001[0])) + to_string((int)floor(stretchedPosMinus001[1] + 1)) + to_string((int)floor(stretchedPosMinus001[2] + 1));

	}

	void clearForces() {
		pressure_force = Vector3f(0, 0, 0);
		viscocity_force = Vector3f(0, 0, 0);
		surface_tension_force = Vector3f(0, 0, 0);
		color_field_gradient = color_field_laplacian = density =0;
	}
};


