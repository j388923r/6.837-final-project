	#include "particleSystem.h"
	#include <algorithm>
	#include <typeinfo>
	#include "Utils.h"

	using namespace std;

	ParticleSystem::ParticleSystem(int numParticles)
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
						Vector3f firstpos = Vector3f(i/Particle::stretcher + 0.5f,j/Particle::stretcher + 3.0f,k/Particle::stretcher - 0.5f); 
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

		//particleEmitter();

		
	}

	vector<Particle *> ParticleSystem::particleEmitter() {

	vector<Particle *> PL;
	float spread = .3;
	float colorSpread = .2;
	int k = 10;
	Vector3f color = (0,0,1);

	for(unsigned i =0;i< k; i++){
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		Particle * p = new Particle(1);
		p->position = Vector3f(0,0,0);
		p->velocity = Vector3f(-1,2,-1) + spread*Vector3f(r1,r2,r3);	
		emitter.push_back(p);
	}

	float epsilon = 0.001;
	float dt = .08;
	//cout << emitter.size() << endl;
	
	for (int i = 0; i < emitter.size(); i++) {
		
		Particle* p = emitter[i];
		p->position += p->velocity*dt;
		p->velocity -= Vector3f(0,.98,0)*dt;
		glVertex(p->position);
		Vector3f pos = p->position;

		
		//p->position.print();
		glPushMatrix();
		//GLfloat ballColor[] = {0.1f, 0.6f, 1.0f, 0.5f};//light blue
		float c1 = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*.1;
		float c2 = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*.3;
		GLfloat ballColor[] = {.0f+c1, .1f+c2, .9f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ballColor);
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.05f,10.0f,10.0f);
		glPopMatrix();
	}
}


	Vector3f particleVelwater(int ind, vector<Vector3f> state)
	{


		return state[2*ind+1];


	}

	vector <Particle *> getNeighbors(ParticleSystem * particleSystem, Particle * p)
	{

		vector <Particle *> neighbors;

		/*map<string, vector<Particle *>>::iterator gridLocNeighbors = particleSystem->neighborList.find(p -> getGridLoc());
		if (gridLocNeighbors != particleSystem->neighborList.end()) {
			return gridLocNeighbors->second;
		}*/

		for ( unsigned i = 0; i < 27; ++i) {
			map<string, vector<Particle *>>::iterator gridLocNeighbors = particleSystem->neighborMap.find(p -> neighborLocs[i] );
			if (gridLocNeighbors != particleSystem->neighborMap.end()) {
				neighbors.insert(neighbors.end(), gridLocNeighbors->second.begin(), gridLocNeighbors->second.end());
			}
		}
	
		//particleSystem->neighborList.insert(pair<string, vector<Particle *>>(p->getGridLoc(), neighbors));
		particleSystem->inside.insert(pair<string, bool>(p -> getGridLoc(), neighbors.size() != 0));
		return neighbors;
		//return vector <Particle *>();	
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
		
		for (unsigned i = 0; i < state.size(); ++i){
			state[i]->clearForces();
			vector<Particle *> neighbors = getNeighbors(this, state[i]);
			for (unsigned j = 0; j < neighbors.size(); ++j){
				Vector3f r = state[i]->position - neighbors[j]->position;
				float distance = r.abs();
				//cout << distance << endl;
				if (h > distance){
					state[i]->density += Utils::Wpoly6(distance, h);
					// cout << state[i]->density << endl;
				}
			}
		}

		float rest_density = 0.2;	
		float k  = 2.6*pow(10, -3);//*pow(10,-24);
		float eta = 2;
		//Vector3f gravity_force = (0, -9.8,0);
	
	//particleEmitter();

		for (unsigned i = 0; i < state.size(); ++i){
			vector<Particle *> neighbors = getNeighbors(this, state[i]);
			// cout << neighbors.size() << endl;
			float density_p = state[i]->density;
			float pressure_p = k*(density_p - rest_density);
			for (unsigned j = 0; j < neighbors.size(); ++j){
				Vector3f r = state[i]->position - neighbors[j]->position;
					float distance = r.abs();
					// cout << distance << endl;
					if (h > distance && distance > 0.01){

						//cout << " H is less of course." << endl;
						float density_n = state[j]->density;

						float pressure_n = k*(density_n - rest_density);

					
						Vector3f spike = r*Utils::WspikyGradient(distance,h);
						if (abs(density_n) > 0.00000001) {
							state[i]->pressure_force += state[j]-> mass * (pressure_p + pressure_n)/(2*density_n)* spike;
						}
								
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
			if(state[i] -> position.y() < -4.8f) {	
				// cout << "hit the floor" << endl;
				state[i] -> viscocity_force = state[i] -> viscocity_force + Vector3f(0, -25 * state[i]->velocity.y()+state[i]->mass, 0);
			}
			//cout << state[i]->pressure_force[1] << "  " << state[i]->viscocity_force[2] << endl;
			//state[i]->pressure_force.print();
			//gravity_force.print();
			//(state[i]->viscocity_force+state[i]->pressure_force).print();
			f.push_back(state[i]->velocity);
			f.push_back(Vector3f(0, -.294 * state[i]->mass, 0)+state[i]->viscocity_force-state[i]->pressure_force);//+state[i]->surface_tension_force); //Vector3f(0, -.098 * state[i]->mass, 0)++ collision_force
			//f.push_back(Vector3f(0, 0, 0));
			//f.push_back(Vector3f(0, 0, 0));
				
		
	}


		return f;
	}

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
	glDisable (GL_BLEND);
	if (true){
		Vector3f locBall = Vector3f(1.0, 1.0, 0.0);
		float radBall = .75;
		float epsilon = 0.1;
		
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

	
	//glVertex3f(0,0,0);
	//glutSolidSphere(0.1f,10.0f,10.0f);
	
	//glDisable (GL_BLEND);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	for (unsigned i = 0; i < m_vVecState.size(); ++i) {
		//cout << (m_vVecState.size()) << endl;
		//glColor3f(1,1,1);
		//glBlendFunc (1.0, 0.0);
	
		
		GLfloat ballColor[] = {0.1f, 0.4f, 1.0f, 0.5f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ballColor);
		Vector3f pos = m_vVecState[i]->position;
		Vector3f posR = Vector3f(round(pos[0] * Particle::stretcher) / Particle::stretcher, round(pos[1] * Particle::stretcher) / Particle::stretcher, round(pos[2] * Particle::stretcher) / Particle::stretcher);
		//cout << "Position" << pos[0] << pos[1] << pos[2] << endl;
		//pos.print();
		/*glPushMatrix();
		
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();*/

		map<string, bool>::iterator hasNeighbors = inside.find(m_vVecState[i] -> getGridLoc());
		if(hasNeighbors->second) {
			unsigned edgeIndex1 = 1;
			for ( unsigned a = 1; a < 8; ++a) {
				edgeIndex1 = edgeIndex1 << 1;
				map<string, bool>::iterator isInside = inside.find(m_vVecState[i] -> MCLocs[a]);
				if (isInside->second) {
					edgeIndex1 += 1;
				}
			}

			//cout << "1:" << edgeIndex1 << endl;

			glEnableClientState(GL_VERTEX_ARRAY);

			for (unsigned a = 0; a < 16; a+=3) {
				if(triTable[edgeIndex1][a] == -1) {
					break;
				} else {
					unsigned triOneIndex1 = triTable[edgeIndex1][a];
					unsigned triTwoIndex1 = triTable[edgeIndex1][a+1];
					unsigned triThreeIndex1 = triTable[edgeIndex1][a+2];
					GLfloat vertices[] = { posR[0] + marchingDelta[triOneIndex1][0], posR[1] + marchingDelta[triOneIndex1][1], posR[2] + marchingDelta[triOneIndex1][2],
										  posR[0] + marchingDelta[triTwoIndex1][0], posR[1] + marchingDelta[triTwoIndex1][1], posR[2] + marchingDelta[triTwoIndex1][2], 
										 posR[0] + marchingDelta[triThreeIndex1][0], posR[1] + marchingDelta[triThreeIndex1][1], posR[2] + marchingDelta[triThreeIndex1][2], 
										 posR[0] + marchingDelta[triThreeIndex1][0], posR[1] + marchingDelta[triThreeIndex1][1], posR[2] + marchingDelta[triThreeIndex1][2], 
										 posR[0] + marchingDelta[triTwoIndex1][0], posR[1] + marchingDelta[triTwoIndex1][1], posR[2] + marchingDelta[triTwoIndex1][2], 
										 posR[0] + marchingDelta[triOneIndex1][0], posR[1] + marchingDelta[triOneIndex1][1], posR[2] + marchingDelta[triOneIndex1][2]};

					glVertexPointer(3, GL_FLOAT, 0, vertices);

					// draw a cube
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}
			
			unsigned edgeIndex2 = 0;
			for ( unsigned a = 8; a < 16; ++a) {
				edgeIndex2 = edgeIndex2 << 1;
				map<string, bool>::iterator isInside = inside.find(m_vVecState[i] -> MCLocs[a]);
				if (isInside->second) {
					edgeIndex2 += 1;
				}
			}

			// cout << "2:" << edgeIndex2 << endl;

			for (unsigned a = 0; a < 16; a+=3) {
				if(triTable[edgeIndex2][a] == -1) {
					break;
				} else {
					unsigned triOneIndex2 = triTable[edgeIndex2][a];
					unsigned triTwoIndex2 = triTable[edgeIndex2][a+1];
					unsigned triThreeIndex2 = triTable[edgeIndex2][a+2];
					GLfloat vertices[] = { posR[0] + marchingDelta[triOneIndex2][0], posR[1] + marchingDelta[triOneIndex2][1], posR[2] + marchingDelta[triOneIndex2][2],
										  posR[0] + marchingDelta[triTwoIndex2][0], posR[1] + marchingDelta[triTwoIndex2][1], posR[2] + marchingDelta[triTwoIndex2][2], 
										 posR[0] + marchingDelta[triThreeIndex2][0], posR[1] + marchingDelta[triThreeIndex2][1], posR[2] + marchingDelta[triThreeIndex2][2], 
										 posR[0] + marchingDelta[triThreeIndex2][0], posR[1] + marchingDelta[triThreeIndex2][1], posR[2] + marchingDelta[triThreeIndex2][2], 
										 posR[0] + marchingDelta[triTwoIndex2][0], posR[1] + marchingDelta[triTwoIndex2][1], posR[2] + marchingDelta[triTwoIndex2][2], 
										 posR[0] + marchingDelta[triOneIndex2][0], posR[1] + marchingDelta[triOneIndex2][1], posR[2] + marchingDelta[triOneIndex2][2]};

					glVertexPointer(3, GL_FLOAT, 0, vertices);

					// draw a cube
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}
	
			unsigned edgeIndex3 = 0;
			for ( unsigned a = 16; a < 24; ++a) {
				edgeIndex3 = edgeIndex3 << 1;
				map<string, bool>::iterator isInside = inside.find(m_vVecState[i] -> MCLocs[a]);
				if (isInside->second) {
					edgeIndex3 += 1;
				}
			}

			// cout << "2:" << edgeIndex3 << endl;

			for (unsigned a = 0; a < 16; a+=3) {
				if(triTable[edgeIndex3][a] == -1) {
					break;
				} else {
					unsigned triOneIndex3 = triTable[edgeIndex3][a];
					unsigned triTwoIndex3 = triTable[edgeIndex3][a+1];
					unsigned triThreeIndex3 = triTable[edgeIndex3][a+2];
					GLfloat vertices[] = { posR[0] + marchingDelta[triOneIndex3][0], posR[1] + marchingDelta[triOneIndex3][1], posR[2] + marchingDelta[triOneIndex3][2],
										  posR[0] + marchingDelta[triTwoIndex3][0], posR[1] + marchingDelta[triTwoIndex3][1], posR[2] + marchingDelta[triTwoIndex3][2], 
										 posR[0] + marchingDelta[triThreeIndex3][0], posR[1] + marchingDelta[triThreeIndex3][1], posR[2] + marchingDelta[triThreeIndex3][2], 
										 posR[0] + marchingDelta[triThreeIndex3][0], posR[1] + marchingDelta[triThreeIndex3][1], posR[2] + marchingDelta[triThreeIndex3][2], 
										 posR[0] + marchingDelta[triTwoIndex3][0], posR[1] + marchingDelta[triTwoIndex3][1], posR[2] + marchingDelta[triTwoIndex3][2], 
										 posR[0] + marchingDelta[triOneIndex3][0], posR[1] + marchingDelta[triOneIndex3][1], posR[2] + marchingDelta[triOneIndex3][2]};

					glVertexPointer(3, GL_FLOAT, 0, vertices);

					// draw a cube
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}

			unsigned edgeIndex4 = 0;
			for ( unsigned a = 24; a < 32; ++a) {
				edgeIndex4 = edgeIndex4 << 1;
				map<string, bool>::iterator isInside = inside.find(m_vVecState[i] -> MCLocs[a]);
				if (isInside->second) {
					edgeIndex4 += 1;
				}
			}

			// cout << "2:" << edgeIndex4 << endl;

			for (unsigned a = 0; a < 16; a+=3) {
				if(triTable[edgeIndex4][a] == -1) {
					break;
				} else {
					unsigned triOneIndex4 = triTable[edgeIndex4][a];
					unsigned triTwoIndex4 = triTable[edgeIndex4][a+1];
					unsigned triThreeIndex4 = triTable[edgeIndex4][a+2];
					GLfloat vertices[] = { posR[0] + marchingDelta[triOneIndex4][0], posR[1] + marchingDelta[triOneIndex4][1], posR[2] + marchingDelta[triOneIndex4][2],
										  posR[0] + marchingDelta[triTwoIndex4][0], posR[1] + marchingDelta[triTwoIndex4][1], posR[2] + marchingDelta[triTwoIndex4][2], 
										 posR[0] + marchingDelta[triThreeIndex4][0], posR[1] + marchingDelta[triThreeIndex4][1], posR[2] + marchingDelta[triThreeIndex4][2], 
										 posR[0] + marchingDelta[triThreeIndex4][0], posR[1] + marchingDelta[triThreeIndex4][1], posR[2] + marchingDelta[triThreeIndex4][2], 
										 posR[0] + marchingDelta[triTwoIndex4][0], posR[1] + marchingDelta[triTwoIndex4][1], posR[2] + marchingDelta[triTwoIndex4][2], 
										 posR[0] + marchingDelta[triOneIndex4][0], posR[1] + marchingDelta[triOneIndex4][1], posR[2] + marchingDelta[triOneIndex4][2]};

					glVertexPointer(3, GL_FLOAT, 0, vertices);

					// draw a cube
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}

			unsigned edgeIndex5 = 0;
			for ( unsigned a = 32; a < 40; ++a) {
				edgeIndex5 = edgeIndex5 << 1;
				map<string, bool>::iterator isInside = inside.find(m_vVecState[i] -> MCLocs[a]);
				if (isInside->second) {
					edgeIndex5 += 1;
				}
			}

			// cout << "2:" << edgeIndex5 << endl;

			for (unsigned a = 0; a < 16; a+=3) {
				if(triTable[edgeIndex5][a] == -1) {
					break;
				} else {
					unsigned triOneIndex5 = triTable[edgeIndex5][a];
					unsigned triTwoIndex5 = triTable[edgeIndex5][a+1];
					unsigned triThreeIndex5 = triTable[edgeIndex5][a+2];
					GLfloat vertices[] = { posR[0] + marchingDelta[triOneIndex5][0], posR[1] + marchingDelta[triOneIndex5][1], posR[2] + marchingDelta[triOneIndex5][2],
										  posR[0] + marchingDelta[triTwoIndex5][0], posR[1] + marchingDelta[triTwoIndex5][1], posR[2] + marchingDelta[triTwoIndex5][2], 
										 posR[0] + marchingDelta[triThreeIndex5][0], posR[1] + marchingDelta[triThreeIndex5][1], posR[2] + marchingDelta[triThreeIndex5][2], 
										 posR[0] + marchingDelta[triThreeIndex5][0], posR[1] + marchingDelta[triThreeIndex5][1], posR[2] + marchingDelta[triThreeIndex5][2], 
										 posR[0] + marchingDelta[triTwoIndex5][0], posR[1] + marchingDelta[triTwoIndex5][1], posR[2] + marchingDelta[triTwoIndex5][2], 
										 posR[0] + marchingDelta[triOneIndex5][0], posR[1] + marchingDelta[triOneIndex5][1], posR[2] + marchingDelta[triOneIndex5][2]};

					glVertexPointer(3, GL_FLOAT, 0, vertices);

					// draw a cube
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}			

			unsigned edgeIndex6 = 0;
			for ( unsigned a = 40; a < 48; ++a) {
				edgeIndex6 = edgeIndex6 << 1;
				map<string, bool>::iterator isInside = inside.find(m_vVecState[i] -> MCLocs[a]);
				if (isInside->second) {
					edgeIndex6 += 1;
				}
			}

			// cout << "2:" << edgeIndex6 << endl;

			for (unsigned a = 0; a < 16; a+=3) {
				if(triTable[edgeIndex6][a] == -1) {
					break;
				} else {
					unsigned triOneIndex6 = triTable[edgeIndex6][a];
					unsigned triTwoIndex6 = triTable[edgeIndex6][a+1];
					unsigned triThreeIndex6 = triTable[edgeIndex6][a+2];
					GLfloat vertices[] = { posR[0] + marchingDelta[triOneIndex6][0], posR[1] + marchingDelta[triOneIndex6][1], posR[2] + marchingDelta[triOneIndex6][2],
										  posR[0] + marchingDelta[triTwoIndex6][0], posR[1] + marchingDelta[triTwoIndex6][1], posR[2] + marchingDelta[triTwoIndex6][2], 
										 posR[0] + marchingDelta[triThreeIndex6][0], posR[1] + marchingDelta[triThreeIndex6][1], posR[2] + marchingDelta[triThreeIndex6][2], 
										 posR[0] + marchingDelta[triThreeIndex6][0], posR[1] + marchingDelta[triThreeIndex6][1], posR[2] + marchingDelta[triThreeIndex6][2], 
										 posR[0] + marchingDelta[triTwoIndex6][0], posR[1] + marchingDelta[triTwoIndex6][1], posR[2] + marchingDelta[triTwoIndex6][2], 
										 posR[0] + marchingDelta[triOneIndex6][0], posR[1] + marchingDelta[triOneIndex6][1], posR[2] + marchingDelta[triOneIndex6][2]};

					glVertexPointer(3, GL_FLOAT, 0, vertices);

					// draw a cube
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}

			unsigned edgeIndex7 = 0;
			for ( unsigned a = 48; a < 56; ++a) {
				edgeIndex7 = edgeIndex7 << 1;
				map<string, bool>::iterator isInside = inside.find(m_vVecState[i] -> MCLocs[a]);
				if (isInside->second) {
					edgeIndex7 += 1;
				}
			}

			// cout << "2:" << edgeIndex7 << endl;

			for (unsigned a = 0; a < 16; a+=3) {
				if(triTable[edgeIndex7][a] == -1) {
					break;
				} else {
					unsigned triOneIndex7 = triTable[edgeIndex7][a];
					unsigned triTwoIndex7 = triTable[edgeIndex7][a+1];
					unsigned triThreeIndex7 = triTable[edgeIndex7][a+2];
					GLfloat vertices[] = { posR[0] + marchingDelta[triOneIndex7][0], posR[1] + marchingDelta[triOneIndex7][1], posR[2] + marchingDelta[triOneIndex7][2],
										  posR[0] + marchingDelta[triTwoIndex7][0], posR[1] + marchingDelta[triTwoIndex7][1], posR[2] + marchingDelta[triTwoIndex7][2], 
										 posR[0] + marchingDelta[triThreeIndex7][0], posR[1] + marchingDelta[triThreeIndex7][1], posR[2] + marchingDelta[triThreeIndex7][2], 
										 posR[0] + marchingDelta[triThreeIndex7][0], posR[1] + marchingDelta[triThreeIndex7][1], posR[2] + marchingDelta[triThreeIndex7][2], 
										 posR[0] + marchingDelta[triTwoIndex7][0], posR[1] + marchingDelta[triTwoIndex7][1], posR[2] + marchingDelta[triTwoIndex7][2], 
										 posR[0] + marchingDelta[triOneIndex7][0], posR[1] + marchingDelta[triOneIndex7][1], posR[2] + marchingDelta[triOneIndex7][2]};

					glVertexPointer(3, GL_FLOAT, 0, vertices);

					// draw a cube
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}

			unsigned edgeIndex8 = 0;
			for ( unsigned a = 56; a < 64; ++a) {
				edgeIndex8 = edgeIndex8 << 1;
				map<string, bool>::iterator isInside = inside.find(m_vVecState[i] -> MCLocs[a]);
				if (isInside->second) {
					edgeIndex8 += 1;
				}
			}

			// cout << "2:" << edgeIndex8 << endl;

			for (unsigned a = 0; a < 16; a+=3) {
				if(triTable[edgeIndex8][a] == -1) {
					break;
				} else {
					unsigned triOneIndex8 = triTable[edgeIndex8][a];
					unsigned triTwoIndex8 = triTable[edgeIndex8][a+1];
					unsigned triThreeIndex8 = triTable[edgeIndex8][a+2];
					GLfloat vertices[] = { posR[0] + marchingDelta[triOneIndex8][0], posR[1] + marchingDelta[triOneIndex8][1], posR[2] + marchingDelta[triOneIndex8][2],
										  posR[0] + marchingDelta[triTwoIndex8][0], posR[1] + marchingDelta[triTwoIndex8][1], posR[2] + marchingDelta[triTwoIndex8][2], 
										 posR[0] + marchingDelta[triThreeIndex8][0], posR[1] + marchingDelta[triThreeIndex8][1], posR[2] + marchingDelta[triThreeIndex8][2], 
										 posR[0] + marchingDelta[triThreeIndex8][0], posR[1] + marchingDelta[triThreeIndex8][1], posR[2] + marchingDelta[triThreeIndex8][2], 
										 posR[0] + marchingDelta[triTwoIndex8][0], posR[1] + marchingDelta[triTwoIndex8][1], posR[2] + marchingDelta[triTwoIndex8][2], 
										 posR[0] + marchingDelta[triOneIndex8][0], posR[1] + marchingDelta[triOneIndex8][1], posR[2] + marchingDelta[triOneIndex8][2]};

					glVertexPointer(3, GL_FLOAT, 0, vertices);

					// draw a cube
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}
		

			// deactivate vertex arrays after drawing
			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}
	drawbox(0,-2,0);
	
}

void ParticleSystem::draw3(){
	GLfloat floorColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floorColor);
    glPushMatrix();
	float epsilon = 0.1;
    glEnable (GL_BLEND);
	float alpha = 1;
    glBlendFunc (alpha, 1.0-alpha);
    glTranslatef(0.0f,-5.0f,0.0f);
    glScaled(50.0f,0.01f,50.0f);
    glutSolidCube(1);
    glPopMatrix();
	
}

void ParticleSystem::drawbox(float x, float y, float z){
	
    GLfloat floorColor[] = {0.5f, 0.5f, .5f, 0.5f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floorColor);
    glPushMatrix();
    float epsilon = .1;
    //glEnable (GL_BLEND);
    //float alpha = .9;
    //glBlendFunc (alpha, 1.0-alpha);
    glTranslatef(2.5f+x,.5f+y,.5f+z);
    glScaled(0.01f,1.0f,1.0f);
    glutSolidCube(1);
    glPopMatrix();
	
    glPushMatrix();
    GLfloat wallColor0[] = {1.f, 0.f, .0f, 1.f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, wallColor0);
    glTranslatef(-2.5f+x,.5f+y,.5f+z);
    glScaled(0.01f,1.0f,1.0f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    GLfloat wallColor1[] = {0.5f, 1.0f, .0f, 1.f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, wallColor1);
    glTranslatef(.25f+x,0.0f+y,.5f+z);
    glScaled(5.51f,.01f,4.0f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    GLfloat wallColor2[] = {1.f, 1.0f, 1.0f, 1.f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, wallColor2);
    glTranslatef(.25f+x,0.5f+y,.0f+z);
    glScaled(5.51f,1.0f,.01f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    GLfloat wallColor3[] = {0.f, 1.0f, 1.0f, 1.f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, wallColor3);
    glTranslatef(.25f+x,.5f+y,1.0f+z);
    glScaled(5.51f,1.01f,.01f);
    glutSolidCube(1);
    glPopMatrix();

	for (int i = 0; i < m_vVecState.size(); i++){
		Vector3f location = m_vVecState[i]->position.x();
		if (m_vVecState[i]->position.y() < .0f+y && -2.0f+x< m_vVecState[i]->position.x()< 2.0f+x && -2.5f + z + epsilon<= m_vVecState[i]->position.z()<= 2.5f+z-epsilon){
			//cout << "I am in the right spot..." << endl;
			m_vVecState[i]->position.y() = y+epsilon; 
			m_vVecState[i]->velocity = Vector3f(0,0,0);
			/*if ( .5f +x > m_vVecState[i]->position.x()){
				m_vVecState[i]->position.x() = .5+x+epsilon;
			}
			else if ( 1.0f +x < m_vVecState[i]->position.x()){
				m_vVecState[i]->position.x() = 1.+x-epsilon;
			}
			else {
				m_vVecState[i]->position.x() = location.x();	
			} 


			if (m_vVecState[i]->position.z() < z){
				m_vVecState[i]->position.z() = z+epsilon;
			}
			else if (m_vVecState[i]->position.z() > 1.0f+z){
				m_vVecState[i]->position.z() = 1.0f+z-epsilon;
			}
			else{
				m_vVecState[i]->position.z() = location.z();
			}
			*/

			//m_vVecState[i]->position.x() = location.x();
			//m_vVecState[i]->position.z() = location.z();
			//m_vVecState[i]->velocity = Vector3f(0,0,0);
		}		
	}

}
void ParticleSystem::draw_scatter(){
	
	for (unsigned i = 0; i < m_vVecState.size(); i++) {
		///vector<Particle *> neighbors = getNeighbors(this, state[i]);
		// cout << neighbors.size() << endl;
		for (unsigned j = 0; j < m_vVecState.size();++j){
		    Vector3f locBall = m_vVecState[j]->position;
			float radBall = .05f;
			float epsilon = 0.1f;
			if (m_vVecState[i] != m_vVecState[j]){
		    	if ((m_vVecState[i]->position - locBall).abs() <= (radBall + epsilon)){

					m_vVecState[i]->position = (locBall + (radBall + epsilon) * (m_vVecState[i]->position - locBall).normalized());
			
				}
		    }
		}
	}
}
