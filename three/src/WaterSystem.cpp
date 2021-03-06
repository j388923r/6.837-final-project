#include <iostream> 
#include <ostream>
#include "WaterSystem.h"
#include <algorithm>

using namespace std;

WaterSystem::WaterSystem(int numParticles)
{
	m_numParticles = numParticles;
	

	//Vector3f fixedPoint = (0,0,0); 
	//Vector3f noMotion = (0,0,0);
	//m_vVecState.push_back(fixedPoint);// for this system, we care about the position and the velocity
	//m_vVecState.push_back(noMotion);
	
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		
		Vector3f firstpos = (i/1.,i/1.,i/1.); 
		Vector3f firstspeed = (0,0,0);

		
		//m_vVecState.push_back(firstpos);// for this system, we care about the position and the velocity
		//m_vVecState.push_back(firstspeed);

		Particle new_particle(1); 
		new_particle.position = firstpos;
		new_particle.velocity = firstspeed;

		cout << "New Particle: " << new_particle.position[0] << endl;
		fluid_particles.push_back(new_particle);
		m_vVecState.push_back(new_particle);

		

	}
}


Vector3f gravitywater(float m)
{

	float g = 9.8;

	return Vector3f(0,-1*m*g,0);
}


Vector3f dragwater(float k, Particle p)
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



vector<Vector3f> WaterSystem::spring (float k, float r, int ind1,int ind2, vector<Vector3f> state, vector<Vector3f> existingSprings)

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

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> WaterSystem::evalF(vector<Particle> state)
{

	cout << "evalF" << endl;
	vector<Vector3f> f;

	// YOUR CODE HERE
	/*vector<Vector3f> springs(state.size()/2.);
	springs = spring(.7,.1,0,1,state,springs);
	springs = spring(.55,.1,1,2,state,springs);
	springs = spring(.4,.1,2,3,state,springs);	

	springList = springs;*/
	stateClone = state;
	for (unsigned i = 0; i < state.size();++i){
		cout << state[i].velocity << endl;
		if (i==0){
		
			f.push_back(Vector3f(0,0,0));
			f.push_back(Vector3f(0,0,0));
			}
		else{
			Vector3f total = (0,0,0);
			total = total+gravitywater(.02);
		
			total = total + dragwater(.1,state[i].velocity);
			cout << "Velocity: " << state[i].velocity[0] << endl;
			
			total = total + springList[i];

			f.push_back(state[i].velocity);
			f.push_back(total);		
				}	
	 	     }
	
	return f;
}

// render the system (ie draw the particles)
void WaterSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
		//cout << (m_vVecState.size()) << endl;
		if (m_vVecState.size() > 0){
			//Vector3f pos = m_vVecState[2*i];//  position of particle i. YOUR CODE HERE
			Vector3f pos = fluid_particles[i].position;
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



