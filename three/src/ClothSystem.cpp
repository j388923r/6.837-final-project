#include "ClothSystem.h"
#include <algorithm>
#include <iostream> 
#include <ostream>

//TODO: Initialize here
ClothSystem::ClothSystem(int numParticles):ParticleSystem(numParticles)
{

m_numParticles = numParticles;
	

	//Vector3f fixedPoint = (0,0,0); 
	//Vector3f noMotion = (0,0,0);
	//m_vVecState.push_back(fixedPoint);// for this system, we care about the position and the velocity
	//m_vVecState.push_back(noMotion);
	
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		
		
		for (int j = 0; j < m_numParticles; j++){
			
			Vector3f firstpos (j, -i, 0); 
			//cout << firstpos[0] << " , " << firstpos[1] << endl;
			Vector3f firstspeed = (0,0,0);


			m_vVecState.push_back(firstpos);// for this system, we care about the position and the velocity
			m_vVecState.push_back(firstspeed);
		}
	}


}


Vector3f gravity(float m)
{

	float g = 9.8;

	return Vector3f(0,-1*m*g,0);
}




Vector3f drag(float k, Vector3f vel)
{
	
	return Vector3f(-k*vel[0],-k*vel[1],-k*vel[2]);

}

Vector3f particlePos(int ind, vector<Vector3f> state)
{


	return state[2*ind];


}


Vector3f particleVel(int ind, vector<Vector3f> state)
{


	return state[2*ind+1];


}


vector<Vector3f> ClothSystem::spring (float k, float r, int ind1,int ind2, vector<Vector3f> state, vector<Vector3f> existingSprings)

{
	//vector<Vector3f> springs;
	Vector3f xi = particlePos(ind1,state);
	Vector3f xj = particlePos(ind2,state);

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



vector<Vector3f> ClothSystem::structuralSprings(vector<Vector3f> springState, vector<Vector3f> state, int row, int r, int s)
{

	for (int k=0; k < state.size()/2.-1; k++){

		if ((k+1)%row  != 0){ 		
			springState = spring(s,r,k,k+1,state,springState);
		}	
	
	}

	for (int h=0; h < state.size()/2.-1; h++){

		if (h+row < state.size()/2.){ 		
			springState = spring(s,r,h,h+row,state,springState);
		}	
	
	}
	
	return springState;

}

vector<Vector3f> ClothSystem::shearSprings(vector<Vector3f> springState, vector<Vector3f> state, int row,int r, int s)
{

	for (int k=0; k < state.size()/2.-row; k++){

		if ((k+1)%row  != 0){ 		
			
			springState = spring(s,r,k,k+row+1,state,springState);
		}	
		if ((k)%row  != 0){ 		
			
			//springs = spring(0,.1,k,k+row+1,state,springs);
			springState = spring(s,r,k,k+row-1,state,springState);
		}	
		
	}
	
	return springState;
}


vector<Vector3f> ClothSystem::flexSprings(vector<Vector3f> springState, vector<Vector3f> state, int row,int r, int s)
{

	for (int h=0; h < state.size()/2.-1; h++){

			if(h%row < row-2){		
				springState = spring(s,r,h,h+2,state,springState);
			}	

			if(h+row*2 < state.size()/2.){		
				springState = spring(s,r,h,h+row*2,state,springState);
			}
		
	}
	
	return springState;

}
// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	
	vector<Vector3f> f;

	// YOUR CODE HERE
	vector<Vector3f> springs(state.size()/2.);
	//springs = spring(0,.1,0,1,state,springs);
	//springs = spring(0,.1,1,2,state,springs);
	//springs = spring(.0,.1,2,3,state,springs);	

	int row = m_numParticles;
	
	
	springs = structuralSprings(springs,state,row, 1., 10.);
	springs = shearSprings(springs,state,row,1.5,5.);
	springs = flexSprings(springs,state,row,2,5.);
	

	
	springList = springs;
	stateClone = state;

	for (unsigned i = 0; i < state.size()/2.;++i){
			
		if (i==0){
		
			f.push_back(Vector3f(0,0,0));
			f.push_back(Vector3f(0,0,0));
			}

		else if (i==row-1){

			f.push_back(Vector3f(0,0,0));
			f.push_back(Vector3f(0,0,0));
			}
		else{
			Vector3f total = (0,0,0);
			total = total+gravity(.1);
		
			total = total + drag(2.005,particleVel(i,state));
			
			total = total + springList[i];
			f.push_back(particleVel(i,state));


			f.push_back(total);		
				}	
	 	     }	
	return f;



	//return vector<Vector3f>();
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{

	for (int i = 0; i < m_vVecState.size(); i++) {
		
		if (m_vVecState.size() > 0){
			//cout << m_vVecState.size() << endl;
			Vector3f pos = m_vVecState[2*i];//  position of particle i. YOUR CODE HERE
			Vector3f pos2 = m_vVecState[2*i+2];
			glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2] );
			//glutSolidSphere(0.075f,10.0f,10.0f);
			glPopMatrix();

			
		}
	}

	//cout << springCoords.size() << endl;
	for (int i=0; i < springCoords.size();i++)
	{
		glLineWidth(2.5);
		glColor3f(1.0, 1.0, 1.0);
		int ind1 = springCoords[i][0];
		int ind2 = springCoords[i][1];
		Vector3f pos1 = stateClone[2*ind1];
		//cout << pos1[0] << " , " << pos1[1] << endl;
		Vector3f pos2 = stateClone[2*ind2];
		//cout << pos2[0] << " , " << pos2[1] << endl;
		glBegin(GL_LINES);
		glVertex3f(pos2[0], pos2[1], pos2[2]);
		glVertex3f(pos1[0], pos1[1], pos1[2]);
		glEnd();
	}
}

