//
// Created by landy on 17/12/16.
//

#include "Nodes.h"


//Constructor
VariableNode::VariableNode(int id, float alpha) {
    this->id = id;
    this->alpha = alpha;

    neighborhood = vector<FactorNode*>();
    neighborhoodMap = map<int, int>();
    rho = ThreeRandomNormalizedNumber();
    decimate = false;
    expectedH = 0;
    sigma_square = 0;
}

//Members Functions
int VariableNode::GetId() {
    return id;
}

void VariableNode::InsertNeighbor(FactorNode *factorNode) {
    //Storage the address
    neighborhood.push_back(factorNode);
    //Get the number of index
    int index = (int)neighborhood.size() - 1;
    //map the id and the index
    neighborhoodMap[factorNode->GetId()] = index;
}

FactorNode *VariableNode::GetNeighborById(int id) {
    int index = neighborhoodMap[id];
    return neighborhood[index];
}

void VariableNode::__ComputeExpectedHAndSigma() {

    this->expectedH = 0;
    this->sigma_square = 0;

    for(auto factor : this->neighborhood)
    {
        float etaNegative;
        float etaZero;
        float etaPositive;

        tie(etaNegative, etaZero, etaPositive) = factor->GetEta(this);

        this->expectedH += (etaPositive - etaNegative);
        this->sigma_square += etaPositive + etaNegative - (etaPositive*etaPositive) + 2*etaPositive*etaNegative - (etaNegative*etaNegative);
    }

    //Check condition, sigma square should value more than zero
    assert(sigma_square > 0);
}

//Check if the update values are equal to the values computed sum through all factor nodes
bool VariableNode::CheckComputeRho(FactorNode* factorNode, float expectedH, float sigma_square) {

    float _expectedH = 0;
    float _sigma_square = 0;

    for(auto factor : this->neighborhood)
    {
        if (factor->GetId() == factorNode->GetId())
            continue;

        float etaNegative;
        float etaZero;
        float etaPositive;

        tie(etaNegative, etaZero, etaPositive) = factor->GetEta(this);

        _expectedH += (etaPositive - etaNegative);
        _sigma_square += (etaPositive + etaNegative - (etaPositive*etaPositive) + 2*etaPositive*etaNegative - (etaNegative*etaNegative));
    }

    assert(_sigma_square > 0);

    bool equals = fabs(expectedH - _expectedH) < 0.001 && fabs(sigma_square - _sigma_square) < 0.001;

    return equals;

}

float VariableNode::ComputeRho(FactorNode* factorNode) {
    if (decimate)
        return this->expectedH;

    float expectedH = this->expectedH;
    float sigma_square = this->sigma_square;

    float etaNegative = 0;
    float etaPositive = 0;
    float etaZero = 0;

    tie(etaNegative, etaZero, etaPositive) = factorNode->GetEta(this);

    expectedH -= (etaPositive - etaNegative);
    sigma_square -= (etaPositive + etaNegative - (etaPositive*etaPositive) + 2*etaPositive*etaNegative - (etaNegative*etaNegative));

    if (sigma_square < 0)
        sigma_square = 0.00001;

    //assert(CheckComputeRho(factorNode, expectedH, sigma_square));

    float argument_negative = (-0.5F - expectedH)/sqrt(2*sigma_square);
    float rhoNegative = .5F * (1 + erf(argument_negative));

    float argument_positive = (0.5F - expectedH)/sqrt(2*sigma_square);
    float rhoPositive = 1 - 0.5F *(1 + erf(argument_positive));

    float rhoZero = 1 - rhoNegative - rhoPositive;

    if (fabs(rhoNegative) < 0.0001)
        rhoNegative = 0;
    if (fabs(rhoZero) < 0.0001)
        rhoZero = 0;
    if (fabs(rhoPositive) < 0.0001)
        rhoPositive = 0;

    assert(!isnan(rhoNegative) and !isnan(rhoZero) and !isnan(rhoPositive));
    assert(rhoNegative >= 0 and rhoZero >= 0 and rhoPositive >= 0);
    assert(fabs(rhoNegative + rhoPositive + rhoZero - 1) < 0.01);

    this->rho = tuple<float, float, float>(rhoNegative, rhoZero, rhoPositive);

    return this->expectedH;
}

void VariableNode::ComputeRho() {
    this->ComputeRho(new FactorNode(-1));
}

tuple<float, float, float> VariableNode::GetRho() {
    if (decimate)
        return decimated_rho;
    return rho;
}

void VariableNode::Decimate() {

    float rhoNegative;
    float rhoZero;
    float rhoPositive;

    tie(rhoNegative, rhoZero, rhoPositive) = this->rho;

    if (fabs(rhoNegative - rhoPositive) < 0.1){
        rhoNegative = 0;
        rhoZero = 1;
        rhoPositive = 0;
    }
    else if (rhoNegative >= rhoZero && rhoNegative >= rhoPositive) {

        rhoNegative = 1;
        rhoZero = 0;
        rhoPositive = 0;
    }
    else if (rhoZero >= rhoNegative && rhoZero >= rhoPositive) {

        rhoNegative = 0;
        rhoZero = 1;
        rhoPositive = 0;

    }
    else if (rhoPositive >= rhoZero && rhoPositive >= rhoNegative){

        rhoNegative = 0;
        rhoZero = 0;
        rhoPositive = 1;
    }


    this->sigma_square = 0;
    this->decimate = true;
    this->decimated_rho = tuple<float, float, float>(rhoNegative, rhoZero, rhoPositive);
}

void VariableNode::CustomDecimate(float rhoNegative, float rhoZero, float rhoPositive) {
    this->sigma_square = 0;
    this->decimate = true;
    this->decimated_rho = tuple<float, float, float>(rhoNegative, rhoZero, rhoPositive);
}

int VariableNode::GetDecimateInterval(){
    float rhoNegative;
    float rhoZero;
    float rhoPositive;

    tie(rhoNegative, rhoZero, rhoPositive) = this->decimated_rho;

    if (rhoNegative == 1)
        return -1;
    else if (rhoZero == 1)
        return 0;
    else
        return +1;
}

//Destructor
VariableNode::~VariableNode() {
    neighborhood.clear();
    neighborhoodMap.clear();
}

//Constructor
FactorNode::FactorNode(int id, float alpha) {
    this->id = id;
    this->neighborhood = vector<VariableNode*>();
    this->neighborhoodMap = map<int, int>();
    this->alpha = alpha;
}

//Members Functions
int FactorNode::GetId() {
    return this->id;
}

void FactorNode::InsertNeighbor(VariableNode *variableNode) {
    //Storage the address
    neighborhood.push_back(variableNode);
    //Get the number of index
    int index = (int)neighborhood.size() - 1;
    //map the id and the index
    neighborhoodMap[variableNode->GetId()] = index;
    eta[variableNode] = ThreeRandomNormalizedNumber();
}

VariableNode *FactorNode::GetNeighborById(int id) {
    int index = neighborhoodMap[id];
    return neighborhood[index];
}

float FactorNode::ComputeEta(VariableNode *variableNode) {

    if (variableNode->decimate)
        return 0;


    vector<VariableNode*> variables = vector<VariableNode*>();

    for(auto variable : neighborhood)
        if (variable->GetId() != variableNode->GetId())
            variables.push_back(variable);

    tuple<float, float, float> newEta;

    if (variables.size() == 1)
    {
        float rhoNegative;
        float rhoZero;
        float rhoPositive;

        variables[0]->ComputeRho(this);
        tie(rhoNegative, rhoZero, rhoPositive) = variables[0]->GetRho();

        newEta = tuple<float, float, float>{rhoPositive, rhoZero, rhoNegative};
    }
    else if (variables.size() == 3)
    {
        float rhoNegativeJ;
        float rhoZeroJ;
        float rhoPositiveJ;

        float rhoNegativeK;
        float rhoZeroK;
        float rhoPositiveK;

        float rhoNegativeL;
        float rhoZeroL;
        float rhoPositiveL;

        variables[0]->ComputeRho(this);
        tie(rhoNegativeJ, rhoZeroJ, rhoPositiveJ) = variables[0]->GetRho();

        variables[1]->ComputeRho(this);
        tie(rhoNegativeK, rhoZeroK, rhoPositiveK) = variables[1]->GetRho();

        variables[2]->ComputeRho(this);
        tie(rhoNegativeL, rhoZeroL, rhoPositiveL) = variables[2]->GetRho();

        float etaZero = rhoZeroJ*rhoZeroK*rhoZeroL -
                        (rhoZeroJ*rhoZeroK + rhoZeroK*rhoZeroL + rhoZeroJ*rhoZeroL) +
                        rhoZeroJ + rhoZeroK + rhoZeroL;

        float etaPositive = rhoNegativeJ*rhoNegativeK*rhoNegativeL+
                            rhoNegativeJ*rhoPositiveK*rhoPositiveL+
                            rhoPositiveJ*rhoNegativeK*rhoPositiveL+
                            rhoPositiveJ*rhoPositiveK*rhoNegativeL;

        float etaNegative = rhoPositiveJ*rhoPositiveK*rhoPositiveL+
                            rhoPositiveJ*rhoNegativeK*rhoNegativeL+
                            rhoNegativeJ*rhoPositiveK*rhoNegativeL+
                            rhoNegativeJ*rhoNegativeK*rhoPositiveL;

        newEta = tuple<float, float, float>{etaNegative, etaZero, etaPositive};
    }

    float etaNegative;
    float etaZero;
    float etaPositive;
    tie(etaNegative, etaZero, etaPositive) = newEta;


    if (etaNegative < 0)
        etaNegative = 0;
    if (etaPositive < 0)
        etaPositive = 0;
    if (etaZero < 0)
        etaZero = 0;

    //Check Condition: the numbers cant be nans
    assert(!isnan(etaNegative) and !isnan(etaZero) and !isnan(etaPositive));

    //Check Condition: the sum should have value equal 1
    assert(fabs(etaNegative + etaZero + etaPositive - 1) < 0.01);

    float etaNegativeOld;
    float etaZeroOld;
    float etaPositiveOld;
    tie(etaNegativeOld, etaZeroOld, etaPositiveOld) = this->eta[variableNode];

    //Check Condition: the numbers cant be nans
    assert(!isnan(etaNegativeOld) and !isnan(etaPositiveOld) and !isnan(etaZeroOld));

    float difference = 0;
    difference = max(difference, fabsf(etaNegativeOld - etaNegative));
    difference = max(difference, fabsf(etaZeroOld - etaZero));
    difference = max(difference, fabsf(etaPositiveOld - etaPositive));

    assert(alpha >= 0 and alpha <= 1);
    etaNegative = alpha*etaNegativeOld + (1-alpha)*etaNegative;
    etaZero = alpha*etaZeroOld + (1-alpha)*etaZero;
    etaPositive = alpha*etaPositiveOld + (1-alpha)*etaPositive;

    assert(!isnan(etaNegative) and !isnan(etaPositive) and !isnan(etaZero));
    assert(etaNegative >= 0 && etaZero >= 0 && etaPositive >= 0 );
    assert(fabs(etaNegative + etaZero + etaPositive - 1) < 0.01);

    //delete the old messages from the factor node
    variableNode->expectedH -= (etaPositiveOld - etaNegativeOld);
    variableNode->sigma_square -= (etaPositiveOld + etaNegativeOld - (etaPositiveOld*etaPositiveOld) + 2*etaPositiveOld*etaNegativeOld - (etaNegativeOld*etaNegativeOld));

    //add the new messages
    variableNode->expectedH += (etaPositive - etaNegative);
    variableNode->sigma_square += (etaPositive + etaNegative - (etaPositive*etaPositive) + 2*etaPositive*etaNegative - (etaNegative*etaNegative));
    this->eta[variableNode] = tuple<float, float, float>(etaNegative, etaZero, etaPositive);

    return difference;
}

tuple<float, float, float> FactorNode::GetEta(VariableNode *variableNode) {
    if (this->id==-1)
        return make_tuple(0,0,0);
    return eta[variableNode];

}

//Destructor
FactorNode::~FactorNode() {
    eta.clear();
    neighborhood.clear();
    neighborhoodMap.clear();
}

