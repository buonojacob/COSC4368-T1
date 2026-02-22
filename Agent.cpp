#include "Agent.h"
#include <iostream>
using namespace std;

Agent::Agent()
{

}

Agent::Agent(int _x, int _y) {
    x = _x;
    y = _y;
    // agent starts without a block
    holdingBlock = false;
}

Agent::~Agent()
{
}

bool Agent::isHoldingBlock() {
    return holdingBlock;
}

void Agent::printLocation(World _w) {
    for (int i = 0; i < _w.getGrid().size(); i++) {
        for (int j = 0; j < _w.getGrid()[i].size(); j++) {
            if (x == i && y == j) {
                cout << "[!]";
            }
            else {
                cout << "[ ]";
            }
        }
        cout << endl;
    }
}

int Agent::getX() {
    return x;
}

int Agent::getY() {
    return y;
}

void Agent::pickup(World &_w) {
    if (holdingBlock) {
        cerr << "ERROR: ALREADY HOLDING BLOCK!" << endl;
        return;
    }
    if (_w.getGrid()[x][y] <= 0) {
        cerr << "ERROR: NOTHING TO PICKUP AT " << x << ", " << y << "!" << endl;
        return;
    }
    _w.pickupAtXY(x, y);
    holdingBlock = true;
}

void Agent::dropoff(World &_w) {
    if (!holdingBlock) {
        cerr << "ERROR: NOT HOLDING BLOCK!" << endl;
        return;
    }
    if ((_w.getGrid()[x][y] >= 0)) {
        cerr << "ERROR: DROPOFF ALREADY FULL AT " << x << ", " << y << "!" << endl;
        return;
    }
    _w.dropoffAtXY(x, y);
    holdingBlock = false;
}

void Agent::goTo(int _x, int _y) {
    x = _x;
    y = _y;
}

void Agent::resetAgent() {
    x = 0;
    y = 4;
}

void Agent::act(char action, World &_w) {
    switch (action) {
        case 'n':
            goTo(x - 1, y);
            break;
        case 's': 
            goTo(x + 1, y);
            break;
        case 'w': 
            goTo(x, y - 1);
            break;
        case 'e': 
            goTo(x, y + 1);
            break;
        case 'p': 
            pickup(_w);
            break;
        case 'd': 
            dropoff(_w);
            break;
    }
}

void Agent::updateQ(QTable& qt, int state, char action, double reward, int nextState, string& nextValidActions, double alpha, double gamma) {
    double currentQ = qt.get(state, action);
    double maxNextQ = qt.getMax(nextState, nextValidActions);

    double updated = currentQ + alpha * (reward + gamma * maxNextQ - currentQ);

    qt.set(state, action, updated);
}

void Agent::updateSARSA(QTable& qt, int state, char action, double reward, int nextState, char nextAction, double alpha, double gamma) {
    double currentQ = qt.get(state, action);
    double nextQ = qt.get(nextState, nextAction);  // uses actual chosen action, not max

    double updated = currentQ + alpha * (reward + gamma * nextQ - currentQ);

    qt.set(state, action, updated);
}