#include <iostream>
#include "SurveyPropagation.h"
#include "climits"
#include <string>


using namespace std;

auto n = 11;
auto fS = 0;
auto fSwC = 0;
auto fSC = 0;
auto maxIterationNumber = 500;
auto obsNumber = 5;
auto startObs = 1;
float epsilon = 0.001;
float alpha = 0.5;


void PrintCommandLine(int argc, char* argv[]) {
    printf("#");
    for (int i = 0; i < argc; ++i) {
        printf("%s ",argv[i]);
    }
    printf("\n");
}

bool ParseCommandLine(int argc, char* argv[]) {
    try {
        for (int index = 1; index < argc; index+=2) {
            string paramName = argv[index];
            char* paramValue = argv[index + 1];

            if (paramName == "-h" || paramName == "-help")
            {
                printf("The options are:\n");
                printf("-h(elp)         Show this help.\n");
                printf("-n()            Set the length of the sequence (default: %d)\n", n);
                printf("-e(psilon)      Set the minimum error (default %f)\n", epsilon);
                printf("-i(teration)    Set the maximum iteration number (default: %d)\n", maxIterationNumber);
                printf("-a(lpha)        Set the dumping factor (default: %f)\n", alpha);
                printf("-t(imes)        Set the number of Observations (default: %d)\n", obsNumber);
                printf("-sO(startObs)   Set the seed to start\n");
                printf("-g(amma)        Set the level of disorder in the system\n");
                printf("-G(amma)        Set the level of disorder in the system is a int in [0...100] where 100 is LABSP\n");
                printf("-setModel       Set the model, constructed by the -sO parameter\n");
                printf("-modelData      Get information about the model (energies values and local minimum)\n");
                printf("-modelDataHE    Get information about the model (energies values and local minimum in hamming intervals.)\n");
                printf("-printGraph     Print the factor graph in GraphML format)\n");
                printf("-PSpin          Use the P-Spin model\n");
                printf("-MeanField      Use the Mean Field model (this is the default option)\n");
                printf("-fS             Amount of Fixed Spin\n");
                printf("-fSwC           Amount of Fixed Spin with number code\n");
                printf("-fSC            The number code of the amount of fixed spin\n");
                return false;
            }
            else if (paramName == "-n")
                n = (unsigned int) atoi(paramValue);
            else if (paramName == "-fS")
                fS = (unsigned int) atoi(paramValue);
            else if (paramName == "-fSwC")
                fSwC = (unsigned int) atoi(paramValue);
            else if (paramName == "-fSC")
                fSC = (unsigned int) atoi(paramValue);
            else if (paramName == "-e")
                epsilon = (float)atof(paramValue);
            else if (paramName == "-i")
                maxIterationNumber = atoi(paramValue);
            else if (paramName == "-a")
                alpha = (float)atof(paramValue);
            else if (paramName == "-t")
                obsNumber = atoi(paramValue);
            else if (paramName == "-sO")
                startObs = (unsigned int)atoi(paramValue);
        }
    }
    catch (exception exception) {
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {

    if (!ParseCommandLine(argc, argv)) {
        printf("Bad format parameters. Check the options with -h(elp)\n");
        return 1;
    }

    PrintCommandLine(argc, argv);
    printf("\n");

    auto groundState_20 = {1, 1, 1, 1, 1, -1, 1, -1, -1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, -1};
    auto groundState_30 = {1, 1, 1, 1, 1, -1, -1, -1, -1, -1, 1, -1, -1, 1, -1, -1, 1, -1, 1, -1, 1, -1, -1, -1, 1, 1,
                           -1, -1, -1, 1};
    auto groundState_35 = {1, 1, 1, 1, 1, 1, 1, -1, 1, 1, -1, -1, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, -1, 1, -1, 1, -1, 1, 1, 1, -1, -1, -1, 1, 1};

    auto groundState_40 = {1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, -1, -1, 1, -1, 1, 1,
                           -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1};
    auto groundState_50 = {1, 1, -1, 1, 1, 1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, -1, -1, 1,
                           -1, 1, 1, -1, -1, 1, 1, 1, 1, -1, 1, -1, -1, -1, -1, 1, -1, 1, 1, 1, 1, -1, -1};
    auto groundState_60 = {1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, 1, -1, 1, -1, -1, 1, -1, -1, -1, -1, 1, -1, 1,
                           -1, 1, -1, -1, -1, 1, -1, 1, 1, -1, -1, -1, -1, 1, 1, -1, 1, -1, -1, -1, 1, 1, -1, -1, 1, 1,
                           -1, 1, -1, -1, 1, 1, -1, -1};
    auto groundState_66 = {1, 1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, -1, -1, -1, -1, -1, -1,
                           -1, -1, -1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1,
                           -1, -1, -1, 1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1};
    auto groundState_67 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, 1, -1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1, -1,
                           -1, -1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, 1, -1, -1, 1, -1, -1, 1, 1, 1, -1, 1,
                           -1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};

    vector<pair<int, vector<int>>> groundStates = {
            make_pair(67, groundState_67),
            make_pair(66, groundState_66),
            make_pair(60, groundState_60),
            make_pair(50, groundState_50),
            make_pair(40, groundState_40),
            make_pair(35, groundState_35),
            make_pair(30, groundState_30),
            make_pair(20, groundState_20)};

//    for (int n = 60; n <= 66; n+=6) {

        pair<int, vector<int>> item = pair<int, vector<int>>(-1, {});
        for (auto _item : groundStates)
            if (_item.first == n) {
                item = _item;
                break;
            }

        for (int fS = 0; fS < n; ++fS) {

            for (auto seed = startObs; seed < startObs + obsNumber; ++seed) {
                auto sp = new SurveyPropagation(n, maxIterationNumber, epsilon, alpha, seed, DEBUG_MIN);
                sp->SetInitConfiguration(item.second, fS);

                pair<Exit, vector<int>> result = sp->ConvergeSP();
            }
        }
        printf("\n\n");
//    }
}
