#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* theWorld);
	virtual void doSomething() = 0;
	void setDead(bool status);
	bool getDead();
	StudentWorld* getWorld();
	virtual bool isAlien();
	virtual bool isProjectile();
	virtual void takeDamage(int damage);
	double gethealth();
	void sethealth(double health);

private:
	double hp;
	bool isDead;
	StudentWorld* myWorld;
};


class NachenBlaster : public Actor {
public:
	NachenBlaster(StudentWorld* world);
	virtual void doSomething();
	void incTorpedoes(int incr);
	int getPercentCabbages();
	int getNumTorpedoes();

private:
	int cabbageEnergy;
	int numTorpedoes;
};


class Star : public Actor {
public:
	Star(double startX, double startY, StudentWorld* world);
	virtual void doSomething();
};

class Explosion : public Actor {
public:
	Explosion(double startX, double startY, StudentWorld* world);
	virtual void doSomething();

private:
	int numTicks;
};

class Cabbage : public Actor {
public:
	Cabbage(double startX, double startY, StudentWorld* world);
	virtual void doSomething();
	virtual bool isProjectile();
};

class Torpedo : public Actor {
public:
	Torpedo(double startX, double startY, StudentWorld* world, bool isEnemy);
	virtual void doSomething();

private:
	bool enemy;
};

class Turnip : public Actor {
public:
	Turnip(double startX, double startY, StudentWorld* world);
	virtual void doSomething();
};

class Alien : public Actor {
public:
	Alien(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* theWorld);
	virtual bool isAlien();
	void setPathLength(int lgt);
	int getPathLength();
	void setPathDir(int direc);
	int getPathDir();
	void setTravelSpeed(double spd);
	double getTravelSpeed();
	void collideWithBlaster(NachenBlaster* blaster, int damage, int score);
	void getKilled(int score);
	void movePath();

private:
	int flightplength;
	int flightpdir;
	double travelspeed;
};

class Smallgon : public Alien {
public:
	Smallgon(double startX, double startY, StudentWorld* world);
	virtual void doSomething();
};

class Smoregon : public Alien {
public:
	Smoregon(double startX, double startY, StudentWorld* world);
	virtual void doSomething();
};

class Snagglegon : public Alien {
public:
	Snagglegon(double startX, double startY, StudentWorld* world);
	virtual void doSomething();
};

class Goodie : public Actor {
public:
	Goodie(int imageID, double startX, double startY, StudentWorld* theWorld);
	void getGoodie();
};

class ExtraLifeGoodie : public Goodie {
public:
	ExtraLifeGoodie(double startX, double startY, StudentWorld* world);
	virtual void doSomething();
};

class RepairGoodie : public Goodie {
public:
	RepairGoodie(double startX, double startY, StudentWorld* world);
	virtual void doSomething();
};

class FlatulenceTorpedoGoodie : public Goodie {
public:
	FlatulenceTorpedoGoodie(double startX, double startY, StudentWorld* world);
	virtual void doSomething();
};

#endif // ACTOR_H_
