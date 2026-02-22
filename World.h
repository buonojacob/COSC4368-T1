#ifndef WORLD_H
#define WORLD_H

#pragma once

#include <vector>
using namespace std;

class World
{
public:
    World();
    // overloaded constructor, define _grid in main and copy it
    World(vector<vector<int>> _grid);
    ~World();
    void printGrid();
    vector<vector<int>> getGrid();
    void setGridAtXY(int x, int y, int value);
    void dropoffAtXY(int x, int y);
    void pickupAtXY(int x, int y);
    bool isTerminal();
    void resetWorld();
private:
    vector<vector<int>> grid;
};

#endif