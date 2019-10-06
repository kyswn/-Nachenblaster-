#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "GraphObject.h"
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp



	Actor::Actor(int imageID, double startX, double startY, Direction dir , double size , unsigned int depth , StudentWorld* ptr )
		: GraphObject(imageID, startX, startY, dir, size, depth)
	{
		mysw = ptr;
		dead = false;
	}
	StudentWorld* Actor:: getWorld()
	{
		return mysw;
	}
	
	bool Actor::isdead()
	{
		return dead;
	}
	void Actor::die()
	{
		dead = true;
	}


	void Star:: doSomething()
	{
		moveTo(getX() - 1, getY());
		if (getX() <= -1)
			die();
	}


	NachenBlaster::NachenBlaster(StudentWorld* ptr) :DamageableObject(IID_NACHENBLASTER, 0, 128, 0, 1, 0, ptr,50)
	{
		cabbageenergy = 30;
		ftorpedo = 0;
	}
	void NachenBlaster::doSomething()
	{
		if (isdead())
			return;

		int ch;
		if (getWorld()->getKey(ch))
		{
			// user hit a key during this tick!
			switch (ch)
			{
			case KEY_PRESS_LEFT:
				//x value < 0 or >= VIEW_WIDTH or a y value < 0
				//	or >= VIEW_HEIGHT
				if (getX() - 6 >= 0 && getX() - 6<VIEW_WIDTH)
					moveTo(getX() - 6, getY());
				break;
			case KEY_PRESS_RIGHT:
				if (getX() + 6 >= 0 && getX() + 6 < VIEW_WIDTH)
					moveTo(getX() + 6, getY());
				break;
			case KEY_PRESS_UP:
				if (getY() + 6 >= 0 && getY() + 6<VIEW_HEIGHT)
					moveTo(getX(), getY() + 6);
				break;
			case KEY_PRESS_DOWN:
				if (getY() - 6 >= 0 && getY() - 6<VIEW_HEIGHT)
					moveTo(getX(), getY() - 6);
				break;
			case KEY_PRESS_SPACE:
				if (cabbageenergy >= 5)
				{
					cabbageenergy -= 5;
					Cabbage* a = new Cabbage(getWorld(), getX() + 12, getY());
					getWorld()->addActor(a);
					getWorld()->playSound(SOUND_PLAYER_SHOOT);
				}
				break;
			case KEY_PRESS_TAB:
				if (ftorpedo >= 1)
				{
					ftorpedo -= 1;
					PlayerLaunchedTorpedo *a = new PlayerLaunchedTorpedo(getWorld(), getX() + 1, getY());
					getWorld()->addActor(a);
					getWorld()->playSound(SOUND_TORPEDO);
					break;
				}


			}

		}
		if (cabbageenergy < 30) cabbageenergy++;

	}


	void NachenBlaster::sufferDamage(double amt, int cause)
	{
		increaseHitPoints(-amt);
		if (hitPoints() <= 0)
			die();
		if (cause == HIT_BY_PROJECTILE)
			getWorld()->playSound(SOUND_BLAST);

	}
	
	void Explosion::doSomething()
	{
		if (lifetime == 4)
		{
			die();
			return;
		}
		setSize(1.5*getSize());
		lifetime++;
	}

	void Goodie::doSomething()
	{
		if (isdead()) return;
		if (getX() < 0 || getX() > VIEW_WIDTH - 1 || getY() < 0 || getY() > VIEW_HEIGHT - 1)
		{
			die();
			return;
		}
		if (getWorld()->getCollidingPlayer(this) != nullptr)
		{
			doSpecialthing(getWorld()->getCollidingPlayer(this));
			return;
		}
		moveTo(getX() - 0.75, getY() - 0.75);
		if (getWorld()->getCollidingPlayer(this) != nullptr)
		{
			doSpecialthing(getWorld()->getCollidingPlayer(this));
			return;
		}

	}

	void ExtraLifeGoodie::doSpecialthing(NachenBlaster* ptr)
	{
		getWorld()->increaseScore(100);
		die();
		getWorld()->playSound(SOUND_GOODIE);
		getWorld()->incLives();
		return;
	}

	void RepairGoodie::doSpecialthing(NachenBlaster* ptr)
	{
		getWorld()->increaseScore(100);
		die();
		getWorld()->playSound(SOUND_GOODIE);
		if (ptr->hitPoints() + 10 <= 50)
			ptr->increaseHitPoints(10);
		else (ptr->increaseHitPoints(50 - ptr->hitPoints()));
		return;
	}

	void TorpedoGoodie::doSpecialthing(NachenBlaster* ptr)
	{
		getWorld()->increaseScore(100);
		die();
		getWorld()->playSound(SOUND_GOODIE);
		ptr->increaseTorpedoes(5);
		return;
	}

	void Projectile::doSomething()
	{
		if (isdead()) return;
		if (getX() < 0 || getX() > VIEW_WIDTH - 1 || getY() < 0 || getY() > VIEW_HEIGHT - 1)
		{
			die();
			return;
		}
		if (dorealthing())
			return;
		moveTo(getX() + m_deltax, getY());
		if (m_rotate == true) setDirection(getDirection() + 20);
		if (dorealthing())
			return;
	}

	bool Cabbage::dorealthing()
	{
		if (getWorld()->getOneCollidingAlien(this) == nullptr)return false;
		getWorld()->getOneCollidingAlien(this)->sufferDamage(2, HIT_BY_PROJECTILE);

		die();
		return true;
	}

	bool Turnip::dorealthing()
	{
		if (getWorld()->getCollidingPlayer(this) == nullptr)return false;
		getWorld()->getCollidingPlayer(this)->sufferDamage(2, HIT_BY_PROJECTILE);
		die();
		return true;
	}

	bool PlayerLaunchedTorpedo::dorealthing()
	{
		if (getWorld()->getOneCollidingAlien(this) == nullptr)return false;
		getWorld()->getOneCollidingAlien(this)->sufferDamage(8, 1);
		die();
		return true;
	}
	bool AlienLaunchedTorpedo::dorealthing()
	{
		if (getWorld()->getCollidingPlayer(this) == nullptr)return false;
		getWorld()->getCollidingPlayer(this)->sufferDamage(8, 1);
		die();
		return true;
	}

	void Alien::doSomething()
	{
		if (isdead())
		{
			
			return;
		}
		if (getX() < 0)
			{
				
				die();
				return;
			}
		if (damageCollidingPlayer(m_damageamount))
			{
			getWorld()->recordAlienDestroyed();
			die();
				getWorld()->increaseScore(m_scorevalue);
				getWorld()->playSound(SOUND_DEATH);
				Explosion* exp = new Explosion(getX(), getY(), getWorld());
				getWorld()->addActor(exp);
				if (type() == "Smoregon")
				{
					if (randInt(1, 3) == 3)
					{
						if (randInt(0, 1) == 0)
						{
							RepairGoodie* a = new RepairGoodie(getWorld(), getX(), getY());
							getWorld()->addActor(a);
						}
						else
						{
							TorpedoGoodie* b = new TorpedoGoodie(getWorld(), getX(), getY());
							getWorld()->addActor(b);
						}
					}
				}
				if (type() == "Snagglegon")
				{
					if (randInt(1, 6) == 1)
					{
						ExtraLifeGoodie* c = new ExtraLifeGoodie(getWorld(), getX(), getY());
						getWorld()->addActor(c);
					}
				}

			}
			if (dospecialthing()) return;
			if (damageCollidingPlayer(m_damageamount))
			{
				die();
				getWorld()->recordAlienDestroyed();
				getWorld()->increaseScore(m_scorevalue);
				getWorld()->playSound(SOUND_DEATH);
				Explosion* exp = new Explosion(getX(), getY(), getWorld());
				getWorld()->addActor(exp);
				if (type() == "Smoregon")
				{
					if (randInt(1, 3) == 3)
					{
						if (randInt(0, 1) == 0)
						{
							RepairGoodie* a = new RepairGoodie(getWorld(), getX(), getY());
							getWorld()->addActor(a);
						}
						else
						{
							TorpedoGoodie* b = new TorpedoGoodie(getWorld(), getX(), getY());
							getWorld()->addActor(b);
						}
					}
				}
				if (type() == "Snagglegon")
				{
					if (randInt(1, 6) == 1)
					{
						ExtraLifeGoodie* c = new ExtraLifeGoodie(getWorld(), getX(), getY());
						getWorld()->addActor(c);
					}
				}
			}

		}
	

	void Alien::sufferDamage(double amt, int cause)
	{
		if (cause == HIT_BY_PROJECTILE)
		{
			increaseHitPoints(-amt);
			if(hitPoints()<=0)
			{
				getWorld()->recordAlienDestroyed();
				getWorld()->increaseScore(m_scorevalue);
				die();
				getWorld()->playSound(SOUND_DEATH);
				Explosion* exp = new Explosion(getX(), getY(), getWorld());
				getWorld()->addActor(exp);
				if (type() == "Smoregon")
				{
					if (randInt(1, 3) == 3)
					{
						if (randInt(0, 1) == 0)
						{
							RepairGoodie* a = new RepairGoodie(getWorld(), getX(), getY());
							getWorld()->addActor(a);
						}
						else
						{
							TorpedoGoodie* b = new TorpedoGoodie(getWorld(), getX(), getY());
							getWorld()->addActor(b);
						}
					}
				}
				if (type() == "Snagglegon")
				{
					if (randInt(1, 6) == 1)
					{
						ExtraLifeGoodie* c = new ExtraLifeGoodie(getWorld(), getX(), getY());
						getWorld()->addActor(c);
					}
				}
			}
			else getWorld()->playSound(SOUND_BLAST);
	  }
	}

	bool Smallgon::dospecialthing()
	{
		if (m_flightlength == 0 || getY() < 0 || getY() >= VIEW_HEIGHT)
		{
			if (getY() >= VIEW_HEIGHT)
				setDeltaY(-1);
			if (getY() < 0)
				setDeltaY(1);
			if (m_flightlength == 0)
				setDeltaY(randInt(-1, 1));
			m_flightlength = randInt(1, 32);
		}
		if (getWorld()->playerInLineOfFire(this))
		{
			if (randInt(1, (20 / getWorld()->getLevel()) + 5) == 1)
			{
				Turnip* tu = new Turnip(getWorld(), getX() - 14, getY());
				getWorld()->addActor(tu);
				getWorld()->playSound(SOUND_ALIEN_SHOOT);
				return true;
			}
		}
		move();
		m_flightlength -= 1;
		return false;
	}

	bool Smoregon::dospecialthing()
	{
		if (m_flightlength == 0 || getY() < 0 || getY() >= VIEW_HEIGHT)
		{
			if (getY() >= VIEW_HEIGHT)
				setDeltaY(-1);
			if (getY() < 0)
				setDeltaY(1);
			if (m_flightlength == 0)
				setDeltaY(randInt(-1, 1));
			m_flightlength = randInt(1, 32);
		}
		if (getWorld()->playerInLineOfFire(this))
		{
			if (randInt(1, (20 / getWorld()->getLevel()) + 5) == 1)
			{
				Turnip* tu = new Turnip(getWorld(), getX() - 14, getY());
				getWorld()->addActor(tu);
				getWorld()->playSound(SOUND_ALIEN_SHOOT);
				return true;
			}
			if (randInt(1, (20 / getWorld()->getLevel()) + 5) == 1)
			{
				setDeltaY(0);
				m_flightlength = VIEW_WIDTH;
				setSpeed(5);
			}
		}
	
		move();
		m_flightlength -= 1;
		return false;
	}

	bool Snagglegon::dospecialthing()
	{
		if (getY() < 0 || getY() >= VIEW_HEIGHT)
		{
			if (getY() >= VIEW_HEIGHT)
				setDeltaY(-1);
			if (getY() < 0)
				setDeltaY(1);
		}
		if (getWorld()->playerInLineOfFire(this))
		{
			if (randInt(1, (15 / getWorld()->getLevel()) + 10) == 1)
			{
				AlienLaunchedTorpedo* tu = new AlienLaunchedTorpedo(getWorld(), getX() - 14, getY());
				getWorld()->addActor(tu);
				getWorld()->playSound(SOUND_TORPEDO);
				return true;
			}
		}
		move();
		return false;
	}


	bool Alien:: damageCollidingPlayer(double amt)
	{
		if (getWorld()->getCollidingPlayer(this) == nullptr) return false;
		getWorld()->getCollidingPlayer(this)->sufferDamage(amt, HIT_BY_SHIP);
		return true;
	}


