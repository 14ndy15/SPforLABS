//
// Created by landy on 17/12/16.
//

#ifndef SP_NODES_H
#define SP_NODES_H

#endif //SP_NODES_H

#include <cmath>
#include <cstdlib>
#include <vector>
#include <map>
#include <tuple>
#include "Helper.h"
#include <cassert>
#include <cstdio>


using namespace std;

class FactorNode;

class VariableNode {
private:
    int id;
    float alpha;
    map<int, int> neighborhoodMap;
    // rho_{-1}, rho{0}, rho{+1}
    tuple<float, float, float> rho;

    // rho_{-1}, rho{0}, rho{+1}
    tuple<float, float, float> decimated_rho;
public:
    float expectedH;
    float sigma_square;
    bool decimate;
    vector<FactorNode*> neighborhood;
    void __ComputeExpectedHAndSigma();
    bool CheckComputeRho(FactorNode* factorNode, float expectedH, float sigma_square);
    float ComputeRho(FactorNode* factorNode);
    void ComputeRho();
    tuple<float, float, float> GetRho();
    int GetDecimateInterval();
    void Decimate();
    void CustomDecimate(float rhoNegative, float rhoZero, float rhoPositive);
    void InsertNeighbor(FactorNode *factorNode);
    FactorNode* GetNeighborById(int id);
    int GetId();
    VariableNode(int id, float alpha = .5);
    ~VariableNode();
};

class FactorNode {
private:
    float alpha;
    map<int, int> neighborhoodMap;

    //Variable -> eta_{-1}, eta_{0}, eta_{+1}
    map<VariableNode*, tuple<float, float, float>> eta;

public:
    vector<VariableNode*> neighborhood;

    float ComputeEta(VariableNode* variableNode);
    tuple<float, float, float> GetEta(VariableNode* variableNode);
    void InsertNeighbor(VariableNode *extendVariableNode);
    VariableNode* GetNeighborById(int id);
    int GetId();
    int id;
    FactorNode(int id, float alpha = .5);
    ~FactorNode();
};