#ifndef LOGGER_H
#define LOGGER_H

#pragma once
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Logger {
public:
    Logger(const string& rewardFile, const string& opsFile)
        : rewardPath(rewardFile), opsPath(opsFile), stepCount(0) {

        rewardOut.open(rewardFile);
        opsOut.open(opsFile);

        // CSV headers
        rewardOut << "step,reward,cumulative_reward\n";
        opsOut << "run,operators_to_terminal\n";
    }

    ~Logger() {
        if (rewardOut.is_open()) rewardOut.close();
        if (opsOut.is_open()) opsOut.close();
    }

    // call once per step with the reward received that step
    void logStep(double reward) {
        stepCount++;
        cumulativeReward += reward;
        rewardOut << stepCount << "," << reward << "," << cumulativeReward << "\n";
    }

    // call each time a terminal state is reached with how many operators it took
    void logRun(int operators) {
        runCount++;
        opsOut << runCount << "," << operators << "\n";
    }

private:
    ofstream rewardOut;
    ofstream opsOut;
    string rewardPath;
    string opsPath;
    int stepCount = 0;
    int runCount = 0;
    double cumulativeReward = 0.0;
};

#endif
