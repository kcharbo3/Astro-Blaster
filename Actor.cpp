#include "Actor.h"
#include "GameWorld.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>


Actor::Actor(int imageID, int startX, int startY, StudentWorld* world, int startDirection, double size, int depth)
:GraphObject(imageID, startX, startY, startDirection, size, depth), sw(world)
{
}

bool Actor::isDead()
{
    return dead;
}

void Actor::setDead()
{
    dead = true;
}

StudentWorld* Actor::getWorld()
{
    return sw;
}

void Actor::hit(int dmg)
{
    return;
}

bool Actor::canGiveDamage()
{
    return false;
}


bool Actor::canGiveUpgrades()
{
    return false;
}

int Actor::getDamage()
{
    return 0;
}



int Actor::getHealth()
{
    return 0;
}

void Actor::addTorpedoes(int num)
{};

bool Actor::firedFromPlayer()
{
    return false;
}

std::string Actor::getString()
{
    return "";
}

Actor::~Actor()
{
    
}



NachenBlaster::NachenBlaster(StudentWorld* world)
:Actor(IID_NACHENBLASTER, 0, 128, world), hitPoints(50), cabbagePoints(30), flatTorpedoes(0)
{

}



bool NachenBlaster::doSomething()
{
    if(isDead())
        return false;
    int dir = 0;
    if(getWorld()->getKey(dir))
    {
        if(dir == KEY_PRESS_SPACE && cabbagePoints >= 5)
        {
            getWorld()->fireCabbage(getX(), getY());
            cabbagePoints -= 5;
            getWorld()->playSound(SOUND_PLAYER_SHOOT);
        }
        else if(dir == KEY_PRESS_TAB && flatTorpedoes > 0)
        {
            getWorld()->fireTorpedo(getX() + 12, getY(), 0);
            flatTorpedoes--;
            getWorld()->playSound(SOUND_TORPEDO);
        }
        else if(dir == KEY_PRESS_LEFT)
        {
            if(getX() - 6 >= 0)
                moveTo(getX() - 6, getY());
        }
        else if(dir == KEY_PRESS_RIGHT)
        {
            if(getX() + 6 <= VIEW_WIDTH)
                moveTo(getX() + 6, getY());
        }
        else if(dir == KEY_PRESS_DOWN)
        {
            if(getY() - 6 >= 0)
                moveTo(getX(), getY() - 6);
        }
        else if(dir == KEY_PRESS_UP)
        {
            if(getY() + 6 <= VIEW_HEIGHT)
                moveTo(getX(), getY() + 6);
        }
    }
    if(cabbagePoints < 30)
        cabbagePoints++;
    
    getWorld()->checkForCollisions(this);
    if(hitPoints <= 0)
        setDead();
    return true;
}

void NachenBlaster::hit(int dmg)
{
    hitPoints -= dmg;
    if(hitPoints <= 0)
        setDead();
}

bool NachenBlaster::canGiveDamage()
{
    return true;
}

int NachenBlaster::getHealth()
{
    return hitPoints;
}

void NachenBlaster::addTorpedoes(int num)
{
    flatTorpedoes += num;
};

std::string NachenBlaster::getString()
{
    
    std::ostringstream oss;
    oss.setf(std::ios::fixed);
    oss.precision(0);
    oss << "Lives: " << getWorld()->getLives() << "  Health: " << (hitPoints / 50.0) * 100 << "%  Score: " << getWorld()->getScore() << "  Level: " << getWorld()->getLevel() << " Cabbages: " << (cabbagePoints / 30.0) * 100 << "%  Torpedoes: " << flatTorpedoes;
    std::string s = oss.str();
    return s;
}

NachenBlaster::~NachenBlaster()
{

}



Star::Star(int startX, int startY, StudentWorld* world)
:Actor(IID_STAR, startX, startY, world, 0, (randInt(5, 50) / 100.), 3)
{
}

bool Star::doSomething()
{
    moveTo(getX() - 1, getY());
    if(getX() <= 0)
    {
        setDead();
        return false;
    }
    return true;
}


Star::~Star()
{
    
}




Explosion::Explosion(int startX, int startY, StudentWorld* world)
:Actor(IID_EXPLOSION, startX, startY, world)
{
    
}

bool Explosion::doSomething()
{
    setSize(1.5 * getSize());
    if(getSize() == (1.5 * 1.5 * 1.5 * 1.5))
    {
        setDead();
        return false;
    }
    return true;
}


Explosion::~Explosion()
{
    
}



Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* world, int direction, double size, int depth, int dmg)
:Actor(imageID, startX, startY, world, direction, size, depth), damage(dmg)
{
    
}

bool Projectile::doSomething()
{
    if(isDead())
        return false;
    if(getX() >= VIEW_WIDTH || getX() < 0)
    {
        setDead();
        return false;
    }
    return true;
}

void Projectile::hit(int dmg)
{
    setDead();
}

bool Projectile::canGiveDamage()
{
    return true;
}

int Projectile::getDamage()
{
    return damage;
}

bool Projectile::firedFromPlayer()
{
    return false;
}

Projectile::~Projectile()
{
    
}

Cabbage::Cabbage(int startX, int startY, StudentWorld* world)
: Projectile(IID_CABBAGE, startX, startY, world, 0, 0.5, 1, 2)
{}

bool Cabbage::doSomething()
{
    if(!Projectile::doSomething())
        return false;
    getWorld()->checkForCollisions(this);
    moveTo(getX() + 12, getY());
    if(getDirection() >= 340)
        setDirection((getDirection() - 360) + 20);
    else
        setDirection(getDirection() + 20);
    getWorld()->checkForCollisions(this);
    return true;
}

bool Cabbage::firedFromPlayer()
{
    return true;
}

Cabbage::~Cabbage()
{
    
}



Torpedo::Torpedo(int startX, int startY, StudentWorld* world, int direction)
:Projectile(IID_TORPEDO, startX, startY, world, direction, 0.5, 1, 8)
{
    
}


bool Torpedo::doSomething()
{
    if(!Projectile::doSomething())
        return false;
    getWorld()->checkForCollisions(this);
    if(getDirection() == 0)
        moveTo(getX() + 12, getY());
    else
        moveTo(getX() - 12, getY());
    getWorld()->checkForCollisions(this);
    return true;
}

bool Torpedo::firedFromPlayer()
{
    if(getDirection() == 0)
        return true;
    return false;
}

Torpedo::~Torpedo()
{
    
}

Turnip::Turnip(int startX, int startY, StudentWorld* world)
:Projectile(IID_TURNIP, startX, startY, world, 0, 0.5, 1, 2)
{
    
}

bool Turnip::doSomething()
{
    if(!Projectile::doSomething())
        return false;
    getWorld()->checkForCollisions(this);
    moveTo(getX() - 6, getY());
    if(getDirection() >= 340)
        setDirection((getDirection() - 360) + 20);
    else
        setDirection(getDirection() + 20);
    getWorld()->checkForCollisions(this);
    return true;
}

Turnip::~Turnip()
{
    
}

Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* world, int direction, double size, int depth)
:Actor(imageID, startX, startY, world, direction, size, depth)
{
    
}


bool Goodie::doSomething()
{
    if(isDead())
        return false;
    if(getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
    {
        setDead();
        return false;
    }
    getWorld()->checkForCollisions(this);
    moveTo(getX() - 0.75, getY() - 0.75);
    getWorld()->checkForCollisions(this);
    return true;
}


void Goodie::hit(int dmg)
{
    if(isDead())
        return;
    getWorld()->increaseScore(100);
    setDead();
    getWorld()->playSound(SOUND_GOODIE);
}


bool Goodie::canGiveUpgrades()
{
    return true;
}



Goodie::~Goodie()
{}



ExtraLife::ExtraLife(int startX, int startY, StudentWorld* world)
: Goodie(IID_LIFE_GOODIE, startX, startY, world)
{}

void ExtraLife::hit(int dmg)
{
    Goodie::hit(dmg);
    getWorld()->incLives();
}

ExtraLife::~ExtraLife()
{}

Repair::Repair(int startX, int startY, StudentWorld* world)
: Goodie(IID_REPAIR_GOODIE, startX, startY, world)
{}


void Repair::hit(int dmg)
{
    Goodie::hit(dmg);
    getWorld()->addHitPoints(10);
}

Repair::~Repair()
{}

TorpedoG::TorpedoG(int startX, int startY, StudentWorld* world)
: Goodie(IID_TORPEDO_GOODIE, startX, startY, world)
{}

void TorpedoG::hit(int dmg)
{
    Goodie::hit(dmg);
    getWorld()->addTorpedoes(5);
}

TorpedoG::~TorpedoG()
{}


Alien::Alien(int imageID, int startX, int startY, StudentWorld* world, int tspeed, int dmg, bool isSmore, int startDirection, double size, int depth)
: Actor(imageID, startX, startY, world, startDirection, size, depth), hitPoints(0), damage(dmg), flightPlanLength(0), travelSpeed(tspeed), yDirection(0), isSmoregon(isSmore)
{}
bool Alien::doSomething()
{
    if(isDead())
        return false;
    if(getX() < 0)
    {
        setDead();
        getWorld()->alienOffScreen();
        return false;
    }
    getWorld()->checkForCollisions(this);
    setFlightPlan();
    //if damage is 5, then this is either a smoregon or smallgon
    if(getWorld()->playerInRange(getX(), getY()) && damage == 5)
    {
        int chance = randInt(1, ((20/getWorld()->getLevel()) + 5));
        if(chance == 1)
        {
            getWorld()->fireTurnip(getX(), getY());
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
            return false;
        }
        if(isSmoregon) //only smoregons can ram
        {
            int chance2 = randInt(1, ((20/getWorld()->getLevel()) + 5));
            if(chance2 == 1)
            {
                setYDirection(0);
                flightPlanLength = VIEW_WIDTH;
                travelSpeed = 5;
            }
        }
    }
    else if(getWorld()->playerInRange(getX(), getY()) && damage == 15) //snagglegons in range
    {
        int chance2 = randInt(1, ((15/getWorld()->getLevel()) + 10));
        if(chance2 == 1)
        {
            getWorld()->fireTorpedo(getX() - 14, getY(), 180);
            getWorld()->playSound(SOUND_TORPEDO);
            return false;
        }
    }
    
    if(getYDirection() == -1)
    {
        moveTo(getX() - travelSpeed, getY() - travelSpeed);
        flightPlanLength--;
    }
    else if(getYDirection() == 1)
    {
        moveTo(getX() - travelSpeed, getY() + travelSpeed);
        flightPlanLength--;
    }
    else
    {
        moveTo(getX() - travelSpeed, getY());
        flightPlanLength--;
    }
    getWorld()->checkForCollisions(this);
    return true;
}

void Alien::hit(int dmg)
{
    if(isDead())
        return;
    hitPoints -= dmg;
    if(hitPoints <= 0)
    {
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->newExplosion(getX(), getY());
        if(damage == 15) //snagglegons
        {
            getWorld()->increaseScore(1000);
            getWorld()->chanceNewLife(getX(), getY());
        }
        else //smallgons and smoregons
        {
            getWorld()->increaseScore(250);
            getWorld()->chanceNewOtherGoodie(getX(), getY());
        }
        getWorld()->alienDestroyed();
        getWorld()->alienOffScreen();
        setDead();
    }
    else
        getWorld()->playSound(SOUND_BLAST);
}
bool Alien::canGiveDamage()
{
    return true;
}
int Alien::getDamage()
{
    return damage;
}
int Alien::getHealth()
{
    return hitPoints;
}

void Alien::setHealth(int amt)
{
    hitPoints = amt;
}

int Alien::getFlightPlanLen()
{
    return flightPlanLength;
}
void Alien::setFlightPlan()
{
    if(getFlightPlanLen() == 0 || getY() >= VIEW_HEIGHT - 1 || getY() <= 0)
    {
        if(getY() >= VIEW_HEIGHT - 1)
        {
            setYDirection(-1);
        }
        else if(getY() <= 0)
        {
            setYDirection(1);
        }
        else if(getFlightPlanLen() == 0 && damage == 5) //smallgons and smoregons
        {
            int chance = randInt(1, 3);
            if(chance == 1)
            {
                setYDirection(-1);
            }
            else if(chance == 2)
            {
                setYDirection(1);
            }
            else
            {
                setYDirection(0);
            }
            int newFlightLen = randInt(1, 32);
            flightPlanLength = newFlightLen; //doesn't matter what this value is for snagglegons, they don't use it
        }
    }
}
    
int Alien::getTravelSpeed()
{
    return travelSpeed;
}


int Alien::getYDirection()
{
    return yDirection;
}

void Alien::setYDirection(int y)
{
    yDirection = y;
}

Alien::~Alien()
{}



Smallgon::Smallgon(int startX, int startY, StudentWorld* world)
: Alien(IID_SMALLGON, startX, startY, world, 2, 5)
{
    setHealth(5 * (1 + (world->getLevel() - 1) * .1)); //sets the aliens health
}

bool Smallgon::doSomething()
{
    if(!Alien::doSomething())
        return false;
    return true;
}

Smallgon::~Smallgon()
{}


Smoregon::Smoregon(int startX, int startY, StudentWorld* world)
: Alien(IID_SMOREGON, startX, startY, world, 2, 5, true)
{
    setHealth(5 * (1 + (world->getLevel() - 1) * .1));
}

bool Smoregon::doSomething()
{
    if(!Alien::doSomething())
        return false;
    return true;
}


Smoregon::~Smoregon()
{}


Snagglegon::Snagglegon(int startX, int startY, StudentWorld* world)
: Alien(IID_SNAGGLEGON, startX, startY, world, 1.75, 15)
{
    setHealth(10 * (1 + (world->getLevel() - 1) * .1));
    setYDirection( -1 );
}

bool Snagglegon::doSomething()
{
    if(!Alien::doSomething())
        return false;
    return true;
}


Snagglegon::~Snagglegon()
{}



















