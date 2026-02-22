#include "State.h"
#include <iostream>
using namespace std;

State::State()
{
}

State::State(Agent _a, World _w) {
    // calculates current state space
    i = _a.getX();
    j = _a.getY();
    x = _a.isHoldingBlock();
    if (x) {
        s = _w.getGrid()[1][4] < 0;
        t = _w.getGrid()[4][0] < 0;
        u = _w.getGrid()[4][2] < 0;
    } else {
        s = _w.getGrid()[0][0] > 0;
        t = _w.getGrid()[2][2] > 0;
        u = _w.getGrid()[4][4] > 0;
    }
}

State::~State()
{
}

void State::printState(Agent _a, World _w) {
    updateState(_a, _w);
    cout << "(i, j, x, s, t, u): (" << i << ", " << j << ", " << x << ", " << s << ", " << t << ", " << u << ")" << endl;
}

void State::printDetailedState(Agent _a, World _w) {
    updateState(_a, _w);
    cout << "Agent is at " << i << ", " << j << "." << endl;
    if (x) {
        cout << "Agent is currently holding a block." << endl;
        if (s) {
            cout << "Dropoff location 1, 4 can hold more blocks." << endl;
        }
        else {
            cout << "Dropoff location 1, 4 is full!" << endl;
        }
        if (t) {
            cout << "Dropoff location 4, 0 can hold more blocks." << endl;
        }
        else {
            cout << "Dropoff location 4, 0 is full!" << endl;
        }
        if (u) {
            cout << "Dropoff location 4, 2 can hold more blocks." << endl;
        }
        else {
            cout << "Dropoff location 4, 2 is full!" << endl;
        }
    }
    else {
        cout << "Agent is not currently holding a block." << endl;
        if (s) {
            cout << "Pickup location 0, 0 has at least 1 block." << endl;
        }
        else {
            cout << "Pickup location 0, 0 is empty!" << endl;
        }
        if (t) {
            cout << "Pickup location 2, 2 has at least 1 block." << endl;
        }
        else {
            cout << "Pickup location 2, 2 is empty!" << endl;
        }
        if (u) {
            cout << "Pickup location 4, 4 has at least 1 block." << endl;
        }
        else {
            cout << "Pickup location 4, 4 is empty!" << endl;
        }
    }
}

void State::updateState(Agent _a, World _w) {
    // recalculates current space
    i = _a.getX();
    j = _a.getY();
    x = _a.isHoldingBlock();
    if (x) {
        s = _w.getGrid()[1][4] < 0;
        t = _w.getGrid()[4][0] < 0;
        u = _w.getGrid()[4][2] < 0;
    } else {
        s = _w.getGrid()[0][0] > 0;
        t = _w.getGrid()[2][2] > 0;
        u = _w.getGrid()[4][4] > 0;
    }
}

string State::aplop(Agent _a, World _w) {
    string APPLICABLEOPS = "";
    updateState(_a, _w);
    if (x) {
        if (((i == 1) && (j == 4) && (s == 1))
        || ((i == 4) && (j == 0) && (t == 1))
        || ((i == 4) && (j == 2) && (u == 1))) {
            APPLICABLEOPS += "d";
        }
    }
    else if (!x) {
        if (((i == 0) && (j == 0) && (s == 1))
         || ((i == 2) && (j == 2) && (t == 1)) 
         || ((i == 4) && (j == 4) && (u == 1))) {
            APPLICABLEOPS += "p";
        }
    }
    if (!(i == 0)) {
        APPLICABLEOPS += "n";
    }
    if (!(i == 4)) {
        APPLICABLEOPS += "s";
    }
    if (!(j == 0)) {
        APPLICABLEOPS += "w";
    }
    if (!(j == 4)) {
        APPLICABLEOPS += "e";
    }
    if (APPLICABLEOPS == "") {
        cerr << "WARNING: NO APPLICABLE OPERATORS!" << endl;
    }
    return APPLICABLEOPS;
}

int State::encode() {
    string combined = to_string(i) + to_string(j) + to_string(x) + to_string(s) + to_string(t) + to_string(u);
    return stoi(combined);
}
