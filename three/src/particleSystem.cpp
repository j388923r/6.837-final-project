	#include "particleSystem.h"
	#include <algorithm>
	#include <typeinfo>
	#include "Utils.h"

	using namespace std;

	ParticleSystem::ParticleSystem(short numParticles)
	{
		m_numParticles = 8 * numParticles * numParticles * numParticles;
	
		//Vector3f fixedPoint = (0,0,0); 
		//Vector3f noMotion = (0,0,0);
		//m_vVecState.push_back(fixedPoint);// for this system, we care about the position and the velocity
		//m_vVecState.push_back(noMotion);
	
		// fill in code for initializing the state based on the number of particles

		for (int i = 0; i < numParticles * 2; ++i) {
			for (int j = 0; j < numParticles * 2; ++j) {
				for (int k = 0; k < numParticles * 2; ++k) {
						Vector3f firstpos = Vector3f(i/Particle::stretcher + 0.5f,j/Particle::stretcher+3,k/Particle::stretcher - 0.5f); 
						Vector3f firstspeed = Vector3f(0.,0.,0.);
						//firstpos.print();
						//m_vVecState.push_back(firstpos);// for this system, we care about the position and the velocity
						//m_vVecState.push_back(firstspeed);

						Particle * new_particle = new Particle(1); 
						new_particle->setPosition(firstpos);
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

	vector <Particle *> getNeighbors(ParticleSystem * particleSystem, Particle * p)
	{

		vector <Particle *> neighbors;
		for ( short i = 26; i >= 0; --i) {
			map<string, vector<Particle *>>::iterator gridLocNeighbors = particleSystem->neighborMap.find(p -> neighborLocs[i] );
			if (gridLocNeighbors != particleSystem->neighborMap.end()) {
				neighbors.insert(neighbors.end(), gridLocNeighbors->second.begin(), gridLocNeighbors->second.end());
			}
		}
	
	
		return neighbors;

	}


	// TODO: implement evalF
	// for a given state, evaluate f(X,t)
	vector<Vector3f> ParticleSystem::evalF(vector<Particle *> state)
	{
		vector<Vector3f> f;
		//cout << "calling evalF" << endl;
		stateClone = state;


		float h = 1.74;

		//density update
		
		for (short i = state.size() - 1; i >= 0; --i){
			vector<Particle *> neighbors = getNeighbors(this, state[i]);
			for (short j = neighbors.size() - 1; j >= 0; --j){
				Vector3f r = state[i]->position - neighbors[j]->position;
				float distance = r.abs();
				if (h > distance){
					state[i]->density += Utils::Wpoly6Laplacian(distance, h);
				}
			}
		}


		float rest_density = 0.2;	
		float k  = 1.3*pow(10, -3);//*pow(10,-24);
		float eta = 2;
		//Vector3f gravity_force = (0, -9.8,0);

		for (short i = state.size() - 1; i >= 0; --i){
			state[i]->clearForces();
			vector<Particle *> neighbors = getNeighbors(this, state[i]);
			// cout << neighbors.size() << endl;
			float density_p = state[i]->density;
			float pressure_p = k*(density_p - rest_density);
			for (short j = neighbors.size() - 1; j >= 0; --j){
				Vector3f r = state[i]->position - neighbors[j]->position;
					float distance = r.abs();
					// cout << distance << endl;
					if (h > distance && distance > 0.01){

						//cout << " H is less of course." << endl;
						float density_n = state[j]->density;

						float pressure_n = k*(density_n - rest_density);

					
						Vector3f spike = r*Utils::WspikyGradient(distance,h);

						state[i]->pressure_force += (pressure_p + pressure_n)/(2*density_n)* spike;
								
						//state[i]->pressure_force.print();

						//cout << state[j]->mass << " " << pressure_p + pressure_n << " " << spike << end;				
					
						//cout << state[j]->mass << " " << Utils::WviscocityLaplacian(r.abs(),h) << endl;
						state[i]->viscocity_force += eta*state[j]->mass*(state[j]->velocity-state[i]->velocity)/density_n * Utils::WviscocityLaplacian(distance,h)*r;

						// state[i]->color_field_gradient += state[j]->mass/density_n* Utils::Wpoly6Gradient(distance, h);

						//state[i]->color_field_laplacian += state[j]->mass/density_n* Utils::Wpoly6Laplacian(distance, h);

						 

					
					}		
			}
		
			/*float gradient_length = state[i]->color_field_gradient;
			//cout << gradient_length << endl;
			if (gradient_length >= 0){
				state[i]->surface_tension_force = -.00518*state[i]->color_field_gradient*state[i]->color_field_laplacian/gradient_length;
			
			} else{
				state[i]->surface_tension_force = Vector3f(0,0,0);
			}*/
		

			//state[i]->viscocity_force.print();
			//state
			f.push_back(state[i]->velocity);
			//cout << state[i]->pressure_force[1] << "  " << state[i]->viscocity_force[2] << endl;
			// state[i]->pressure_force.print();
			// (state[i]->viscocity_force+state[i]->pressure_force).print();
			f.push_back(Vector3f(0, -.294 * state[i]->mass, 0)+state[i]->viscocity_force+state[i]->pressure_force);//+state[i]->surface_tension_force); //Vector3f(0, -.098 * state[i]->mass, 0)+
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
	/*for (int i = 0; i < m_vVecState.size(); ++i) {
		//cout << (m_vVecState.size()) << endl;
		if (m_vVecState.size() > 0){
			glColor3f(1,1,1);
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
	}*/

	//cout << springCoords.size() << endl;
	
	//glColor3f(1,1,1);
	//glutSolidSphere(1,10,10);
	GLfloat ballColor[] = {0.6f, 0.7f, 1.0f, 0.5f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ballColor);

	if (true){
		Vector3f locBall = Vector3f(1.0, 0, 0.0);
		float radBall = 0.5;
		float epsilon = 0.01;
		
		GLfloat ballColor[] = {0.9f, 0.6f, 1.0f, 0.5f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ballColor);
		glPushMatrix();
		glTranslatef(locBall.x(), locBall.y(), locBall.z());
		glutSolidSphere(radBall, 10.0, 10.0);
		glPopMatrix();
		glColor3f(0.0, 1.0, 0.0);
		for (unsigned i = 0; i < m_vVecState.size(); ++i) {
		    if ((m_vVecState[i]->position - locBall).abs() <= (radBall + epsilon)){
			m_vVecState[i]->position = (locBall + (radBall + epsilon) * (m_vVecState[i]->position - locBall).normalized());			
		    }
		}
	}
}

void ParticleSystem::draw2(){	
	for (unsigned i = 0; i < m_vVecState.size(); ++i) {
		//cout << (m_vVecState.size()) << endl;
		if (m_vVecState.size() > 0){
			//glColor3f(1,1,1);
			//glBlendFunc (1.0, 0.0);
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			GLfloat ballColor[] = {0.1f, 0.4f, 1.0f, 0.5f};
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ballColor);
			Vector3f pos = m_vVecState[i]->position;
			// cout << "Position" << pos[0] << pos[1] << pos[2] << endl;
			//pos.print();
			glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2] );
			glutSolidSphere(0.075f,10.0f,10.0f);
			glPopMatrix();

		
		}
	}
}

void ParticleSystem::draw3(){
	GLfloat floorColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floorColor);
    glPushMatrix();
	float epsilon = 0.01;
    glEnable (GL_BLEND);
	float alpha = 1;
    glBlendFunc (alpha, 1.0-alpha);
    glTranslatef(0.0f,-5.0f,0.0f);
    glScaled(50.0f,0.01f,50.0f);
    glutSolidCube(1);
    glPopMatrix();
	for (unsigned i = 0; i < m_vVecState.size(); i++) {
		if (m_vVecState[i]->position.y() <= -5){
			m_vVecState[i]->position.y() = -5+epsilon;
		}
	}
}


void ParticleSystem::draw_scatter(){
	float radBall = 0.075;
	float epsilon = 0.01;
	Vector3f adjusting_push = (.01,.01,.01);
	for (unsigned i = 0; i < m_vVecState.size(); i++) {
		for (unsigned j = 0; j < m_vVecState.size(); j++) {
			Vector3f r = m_vVecState[i]->position == m_vVecState[j]->position;
			Vector3f locBall = m_vVecState[j]->position;
			//if ((m_vVecState[i]->position - locBall).abs() <= (radBall + epsilon)){
			//m_vVecState[i]->position = (locBall + (radBall + epsilon) * (m_vVecState[i]->position - locBall).normalized());			
		    //}
			//if (r.abs() <= .075){
			//	m_vVecState[i]->velocity +=	adjusting_push;
			//	m_vVecState[j]->velocity -= adjusting_push;	
			//}
		}
	}
}
