#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	void addActor(Actor* newActor);
	bool hasCollided(Actor* toCheck, Actor*& toChange);
	bool hasCollidedwithBlaster(Actor* toCheck, NachenBlaster*& blaster);
	NachenBlaster* getBlaster();
	void killAlien();
	void dieAlien();


private:
	std::vector<Actor*> actors;
	NachenBlaster* nachenBlaster;
	int numberAliensDead;
	int numberAliensAlive;
};

#endif // STUDENTWORLD_H_
