#include "aBehaviors.h"

#include <math.h>
#include "GL/glew.h"
#include "GL/glut.h"

// Base Behavior
///////////////////////////////////////////////////////////////////////////////
Behavior::Behavior()
{
}

Behavior::Behavior( char* name) 
{
	m_name = name;
	m_pTarget = NULL;
}

Behavior::Behavior( Behavior& orig) 
{
	m_name = orig.m_name;
	m_pTarget = NULL;
}

string& Behavior::GetName() 
{
    return m_name;
}

// Behaviors derived from Behavior
//----------------------------------------------------------------------------//
// Seek behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Seek returns a maximum velocity towards the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position


Seek::Seek( AJoint* target) 
{
	m_name = "seek";
	m_pTarget = target;

}

Seek::Seek( Seek& orig) 
{
	m_name = "seek";
	m_pTarget = orig.m_pTarget;
}


Seek::~Seek()
{
}

vec3 Seek::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TEST: add your code here to compute Vdesired
	vec3 desired_offset = targetPos-actorPos;
	Vdesired = (desired_offset/desired_offset.Length())*actor->gMaxSpeed;

	return Vdesired;
}


// Flee behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Flee calculates a a maximum velocity away from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position

Flee::Flee( AJoint* target) 
{
	m_name = "flee";
	m_pTarget = target;
}

Flee::Flee( Flee& orig) 
{
	m_name = "flee";
	m_pTarget = orig.m_pTarget;
}

Flee::~Flee()
{
}

vec3 Flee::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TEST: add your code here to compute Vdesired
	vec3 desired_offset = actorPos-targetPos;
	Vdesired = (desired_offset/desired_offset.Length())*actor->gMaxSpeed;

	return Vdesired;

}

// Arrival behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// the actors distance from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Arrival strength is in BehavioralController::KArrival


Arrival::Arrival( AJoint* target) 
{
	m_name = "arrival";
	m_pTarget = target;
}

Arrival::Arrival( Arrival& orig) 
{
	m_name = "arrival";
	m_pTarget = orig.m_pTarget;
}

Arrival::~Arrival()
{
}

vec3 Arrival::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TEST: add your code here to compute Vdesired
	vec3 target_offset = targetPos-actorPos;
	float ramped_speed = actor->gMaxSpeed*(target_offset.Length()/ 1000.0);
	float clipped_speed = ramped_speed > actor->gMaxSpeed ? actor->gMaxSpeed : ramped_speed;
	Vdesired = (clipped_speed/target_offset.Length())*target_offset;

	return Vdesired;
}


// Departure behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// 1/(actor distance) from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Departure strength is in BehavioralController::KDeparture

Departure::Departure(AJoint* target) 
{
	m_name = "departure";
	m_pTarget = target;
}

Departure::Departure( Departure& orig) 
{
	m_name = "departure";
	m_pTarget = orig.m_pTarget;
}

Departure::~Departure()
{
}

vec3 Departure::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TEST: add your code here to compute Vdesired
	vec3 target_offset = targetPos-actorPos;
	Vdesired = actor->KDeparture*(-1.0*target_offset/(target_offset.Length()*target_offset.Length()));

	return Vdesired;
}


// Avoid behavior
///////////////////////////////////////////////////////////////////////////////
//  For the given the actor, return a desired velocity in world coordinates
//  If an actor is near an obstacle, avoid adds a normal response velocity to the 
//  the desired velocity vector computed using arrival
//  Agent bounding sphere radius is in BehavioralController::radius
//  Avoidance parameters are  BehavioralController::TAvoid and BehavioralController::KAvoid

Avoid::Avoid(AJoint* target, vector<Obstacle>* obstacles) 
{
	m_name = "avoid";
	m_pTarget = target;
	mObstacles = obstacles;
}

Avoid::Avoid( Avoid& orig) 
{
	m_name = "avoid";
	m_pTarget = orig.m_pTarget;
	mObstacles = orig.mObstacles;
}

Avoid::~Avoid()
{
}

vec3 Avoid::calcDesiredVel( BehaviorController* actor)
{

	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	m_actorPos = actor->getPosition();
	m_actorVel = actor->getVelocity();

	//TEST: add your code here
	vec3 Varrival(0, 0, 0);
	// Step 1. compute initial value for Vdesired = Varrival so agent moves toward target
	vec3 target_offset = m_pTarget->getGlobalTranslation() - m_actorPos;
	float ramped_speed = actor->gMaxSpeed*(target_offset.Length() / 1000.0);
	float clipped_speed = ramped_speed > actor->gMaxSpeed ? actor->gMaxSpeed : ramped_speed;
	Varrival = (clipped_speed / target_offset.Length())*target_offset;
	Vdesired = Varrival;


	vec3 Vavoid(0, 0, 0);
	//TEST: add your code here to compute Vavoid 


	// Step 2. compute Lb
	//TEST: add your code here
	double Lb = actor->TAvoid*actor->getVelocity().Length();


	// Step 3. find closest obstacle 
	//TEST: add your code here
	double obdist = -10.0;
	Obstacle obs;
	for (int i = 0; i < mObstacles->size(); i++) {
		double currdist = (mObstacles->at(i).m_Center.getGlobalTranslation()-m_actorPos).Length();
		if (currdist < obdist || obdist < 0.0) {
			obdist = currdist;
			obs = mObstacles->at(i);
		}
	}


	// Step 4. determine whether agent will collide with closest obstacle (only consider obstacles in front of agent)
	//TEST: add your code here
	double theta = actor->getOrientation()[1];
	double distx = cos(theta)*obdist;
	double distz = sin(theta)*obdist;
	bool avoid = distx <= Lb + obs.m_Radius + actor->gAgentRadius && distz <= obs.m_Radius + actor->gAgentRadius;


	// Step 5.  if potential collision detected, compute Vavoid and set Vdesired = Varrival + Vavoid
	//TEST: add your code here
	Vavoid = -1.0*(obs.m_Center.getLocalTranslation() - actor->getPosition());
	Vdesired = Varrival + Vavoid;


	return Vdesired;
	
}

void Avoid::display( BehaviorController* actor)
{
	//  Draw Debug info
	vec3 angle = actor->getOrientation();
	vec3 vel = actor->getVelocity();
	vec3 dir = vec3(cos(angle[1]), 0, sin(angle[1]));
	vec3 probe = dir * (vel.Length()/BehaviorController::gMaxSpeed)*BehaviorController::TAvoid;
	
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_obstaclePos[0], m_obstaclePos[1], m_obstaclePos[2]);
	glColor3f(0, 1, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_actorPos[0] + probe[0], m_actorPos[1] + probe[1], m_actorPos[2] + probe[2]);
	glEnd();
}


// Wander Behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Wander returns a desired velocity vector whose direction changes at randomly from frame to frame
// Wander strength is in BehavioralController::KWander

Wander::Wander() 
{
	m_name = "wander";
	m_Wander = vec3(1.0, 0.0, 0.0);
}

Wander::Wander( Wander& orig) 
{
	m_name = "wander";
	m_Wander = orig.m_Wander;
}

Wander::~Wander()
{
}

vec3 Wander::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();

	// compute Vdesired = Vwander

	// Step. 1 find a random direction
	//TEST: add your code here
	vec3 dir = vec3(rand()%100*1.0, rand() % 100 * 1.0, rand() % 100 * 1.0);
	dir.Normalize();


	// Step2. scale it with a noise factor
	//TEST: add your code here
	dir *= actor->KNoise;


	// Step3. change the current Vwander  to point to a random direction
	//TEST: add your code here
	m_Wander = dir;



	// Step4. scale the new wander velocity vector and add it to the nominal velocity
	//TEST: add your code here
	Vdesired = m_Wander + actor->getVelocity();


	return Vdesired;
}


// Alignment behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity vector in world coordinates
// Alignment returns the average velocity of all active agents in the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Alignment parameters are in BehavioralController::RNeighborhood and BehavioralController::KAlign


Alignment::Alignment(AJoint* target, vector<AActor>* agents) 
{
	m_name = "alignment";
	m_pAgentList = agents;
	m_pTarget = target;
}



Alignment::Alignment( Alignment& orig) 
{
	m_name = orig.m_name;
	m_pAgentList = orig.m_pAgentList;
	m_pTarget = orig.m_pTarget;

}

Alignment::~Alignment()
{
}

vec3 Alignment::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_pAgentList;
	

	// compute Vdesired 
	
	// Step 1. compute value of Vdesired for fist agent (i.e. m_AgentList[0]) using an arrival behavior so it moves towards the target
	 
	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	//TEST: add your code here
	if (actor == leader) {
		vec3 target_offset = targetPos - actorPos;
		float ramped_speed = actor->gMaxSpeed*(target_offset.Length() / 1000.0);
		float clipped_speed = ramped_speed > actor->gMaxSpeed ? actor->gMaxSpeed : ramped_speed;
		Vdesired = (clipped_speed / target_offset.Length())*target_offset;
	}
	else {
		int count = 0;
		for (int i = 1; i < agentList.size(); i++) {
			if (Distance(agentList[i].getBehaviorController()->getPosition(), actor->getPosition()) < actor->gKNeighborhood) {
				Vdesired += actor->KAlignment*agentList[i].getBehaviorController()->getVelocity();
				count++;
			}
		}
		Vdesired /= count;
	}


	// Step 2. if not first agent compute Valign as usual
	//TEST: add your code here
	
	
	return Vdesired;
}

// Separation behavior
///////////////////////////////////////////////////////////////////////////////
// For the given te actor, return a desired velocity vector in world coordinates
// Separation tries to maintain a constant distance between all agents
// within the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Separation settings are in BehavioralController::RNeighborhood and BehavioralController::KSeperate

 

Separation::Separation( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "separation";
	m_AgentList = agents;
	m_pTarget = target;
}

Separation::~Separation()
{
}

Separation::Separation( Separation& orig) 
{
	m_name = "separation";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

vec3 Separation::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vseparate
	// TEST: add your code here to compute Vdesired 
	int count = 0;
	for (int i = 1; i < agentList.size(); i++) {
		if (Distance(agentList[i].getBehaviorController()->getPosition(), actor->getPosition()) < actor->gKNeighborhood) {
			Vdesired += actorPos - agentList[i].getBehaviorController()->getPosition();
			count++;
		}
	}
	Vdesired *= actor->KSeparation;
	Vdesired /= count;


	if (Vdesired.Length() < 5.0)
		Vdesired = 0.0;
	
	return Vdesired;
}


// Cohesion behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// Cohesion moves actors towards the center of the group of agents in the neighborhood
//  agents[i] gives the pointer to the ith agent in the environment
//  Cohesion parameters are in BehavioralController::RNeighborhood and BehavioralController::KCohesion


Cohesion::Cohesion( vector<AActor>* agents) 
{
	m_name = "cohesion";
	m_AgentList = agents;
}

Cohesion::Cohesion( Cohesion& orig) 
{
	m_name = "cohesion";
	m_AgentList = orig.m_AgentList;
}

Cohesion::~Cohesion()
{
}

vec3 Cohesion::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vcohesion
	// TEST: add your code here 
	int count = 0;
	vec3 xcm;
	for (int i = 1; i < agentList.size(); i++) {
		if (Distance(agentList[i].getBehaviorController()->getPosition(), actor->getPosition()) < actor->gKNeighborhood) {
			xcm += agentList[i].getBehaviorController()->getPosition();
			count++;
		}
	}
	xcm /= count;
	
	Vdesired = actor->KCohesion*xcm - actorPos;


	return Vdesired;
}

// Flocking behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector  in world coordinates
// Flocking combines separation, cohesion, and alignment behaviors
//  Utilize the Separation, Cohesion and Alignment behaviors to determine the desired velocity vector


Flocking::Flocking( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "flocking";
	m_AgentList = agents;
	m_pTarget = target;
}

Flocking::Flocking( Flocking& orig) 
{
	m_name = "flocking";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Flocking::~Flocking()
{
}

vec3 Flocking::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// compute Vdesired = Vflocking
	// TEST: add your code here 
	int count = 0;
	vec3 Vsep;
	for (int i = 1; i < agentList.size(); i++) {
		if (Distance(agentList[i].getBehaviorController()->getPosition(), actor->getPosition()) < actor->gKNeighborhood) {
			Vsep += actorPos - agentList[i].getBehaviorController()->getPosition();
			count++;
		}
	}
	Vsep *= actor->KSeparation;
	Vsep /= count;

	if (Vsep.Length() < 5.0)
		Vsep = 0.0;

	count = 0;
	vec3 xcm;
	for (int i = 1; i < agentList.size(); i++) {
		if (Distance(agentList[i].getBehaviorController()->getPosition(), actor->getPosition()) < actor->gKNeighborhood) {
			xcm += agentList[i].getBehaviorController()->getPosition();
			count++;
		}
	}
	xcm /= count;

	vec3 VCoh = actor->KCohesion*xcm - actorPos;

	BehaviorController* leader = agentList[0].getBehaviorController();
	vec3 VAlign;
	if (actor == leader) {
		vec3 target_offset = m_pTarget->getGlobalTranslation() - actorPos;
		float ramped_speed = actor->gMaxSpeed*(target_offset.Length() / 1000.0);
		float clipped_speed = ramped_speed > actor->gMaxSpeed ? actor->gMaxSpeed : ramped_speed;
		VAlign = (clipped_speed / target_offset.Length())*target_offset;
	}
	else {
		int count = 0;
		for (int i = 1; i < agentList.size(); i++) {
			if (Distance(agentList[i].getBehaviorController()->getPosition(), actor->getPosition()) < actor->gKNeighborhood) {
				VAlign += actor->KAlignment*agentList[i].getBehaviorController()->getVelocity();
				count++;
			}
		}
		VAlign /= count;
	}

	double CSeparation = 2.0;
	double CAlign = 2.0;
	double CCoh = 3.0;
	Vdesired = CSeparation*Vsep + CAlign*VAlign + CCoh*VCoh;


	return Vdesired;
}

//	Leader behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// If the agent is the leader, move towards the target; otherwise, 
// follow the leader at a set distance behind the leader without getting to close together
//  Utilize Separation and Arrival behaviors to determine the desired velocity vector
//  You need to find the leader, who is always agents[0]

Leader::Leader( AJoint* target, vector<AActor>* agents) 
{
	m_name = "leader";
	m_AgentList = agents;
	m_pTarget = target;
}

Leader::Leader( Leader& orig) 
{
	m_name = "leader";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Leader::~Leader()
{
}

vec3 Leader::calcDesiredVel( BehaviorController* actor)
{
	
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// TEST: compute Vdesired  = Vleader
	// followers should stay directly behind leader at a distance of -200 along the local z-axis

	float CSeparation = 4.0;  float CArrival = 2.0;

	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	mat3 Rmat = leader->getGuide().getLocalRotation();  // is rotattion matrix of lead agent

	vec3 target_offset = leader->getPosition() - actorPos;
	float ramped_speed = actor->gMaxSpeed*(target_offset.Length() / 1000.0);
	float clipped_speed = ramped_speed > actor->gMaxSpeed ? actor->gMaxSpeed : ramped_speed;
	vec3 Varrival = (clipped_speed / target_offset.Length())*target_offset;

	int count = 0;
	vec3 Vsep;
	for (int i = 1; i < agentList.size(); i++) {
		if (Distance(agentList[i].getBehaviorController()->getPosition(), actor->getPosition()) < actor->gKNeighborhood) {
			Vsep += actorPos - agentList[i].getBehaviorController()->getPosition();
			count++;
		}
	}
	Vsep *= actor->KSeparation;
	Vsep /= count;

	if (Vsep.Length() < 5.0)
		Vsep = 0.0;

	Vdesired = CSeparation*Vsep + CArrival*Varrival;

	return Vdesired;





	return Vdesired;
}

///////////////////////////////////////////////////////////////////////////////

