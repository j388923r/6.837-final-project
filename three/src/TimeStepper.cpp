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





///this is actually an RK4 integrator that we have made
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
float h = stepSize;


vector<Particle> state = particleSystem->getState();
vector<Vector3f> f0 = particleSystem->evalF(state);
vector<Particle> state1;
vector<Particle> state2;
vector<Particle> state3;
vector<Particle> fin;

for (unsigned i = 0; i < particleSystem->m_numParticles ; ++i){

	//cout << "State size: " << state.size() << endl;
	//cout << "Derivative size: " << f0.size() << endl;

	//Vector3f newVec = state[i].position+h*f0[2*i+1];
	Vector3f newVec = state[i].position+ h/2.*f0[2*i];                                               
	//cout << "F0 number 0: " << f0[0][0] <<" " << f0[0][1] <<" " <<  f0[0][2] << endl;
	//cout << "Gravity: " << f0[1][0] << " "<<  f0[1][1] << " " << f0[1][2] << endl;
	Particle p1 = Particle(state[i].mass);
	p1.position = newVec;
	p1.velocity = state[i].velocity+h/2.*f0[2*i+1];    
	p1.density = state[i].density;
	p1.pressure_force = state[i].pressure_force;
	p1.viscocity_force= state[i].viscocity_force;
	p1.color_field_gradient = state[i].color_field_gradient;
	p1.color_field_laplacian = state[i].color_field_laplacian;
	state1.push_back(p1);
	}


vector<Vector3f> f1 = particleSystem->evalF(state1);


	for (unsigned j = 0; j < particleSystem->m_numParticles ; ++j){

		//Vector3f newVec1 = state[j].position+(h/2.)*(f0[2*j+1]+f1[2*j+1]);
		Vector3f newVec1 = state[j].position+(h/2.)*f1[2*j];
		Particle p2 = Particle(state[j].mass);
		p2.position = newVec1;
		p2.velocity = state[j].velocity+(h/2.)*f1[2*j+1];
		p2.density = state[j].density;
		p2.pressure_force = state[j].pressure_force;
		p2.viscocity_force= state[j].viscocity_force;
		p2.color_field_gradient = state[j].color_field_gradient;
		p2.color_field_laplacian = state[j].color_field_laplacian;

		state2.push_back(p2);
	}

vector<Vector3f> f2 = particleSystem->evalF(state2);


	for (unsigned j = 0; j < particleSystem->m_numParticles ; ++j){

		//Vector3f newVec1 = state[j].position+(h/2.)*(f0[2*j+1]+f1[2*j+1]);
		Vector3f newVec1 = state[j].position+(h)*f2[2*j];
		Particle p2 = Particle(state[j].mass);
		p2.position = newVec1;
		p2.velocity = state[j].velocity+(h)*f2[2*j+1];
		p2.density = state[j].density;
		p2.pressure_force = state[j].pressure_force;
		p2.viscocity_force= state[j].viscocity_force;
		p2.color_field_gradient = state[j].color_field_gradient;
		p2.color_field_laplacian = state[j].color_field_laplacian;

		state3.push_back(p2);
	}	

vector<Vector3f> f3 = particleSystem->evalF(state3);


	for (unsigned j = 0; j < particleSystem->m_numParticles ; ++j){

		Vector3f newVec1 = state[j].position+(h/6.)*(f0[2*j]+2*f1[2*j]+2*f2[2*j]+f3[2*j]);
		Particle p2 = Particle(state[j].mass);
		p2.position = newVec1;
		p2.velocity = state[j].velocity+(h/6.)*(f0[2*j+1]+2*f1[2*j+1]+2*f2[2*j+1]+f3[2*j+1]);;
		p2.density = state[j].density;
		p2.pressure_force = state[j].pressure_force;
		p2.viscocity_force= state[j].viscocity_force;
		p2.color_field_gradient = state[j].color_field_gradient;
		p2.color_field_laplacian = state[j].color_field_laplacian;

		fin.push_back(p2);
	}

particleSystem->neighborMap = map<string, vector<Particle>>();

// SPEED UP POSSIBLE

for (unsigned j = 0; j < particleSystem->m_numParticles ; ++j){
	string a = fin[j].getGridLoc();
	map<string, vector<Particle>>::iterator gridLocNeighbors = particleSystem->neighborMap.find(a);
	if (gridLocNeighbors != particleSystem->neighborMap.end()) {
		gridLocNeighbors->second.push_back(fin[j]);
	} else {
		vector<Particle> tempvector;
		tempvector.push_back(fin[j]);
		particleSystem->neighborMap.insert(pair<string, vector<Particle>>(a, tempvector));
	}
}

// particleSystem->neighborMap.find("222") -> second[0].velocity.print();

particleSystem->setState(fin);


}



