#ifndef AGENT_H
#define AGENT_H

#pragma once

#include "World.h"
#include "QTable.h"
using namespace std;

class Agent
{
public:
    Agent();
    // overloaded constructor, define _x, _y in main and copy it
    Agent(int _x, int _y);
    ~Agent();
    bool isHoldingBlock();
    void printLocation(World _w);
    int getX();
    int getY();
    void pickup(World &_w);
    void dropoff(World &_w);
    void goTo(int _x, int _y);
    void resetAgent();
    void act(char action, World &w);
    // q learning update function
    void updateQ(QTable& qt, int state, char action, double reward, int nextState, string& nextValidActions, double alpha, double gamma);
    // SARSA learning update function
    void updateSARSA(QTable& qt, int state, char action, double reward, int nextState, char nextAction, double alpha, double gamma);
private:
    int x;
    int y;
    bool holdingBlock;
};

#endif