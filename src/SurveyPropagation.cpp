//
// Created by landy on 17/12/16.
//

#include <iostream>
#include "SurveyPropagation.h"
#include <ctime>

SurveyPropagation::SurveyPropagation(int n, int maxIterationNumber,
                                     float epsilon, float alpha,
                                     int seed, Debug_Level debug_level) {
    this->n = n;
    this->maxIterationNumber = maxIterationNumber;
    this->epsilon = epsilon;
    this->alpha = alpha;
    this->debug_level = debug_level;
    this->constantTermInModel = 0;

    this->variablesNodes = vector<VariableNode*>();
    this->factorNodes = vector<FactorNode*>();
    usrand(seed);
}

void SurveyPropagation::ConstructFactorGraph() {

    //Construct the Variable Nodes
    for (int i = 0; i < n; ++i) {
        auto *variable = new VariableNode(i, this->alpha);
        variablesNodes.push_back(variable);
    }


    //construct the Interactions and Factor Nodes
    int factorId = 0;
    for (int k = 1; k < n; k++)
        for (int i = 0; i < n - k; i++)
            for (int i_ = 0; i_ < n - k; i_++)
            {
                int j = i + k;
                int j_ = i_ + k;

                vector<int> interactions = {i, j, i_, j_};
                interactions = JoinAndFilter(interactions);

                if (interactions.empty()) {
                    constantTermInModel += 1;
                    continue;
                }

                //Construct the Factor Nodes
                auto factorNode = new FactorNode(factorId, this->alpha);
                factorNodes.push_back(factorNode);

                //Set the interactions
                if (interactions.size() == 2) {

                    int id1 = interactions[0];
                    int id2 = interactions[1];

                    VariableNode* variableNode_i = variablesNodes[id1];
                    VariableNode* variableNode_j = variablesNodes[id2];

                    factorNode->InsertNeighbor(variableNode_i);
                    factorNode->InsertNeighbor(variableNode_j);

                    variableNode_i->InsertNeighbor(factorNode);
                    variableNode_j->InsertNeighbor(factorNode);
                }
                //Set the interactions
                else if (interactions.size() == 4) {

                    int id1 = interactions[0];
                    int id2 = interactions[1];
                    int id3 = interactions[2];
                    int id4 = interactions[3];

                    VariableNode* variableNode_i = variablesNodes[id1];
                    VariableNode* variableNode_j = variablesNodes[id2];
                    VariableNode* variableNode_k = variablesNodes[id3];
                    VariableNode* variableNode_l = variablesNodes[id4];

                    factorNode->InsertNeighbor(variableNode_i);
                    factorNode->InsertNeighbor(variableNode_j);
                    factorNode->InsertNeighbor(variableNode_k);
                    factorNode->InsertNeighbor(variableNode_l);

                    variableNode_i->InsertNeighbor(factorNode);
                    variableNode_j->InsertNeighbor(factorNode);
                    variableNode_k->InsertNeighbor(factorNode);
                    variableNode_l->InsertNeighbor(factorNode);
                }

                factorId++;
            }
}

void SurveyPropagation::InitRhos(){
    for (auto variable : variablesNodes)
        variable->__ComputeExpectedHAndSigma();
}

float SurveyPropagation::SendMessageToNeighborhood(FactorNode *factorNode) {
    auto localBiggerDifference = 0.0F;

    for (auto variableNode : factorNode->neighborhood) {
        assert(variableNode->GetId() < variablesNodes.size());
        float difference = factorNode->ComputeEta(variableNode);
        localBiggerDifference =  fmax(localBiggerDifference, difference);
    }

    return localBiggerDifference;
}

vector<tuple<bool, int, float>> SurveyPropagation::SortVariablesByBias() {

    vector<tuple<bool, int, float>> variablesAndBias = vector<tuple<bool, int, float>>();

    for (auto variable : variablesNodes){
        if (variable->decimate)
            continue;

        variable->ComputeRho();

        float rhoNegative;
        float rhoZero;
        float rhoPositive;
        tie(rhoNegative, rhoZero, rhoPositive) = variable->GetRho();

        if (fabs(rhoNegative - rhoPositive) < 0.1 or (rhoZero > rhoPositive && rhoZero > rhoNegative))
            variablesAndBias.emplace_back(true, variable->GetId(), rhoZero);
        else if (rhoNegative >= rhoZero && rhoNegative >= rhoPositive)
            variablesAndBias.emplace_back(false, variable->GetId(), rhoNegative);
        else
            variablesAndBias.emplace_back(false, variable->GetId(), rhoPositive);
    }

    sort(variablesAndBias.begin(), variablesAndBias.end(),
         [](tuple<bool, int, float> const &a, tuple<bool, int, float> const &b) {
             bool zero_0;
             int id_0;
             float max_0;
             tie(zero_0, id_0, max_0) = a;

             bool zero_1;
             int id_1;
             float max_1;
             tie(zero_1, id_1, max_1) = b;

             return max_0 > max_1;
         });

    return variablesAndBias;

}

void SurveyPropagation::SetInitConfiguration(vector<int> configuration, int amountSpinsToIgnore){
    ConstructFactorGraph();

    auto ignorePosition = RandomDifferentInt(amountSpinsToIgnore, n);
    vector<int> positions = vector<int>();
    for(int i = 0; i < n; i++) {
        //the element exist
        if (find(ignorePosition.begin(), ignorePosition.end(), i) != ignorePosition.end())
            continue;
        positions.emplace_back(i);
    }

    for(auto position : positions){
        if (configuration[position] == 1)
            variablesNodes[position]->CustomDecimate(0,0,1);
        else
            variablesNodes[position]->CustomDecimate(1,0,0);
    }
}

void SurveyPropagation::SetCombinationConfiguration(vector<int> configuration, vector<int> setPosition){
    ConstructFactorGraph();

    for(auto position : setPosition){
        if (configuration[position] == 1)
            variablesNodes[position]->CustomDecimate(0,0,1);
        else
            variablesNodes[position]->CustomDecimate(1,0,0);
    }
}

pair<Exit, vector<int>> SurveyPropagation::ConvergeSP() {
//    ConstructFactorGraph();
    InitRhos();

    auto variablesDecimation = 0;
    for (auto variable: variablesNodes)
        if (variable->decimate)
            variablesDecimation +=1;

    auto totalIterations = 0;
    vector<int> decimateSpins = vector<int >();

    auto rhoNegative = 0.0F;
    auto rhoZero = 0.0F;
    auto rhoPositive = 0.0F;

    int factorNodesAmount = factorNodes.size();
        assert(factorNodesAmount > 0 and factorNodesAmount < n*n*n);

    int iterations = 0;
    float globalBiggerDifference = 0;
    clock_t begin = clock();

    do {
        globalBiggerDifference = 0;

        for (int factorId = 0; factorId < factorNodesAmount; ++factorId) {

            auto randomNumber = randint(factorNodesAmount - 1);
                assert(randomNumber < factorNodesAmount);

            auto factorNode = factorNodes[randomNumber];

            auto localDifference = SendMessageToNeighborhood(factorNode);
            globalBiggerDifference = max(globalBiggerDifference, localDifference);
        }
        iterations += 1;

    } while (iterations < maxIterationNumber && globalBiggerDifference > epsilon);

    int count = 0;
    int count_5 = 0;
    int count_6 = 0;
    int count_7 = 0;
    int count_9 = 0;
    for(auto variables: variablesNodes){
        if (variables->decimate)
            continue;

        rhoNegative = 0.0F;
        rhoZero = 0.0F;
        rhoPositive = 0.0F;
        tie(rhoNegative, rhoZero, rhoPositive) = variables->GetRho();
        float max_value = max(max(rhoZero, rhoNegative), rhoPositive);

        if (fabs(rhoNegative-rhoPositive)>0.1)
            count++;
        if (max_value > 0.5)
            count_5++;
        if (max_value > 0.6)
            count_6++;
        if (max_value > 0.7)
            count_7++;
        if (max_value > 0.9)
            count_9++;
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%.3g\n",
           n, n-variablesDecimation, count, count_5, count_6, count_7, count_9, iterations, elapsed_secs);

    return pair<Exit, vector<int>> ((totalIterations >= n*maxIterationNumber ? MaxIteration : Epsilon), decimateSpins);
}


vector<tuple<float, float, float>> SurveyPropagation::GetAllRhos() {

    vector<tuple<float, float, float>> rhos = vector<tuple<float, float, float>>();

    for (auto variable : variablesNodes) {

        float rhoNegative;
        float rhoZero;
        float rhoPositive;

        tie(rhoNegative, rhoZero, rhoPositive) = variable->GetRho();

        if (this->debug_level == DEBUG_ALL or this->debug_level == DEBUG_MEDIUM)
            printf("%d\t%g\t%g\t%g\n", variable->GetId(), rhoNegative, rhoZero, rhoPositive);

        rhos.emplace_back(variable->GetRho());
    }

    return rhos;
}

SurveyPropagation::~SurveyPropagation() {
    for(auto variable : variablesNodes)
        delete variable;

    for (auto factor : factorNodes)
        delete factor;

    variablesNodes.clear();
    factorNodes.clear();
}