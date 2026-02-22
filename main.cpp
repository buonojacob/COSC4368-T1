#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <numeric>
#include "Agent.cpp"
#include "World.cpp"
#include "State.cpp"
#include "QTable.cpp"
using namespace std;

int main(int, char**){
    // -------------------------------------------- [INITIALIZATION] --------------------------------------------
    // setup rng
    random_device rd;
    mt19937 gen(rd());

    // initalize the gridworld
    vector<vector<int>> gridworld(5, vector<int>(5, 0));
    // indicate pickup cells with number of blocks
    gridworld[0][0] = 5;
    gridworld[2][2] = 5;
    gridworld[4][4] = 5;
    // indicate dropoff cells with negative values
    gridworld[1][4] = -5;
    gridworld[4][0] = -5;
    gridworld[4][2] = -5;

    // create World object
    World w(gridworld);

    // create Agent object
    Agent a(0, 4);

    // create State object
    State s(a, w); 

     // initalize alpha and gamma as specified by assignment
    double alpha = 0.3;
    double gamma = 0.5;

    // initalize valid actions string
    string validActions = "";



    // -------------------------------------------- [EXPERIMENT 1] --------------------------------------------
    cout << "\n--- [EXPERIMENT 1] ---" << endl;
    // create QTable object for experiment 1
    QTable qt_e1;
    // each time a terminal state is reached, it gets added to this vector
    vector<int> numOperatorsUsed_e1;
    int numOperators_e1 = 0;
    double rewardSum_e1 = 0;

    // --------------------- RANDOM POLICY FOR 4000 STEPS ---------------------
    for (int i = 0; i < 4000; i++) {
        if (w.isTerminal()) {
            numOperatorsUsed_e1.push_back(numOperators_e1);
            numOperators_e1 = 0;
            w.resetWorld();
            a.resetAgent();
        }
        // current state
        int state = s.encode();
        validActions = s.aplop(a, w);

        // choose action
        char chosenAction;
        if (validActions.find('p') != string::npos) {
            chosenAction = 'p';
        }
        else if (validActions.find('d') != string::npos) {
            chosenAction = 'd';
        }
        else {  // RANDOM POLICY
            uniform_int_distribution<> distrib(0, static_cast<int>(validActions.size() - 1));
            chosenAction = validActions[distrib(gen)];
        }
        // execute action
        a.act(chosenAction, w);
        numOperators_e1++;

        // determine reward
        double reward = -1; // default movement penalty
        if ((chosenAction == 'p') || (chosenAction == 'd')) {
            reward = 13;
        }
        rewardSum_e1 += reward;

        // next state
        s.updateState(a, w);
        int nextState = s.encode();
        string nextValidActions = s.aplop(a, w);

        // Q-learning update
        a.updateQ(qt_e1, state, chosenAction, reward, nextState, nextValidActions, alpha, gamma);

        // reset valid actions
        validActions = "";
    }

    // --------------------- GREEDY POLICY FOR 4000 STEPS ---------------------
    for (int i = 0; i < 4000; i++) {
        if (w.isTerminal()) {
            numOperatorsUsed_e1.push_back(numOperators_e1);
            numOperators_e1 = 0;
            w.resetWorld();
            a.resetAgent();
        }
        // current state
        int state = s.encode();
        validActions = s.aplop(a, w);

        // choose action
        char chosenAction;
        if (validActions.find('p') != string::npos) {
            chosenAction = 'p';
        }
        else if (validActions.find('d') != string::npos) {
            chosenAction = 'd';
        }
        else {  // GREEDY POLICY
            chosenAction = qt_e1.bestAction(state, validActions, gen);
        }

        // execute action
        a.act(chosenAction, w);
        numOperators_e1++;

        // determine reward
        double reward = -1; // default movement penalty
        if ((chosenAction == 'p') || (chosenAction == 'd')) {
            reward = 13;
        }
        rewardSum_e1 += reward;

        // next state
        s.updateState(a, w);
        int nextState = s.encode();
        string nextValidActions = s.aplop(a, w);

        // Q-learning update
        a.updateQ(qt_e1, state, chosenAction, reward, nextState, nextValidActions, alpha, gamma);

        // reset valid actions
        validActions = "";
    }

    int sum_e1 = accumulate(numOperatorsUsed_e1.begin(), numOperatorsUsed_e1.end(), 0);
    cout << "The agent completed " << numOperatorsUsed_e1.size() << " runs." << endl;
    cout << "On average, it took " << sum_e1/numOperatorsUsed_e1.size() << " operators to reach a terminal state." << endl;
    cout << "The final run took " << numOperatorsUsed_e1[numOperatorsUsed_e1.size() - 1] << " operators to reach a terminal state." << endl;
    cout << "The final sum of rewards gained by the agent was " << rewardSum_e1 << "." << endl;


    
    // -------------------------------------------- [RESET] -------------------------------------------- 
    w.resetWorld();
    a.resetAgent();



    // -------------------------------------------- [EXPERIMENT 2] --------------------------------------------
    cout << "\n--- [EXPERIMENT 2] ---" << endl;
    // create QTable object for experiment 2
    QTable qt_e2;
    // each time a terminal state is reached, it gets added to this vector
    vector<int> numOperatorsUsed_e2;
    int numOperators_e2 = 0;
    double rewardSum_e2 = 0;
    // random number generator from 0.0 to 1.0 to be used by exploit policy
    uniform_real_distribution<> dis(0.0, 1.0);

    // --------------------- RANDOM POLICY FOR 200 STEPS ---------------------
    for (int i = 0; i < 200; i++) {
        if (w.isTerminal()) {
            numOperatorsUsed_e2.push_back(numOperators_e2);
            numOperators_e2 = 0;
            w.resetWorld();
            a.resetAgent();
        }
        // current state
        int state = s.encode();
        validActions = s.aplop(a, w);

        // choose action
        char chosenAction;
        if (validActions.find('p') != string::npos) {
            chosenAction = 'p';
        }
        else if (validActions.find('d') != string::npos) {
            chosenAction = 'd';
        }
        else {  // RANDOM POLICY
            uniform_int_distribution<> distrib(0, static_cast<int>(validActions.size() - 1));
            chosenAction = validActions[distrib(gen)];
        }
        // execute action
        a.act(chosenAction, w);
        numOperators_e2++;

        // determine reward
        double reward = -1; // default movement penalty
        if ((chosenAction == 'p') || (chosenAction == 'd')) {
            reward = 13;
        }
        rewardSum_e2 += reward;

        // next state
        s.updateState(a, w);
        int nextState = s.encode();
        string nextValidActions = s.aplop(a, w);

        // Q-learning update
        a.updateQ(qt_e2, state, chosenAction, reward, nextState, nextValidActions, alpha, gamma);

        // reset valid actions
        validActions = "";
    }

    // --------------------- EXPLOIT POLICY FOR 7800 STEPS ---------------------
    for (int i = 0; i < 7800; i++) {
        if (w.isTerminal()) {
            numOperatorsUsed_e2.push_back(numOperators_e2);
            numOperators_e2 = 0;
            w.resetWorld();
            a.resetAgent();
        }
        // current state
        int state = s.encode();
        validActions = s.aplop(a, w);

        // choose action
        char chosenAction;
        if (validActions.find('p') != string::npos) {
            chosenAction = 'p';
        }
        else if (validActions.find('d') != string::npos) {
            chosenAction = 'd';
        }
        else {  // GREEDY POLICY
            double randomValue = dis(gen);
            if (randomValue < 0.2) { // 20% probability for random
                uniform_int_distribution<> distrib(0, static_cast<int>(validActions.size() - 1));
                chosenAction = validActions[distrib(gen)];
            }
            else { // 80% probability for best
                chosenAction = qt_e2.bestAction(state, validActions, gen);
            }
        }

        // execute action
        a.act(chosenAction, w);
        numOperators_e2++;

        // determine reward
        double reward = -1; // default movement penalty
        if ((chosenAction == 'p') || (chosenAction == 'd')) {
            reward = 13;
        }
        rewardSum_e2 += reward;

        // next state
        s.updateState(a, w);
        int nextState = s.encode();
        string nextValidActions = s.aplop(a, w);

        // Q-learning update
        a.updateQ(qt_e2, state, chosenAction, reward, nextState, nextValidActions, alpha, gamma);

        // reset valid actions
        validActions = "";
    }

    int sum_e2 = accumulate(numOperatorsUsed_e2.begin(), numOperatorsUsed_e2.end(), 0);
    cout << "The agent completed " << numOperatorsUsed_e2.size() << " runs." << endl;
    cout << "On average, it took " << sum_e2/numOperatorsUsed_e2.size() << " operators to reach a terminal state." << endl;
    cout << "The final run took " << numOperatorsUsed_e2[numOperatorsUsed_e2.size() - 1] << " operators to reach a terminal state." << endl;
    cout << "The final sum of rewards gained by the agent was " << rewardSum_e2 << "." << endl;



    // -------------------------------------------- [RESET] -------------------------------------------- 
    w.resetWorld();
    a.resetAgent();



    // -------------------------------------------- [EXPERIMENT 3] --------------------------------------------
    cout << "\n--- [EXPERIMENT 3] ---" << endl;
    // create QTable object for experiment 3
    QTable qt_e3;
    // each time a terminal state is reached, it gets added to this vector
    vector<int> numOperatorsUsed_e3;
    int numOperators_e3 = 0;
    double rewardSum_e3 = 0;

    // --------------------- RANDOM POLICY FOR 200 STEPS ---------------------
    // SARSA: pre-select the first action before entering the loop
    s.updateState(a, w);
    int sarsa_state = s.encode();
    validActions = s.aplop(a, w);
    char chosenAction_e3;
    if (validActions.find('p') != string::npos) {
        chosenAction_e3 = 'p';
    } else if (validActions.find('d') != string::npos) {
        chosenAction_e3 = 'd';
    } else {
        uniform_int_distribution<> distrib(0, static_cast<int>(validActions.size() - 1));
        chosenAction_e3 = validActions[distrib(gen)];
    }

    for (int i = 0; i < 200; i++) {
        if (w.isTerminal()) {
            numOperatorsUsed_e3.push_back(numOperators_e3);
            numOperators_e3 = 0;
            w.resetWorld();
            a.resetAgent();
            s.updateState(a, w);
            sarsa_state = s.encode();
            validActions = s.aplop(a, w);
            if (validActions.find('p') != string::npos) {
                chosenAction_e3 = 'p';
            } else if (validActions.find('d') != string::npos) {
                chosenAction_e3 = 'd';
            } else {
                uniform_int_distribution<> distrib(0, static_cast<int>(validActions.size() - 1));
                chosenAction_e3 = validActions[distrib(gen)];
            }
        }

        // execute the pre-chosen action
        a.act(chosenAction_e3, w);
        numOperators_e3++;

        // determine reward
        double reward = -1;
        if (chosenAction_e3 == 'p' || chosenAction_e3 == 'd') reward = 13;
        rewardSum_e3 += reward;

        // next state
        s.updateState(a, w);
        int nextState = s.encode();
        string nextValidActions = s.aplop(a, w);

        // SARSA: select next action via policy NOW, before the update
        char nextAction_e3;
        if (nextValidActions.find('p') != string::npos) {
            nextAction_e3 = 'p';
        } else if (nextValidActions.find('d') != string::npos) {
            nextAction_e3 = 'd';
        } else {
            uniform_int_distribution<> distrib(0, static_cast<int>(nextValidActions.size() - 1));
            nextAction_e3 = nextValidActions[distrib(gen)];
        }

        // SARSA update: uses the actual next action, not the max
        a.updateSARSA(qt_e3, sarsa_state, chosenAction_e3, reward, nextState, nextAction_e3, alpha, gamma);

        // carry state and action forward
        sarsa_state = nextState;
        validActions = nextValidActions;
        chosenAction_e3 = nextAction_e3;
    }

    // --------------------- EXPLOIT POLICY FOR 7800 STEPS ---------------------
    // SARSA: chosenAction_e3 is already carried forward from the PRANDOM phase
    for (int i = 0; i < 7800; i++) {
        if (w.isTerminal()) {
            numOperatorsUsed_e3.push_back(numOperators_e3);
            numOperators_e3 = 0;
            w.resetWorld();
            a.resetAgent();
            s.updateState(a, w);
            sarsa_state = s.encode();
            validActions = s.aplop(a, w);
            if (validActions.find('p') != string::npos) {
                chosenAction_e3 = 'p';
            } else if (validActions.find('d') != string::npos) {
                chosenAction_e3 = 'd';
            } else if (dis(gen) < 0.2) {
                uniform_int_distribution<> distrib(0, static_cast<int>(validActions.size() - 1));
                chosenAction_e3 = validActions[distrib(gen)];
            } else {
                chosenAction_e3 = qt_e3.bestAction(sarsa_state, validActions, gen);
            }
        }

        // execute the pre-chosen action
        a.act(chosenAction_e3, w);
        numOperators_e3++;

        // determine reward
        double reward = -1;
        if (chosenAction_e3 == 'p' || chosenAction_e3 == 'd') reward = 13;
        rewardSum_e3 += reward;

        // next state
        s.updateState(a, w);
        int nextState = s.encode();
        string nextValidActions = s.aplop(a, w);

        // SARSA: select next action via policy NOW, before the update
        char nextAction_e3;
        if (nextValidActions.find('p') != string::npos) {
            nextAction_e3 = 'p';
        } else if (nextValidActions.find('d') != string::npos) {
            nextAction_e3 = 'd';
        } else if (dis(gen) < 0.2) {
            uniform_int_distribution<> distrib(0, static_cast<int>(nextValidActions.size() - 1));
            nextAction_e3 = nextValidActions[distrib(gen)];
        } else {
            nextAction_e3 = qt_e3.bestAction(nextState, nextValidActions, gen);
        }

        // SARSA update: uses the actual next action, not the max
        a.updateSARSA(qt_e3, sarsa_state, chosenAction_e3, reward, nextState, nextAction_e3, alpha, gamma);

        // carry state and action forward
        sarsa_state = nextState;
        validActions = nextValidActions;
        chosenAction_e3 = nextAction_e3;
    }

    int sum_e3 = accumulate(numOperatorsUsed_e3.begin(), numOperatorsUsed_e3.end(), 0);
    cout << "The agent completed " << numOperatorsUsed_e3.size() << " runs." << endl;
    cout << "On average, it took " << sum_e3/numOperatorsUsed_e3.size() << " operators to reach a terminal state." << endl;
    cout << "The final run took " << numOperatorsUsed_e3[numOperatorsUsed_e3.size() - 1] << " operators to reach a terminal state." << endl;
    cout << "The final sum of rewards gained by the agent was " << rewardSum_e3 << "." << endl;
}