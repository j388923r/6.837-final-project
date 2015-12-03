#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include "extra.h"
#include <vector>
#include "Utils.h"
#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	WaterSystem(int numParticles);

	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();
	

private:

};


#endif
