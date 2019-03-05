#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

class Actor;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void chanceNewStar();
    void pushNewActor(Actor* act);
    int numActors();
    void fireCabbage(int x, int y);
    void fireTorpedo(int x, int y, int dir);
    void fireTurnip(int x, int y);
    void checkForCollisions(Actor* act);
    bool withinDistance(Actor* a1, Actor* a2);
    void addHitPoints(int amt);
    void addTorpedoes(int num);
    void newExplosion(int x, int y);
    void chanceNewLife(int x, int y);
    void chanceNewOtherGoodie(int x, int y);
    bool playerInRange(int x, int y);
    void alienDestroyed();
    void chanceNewAlien();
    void newSmallgon(int x, int y);
    void newSmoregon(int x, int y);
    void newSnagglegon(int x, int y);
    void alienOffScreen();
    virtual ~StudentWorld();

private:
    std::list<Actor*> actors;
    Actor* mainShip;
    int numAliensDestroyed;
    int numAliensCreated;
    int numAlienScreen;
};




#endif // STUDENTWORLD_H_
