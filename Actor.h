#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
#include "StudentWorld.h"
using namespace std;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int HIT_BY_SHIP = 0;
const int HIT_BY_PROJECTILE = 1;
class StudentWorld;

class Actor :public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, Direction dir = 0, double size = 1.0, unsigned int depth = 0, StudentWorld* ptr = nullptr);
	virtual ~Actor() {}
	StudentWorld* getWorld();
	virtual void doSomething() = 0;
	virtual string type() = 0;
	bool isdead();
	void die();
	virtual void sufferDamage(double amt, int cause)
	{}
private:
	StudentWorld * mysw;
	bool dead;
};

class Star :public Actor
{
	
public:
	
	Star(double startX, double startY, StudentWorld* ptr) :Actor(IID_STAR, startX, startY, 0, randInt(5, 50) / 100.0, 3, ptr)
	{}
	virtual ~Star() {};
	virtual void doSomething();	
	string type()
	{
		return "Star";
	}

};

class Explosion :public Actor
{
public:
	Explosion(double startX, double startY, StudentWorld* ptr) :Actor(IID_EXPLOSION, startX, startY, 0, 1, 0, ptr)
	{
		lifetime = 0;
	}
	virtual ~Explosion() {}
	string type()
	{
		return "Explosion";
	}
	virtual void doSomething();
private:
	int lifetime;
	
};

class DamageableObject : public Actor
{
public:
	DamageableObject(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* ptr, double hitPoints)
		:Actor(imageID, startX, startY, dir, size, depth, ptr)
	{
		m_hitpoint = hitPoints;
	}

	virtual ~DamageableObject() {}
	// How many hit points does this actor have left?
	double hitPoints() const
	{
		return m_hitpoint;
	}

	// Increase this actor's hit points by amt.
	void increaseHitPoints(double amt)
	{
		m_hitpoint += amt;
	}

	// This actor suffers an amount of damage caused by being hit by either
	// a ship or a projectile (see constants above).
	virtual void sufferDamage(double amt, int cause)
	{}

private:
	double m_hitpoint;
};

class  NachenBlaster :public DamageableObject
{
public:
NachenBlaster(StudentWorld* ptr);
virtual	void doSomething();
virtual ~NachenBlaster() {}
string type()
{
	return "NachenBlaster";
}
void increaseTorpedoes(int amt)
{
	ftorpedo += amt;
}
int healthPct() const
{
	return hitPoints() * 100 / 50;
}
int cabbagePct() const
{
	return cabbageenergy*100/30;
}
int numTorpedoes() const
{
	return ftorpedo;
}
void sufferDamage(double amt, int cause);
private:
int cabbageenergy;
int ftorpedo;
};
/*class NachenBlaster : public DamageableObject
{
public:
	NachenBlaster(StudentWorld* w);
	virtual void doSomething();
	virtual void increaseHitPoints(double amt);
	virtual void sufferDamage(double amt, int cause);

	// Incease the number of torpedoes the player has by amt.
	void increaseTorpedoes(int amt);

	// Get the player's health percentage.
	int healthPct() const;

	// Get the player's cabbage energy percentage.
	int cabbagePct() const;

	// Get the number of torpedoes the player has.
	int numTorpedoes() const;
};*/

class Alien : public DamageableObject
{
public:
	Alien(StudentWorld* ptr, double startX, double startY, int imageID,
		double hitPoints, double damageAmt, double deltaX,
		double deltaY, double speed, unsigned int scoreValue) :DamageableObject(imageID, startX, startY, 0, 1.5, 1, ptr,hitPoints)
	{
		m_damageamount = damageAmt;
		m_deltaX = deltaX;
		m_deltaY = deltaY;
		m_speed = speed;
		m_scorevalue = scoreValue;
	}
	virtual ~Alien() {}
	virtual void sufferDamage(double amt, int cause);
	void doSomething();
	virtual bool dospecialthing() = 0;
	


	// Move the player by the current speed in the direction indicated
	// by the x and y deltas.
	void move()
	{
		moveTo(getX() + m_speed*m_deltaX, getY() + m_speed*m_deltaY);
	}

	// Set the player's y direction.
	void setDeltaY(double dy)
	{
		m_deltaY = dy;
	}

	// Set the player's speed.
	void setSpeed(double speed)
	{
		m_speed = speed;
	}

	// If this alien collided with the player, damage the player and return
	// true; otherwise, return false.
	virtual bool damageCollidingPlayer(double amt);


private:
	double m_damageamount;
	double m_deltaX;
	double m_deltaY;
	double m_speed;
	unsigned int m_scorevalue;
	

};

class Smallgon : public Alien
{
public:
	Smallgon(StudentWorld* w, double startX, double startY)
		:Alien(w, startX, startY, IID_SMALLGON, 5 * (1 + (w->getLevel() - 1) * .1), 5, -1, 0, 2, 250)
	{
		m_flightlength = 0;
	}
	virtual ~Smallgon() {}
	virtual bool dospecialthing();
	virtual string type() {
		return "Smallgon";
	}
private:
	double m_flightlength;
};

class Smoregon : public Alien
{
public:
	Smoregon(StudentWorld* w, double startX, double startY)
		:Alien(w,startX,startY, IID_SMOREGON, 5 * (1 + (w->getLevel()-1)* .1),5,-1,0,2,250)
	{
		m_flightlength = 0;
	}
	virtual ~Smoregon() {}
	virtual bool dospecialthing();
	virtual string type()
	{
		return "Smoregon";
	}
private:
	double m_flightlength;
};

class Snagglegon : public Alien
{
public:
	Snagglegon(StudentWorld* w, double startX, double startY)
		:Alien(w, startX, startY, IID_SNAGGLEGON, 10 * (1 + (w->getLevel() - 1) * .1), 15, -1, -1, 1.75, 1000)
	{}
	virtual ~Snagglegon() {}
	virtual bool dospecialthing();
	virtual string type()
	{
		return "Snagglegon";
	}
};

class Projectile : public Actor
{
public:
	Projectile(StudentWorld* w, double startX, double startY, int imageID,
		double damageAmt, double deltaX, bool rotates, int imageDir) :
		Actor(imageID, startX, startY, imageDir, 0.5, 1, w) 
	{
		m_rotate = rotates;
		m_damage = m_damage;
		m_deltax = deltaX;
		m_dir = imageDir;
	}
	virtual ~Projectile() {}
	void doSomething();
	virtual bool dorealthing() = 0;
private:
	bool m_rotate;
	double m_damage;
	double m_deltax;
	int m_dir;
	
};

class Cabbage : public Projectile
{
public:
	Cabbage(StudentWorld* w, double startX, double startY)
		:Projectile(w, startX, startY, IID_CABBAGE, 2, 8, true, 0)
	{}
	virtual bool dorealthing();
	virtual string type()
	{
		return "Cabbage";
	}
};

class Turnip : public Projectile
{
public:
	Turnip(StudentWorld* w, double startX, double startY) :
		Projectile(w, startX, startY, IID_TURNIP, 2, -6, true, 0)
	{}
	virtual bool dorealthing();
	virtual string type()
	{
		return "Trunip";
	}
};


class PlayerLaunchedTorpedo : public Projectile
{
public:
	PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY) :
		Projectile(w, startX, startY, IID_TORPEDO, 8, 8, false, 0)
	{}
	virtual bool dorealthing();
	virtual string type()
	{
		return "PlayerLaunchedTorpedo";
	}
};

class AlienLaunchedTorpedo : public Projectile
{
public:
	AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY) :
		Projectile(w, startX, startY, IID_TORPEDO, 8, -8, false, 180)
	{}
	virtual bool dorealthing();
	virtual string type()
	{
		return "AlienLaunchedTorpedo";
	}
};

class Goodie : public Actor
{
public:
	Goodie(StudentWorld* w, double startX, double startY, int imageID) :Actor(imageID, startX, startY, 0, 0.5, 1, w)
	{}
	virtual ~Goodie() {}
	void doSomething();
	virtual void doSpecialthing(NachenBlaster* ptr) = 0;
};

class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(StudentWorld* w, double startX, double startY) :Goodie(w, startX, startY, IID_LIFE_GOODIE)
	{}
	void doSpecialthing(NachenBlaster* ptr);
	virtual string type()
	{
		return "ExtraLifeGoodie";
	}
};

class RepairGoodie : public Goodie
{
public:
	RepairGoodie(StudentWorld* w, double startX, double startY) :Goodie(w, startX, startY, IID_REPAIR_GOODIE)
	{}
	void doSpecialthing(NachenBlaster* ptr);
	virtual string type()
	{
		return "RepairGoodie";
	}
};

class TorpedoGoodie : public Goodie
{
public:
	TorpedoGoodie(StudentWorld* w, double startX, double startY) :Goodie(w, startX, startY, IID_TORPEDO_GOODIE)
	{}
    void doSpecialthing(NachenBlaster* ptr);
	virtual string type()
	{
		return "TorpedoGoodie";
	}
};






#endif // ACTOR_H_
