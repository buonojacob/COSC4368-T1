#ifndef QTABLE_H
#define QTABLE_H

#pragma once
#include <unordered_map>
#include <vector>
#include <random>

class QTable
{
public:
    QTable();
    ~QTable();
    double get(int state, char action);
    void set(int state, char action, double value);
    double getMax(int state, string& validActions);
    char bestAction(int state, string& validActions, mt19937& rng);
    unordered_map<int, unordered_map<char, double>>& data();
    void clear();
    void print();
private:
    unordered_map<int, unordered_map<char, double>> table;
};

#endif