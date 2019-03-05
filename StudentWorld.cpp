#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir), numAliensCreated(0), numAliensDestroyed(0), numAlienScreen(0)
{
}

int StudentWorld::init()
{
    mainShip = new NachenBlaster(this);
    for(int i = 0; i < 30; i++)
    {
        int randX = randInt(0, VIEW_WIDTH - 1);
        int randY = randInt(0, VIEW_HEIGHT - 1);
        Actor * newStar = new Star(randX, randY, this);
        actors.push_back(newStar);
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    list<Actor*> :: iterator it;
    mainShip->doSomething();
    for(it = actors.begin(); it != actors.end(); it++)
    {
        (*it)->doSomething();
        if(mainShip->isDead())
        {
            //sets these to 0 so the chances for creating a new alien on screen are correct
            numAliensDestroyed = 0;
            numAliensCreated = 0;
            numAlienScreen = 0;
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
    }
    if(numAliensDestroyed >= (6 + (4 * getLevel())))
    {
        numAliensDestroyed = 0;
        numAliensCreated = 0;
        numAlienScreen = 0;
        return GWSTATUS_FINISHED_LEVEL;
    }
    chanceNewStar();
    for(it = actors.begin(); it != actors.end(); it++)
    {
        if((*it)->isDead())
        {
            delete *it;
            it = actors.erase(it);
        }
    }
    setGameStatText(mainShip->getString());
    chanceNewAlien();

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    list<Actor*> :: iterator it;
    it = actors.begin();
    while(it != actors.end())
    {
        delete *it;
        it = actors.erase(it);
    }
    delete mainShip;
    mainShip = nullptr;
}

void StudentWorld::chanceNewStar()
{
    int newStarChance = randInt(1, 15);
    if(newStarChance == 15)
    {
        int randY = randInt(0, VIEW_HEIGHT - 1);
        Star* newStar = new Star(VIEW_WIDTH - 1, randY, this);
        actors.push_back(newStar);
    }
}

void StudentWorld::pushNewActor(Actor* act)
{
    actors.push_back(act);
}

int StudentWorld::numActors()
{
    return actors.size() + 1;
}


bool StudentWorld::withinDistance(Actor* a1, Actor* a2)
{
    //calculates if the two actors a1 and a2 are within colliding distance
    double ret = sqrt((a2->getX() - a1->getX())*(a2->getX() - a1->getX()) + (a2->getY() - a1->getY())*(a2->getY() - a1->getY()));
    if(ret < (.75) * (a1->getRadius() + a2->getRadius()))
       return true;
    return false;
}


void StudentWorld::checkForCollisions(Actor* act)
{
    list<Actor*> :: iterator it;
    if(act != mainShip)
    {
        //act = projectile or alien
        //collision of mainship with projectile or alien
        if(withinDistance(act, mainShip) && act->canGiveDamage())
        {
            mainShip->hit(act->getDamage());
            act->hit(100);
        }
        //act = goodie
        //collision of mainship with goodie
        else if(withinDistance(act, mainShip) && act->canGiveUpgrades())
        {
            act->hit(0);
        }
        for(it = actors.begin(); it != actors.end(); it++)
        {
            //act = projectile, it = alien
            if(withinDistance(act, *it) && act->canGiveDamage() && *it != act && act->getDamage() != 5 && act->getDamage() != 15 && (*it)->getHealth() != 0)
            {
                if(act->firedFromPlayer())
                {
                    (*it)->hit(act->getDamage());
                    act->hit(0);
                }
            }
            //act = alien, it = projectile
            else if(withinDistance(act, *it) && (*it)->canGiveDamage() && *it != act && (*it)->getDamage() != 5 && (*it)->getDamage() != 15 && act->getHealth() != 0)
            {
                if((*it)->firedFromPlayer())
                {
                    act->hit((*it)->getDamage());
                    (*it)->hit(0);
                }
            }
        }
    }
    else
    {
        for(it = actors.begin(); it != actors.end(); it++)
        {
            //act = mainShip, it = alien or projectile
            if(withinDistance(mainShip, *it) && (*it)->canGiveDamage())
            {
                mainShip->hit((*it)->getDamage());
                (*it)->hit(100);
            }
            //act = mainShip, it = Goodie
            else if(withinDistance(act, mainShip) && act->canGiveUpgrades())
            {
                (*it)->hit(0);
            }
        }
    }
}


void StudentWorld::fireCabbage(int x, int y)
{
    Actor* firedCabbage = new Cabbage(x + 12, y, this);
    pushNewActor(firedCabbage);
}

void StudentWorld::fireTorpedo(int x, int y, int dir)
{
    
    Actor* firedTorpedo = new Torpedo(x, y, this, dir);
    pushNewActor(firedTorpedo);
}

void StudentWorld::fireTurnip(int x, int y)
{
    Actor* firedTurnip = new Turnip(x - 14, y, this);
    pushNewActor(firedTurnip);
}

void StudentWorld::newExplosion(int x, int y)
{
    Actor* expl = new Explosion(x, y, this);
    pushNewActor(expl);
}

void StudentWorld::addHitPoints(int amt)
{
    //negative ints in hit because hit takes away health, but we want to add health
    if(mainShip->getHealth() + amt >= 50)
        mainShip->hit(-1 * (50 - mainShip->getHealth()));
    else
        mainShip->hit(amt * -1);
}

void StudentWorld::addTorpedoes(int num)
{
    mainShip->addTorpedoes(num);
}

void StudentWorld::chanceNewLife(int x, int y)
{
    int chance = randInt(1, 6);
    if(chance == 1)
    {
        Actor* newLife = new ExtraLife(x, y, this);
        pushNewActor(newLife);
    }
}
void StudentWorld::chanceNewOtherGoodie(int x, int y)
{
    int chance = randInt(1, 3);
    if(chance == 1)
    {
        int newChance = randInt(1, 2);
        if(newChance == 1)
        {
            Actor* newRep = new Repair(x, y, this);
            pushNewActor(newRep);
        }
        else
        {
            Actor* newTorpedoG = new TorpedoG(x, y, this);
            pushNewActor(newTorpedoG);
        }
    }
}

bool StudentWorld::playerInRange(int x, int y)
{
    if(x > mainShip->getX())
    {
        if(y <= mainShip->getY() + 4 && y >= mainShip->getY() - 4)
            return true;
    }
    return false;
}

void StudentWorld::alienDestroyed()
{
    numAliensDestroyed++;
}

void StudentWorld::alienOffScreen()
{
    numAlienScreen--;
}

void StudentWorld::newSmallgon(int x, int y)
{
    Actor* newSmall = new Smallgon(x, y, this);
    pushNewActor(newSmall);
}

void StudentWorld::newSmoregon(int x, int y)
{
    Actor* newSmore = new Smoregon(x, y, this);
    pushNewActor(newSmore);
}

void StudentWorld::newSnagglegon(int x, int y)
{
    Actor* newSnag = new Snagglegon(x, y, this);
    pushNewActor(newSnag);
}

void StudentWorld::chanceNewAlien()
{
    //calculate if there is need for a new alien to be created, and if so, then calculate the chances and do it
    int numRemaining = (6 + (4 * getLevel())) - numAliensDestroyed;
    double max = 4 + (0.5 * getLevel());
    if((numAlienScreen < max) && (numAlienScreen < numRemaining))
    {
        int smallGon = 60;
        int smoreGon = 20 + getLevel() * 5;
        int snaggleGon = 5 + getLevel() * 10;
        int total = smallGon + smoreGon + snaggleGon;
        int randY = randInt(0, VIEW_HEIGHT - 1);
        int chance = randInt(1, total);
        if(chance <= smallGon)
            newSmallgon(VIEW_WIDTH - 1, randY);
        else if(chance <= smallGon + smoreGon)
            newSmoregon(VIEW_WIDTH - 1, randY);
        else
            newSnagglegon(VIEW_WIDTH - 1, randY);
        numAliensCreated++;
        numAlienScreen++;
    }

}


StudentWorld::~StudentWorld()
{
    cleanUp();
}




