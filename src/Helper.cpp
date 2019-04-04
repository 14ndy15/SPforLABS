//
// Created by landy on 17/12/16.
//

#include "Helper.h"
#include <time.h>
#include <cmath>


/*
	urand(), urand0() return uniformly distributed unsigned random ints
		all available bits are random, e.g. 32 bits on many platforms
	usrand(seed) initializes the generator
		a seed of 0 uses the current time as seed

	urand0() is the additive number generator "Program A" on p.27 of Knuth v.2
	urand() is urand0() randomized by shuffling, "Algorithm B" on p.32 of Knuth v.2
	urand0() is one of the fastest known generators that passes randomness tests
	urand() is somewhat slower, and is presumably better
*/

static unsigned rand_x[56], rand_y[256], rand_z;
static int rand_j, rand_k;

void usrand (unsigned seed)
{
    int j;

    rand_x[1] = 1;
    if (seed) rand_x[2] = seed;
    else rand_x[2] = time (NULL);
    for (j=3; j<56; ++j) rand_x[j] = rand_x[j-1] + rand_x[j-2];

    rand_j = 24;
    rand_k = 55;
    for (j=255; j>=0; --j) urand0 ();
    for (j=255; j>=0; --j) rand_y[j] = urand0 ();
    rand_z = urand0 ();
}

unsigned urand0 (void)
{
    if (--rand_j == 0) rand_j = 55;
    if (--rand_k == 0) rand_k = 55;
    return rand_x[rand_k] += rand_x[rand_j];
}

unsigned urand (void)
{
    int j;

    j =  rand_z >> 24;
    rand_z = rand_y[j];
    if (--rand_j == 0) rand_j = 55;
    if (--rand_k == 0) rand_k = 55;
    rand_y[j] = rand_x[rand_k] += rand_x[rand_j];
    return rand_z;
}

int randint(int upto)
//gives a random integer uniformly in [0..upto - 1]
{
    upto = upto - 1;
	return((float)upto*(float)rand())/(RAND_MAX+1.0);
    return(((double)upto*(double)urand())/((double)MAX_URAND+1.0));
}

double randreal()
//gives a random integer uniformly in (0,1)
{
//	return (rand()/(RAND_MAX+1.0));
    return (urand()/(double)((double)MAX_URAND+1.0));
}

vector<int> JoinAndFilter(vector<int> list) {

    vector<int> realList = vector<int>();
    sort(list.begin(), list.end());

    for (int i = 0; i < 4; ++i) {
        if (i < (list.size() - 1) and list[i] != list[i+1]) {
            realList.push_back(list[i]);
        }
        else if (i == list.size() - 1)
            realList.push_back(list[i]);
        else
            i+=1;
    }

    return realList;
}

tuple<float, float, float> ThreeRandomNormalizedNumber() {
    auto number1 = (float)randreal();
    auto number2 = (float)randreal();
    auto number3 = (float)randreal();

    float Z = number1+number2+number3;

    number1 /= Z;
    number2 /= Z;
    number3 /= Z;

    assert(!isnan(number1) and !isnan(number2) and !isnan(number3));
    assert(fabs(number1+number2+number3 - 1) < 0.001);

    return tuple<float, float, float>(number1, number2, number3);
}

int EnergyLABS(vector<int> configuration) {
    int energy = 0;
    for (int k = 1; k < (int)configuration.size(); ++k) {
        int correlation = 0;
        for (int i = 0; i < (int)configuration.size() - k; ++i) {
            correlation += configuration[i]*configuration[i+k];
        }
        energy += (correlation*correlation);
    }
    return energy;
}

vector<int> Combinations_ComputeEnergyWithEmptySpaces(vector<int> configuration, vector<int> pos, int current_pos, vector<int> energies) {

    if (current_pos == pos.size()) {
        int energy = EnergyLABS(configuration);
        energies.push_back(energy);
        return energies;
    }
    else{
        configuration[pos[current_pos]] = 1;
        energies = Combinations_ComputeEnergyWithEmptySpaces(configuration, pos, current_pos+1, energies);
        configuration[pos[current_pos]] = -1;
        return Combinations_ComputeEnergyWithEmptySpaces(configuration, pos, current_pos+1, energies);
    }

}

vector<int> ComputeEnergyWithEmptySpaces(vector<int> configuration) {
    vector<int> pos = vector<int>();
    vector<int> energies = vector<int>();

    for(int i = 0; i<configuration.size(); i++)
        if(configuration[i] == 0)
            pos.push_back(i);

    energies = Combinations_ComputeEnergyWithEmptySpaces(configuration, pos, 0, energies);
    return energies;
}

vector<int> RandomDifferentInt(int length, int max){
    auto list = vector<int>();
    while (list.size() < length){
        auto random = randint(max);
        auto contain = false;
        for (auto item : list)
            if (item == random) {
                contain = true;
                break;
            }
        if (!contain)
            list.emplace_back(random);
    }
        assert(list.size() == length);
    return list;
}

vector<vector<int>> AllCombinationWithLength_Combination(vector<vector<int>> combinations, vector<int> combination, int length, int pos){
    if (pos==length)
    {
        combinations.push_back(combination);
        return combinations;
    }
    else{
        combination[pos] = 1;
        combinations = AllCombinationWithLength_Combination(combinations, combination, length, pos+1);
        combination[pos] = -1;
        return AllCombinationWithLength_Combination(combinations, combination, length, pos+1);
    }
}

vector<vector<int>> AllCombinationWithLength(int n){
    auto combinations = vector<vector<int>>();
    auto combination = vector<int>();
    for(auto i = 0; i< n; i++)
        combination.emplace_back(1);
    combinations = AllCombinationWithLength_Combination(combinations, combination, n, 0);

        double amount = pow(2.0,n*1.0);
        auto amount_int = (int)round(amount);
        assert(combinations.size() == amount_int);

//    auto combinationsFilter = vector<vector<int>>();
//    for (int i =0; i<combinations.size()/2; i++)
//        combinationsFilter.push_back(combinations[i]);

//    return combinationsFilter;
    return combinations;
}

vector<int> NumberToConfiguration(int n, int length){
    auto configuration = vector<int>();
    for (int i =0; i< length; i++)
        configuration.push_back(1);

    int pos = configuration.size() - 1;
    while (n!=0 && n!=1){
        if (n%2 == 1)
            configuration[pos] = -1;
        n = n/2;
        pos--;
    }
    if (n == 1)
        configuration[pos] = -1;

    return configuration;
}