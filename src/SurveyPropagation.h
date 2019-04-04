//
// Created by landy on 17/12/16.
//

#ifndef SP_SURVEYPROPAGATION_H
#define SP_SURVEYPROPAGATION_H

#endif //SP_SURVEYPROPAGATION_H

#include "Nodes.h"
#include "Helper.h"
#include <cstdio>
#include <cmath>
#include "climits"
#include <algorithm>

enum Model {
    PSpin,
    MeanField
};

enum Exit {
    MaxIteration,
    Epsilon
};

enum Debug_Level{
    DEBUG_MIN,
    DEBUG_MEDIUM,
    DEBUG_ALL
};

class SurveyPropagation {
    private:
        int n;
        int maxIterationNumber;
        float epsilon;
        float alpha;
        unsigned int constantTermInModel;
        Debug_Level debug_level;
        vector<VariableNode*> variablesNodes;
        vector<FactorNode*> factorNodes;

        float SendMessageToNeighborhood(FactorNode* factorNode);
        void ConstructFactorGraph();
        void InitRhos();
        vector<tuple<bool, int, float>> SortVariablesByBias();
    public:
        SurveyPropagation(int n, int maxIterationNumber,
                          float epsilon, float alpha,
                          int seed, Debug_Level debug_level);
        void SetInitConfiguration(vector<int> configuration, int amountSpinsToIgnore);
        void SetCombinationConfiguration(vector<int> configuration, vector<int> setPosition);
        pair<Exit, vector<int>> ConvergeSP();
        vector<tuple<float, float, float>> GetAllRhos();
    ~SurveyPropagation();
};