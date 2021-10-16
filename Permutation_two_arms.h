//
// Created by 10372 on 2021/9/25.
//

#ifndef MYPAPER_EX_2_PERMUTATION_TWO_ARMS_H
#define MYPAPER_EX_2_PERMUTATION_TWO_ARMS_H

#include "vector"
#include "Convey_2arm.h"


class Permutation_two_arms {
public:
    std::vector<object> best_permutation_total;
    std::vector<object> best_0_per;
    std::vector<object> best_1_per;
    int best_missing;
    float best_time;

    Permutation_two_arms():best_missing(0),best_time(0){};
    int getPermutationTotal(std::vector<object> permutation_vector);
    int findBest(const  std::vector<object> & permutation_2arm);
    int combine(const std::vector<object> &source, int index);//求组合数量为index的全部组合
    void combine_inner(const std::vector<object> &data, int start, int n, int m, int depth, std::vector<object> temp);
    int findBest_inner(const std::vector<object>& temp_, const std::vector<object> &source);//对某一个双机械臂抓取组合进行时间计算，以及与最快策略进行比较并更新


};


#endif //MYPAPER_EX_2_PERMUTATION_TWO_ARMS_H
