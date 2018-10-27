#include "Boid.h"
#include "Flock.h"
#include <math.h>
using namespace std;

// =============================================== //
// ======== Flock Functions from Flock.h ========= //
// =============================================== //

int Flock::getSize()
{
	return flock.size();
}

Boid Flock::getBoid(int i)
{
	return flock[i];
}

void Flock::addBoid(Boid b)
{
	flock.push_back(b);
}

// Runs the run function for every boid in the flock checking against the flock
// itself. Which in turn applies all the rules to the flock.
void Flock::flocking() 
{
	for (int i = 0; i < flock.size(); i++)
	{
		flock[i].run(flock);
	}
}
// Runs the swarm function for every boid in the flock checking against the flock
// itself. Which in turn applies all the rules to the flock.
void Flock::swarming()
{
	for (int i = 0; i < flock.size(); i++)
	{
		flock[i].swarm(flock);
	}
}

//Runs the formation algorithm which updaets the positions based on the position of the leader and 
//relative position in the formation pattern of the other NPCs.
void Flock::cFormation(Player& fLeader)
{
	float pi = 3.141592653589793;
	int fSize = flock.size();
	int formationNumber = 0;
	int npcRadius = 20;
	
	//if the varied angle reaches max for circle reset to 0
	if (variedCFormationAngle >= 2 * pi)
	{
		variedCFormationAngle = 0;
	}
	//add to the varied angle to make the formation spin
	variedCFormationAngle+= 0.0001f;
	for (int i = 0; i < fSize; i++)
	{
		//if we reach the 4th enemy, increase the radius and reset formation number
		if (i % 4 == 0)
		{
			formationNumber = 0;
			npcRadius += 10;
		}
		Pvector	sub(0, 0);
		Pvector sum(0,0);
		int closeEnough = 10;
		float angleAroundCircle = 0.0;
		Pvector targetSlot(0, 0);
		Player target = fLeader; // Our designated leader
		
			angleAroundCircle = (float) formationNumber / (4);
			angleAroundCircle = angleAroundCircle * pi * 2 + variedCFormationAngle;
			float radius = npcRadius / sin(pi / (fSize));

			targetSlot = Pvector(target.getPos().x, target.getPos().y);
			targetSlot.x = targetSlot.x + radius * cos(angleAroundCircle);
			targetSlot.y = targetSlot.y + radius * sin(angleAroundCircle);
			sub = sub.subTwoVector(targetSlot, flock[i].location);
			float D = sub.magnitude();
			if (D > closeEnough)
			{
				sum = sub;
				sum.normalize();
				sum.mulScalar(flock[i].maxSpeed);
				flock[i].applyForce(sum);
				flock[i].update();
				flock[i].borders();
			}
			else
			{
				flock[i].velocity = Pvector(target.getVel().x, target.getVel().y); //Match the leader's velocity if we are close enough
			}
			formationNumber++;
		}
	
}