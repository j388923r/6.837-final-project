#ifndef WATERSYSTEM_H
#define WATERSYSTEM_H

#include "extra.h"
#include <vector>
#include "Utils.h"
#include "particleSystem.h"

class WaterSystem: public ParticleSystem
{
public:
	WaterSystem(int numParticles);
	
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();
	
	vector<Vector3f> springList;
	
	vector<Vector3f> springCoords;

	vector<Vector3f> spring (float k, float r, int ind1,int ind2, vector<Vector3f> state, vector<Vector3f> existingSprings);

	vector<Vector3f> stateClone;

	
	vector<Particle> fluid_particles;


	vector<Vector3f> pressure_force_vector;

	vector<Vector3f> viscosity_force_vector;

	vector<Vector3f> color_field_gradient;

	vector<Vector3f> color_field_laplacian;


protected:

	//vector<Vector3f> springList;
	

};

#endif
