#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include <string>

class StudentWorld;


class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, StudentWorld* world, int startDirection = 0, double size = 1.0, int depth = 0);
    virtual bool doSomething() = 0;
    bool isDead();
    void setDead();
    StudentWorld* getWorld();
    virtual void hit(int dmg);
    virtual bool canGiveDamage();
    virtual bool canGiveUpgrades();
    virtual int getDamage();
    virtual int getHealth();
    virtual void addTorpedoes(int num);
    virtual bool firedFromPlayer();
    virtual std::string getString();
    virtual ~Actor();
    
    
private:
    bool dead = false;
    StudentWorld* sw;

};



class NachenBlaster : public Actor
{
public:
    NachenBlaster(StudentWorld* world);
    virtual bool doSomething();
    virtual bool canGiveDamage();
    virtual void hit(int dmg);
    virtual int getHealth();
    virtual void addTorpedoes(int num);
    virtual std::string getString();
    virtual ~NachenBlaster();
    
private:
    int hitPoints;
    int cabbagePoints;
    int flatTorpedoes;

};
 
 

class Star : public Actor
{
public:
    Star(int startX, int startY, StudentWorld* world);
    virtual bool doSomething();
    virtual ~Star();
    
private:
};



class Explosion : public Actor
{
public:
    Explosion(int startX, int startY, StudentWorld* world);
    virtual bool doSomething();
    virtual ~Explosion();
    
    
private:
    
};

class Projectile : public Actor
{
public:
    Projectile(int imageID, int startX, int startY, StudentWorld* world, int direction, double size, int depth, int dmg);
    virtual bool doSomething() = 0;
    virtual void hit(int dmg);
    virtual bool canGiveDamage();
    virtual int getDamage();
    virtual bool firedFromPlayer();
    virtual ~Projectile();
    
private:
    int damage;
};

class Cabbage : public Projectile
{
public:
    Cabbage(int startX, int startY, StudentWorld* world);
    virtual bool doSomething();
    virtual bool firedFromPlayer();
    virtual ~Cabbage();
    
private:
    
    
};

class Torpedo : public Projectile
{
public:
    Torpedo(int startX, int startY, StudentWorld* world, int direction);
    virtual bool doSomething();
    virtual bool firedFromPlayer();
    virtual ~Torpedo();
    
private:
    
    
};

class Turnip : public Projectile
{
public:
    Turnip(int startX, int startY, StudentWorld* world);
    virtual bool doSomething();
    virtual ~Turnip();
    
private:
    
    
};

class Goodie : public Actor
{
public:
    Goodie(int imageID, int startX, int startY, StudentWorld* world, int direction = 0, double size = 0.5, int depth = 1);
    virtual bool doSomething();
    virtual void hit(int dmg);
    virtual bool canGiveUpgrades();
    virtual ~Goodie();
    
private:
};

class ExtraLife : public Goodie
{
public:
    ExtraLife(int startX, int startY, StudentWorld* world);
    virtual void hit(int dmg);
    virtual ~ExtraLife();
    
private:
    
};

class Repair : public Goodie
{
public:
    Repair(int startX, int startY, StudentWorld* world);
    virtual void hit(int dmg);
    virtual ~Repair();
    
private:
    
};

class TorpedoG : public Goodie
{
public:
    TorpedoG(int startX, int startY, StudentWorld* world);
    virtual void hit(int dmg);
    virtual ~TorpedoG();
    
private:
    
};



class Alien : public Actor
{
public:
    Alien(int imageID, int startX, int startY, StudentWorld* world, int tspeed, int damage, bool isSmore = false, int startDirection = 0, double size = 1.5, int depth = 1);
    virtual bool doSomething();
    virtual void hit(int dmg);
    virtual bool canGiveDamage();
    virtual int getDamage();
    virtual int getHealth();
    void setHealth(int amt);
    int getFlightPlanLen();
    virtual void setFlightPlan();
    int getTravelSpeed();
    int getYDirection();
    void setYDirection(int y);
    virtual ~Alien();
    
private:
    int damage;
    int hitPoints;
    int flightPlanLength;
    int travelSpeed;
    int yDirection;
    bool isSmoregon;

};

class Smallgon : public Alien
{
public:
    Smallgon(int startX, int startY, StudentWorld* world);
    virtual bool doSomething();
    virtual ~Smallgon();
};

class Smoregon : public Alien
{
public:
    Smoregon(int startX, int startY, StudentWorld* world);
    virtual bool doSomething();
    virtual ~Smoregon();
};

class Snagglegon : public Alien
{
public:
    Snagglegon(int startX, int startY, StudentWorld* world);
    virtual bool doSomething();
    virtual ~Snagglegon();
};






#endif





