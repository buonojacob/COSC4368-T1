#ifndef STATE_H
#define STATE_H

#pragma once
#include "Agent.h"
#include "World.h"
#include <string>

// only designed for this assignment's gridworld to make it more optimal
class State
{
public:
    State();
    State(Agent _a, World _w);
    ~State();
    void printState(Agent _a, World _w);
    void printDetailedState(Agent _a, World _w);
    void updateState(Agent _a, World _w);
    string aplop(Agent _a, World _w);
    int encode();
private:
    // relates to agent's x and y respectively
    int i, j;
    // relates to whether the agent is holding a block
    bool x;
    // relates to status of points [1][4], [4][0], and [4][2] respectively
    bool s, t, u;
};

#endif