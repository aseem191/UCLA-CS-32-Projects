#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir)
{
}

int StudentWorld::init()
{
	nachenBlaster = new NachenBlaster(this);
	for (int i = 0; i < 30; i++) {
		actors.push_back(new Star(randInt(0, VIEW_WIDTH - 1), randInt(0, VIEW_HEIGHT - 1), this));
	}

	numberAliensDead = 0;
	numberAliensAlive = 0;

	ostringstream oss;
	oss << "Lives: " << getLives() << "  Health: " << nachenBlaster->gethealth() * 2 << "%" << "  Score: " << getScore() << "  Level: " << getLevel() << "  Cabbages: " << nachenBlaster->getPercentCabbages() << "%  Torpedoes: " << nachenBlaster->getNumTorpedoes();
	setGameStatText(oss.str());

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();
	nachenBlaster->doSomething();


	for (int i = 0; i < actors.size(); i++){
		actors[i]->doSomething();
	}

	if (nachenBlaster->getDead()) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (6 + (4 * getLevel()) - numberAliensDead == 0) {
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	vector<Actor*>::iterator it = actors.begin();

	while (it != actors.end()) {
		if ((*it)->getDead()) {
			delete *it;
			it = actors.erase(it);
		}
		else {
			it++;
		}
	}

	if (numberAliensAlive < min(1.0*6 + (4 * getLevel()) - numberAliensDead, 4 + (.5 * getLevel()))) {
		int chance = randInt(1, 60 + 20 + getLevel() * 5 + 5 + getLevel() * 10);
		if (chance <= 60) {
			actors.push_back(new Smallgon(VIEW_WIDTH - 1, 1.0*randInt(0, VIEW_HEIGHT - 1), this));
		}
		else if (chance > 60 && chance <= 60 + 20 + getLevel() * 5) {
			actors.push_back(new Smoregon(VIEW_WIDTH - 1, 1.0*randInt(0, VIEW_HEIGHT - 1), this));
		}
		else if (chance > 60 + 20 + getLevel() * 5 && chance <= 60 + 20 + getLevel() * 5 + 5 + getLevel() * 10) {
			actors.push_back(new Snagglegon(VIEW_WIDTH - 1, 1.0*randInt(0, VIEW_HEIGHT - 1), this));
		}
		numberAliensAlive++;
	}

	if (randInt(1, 15) == 1) {
		actors.push_back(new Star(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this));
	}
		
	ostringstream oss;
	oss << "Lives: " << getLives() << "  Health: " << nachenBlaster->gethealth() * 2 << "%" << "  Score: " << getScore() << "  Level: " << getLevel() << "  Cabbages: " << nachenBlaster->getPercentCabbages() << "%  Torpedoes: " << nachenBlaster->getNumTorpedoes();
	setGameStatText(oss.str());
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete nachenBlaster;

	vector<Actor*>::iterator it = actors.begin();

	while (it != actors.end()) {
		delete *it;
		it = actors.erase(it);
	}
}

void StudentWorld::addActor(Actor* newActor) {
	actors.push_back(newActor);
}

bool StudentWorld::hasCollided(Actor* toCheck, Actor*& toChange) {
	
	vector<Actor*>::iterator it = actors.begin();
	bool found = false;

	while (it != actors.end() && toChange != nullptr) {
		if (*it == toChange) {
			it++;
			found = true;
			break;
		}
		it++;
	}

	if (found == false) {
		it = actors.begin();
	}


	while(it != actors.end()){
		if (sqrt(((*it)->getX() - toCheck->getX())*((*it)->getX() - toCheck->getX()) + ((*it)->getY() - toCheck->getY())*((*it)->getY() - toCheck->getY())) < .75 * ((*it)->getRadius() + toCheck->getRadius())) {
			toChange = *it;
			return true;
		}
		it++;
	}
	return false;
}

bool StudentWorld::hasCollidedwithBlaster(Actor* toCheck, NachenBlaster*& blaster){
	if (sqrt((nachenBlaster->getX() - toCheck->getX())*(nachenBlaster->getX() - toCheck->getX()) + (nachenBlaster->getY() - toCheck->getY())*(nachenBlaster->getY() - toCheck->getY())) < .75 * (nachenBlaster->getRadius() + toCheck->getRadius())) {
		blaster = nachenBlaster;
		return true;
	}

	return false;
}

NachenBlaster* StudentWorld::getBlaster() {
	return nachenBlaster;
}

void StudentWorld::killAlien() {
	numberAliensDead += 1;
	numberAliensAlive -= 1;
}

void StudentWorld::dieAlien() {
	numberAliensAlive -= 1;
}