#include "particleSystem.h"
#include <algorithm>
#include <typeinfo>
#include "Utils.h"

using namespace std;

ParticleSystem::ParticleSystem(int numParticles)
{
	m_numParticles = numParticles * numParticles * numParticles;
	

	//Vector3f fixedPoint = (0,0,0); 
	//Vector3f noMotion = (0,0,0);
	//m_vVecState.push_back(fixedPoint);// for this system, we care about the position and the velocity
	//m_vVecState.push_back(noMotion);
	
	// fill in code for initializing the state based on the number of particles

	for (int i = 0; i < numParticles; ++i) {
		for (int j = 0; j < numParticles; ++j) {
			for (int k = 0; k < numParticles; ++k) {
					Vector3f firstpos = Vector3f(i*1./1,j*1./1,k*1./1); 
					Vector3f firstspeed = Vector3f(0.,0.,0.);
	
					//m_vVecState.push_back(firstpos);// for this system, we care about the position and the velocity
					//m_vVecState.push_back(firstspeed);

					Particle * new_particle = new Particle(1); 
					new_particle->position = firstpos;
					new_particle->velocity = firstspeed;

					//cout << "New Particle: " << new_particle.position[0] << endl;
					//fluid_particles.push_back(new_particle);
					m_vVecState.push_back(new_particle);
			}
		}
	}
}


Vector3f gravitywater(float m)
{

	float g = 9.8;

	return Vector3f(0,-1*m*g,0);
}


Vector3f dragwater(float k, Particle &p)
{
	
	return Vector3f(-k*p.velocity[0],-k*p.velocity[1],-k*p.velocity[2]);

}

Vector3f dragwater(float k, Vector3f vel)
{
	
	return Vector3f(-k*vel[0],-k*vel[1],-k*vel[2]);

}

Vector3f particlePoswater(int ind, vector<Vector3f> state)
{


	return state[2*ind];


}


Vector3f particleVelwater(int ind, vector<Vector3f> state)
{


	return state[2*ind+1];


}



vector<Vector3f> ParticleSystem::spring (float k, float r, int ind1,int ind2, vector<Vector3f> state, vector<Vector3f> existingSprings)

{
	//vector<Vector3f> springs;
	Vector3f xi = particlePoswater(ind1,state);
	Vector3f xj = particlePoswater(ind2,state);

	Vector3f d = xi-xj;
	Vector3f fi = -k*(d.abs()-r)*d/d.abs();
	Vector3f fj = k*(d.abs()-r)*d/d.abs();

	existingSprings[ind1] += fi;
	existingSprings[ind2] += fj;	

	if (std::find(springCoords.begin(), springCoords.end(), Vector3f(ind1,ind2,0)) == springCoords.end()){

		springCoords.push_back(Vector3f(ind1,ind2,0));
	}


	return existingSprings;
}



vector <Particle *> getNeighbors(ParticleSystem * particleSystem, Particle * p)
{

	vector <Particle *> neighbors;
	short a = (short)p->position[0];
	short b = (short)p->position[1];
	short c = (short)p->position[2];
	for ( short i = -1; i < 2; ++i) {
		for ( short j = -1; j < 2; ++j) {
			for ( short k = -1; k < 2; ++k) {
				if (i != 0 && j != 0 && k != 0) {
					stringstream glStream;
					glStream << a + i << b + j << c + k;
					map<string, vector<Particle *>>::iterator gridLocNeighbors = particleSystem->neighborMap.find(glStream.str());
					if (gridLocNeighbors != particleSystem->neighborMap.end()) {
						neighbors.insert(neighbors.end(), gridLocNeighbors->second.begin(), gridLocNeighbors->second.end());
					}
				}
			}
		}
	}
	
	
	return neighbors;

}



// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ParticleSystem::evalF(vector<Particle *> state)
{
	vector<Vector3f> f;

	cout << "calling evalF" << endl;
	// YOUR CODE HERE
	/*vector<Vector3f> springs(state.size()/2.);
	springs = spring(.7,.1,0,1,state,springs);
	springs = spring(.55,.1,1,2,state,springs);
	springs = spring(.4,.1,2,3,state,springs);	

	springList = springs;*/
	stateClone = state;


	float h = 1.74;

	//density update
	/*
	for (unsigned i = 0; i < state.size();++i){
		for (unsigned j = 0; j < getNeighbors(i).size();++j){
			Vector3f r = state[i]->position - getNeighbors(i)[j].position;
			if (h <= r.abs()){
				state[i]->density += state[j]->mass * Utils::Wpoly6Laplacian(r, h);
			}
		}
	}*/


	float rest_density = .2;	
	float k  = 1.3*pow(10,-24);
	float eta = 1;
	//Vector3f gravity_force = (0, -9.8,0);

	for (unsigned i = 0; i < state.size();++i){
		vector<Particle *> neighbors = getNeighbors(this, state[i]);
		for (unsigned j = 0; j < neighbors.size();++j){
			state[i]->clearForces();
			Vector3f r = state[i]->position - neighbors[j]->position;
				float distance = r.abs();
				if (h > distance){

					//cout << " H is less of course." << endl;
					float density_p = state[i]->density;
					float density_n = state[j]->density;

					float pressure_p = k*(density_p - rest_density);
					float pressure_n = k*(density_n - rest_density);

					
					Vector3f spike = r*Utils::WspikyGradient(r.abs(),h);

					state[i]->pressure_force += state[j]->mass *(pressure_p + pressure_n)/(2*density_n)* spike;
								
					//state[i]->pressure_force.print();

					//cout << state[j]->mass << " " << pressure_p + pressure_n << " " << spike << end;				
					
					state[i]->viscocity_force += eta*state[j]->mass*(state[j]->velocity-state[i]->velocity)/density_n * Utils::WviscocityLaplacian(r.abs(),h);

					state[i]->color_field_gradient += state[j]->mass/density_n* Utils::Wpoly6Gradient(r.abs(), h);

					state[i]->color_field_laplacian += state[j]->mass/density_n* Utils::WviscocityLaplacian(r.abs(), h);

					 

					
				}		
		}
		
		float gradient_length = state[i]->color_field_gradient;
		if (gradient_length >= .2){
			state[i]->surface_tension_force = -.00518*state[i]->color_field_gradient*state[i]->color_field_laplacian/gradient_length;
			
		} else{
			state[i]->surface_tension_force = 0;
		}
		//gravity_force.print();
		//state
		f.push_back(state[i]->velocity);
		//cout << state[i]->pressure_force[1] << "  " << state[i]->viscocity_force[2] << endl;
		//state[i]->pressure_force.print();
		//gravity_force.print();
		f.push_back(Vector3f(0, -.98 * state[i] -> mass, 0)+state[i]->pressure_force); //+ state[i]->surface_tension_force+state[i]->pressure_force);  
		//f.push_back(state[i]->pressure_force + state[i]->viscocity_force+ gravity_force * state[i]->mass + state[i]->surface_tension_force);
				
		
	}


	return f;
}

/*// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ParticleSystem::evalF(vector<Vector3f> state)
{
	cout << "Cloth" << endl;
	return vector<Vector3f>();
}*/

// render the system (ie draw the particles)
void ParticleSystem::draw()
{
	for (int i = 0; i < m_vVecState.size(); i++) {
		//cout << (m_vVecState.size()) << endl;
		if (m_vVecState.size() > 0){
			Vector3f pos = m_vVecState[i]->position;
			//pos.print();
			glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2] );
			glutSolidSphere(0.075f,10.0f,10.0f);
			glPopMatrix();

			//glLineWidth(2.5);
			//glColor3f(1.0, 0.0, 0.0);
			//glBegin(GL_LINES);
			//glVertex3f(pos2[0], pos2[2], pos2[2]);
			//glVertex3f(pos[0], pos[1], pos[2]);
			//glEnd();
		}
	}
	
	//cout << springCoords.size() << endl;
	/*for (int i=0; i < springCoords.size();i++)
	{
		glLineWidth(2.5);
		glColor3f(1.0, 1.0, 1.0);
		int ind1 = springCoords[i][0];
		int ind2 = springCoords[i][1];
		Vector3f pos1 = m_vVecState[2*ind1];
		Vector3f pos2 = m_vVecState[2*ind2];
		glBegin(GL_LINES);
		glVertex3f(pos2[0], pos2[1], pos2[2]);
		glVertex3f(pos1[0], pos1[1], pos1[2]);
		glEnd();
	}*/

}



