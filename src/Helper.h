//
// Created by landy on 17/12/16.
//

#ifndef SP_HELPER_H
#define SP_HELPER_H

#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <tuple>

using namespace std;

void usrand (unsigned seed);
unsigned urand0 (void);
unsigned urand (void);
int randint(int upto);
double randreal();

#define MAX_URAND 0xFFFFFFFFL

tuple<float, float, float> ThreeRandomNormalizedNumber();
vector<int> JoinAndFilter(vector<int> list);
int EnergyLABS(vector<int> configuration);
vector<int> ComputeEnergyWithEmptySpaces(vector<int> configuration);
vector<int> RandomDifferentInt(int length, int max);
vector<int> Combinations_ComputeEnergyWithEmptySpaces(vector<int> configuration, vector<int> pos, int current_pos, vector<int> energies);
vector<vector<int>> AllCombinationWithLength_Combination(vector<vector<int>> combinations, vector<int> combination, int length, int pos);
vector<vector<int>> AllCombinationWithLength(int n);
vector<int> NumberToConfiguration(int n, int length);


#endif //SP_HELPER_H

