#include "TimeStepper.h"
#include "Vector3f.h"
#include <algorithm>
#include <functional> 
#include <iostream>
#include "particleSystem.h"

using namespace std;

///TODO: implement Explicit Euler time integrator here
/*void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
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

}*/





///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
float h = stepSize;


vector<Particle> state = particleSystem->getState();
vector<Vector3f> f0 = particleSystem->evalF(state);
vector<Particle> state1;
vector<Particle> fin;

for (unsigned i = 0; i < particleSystem->m_numParticles ; ++i){

	//cout << "State size: " << state.size() << endl;
	//cout << "Derivative size: " << f0.size() << endl;

	Vector3f newVec = state[i].position+h*f0[2*i+1];
	//cout << "F0 number 0: " << f0[0][0] <<" " << f0[0][1] <<" " <<  f0[0][2] << endl;
	//cout << "Gravity: " << f0[1][0] << " "<<  f0[1][1] << " " << f0[1][2] << endl;
	Particle p1 = Particle(state[i].mass);
	p1.position = newVec;
	p1.velocity = state[i].velocity;
	p1.density = state[i].density;
	p1.pressure_force = state[i].pressure_force;
	p1.viscocity_force= state[i].viscocity_force;
	p1.color_field_gradient = state[i].color_field_gradient;
	p1.color_field_laplacian = state[i].color_field_laplacian;
	state1.push_back(p1);
	}


vector<Vector3f> f1 = particleSystem->evalF(state1);


	for (unsigned j = 0; j < particleSystem->m_numParticles ; ++j){

		Vector3f newVec1 = state[j].position+(h/2.)*(f0[2*j+1]+f1[2*j+1]);
		Particle p2 = Particle(state[j].mass);
		p2.position = newVec1;
		p2.velocity = state[j].velocity;
		p2.density = state[j].density;
		p2.pressure_force = state[j].pressure_force;
		p2.viscocity_force= state[j].viscocity_force;
		p2.color_field_gradient = state[j].color_field_gradient;
		p2.color_field_laplacian = state[j].color_field_laplacian;

		fin.push_back(p2);
	}



particleSystem->setState(fin);


}



