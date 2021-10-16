//
// Created by 10372 on 2021/9/25.
//

#include "Permutation_two_arms.h"

int cmp(object A, object B)
{
    return A.x > B.x;
}

// use STL next_permutation and assume objects are made from a sequence in x-axis
int Permutation_two_arms::getPermutationTotal(std::vector<object> permutation_vector)
{
    while (std::next_permutation(permutation_vector.begin(), permutation_vector.end(), cmp))
    {
        findBest(permutation_vector);//找出该总排序的最优双臂抓取策略
    }

    return 0;
}

int Permutation_two_arms::findBest(const std::vector<object> &permutation_2arm)
{
    for (int i = 1; i < permutation_2arm.size()+1; ++i)
    {
        combine(permutation_2arm,i);
    }
    return 0;
}

int Permutation_two_arms::combine(const std::vector<object> & source, int index)
{
    int depth = 0;
    std::vector<object> temp(index,0);
    combine_inner(source, 0, source.size(), index, depth, temp);
    return 0;
}

void Permutation_two_arms::combine_inner(const std::vector<object> &data, int start, int n, int m, int depth,
                                         std::vector<object> temp){
    if (depth == m - 1)
    {
        //最内层循环 将temp加入result
        for (int i = start; i < n - (m - depth - 1); ++i)
        {
            temp[depth] = data[i];
            findBest_inner(temp, data);
        }
    }
    else
        for (int i = start; i < n - (m - depth - 1);++i)
        {
            temp[depth] = data[i];//每层输出一个元素
            combine_inner(data,i + 1, n, m, depth+1,temp);
        }

}

int Permutation_two_arms::findBest_inner(const std::vector<object>& temp_, const std::vector<object> &source)
{
    std::vector<object> sequence1;
    auto iter = temp_.begin();
    // 将 0号机 和 1号机 的抓取序列分离出来 temp ---> 0号
    for (auto & ob: source)
    {
        if (ob.x == iter->x)
        {
            iter++;
        } else
        {
            sequence1.push_back(ob);
        }
    }

    int miss_0 = 0;
    float time_0 = 0;
    float T = 0;
    //计算0号的结果
    for (auto &ob : temp_)
    {
       float pnp = getPNPtime0(ob,T);
        if (pnp > 0)
        {
            T += pnp;
        } else
        {
            miss_0++;
        }
    }
    time_0 = T;
    // 计算1号的结果
    T = 0;
    int miss_1 = 0;
    float time_1 = 0;
    for (auto &ob : sequence1)
    {
        float pnp = getPNPtime1(ob,T);
        if (pnp > 0)
        {
            T += pnp;
        } else
        {
            miss_1++;
        }
    }
    time_1 = T;

    //进行总结、比对、更新
    int miss = miss_0 + miss_1;
    float time = time_1>time_0 ? time_1 : time_0;
    if (best_time == 0)
    {
        best_time = time;
        best_missing = miss;
        best_permutation_total = source;
        best_0_per = temp_;
        best_1_per = sequence1;
    } else
    {
        if (miss == best_missing)
        {
            if (time < best_time)
            {
                best_time = time;

                best_permutation_total.clear();
                best_permutation_total = source;
                best_0_per = temp_;
                best_1_per = sequence1;
            }
        } else if (miss < best_missing)
        {
            best_time = time;
            best_missing = miss;
            best_permutation_total.clear();
            best_permutation_total = source;
            best_0_per = temp_;
            best_1_per = sequence1;
        }
    }

    return 0;
}

