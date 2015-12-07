#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>
#include "extra.h"
#include <iostream> 
#include <ostream>
#include <sstream>
#include "Utils.h"
#include <map>

using namespace std;

class ParticleSystem
{
public:
	ParticleSystem(int numParticles);

	int m_numParticles;
	
	vector<Vector3f> evalF(vector<Particle *> state);
	
	void draw();

	void draw2();

	void draw3();
	
	void draw_scatter();
	
	vector<Vector3f> springList;
	
	vector<Vector3f> springCoords;

	vector<Vector3f> spring (float k, float r, int ind1,int ind2, vector<Vector3f> state, vector<Vector3f> existingSprings);

	vector<Particle *> stateClone;

	vector<Particle *> particleEmitter();

	vector<Particle *> emitter;
	
	vector<Particle *> fluid_particles;


	//vector<Vector3f> evalF(vector<Vector3f> state);
		
	std::map<string, vector<Particle *>> neighborMap;		
	// getter method for the system's state
	vector<Particle *> getState(){ return m_vVecState; };
	
	// setter method for the system's state
	void setState(const vector<Particle *>  & newState) { m_vVecState = newState; };

protected:

	vector<Particle *> m_vVecState;
};

#endif
