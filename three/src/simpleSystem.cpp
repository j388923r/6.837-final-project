#include <iostream> 
#include <ostream>
//#include <sstream>
#include <iterator>
#include <numeric> 
#include "simpleSystem.h"


using namespace std;

SimpleSystem::SimpleSystem(int numParticles):ParticleSystem(numParticles)
{

	m_numParticles = numParticles;

	for (int i = 0; i < m_numParticles; i++) {
		
		Vector3f first = (i/1+1.,i/1.+1,1);
		m_vVecState.push_back(first);
	}
}





// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	//cout << state[0] << endl;

	
	vector<Vector3f> f;

		for (unsigned i = 0; i < state.size();++i){

			f.push_back(Vector3f(state[i][1],state[i][0]*-1,0));		
				}	

	
	//cout << f[0] << endl;	
	return f;
}





// render the system (ie draw the particles)
void SimpleSystem::draw()
{
		//Vector3f first = (1,1,0);

		//m_vVecState.push_back(first);
		//cout << (m_vVecState[0]) << endl;
		if (m_vVecState.size() > 0){
			for (int i = 0; i<m_vVecState.size();i++){
			Vector3f pos = m_vVecState[i];//YOUR PARTICLE POSITION
	  		glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2] );
			glutSolidSphere(0.075f,10.0f,10.0f);
			glPopMatrix();
			//cout << (m_vVecState[0]) << endl;
			}
			}
		else if (m_vVecState.size() > 1){

			cout << m_vVecState[1] << endl;
			} 
		else {  cout << "Something is off.." << endl;
				}	
}
