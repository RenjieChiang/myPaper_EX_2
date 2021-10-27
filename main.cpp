#include <iostream>
#include <ctime>
#include "vector"

#include "Convey_2arm.h"
#include "Permutation_two_arms.h"
#include "DP_Permutation.h"

#include "classic_strategy.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    // initialize
    float T = 0;//time now
    int number_picked = 0;//number of object which have picked
    clock_t start,end;//time of program running exhaust
    Convey_2arm myConvey(2);

    //permutation
//    start = clock();
//    Permutation_two_arms myBest;
//    myBest.getPermutationTotal(myConvey.convey_object);
//
//    for (auto obj : myBest.best_permutation_total)
//    {
//        std::cout<< obj.x << "  " << obj.y << std::endl;
//    }
//    std::cout << " best time "<< myBest.best_time << "   best miss  " << myBest.best_missing << std::endl;
//
//    std::cout << " for 0 arm: " << std::endl;
//    for (auto obj : myBest.best_0_per)
//    {
//        std::cout<< obj.x << "  " << obj.y << std::endl;
//    }
//
//    std::cout << " for 1 arm: " << std::endl;
//    for (auto obj : myBest.best_1_per)
//    {
//        std::cout<< obj.x << "  " << obj.y << std::endl;
//    }
//
//    end = clock();
//    std::cout<< std::endl << "time needed Permutation : " << end-start << "ms" << std::endl;

    std::cout << " dp : " << std::endl;
    start = clock();
    DP_Permutation myDPBest;
    myDPBest.getPermutationTotal(myConvey.convey_object);

    for (auto obj : myDPBest.best_permutation_total)
    {
        std::cout<< obj.x << "  " << obj.y << std::endl;
    }
    std::cout << " best time "<< myDPBest.best_time << "   best miss  " << myDPBest.best_missing << std::endl;

    std::cout << " for 0 arm: " << std::endl;
    for (auto obj : myDPBest.best_0_per)
    {
        std::cout<< obj.x << "  " << obj.y << std::endl;
    }

    std::cout << " for 1 arm: " << std::endl;
    for (auto obj : myDPBest.best_1_per)
    {
        std::cout<< obj.x << "  " << obj.y << std::endl;
    }
    end = clock();
    std::cout<< std::endl << "time needed Permutation : " << end-start << "ms" << std::endl;

    classic_strategy mySPT(myConvey.convey_object, 0);
    mySPT.pick();
    classic_strategy myFIFO(myConvey.convey_object, 1);
    myFIFO.pick();
    std::cout << " ";


    return 0;
}
