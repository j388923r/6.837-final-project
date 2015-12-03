#include "TimeStepper.h"
#include "Vector3f.h"
#include <algorithm>
#include <functional> 
#include <iostream>
#include "particleSystem.h"

using namespace std;

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
float h = stepSize;


vector<Vector3f> state = particleSystem->getState();
vector<Vector3f> f0 = particleSystem->evalF(state);
vector<Vector3f> fin;


for (unsigned i = 0; i < particleSystem->m_numParticles ; ++i){

	Vector3f newVec = state[i]+h*f0[i];
	
	fin.push_back(newVec);
	
		}



particleSystem->setState(fin);

}





///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
float h = stepSize;


vector<Vector3f> state = particleSystem->getState();
vector<Vector3f> f0 = particleSystem->evalF(state);
vector<Vector3f> state1;
vector<Vector3f> fin;

for (unsigned i = 0; i < particleSystem->m_numParticles ; ++i){

	Vector3f newVec = state[i]+h*f0[i];

	state1.push_back(newVec);
	}


vector<Vector3f> f1 = particleSystem->evalF(state1);


for (unsigned j = 0; j < particleSystem->m_numParticles ; ++j){

	Vector3f newVec1 = state[j]+(h/2.)*(f0[j]+f1[j]);

	fin.push_back(newVec1);
	}



particleSystem->setState(fin);


}





