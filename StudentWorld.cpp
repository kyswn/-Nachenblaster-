#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir)
{
	thenb = nullptr;
	aliendestroyed = 0;

}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

int StudentWorld::init()
{
	thenb = new NachenBlaster(this);
	for (int i = 0; i < 30; i++)
	{
		m_Actor.push_back(new Star(randInt(0, VIEW_WIDTH - 1), randInt(0, VIEW_HEIGHT - 1), this));
	}
	
	aliendestroyed = 0;
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	if (thenb->isdead())
	{
		delete thenb;
		thenb = nullptr;
		return GWSTATUS_PLAYER_DIED;
	}
	else thenb->doSomething();
	for (int i = 0; i < m_Actor.size(); i++)
	{
		if (!m_Actor[i]->isdead())
		{
			m_Actor[i]->doSomething();
			if (thenb->isdead())
			{
				decLives();
				delete thenb;
				thenb = nullptr;
				return GWSTATUS_PLAYER_DIED;
				
			}
			//int howmanymore = 6 + (4 * getLevel()) - aliendestroyed;
			if (howmanymore() <= 0)
			{
				return GWSTATUS_FINISHED_LEVEL;
			}

		}
	}
	vector<Actor*>::iterator  cfd;
	cfd = m_Actor.begin();
	while (cfd != m_Actor.end())
	{
		if ((*cfd)->isdead())
		{
			delete (*cfd);
			cfd = m_Actor.erase(cfd);
		}
		else cfd++;
	}

	ostringstream oss;
	
	oss << "Lives: " << getLives() << "  Health: " << thenb->healthPct() << "%  Score: " << getScore() <<
		"  Level: " << getLevel() << "  Cabbages: " << thenb->cabbagePct() << "%  Torpedoes: " << thenb->numTorpedoes();
	string a = oss.str();
	setGameStatText(a);
	int b = randInt(1, 15);
	if (b == 1)
		m_Actor.push_back(new Star(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this));
	int ma = 4 + (.5 * getLevel());
	int aliennumber = 0;
	for (int i = 0; i < m_Actor.size(); i++)
	{
		if (m_Actor[i]->type() == "Smallgon" || m_Actor[i]->type() == "Smoregon" || m_Actor[i]->type() == "Snagglegon")
			aliennumber++;
	}
	if (aliennumber < min(ma, howmanymore()))
	{
		int S1 = 60;
		int S2 = 20 + getLevel() * 5;
		int S3 = 5 + getLevel() * 10;
		int S = S1 + S2 + S3;
		
		int chance = randInt(1, S);
		if (chance <= S1)
		{
			Smallgon* s1 = new Smallgon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
			addActor(s1);
		}
		else if (chance > S1+S2)
		{
			Snagglegon* s2 = new Snagglegon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
			addActor(s2);
		}
		else
		{
			Smoregon* s3 = new Smoregon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
			addActor(s3);
		}
	}


	return GWSTATUS_CONTINUE_GAME;

	/*thenb->doSomething();
	
	vector<Actor*>::iterator  cfd;
	cfd = m_Actor.begin();
	while (cfd != m_Actor.end())
	{
		if ((*cfd)->isdead())
		{
			delete (*cfd);
			cfd = m_Actor.erase(cfd);
		}
		else cfd++;
	}
	vector<Actor*>::iterator  it;
	it = m_Actor.begin();
	while (it != m_Actor.end())
	{
		(*it)->doSomething();
		it++;
	}
	int a = randInt(1, 15);
	if (a == 1)
		m_Actor.push_back(new Star(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this));
	*/


	
}

void StudentWorld::cleanUp()
{
	if (thenb != nullptr)
	{
		delete thenb;
		thenb = nullptr;
	}
	vector<Actor*>::iterator  it;
	it = m_Actor.begin();
	while (it != m_Actor.end())
	{
		delete (*it);
		it++;
	}
	m_Actor.clear();

}

Actor* StudentWorld::getOneCollidingAlien(const Actor* a)  const
{
	for (int i = 0; i < m_Actor.size(); i++)
	{
		if(m_Actor[i]->type()=="Smallgon"|| m_Actor[i]->type() == "Smoregon"|| m_Actor[i]->type() == "Snagglegon")
		if (dist(a->getX(), a->getY(), m_Actor[i]->getX(), m_Actor[i]->getY()) < .75*(a->getRadius() + m_Actor[i]->getRadius()))
			return m_Actor[i];
	}
	return nullptr;
}

NachenBlaster* StudentWorld::getCollidingPlayer(const Actor* a) const
{
	if (dist(a->getX(), a->getY(), thenb->getX(), thenb->getY()) < .75 * (a->getRadius() + thenb->getRadius()))
		return thenb;
	else return nullptr;
}

bool StudentWorld::playerInLineOfFire(const Actor* a) const
{
	double difference = thenb->getY() - a->getY();
	if (thenb->getX() < a->getX() && difference >= -4 && difference <= 4)
		return true;
	return false;
}


void StudentWorld::addActor(Actor* a)
{
	m_Actor.push_back(a);
}

void StudentWorld::recordAlienDestroyed()
{
	aliendestroyed++;
}

