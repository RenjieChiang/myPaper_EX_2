//
// Created by 10372 on 2021/10/16.
//


#ifndef MYPAPER_EX_2_DP_PERMUTATION_H
#define MYPAPER_EX_2_DP_PERMUTATION_H

#include "vector"
#include "Convey_2arm.h"
#include "unordered_map"

//该结构体代表一个最速组合，在DP过程中记录
struct DP_Sequence
{
    int number_objects;//组合中元素的个数
    std::vector<object> best_permutation;
    float best_time;
    int best_miss;

    //记录相比较最佳时间，双臂最后一个任务完成时间，用负差值表示
    float easy0time;
    float easy1time;

    DP_Sequence() : number_objects(0),best_time(0),best_miss(0),easy0time(0),easy1time(0){};
};

class DP_Permutation {
public:
    std::vector<object> best_permutation_total;
    std::vector<object> best_0_per;
    std::vector<object> best_1_per;
    int best_missing;
    float best_time;

//    std::vector<DP_Sequence> DP1Sequence_combine;//记录以1机械臂结束作业的各个组合的最优序列
//    std::vector<DP_Sequence> DP0Sequence_combine;//记录以0机械臂结束作业的各个组合的最优序列

    std::unordered_map<int, DP_Sequence> DP1Sequence_combine;//记录以1机械臂结束作业的各个组合的最优序列
    std::unordered_map<int, DP_Sequence> DP0Sequence_combine;//记录以0机械臂结束作业的各个组合的最优序列
    //key为组合的二进制转为十进制

    DP_Permutation():best_missing(0),best_time(0){};
    int getPermutationTotal(std::vector<object> permutation_vector);//总接口


    int getID(std::vector<object> objects);//返回的是objects的物体序号得来的二进制数转换的十进制整数，昨晚哈希存储的id

    //实现组合
    int combine(const std::vector<object> &source, int index);//求组合数量为index的全部组合
    void combine_inner(const std::vector<object> &data, int start, int n, int m, int depth, std::vector<object> temp);

    //进行update
    int update(const std::vector<object>& temp_, const std::vector<object> &source);


};


#endif //MYPAPER_EX_2_DP_PERMUTATION_H
