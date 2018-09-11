#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* theWorld) : GraphObject(imageID, startX, startY, dir, size, depth)
{
	isDead = false;
	myWorld = theWorld;
	hp = 0;
}

void Actor::setDead(bool status) {
	isDead = status;
}

bool Actor::getDead() {
	return isDead;
}

StudentWorld* Actor::getWorld() {
	return myWorld;
}

bool Actor::isAlien() {
	return false;
}

bool Actor::isProjectile() {
	return false;
}

double Actor::gethealth() {
	return hp;
}

void Actor::sethealth(double health) {
	hp = health;
}

void Actor::takeDamage(int damage) {
	if (damage < 0) {
		hp -= damage;
		if (hp > 50) {
			hp = 50;
		}
		return;
	}
	hp -= damage;
}

NachenBlaster::NachenBlaster(StudentWorld* world) : Actor(IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, world)
{
	sethealth(50);
	cabbageEnergy = 30;
	numTorpedoes = 0;
}

void NachenBlaster::doSomething() {
	if (getDead()) {
		return;
	}

	if (gethealth() <= 0) {
		setDead(true);
		return;
	}

	int ch;
	if (getWorld()->getKey(ch))
	{
		// user hit a key during this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (!(getX() - 6 < 0)) {
				moveTo(getX() - 6, getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			if (!(getX() + 6 >= VIEW_WIDTH)) {
				moveTo(getX() + 6, getY());
			}
			break;
		case KEY_PRESS_UP:
			if (!(getY() + 6 >= VIEW_HEIGHT)) {
				moveTo(getX(), getY() + 6);
			}
			break;
		case KEY_PRESS_DOWN:
			if (!(getY() - 6 < 0)) {
				moveTo(getX(), getY() - 6);
			}
			break;
		case KEY_PRESS_SPACE:
			if (cabbageEnergy >= 5) {
				cabbageEnergy -= 5;
				getWorld()->playSound(SOUND_PLAYER_SHOOT);
				getWorld()->addActor(new Cabbage(getX() + 12, getY(), getWorld()));
			}
			break;
		case KEY_PRESS_TAB:
			if (numTorpedoes > 0) {
				numTorpedoes -= 1;
				getWorld()->playSound(SOUND_TORPEDO);
				getWorld()->addActor(new Torpedo(getX() + 12, getY(), getWorld(), false));
			}
			break;
		}
	}
	if (cabbageEnergy < 30) {
		cabbageEnergy++;
	}

}

void NachenBlaster::incTorpedoes(int incr) {
	numTorpedoes += incr;
}

int NachenBlaster::getPercentCabbages() {
	return cabbageEnergy * 100 / 30;
}

int NachenBlaster::getNumTorpedoes() {
	return numTorpedoes;
}

Star::Star(double startX, double startY, StudentWorld* world) :Actor(IID_STAR, startX, startY, 0, (1.00*randInt(5, 50)) / 100, 3, world)
{
}

void Star::doSomething() {
	if (getDead()) {
		return;
	}

	moveTo(getX() - 1, getY());
	if (getX() <= -1) {
		setDead(true);
	}
}

Explosion::Explosion(double startX, double startY, StudentWorld* world) :Actor(IID_EXPLOSION, startX, startY, 0, 1, 0, world)
{
	numTicks = 0;
}

void Explosion::doSomething() {
	if (getDead()) {
		return;
	}

	if (numTicks > 4) {
		setDead(true);
		return;
	}

	setSize(1.5*getSize());

	numTicks++;
}

Cabbage::Cabbage(double startX, double startY, StudentWorld* world) :Actor(IID_CABBAGE, startX, startY, 0, .5, 1, world)
{
}

void Cabbage::doSomething() {
	if (getDead()) {
		return;
	}

	if (getX() <= -1 || getX() >= VIEW_WIDTH) {
		setDead(true);
		return;
	}

	Actor* other = nullptr;
	while (getWorld()->hasCollided(this, other)) {
		if (other->isAlien()) {
			setDead(true);
			return;
		}
	}

	moveTo(getX()+8, getY());
	setDirection(getDirection() + 20);
}

bool Cabbage::isProjectile() {
	return true;
}

Torpedo::Torpedo(double startX, double startY, StudentWorld* world, bool isEnemy): Actor(IID_TORPEDO, startX, startY, 0, .5, 1, world)
{
	enemy = isEnemy;
	if (enemy) {
		setDirection(getDirection() + 180);
	}
}

void Torpedo::doSomething() {
	if (getDead()) {
		return;
	}

	if (getX() <= -1 || getX() >= VIEW_WIDTH) {
		setDead(true);
		return;
	}

	if (enemy) {
		NachenBlaster* blaster = nullptr;
		if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
			blaster->takeDamage(8);
			getWorld()->playSound(SOUND_BLAST);
			setDead(true);
			return;
		}
	}
	else {
		Actor* other = nullptr;
		while (getWorld()->hasCollided(this, other)) {
			if (other->isAlien()) {
				other->takeDamage(8);
				if (other->gethealth() > 0) {
					getWorld()->playSound(SOUND_BLAST);
				}
				setDead(true);
				return;
			}
		}
	}
	

	if (enemy) {
		moveTo(getX() - 8, getY());
	}
	else {
		moveTo(getX() + 8, getY());
	}


	if (enemy) {
		NachenBlaster* blaster = nullptr;
		if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
			blaster->takeDamage(8);
			getWorld()->playSound(SOUND_BLAST);
			setDead(true);
			return;
		}
	}
	else {
		Actor* other = nullptr;
		while (getWorld()->hasCollided(this, other)) {
			if (other->isAlien()) {
				other->takeDamage(8);
				if (other->gethealth() > 0) {
					getWorld()->playSound(SOUND_BLAST);
				}
				setDead(true);
				return;
			}
		}
	}
}

Turnip::Turnip(double startX, double startY, StudentWorld* world) :Actor(IID_TURNIP, startX, startY, 0, .5, 1, world)
{
}

void Turnip::doSomething() {
	if (getDead()) {
		return;
	}

	if (getX() <= -1 || getX() >= VIEW_WIDTH) {
		setDead(true);
		return;
	}

	NachenBlaster* blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		blaster->takeDamage(2);
		getWorld()->playSound(SOUND_BLAST);
		setDead(true);
		return;
	}

	moveTo(getX() - 6, getY());
	setDirection(getDirection() + 20);

	blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		blaster->takeDamage(2);
		getWorld()->playSound(SOUND_BLAST);
		setDead(true);
		return;
	}
}

Alien::Alien(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* theWorld) : Actor(imageID, startX, startY, dir, size, depth, theWorld) {
	flightplength = 0;
	flightpdir = 0;
	travelspeed = 0;
}

bool Alien::isAlien() {
	return true;
}

void Alien::setPathLength(int lgt) {
	flightplength = lgt;
}

int Alien::getPathLength() {
	return flightplength;
}

//0 is DL, 1 is UL, 2 is L
void Alien::setPathDir(int direc) {
	flightpdir = direc;
}

//0 is DL, 1 is UL, 2 is L
int Alien::getPathDir() {
	return flightpdir;
}

void Alien::setTravelSpeed(double spd) {
	travelspeed = spd;
}

double Alien::getTravelSpeed() {
	return travelspeed;
}

void Alien::collideWithBlaster(NachenBlaster* blaster, int damage, int score) {
	blaster->takeDamage(damage);
	setDead(true);
	getWorld()->increaseScore(score);
	getWorld()->playSound(SOUND_DEATH);
	getWorld()->killAlien();
	getWorld()->addActor(new Explosion(getX(), getY(), getWorld()));
}

void Alien::getKilled(int score) {
	getWorld()->increaseScore(score);
	setDead(true);
	getWorld()->playSound(SOUND_DEATH);
	getWorld()->killAlien();
	getWorld()->addActor(new Explosion(getX(), getY(), getWorld()));
}

void Alien::movePath() {
	switch (getPathDir()) {
	case 0:
		moveTo(getX() - getTravelSpeed(), getY() - getTravelSpeed());
		break;
	case 1:
		moveTo(getX() - getTravelSpeed(), getY() + getTravelSpeed());
		break;
	case 2:
		moveTo(getX() - getTravelSpeed(), getY());
		break;
	}
}

Smallgon::Smallgon(double startX, double startY, StudentWorld* world) : Alien(IID_SMALLGON, startX, startY, 0, 1.5, 1, world) {
	sethealth(5 * (1 + (world->getLevel() - 1) * .1));
	setTravelSpeed(2.0);
}

void Smallgon::doSomething() {
	if (getDead()) {
		return;
	}

	if (getX() < 0 ) {
		getWorld()->dieAlien();
		setDead(true);
		return;
	}

	if (gethealth() <= 0) {
		getKilled(250);
		return;
	}

	NachenBlaster* blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		collideWithBlaster(blaster, 5, 250);
		return;
	}

	Actor* other = nullptr;
	while (getWorld()->hasCollided(this, other)) {
		if (other->isProjectile()) {
			takeDamage(2);
			if (gethealth() <= 0) {
				getKilled(250);
				other->setDead(true);
				return;
			}
			else {
				other->setDead(true);
				getWorld()->playSound(SOUND_BLAST);
				return;
			}
		}
	}

	
	if (getY() <= 0) {
		setPathDir(1);
		setPathLength(randInt(1, 32));
	}

	else if (getY() >= VIEW_HEIGHT - 1) {
		setPathDir(0);
		setPathLength(randInt(1, 32));
	}

	else if(getPathLength() <= 0) {
		setPathDir(randInt(0, 2));
		setPathLength(randInt(1, 32));
	}

	blaster = getWorld()->getBlaster();
	if (getX() > blaster->getX() && abs(blaster->getY() - getY()) <= 4) {
		if (randInt(1, (20 / getWorld()->getLevel()) + 5) == 1) {
			getWorld()->addActor(new Turnip(getX() - 14, getY(), getWorld()));
			getWorld()->playSound(SOUND_ALIEN_SHOOT);
			return;
		}
	}

	if (getPathLength() > 0) {
		movePath();
		setPathLength(getPathLength() - 1);
	}

	blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		collideWithBlaster(blaster, 5, 250);
		return;
	}

	other = nullptr;
	while (getWorld()->hasCollided(this, other)) {
		if (other->isProjectile()) {
			takeDamage(2);
			if (gethealth() <= 0) {
				getKilled(250);
				other->setDead(true);
				return;
			}
			else {
				other->setDead(true);
				getWorld()->playSound(SOUND_BLAST);
				return;
			}
		}
	}
}

Smoregon::Smoregon(double startX, double startY, StudentWorld* world) : Alien(IID_SMOREGON, startX, startY, 0, 1.5, 1, world) {
	sethealth(5 * (1 + (world->getLevel() - 1) * .1));
	setTravelSpeed(2.0);
}

void Smoregon::doSomething() {
	if (getDead()) {
		return;
	}

	if (getX() < 0) {
		getWorld()->dieAlien();
		setDead(true);
		return;
	}

	if (gethealth() <= 0) {
		getKilled(250);
		if (randInt(1, 3) == 3) {
			if (randInt(1, 2) == 2) {
				getWorld()->addActor(new RepairGoodie(getX(), getY(), getWorld()));
			}
			else {
				getWorld()->addActor(new FlatulenceTorpedoGoodie(getX(), getY(), getWorld()));
			}
		}
		return;
	}

	NachenBlaster* blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		collideWithBlaster(blaster, 5, 250);
		if (randInt(1, 3) == 3) {
			if (randInt(1, 2) == 2) {
				getWorld()->addActor(new RepairGoodie(getX(), getY(), getWorld()));
			}
			else {
				getWorld()->addActor(new FlatulenceTorpedoGoodie(getX(), getY(), getWorld()));
			}
		}
		return;
	}

	Actor* other = nullptr;
	while (getWorld()->hasCollided(this, other)) {
		if (other->isProjectile()) {
			takeDamage(2);
			if (gethealth() <= 0) {
				getKilled(250);
				if (randInt(1, 3) == 3) {
					if (randInt(1, 2) == 2) {
						getWorld()->addActor(new RepairGoodie(getX(), getY(), getWorld()));
					}
					else {
						getWorld()->addActor(new FlatulenceTorpedoGoodie(getX(), getY(), getWorld()));
					}
				}
				other->setDead(true);
				return;
			}
			else {
				other->setDead(true);
				getWorld()->playSound(SOUND_BLAST);
				return;
			}
		}
	}


	if (getY() <= 0) {
		setPathDir(1);
		setPathLength(randInt(1, 32));
	}

	else if (getY() >= VIEW_HEIGHT - 1) {
		setPathDir(0);
		setPathLength(randInt(1, 32));
	}

	else if (getPathLength() <= 0) {
		setPathDir(randInt(0, 2));
		setPathLength(randInt(1, 32));
	}

	blaster = getWorld()->getBlaster();
	if (getX() > blaster->getX() && abs(blaster->getY() - getY()) <= 4) {
		if (randInt(1, (20 / getWorld()->getLevel()) + 5) == 1) {
			getWorld()->addActor(new Turnip(getX() - 14, getY(), getWorld()));
			getWorld()->playSound(SOUND_ALIEN_SHOOT);
			return;
		}
		if (randInt(1, (20 / getWorld()->getLevel()) + 5) == 1) {
			setPathDir(2);
			setPathLength(VIEW_WIDTH);
			setTravelSpeed(5);
		}
	}

	if (getPathLength() > 0) {
		movePath();
		setPathLength(getPathLength() - 1);
	}

	blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		collideWithBlaster(blaster, 5, 250);
		if (randInt(1, 3) == 3) {
			if (randInt(1, 2) == 2) {
				getWorld()->addActor(new RepairGoodie(getX(), getY(), getWorld()));
			}
			else {
				getWorld()->addActor(new FlatulenceTorpedoGoodie(getX(), getY(), getWorld()));
			}
		}
		return;
	}

	other = nullptr;
	while (getWorld()->hasCollided(this, other)) {
		if (other->isProjectile()) {
			takeDamage(2);
			if (gethealth() <= 0) {
				getKilled(250);
				if (randInt(1, 3) == 3) {
					if (randInt(1, 2) == 2) {
						getWorld()->addActor(new RepairGoodie(getX(), getY(), getWorld()));
					}
					else {
						getWorld()->addActor(new FlatulenceTorpedoGoodie(getX(), getY(), getWorld()));
					}
				}
				other->setDead(true);
				return;
			}
			else {
				other->setDead(true);
				getWorld()->playSound(SOUND_BLAST);
				return;
			}
		}
	}
}

Snagglegon::Snagglegon(double startX, double startY, StudentWorld* world) : Alien(IID_SNAGGLEGON, startX, startY, 0, 1.5, 1, world) {
	sethealth(10 * (1 + (world->getLevel() - 1) * .1));
	setTravelSpeed(1.75);
	setPathDir(0);
}

void Snagglegon::doSomething() {
	if (getDead()) {
		return;
	}

	if (getX() < 0) {
		getWorld()->dieAlien();
		setDead(true);
		return;
	}

	if (gethealth() <= 0) {
		getKilled(1000);
		if (randInt(1, 6) == 6) {
			getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
		}
		return;
	}

	NachenBlaster* blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		collideWithBlaster(blaster, 15, 1000);
		if (randInt(1, 6) == 6) {
			getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
		}
		return;
	}

	Actor* other = nullptr;
	while (getWorld()->hasCollided(this, other)) {
		if (other->isProjectile()) {
			takeDamage(2);
			if (gethealth() <= 0) {
				getKilled(1000);
				if (randInt(1, 6) == 6) {
					getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
				}
				other->setDead(true);
				return;
			}
			else {
				other->setDead(true);
				getWorld()->playSound(SOUND_BLAST);
				return;
			}
		}
	}


	if (getY() <= 0) {
		setPathDir(1);
	}

	else if (getY() >= VIEW_HEIGHT - 1) {
		setPathDir(0);
	}

	blaster = getWorld()->getBlaster();
	if (getX() > blaster->getX() && abs(blaster->getY() - getY()) <= 4) {
		if (randInt(1, (15 / getWorld()->getLevel()) + 10) == 1) {
			getWorld()->addActor(new Torpedo(getX() - 14, getY(), getWorld(), true));
			getWorld()->playSound(SOUND_TORPEDO);
			return;
		}
	}

	movePath();
		

	blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		collideWithBlaster(blaster, 15, 1000);
		if (randInt(1, 6) == 6) {
			getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
		}
		return;
	}

	other = nullptr;
	while (getWorld()->hasCollided(this, other)) {
		if (other->isProjectile()) {
			takeDamage(2);
			if (gethealth() <= 0) {
				getKilled(1000);
				if (randInt(1, 6) == 6) {
					getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
				}
				other->setDead(true);
				return;
			}
			else {
				other->setDead(true);
				getWorld()->playSound(SOUND_BLAST);
				return;
			}
		}
	}
}

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* theWorld) : Actor(imageID, startX, startY, 0, 0.5, 1, theWorld) {
}

void Goodie::getGoodie() {
	setDead(true);
	getWorld()->increaseScore(100);
	getWorld()->playSound(SOUND_GOODIE);
}

ExtraLifeGoodie::ExtraLifeGoodie(double startX, double startY, StudentWorld* world) : Goodie(IID_LIFE_GOODIE, startX, startY, world) {
}

void ExtraLifeGoodie::doSomething() {
	if (getDead()) {
		return;
	}

	if (getX() <= -1 || getX() >= VIEW_WIDTH || getY() <= -1 || getY() >= VIEW_HEIGHT) {
		setDead(true);
		return;
	}

	NachenBlaster* blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		getGoodie();
		getWorld()->incLives();
		return;
	}

	moveTo(getX() - .75, getY() - .75);

	blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		getGoodie();
		getWorld()->incLives();
		return;
	}
}

RepairGoodie::RepairGoodie(double startX, double startY, StudentWorld* world) : Goodie(IID_REPAIR_GOODIE, startX, startY, world) {

}

void RepairGoodie::doSomething() {
	if (getDead()) {
		return;
	}

	if (getX() <= -1 || getX() >= VIEW_WIDTH || getY() <= -1 || getY() >= VIEW_HEIGHT) {
		setDead(true);
		return;
	}

	NachenBlaster* blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		getGoodie();
		blaster->takeDamage(-10);
		return;
	}

	moveTo(getX() - .75, getY() - .75);

	blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		getGoodie();
		blaster->takeDamage(-10);
		return;
	}
}

FlatulenceTorpedoGoodie::FlatulenceTorpedoGoodie(double startX, double startY, StudentWorld* world) : Goodie(IID_TORPEDO_GOODIE, startX, startY, world) {

}

void FlatulenceTorpedoGoodie::doSomething() {
	if (getDead()) {
		return;
	}

	if (getX() <= -1 || getX() >= VIEW_WIDTH || getY() <= -1 || getY() >= VIEW_HEIGHT) {
		setDead(true);
		return;
	}

	NachenBlaster* blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		getGoodie();
		blaster->incTorpedoes(5);
		return;
	}

	moveTo(getX() - .75, getY() - .75);

	blaster = nullptr;
	if (getWorld()->hasCollidedwithBlaster(this, blaster)) {
		getGoodie();
		blaster->incTorpedoes(5);
		return;
	}
}