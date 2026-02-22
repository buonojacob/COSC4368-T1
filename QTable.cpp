#include "QTable.h"
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iomanip>
using namespace std;

QTable::QTable()
{
}

QTable::~QTable()
{
}

// gets qvalue of an action in a state
double QTable::get(int state, char action) {
    auto stateIterator = table.find(state);
    if (stateIterator == table.end()) {
        return 0.0;
    }
    auto actionIterator = stateIterator->second.find(action);
    if (actionIterator == stateIterator->second.end()) {
        return 0.0;
    }
    return actionIterator->second;
}

// sets qvalue of an action in a state
void QTable::set(int state, char action, double value) {
    table[state][action] = value;
}

// returns the max qvalue within valid actions
double QTable::getMax(int state, string& validActions) {
    if (validActions.empty()) { 
        return 0.0;
    }
    double best = -std::numeric_limits<double>::infinity();
    for (char a : validActions) {
        best = max(best, get(state, a));
    }
    return best;
}

// determines best action based on values within QTable
char QTable::bestAction(int state, string& validActions, mt19937& rng) {
    if (validActions.empty()) {
        throw std::runtime_error("ERROR: QTable bestAction() called with no valid actions!");
    }
    double best = -std::numeric_limits<double>::infinity();
    string bestActions;
    for (char a : validActions) {
        double value = get(state, a);
        if (value > best) {
            best = value;
            bestActions.clear();
            bestActions.push_back(a);
        }
        else if (value == best) {
            bestActions.push_back(a);
        }
    }
    // tie breaker
    uniform_int_distribution<size_t> distrib(0, bestActions.size() - 1);
    return bestActions[distrib(rng)];
}

// retrieves data from private member table
unordered_map<int, unordered_map<char, double>>& QTable::data() {
    return table;
}

// clears table
void QTable::clear() {
    table.clear();
}

// debug print function
void QTable::print() {
    if (table.empty()) {
        cout << "QTable is empty." << endl;
        return;
    }
    cout << "----------[QTABLE]----------" << endl;
    for (auto& statePair : table) {
        int state = statePair.first;
        auto& actions = statePair.second;
        cout << "State " << state << ":" << endl;
        for (auto& actionPair : actions) {
            char action = actionPair.first;
            double value = actionPair.second;
            cout << "  Action " << action << " -> " << std::fixed << std::setprecision(3) << value << endl;
        }
        cout << endl;
    }
    cout << "-------------------------" << endl;
}