#include "World.h"
#include <iostream>
using namespace std;

World::World()
{

}

World::World(vector<vector<int>> _grid)
{
    grid = _grid;
}

World::~World()
{

}

vector<vector<int>> World::getGrid() {
    return grid;
}

void World::printGrid() {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            cout << "[" << grid[i][j];
            if (grid[i][j] >= 0) {
                cout << " ";
            }
            cout << "]";
        }
        cout << endl;
    }
}

void World::setGridAtXY(int x, int y, int value) {
    grid[x][y] = value;
}

void World::dropoffAtXY(int x, int y) {
    if (grid[x][y] >= 0) {
        cerr << "ERROR: DROPOFF ALREADY FULL!" << endl;
    }
    grid[x][y]++;
}

void World::pickupAtXY(int x, int y) {
    if (grid[x][y] <= 0) {
        cerr << "ERROR: PICKUP ALREADY EMPTY!" << endl;
    }
    grid[x][y]--;
}

bool World::isTerminal() {
    // retrieve every notable node
    int n1 = grid[1][4];
    int n2 = grid[4][0];
    int n3 = grid[4][2];
    int n4 = grid[0][0];
    int n5 = grid[2][2];
    int n6 = grid[4][4];
    // if every pickup is empty and every dropoff is full
    if (n1 == 0 && n2 == 0 && n3 == 0 && n4 == 0 && n5 ==  0 && n6 == 0) {
        return true;
    }
    return false;
}

void World::resetWorld() {
    // resets world as specified by assignment
    // indicate pickup cells with number of blocks
    grid[0][0] = 5;
    grid[2][2] = 5;
    grid[4][4] = 5;
    // indicate dropoff cells with negative values
    grid[1][4] = -5;
    grid[4][0] = -5;
    grid[4][2] = -5;
}