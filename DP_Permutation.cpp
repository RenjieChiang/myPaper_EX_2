//
// Created by 10372 on 2021/10/16.
//

#include "DP_Permutation.h"

int DP_Permutation::getPermutationTotal(std::vector<object> permutation_vector)
{
    float T = 0;
    DP_Sequence temp_DP_unit;
    //计算DP初始数据，即一个元素的组合的最速序列
    for (auto vec : permutation_vector)
    {
        temp_DP_unit.number_objects = 1;
        temp_DP_unit.best_permutation.push_back(vec);
        float T0 = 0,T1 = 0;
        int miss_0 = 0, miss_1 =0;

        //用0抓
        float pnp = getPNPtime0(vec,T);
        if (pnp > 0)
        {
            T0 += pnp;
        } else
        {
            miss_0++;
        }
        //用1抓
        pnp = getPNPtime1(vec,T);
        if (pnp > 0)
        {
            T1 += pnp;
        } else
        {
            miss_1++;
        }

        //构造初始的0 1 序列
        //0序列
        temp_DP_unit.best_time = T0;
        temp_DP_unit.best_miss = miss_0;
        temp_DP_unit.easy0time = 0;
        temp_DP_unit.easy1time = miss_0 == 0? 0-T0 : 0;
        DP0Sequence_combine[getID(temp_DP_unit.best_permutation)] = temp_DP_unit;

        //1序列
        temp_DP_unit.best_time = T1;
        temp_DP_unit.best_miss = miss_1;
        temp_DP_unit.easy0time = miss_1 == 0? 0-T1 : 0;
        temp_DP_unit.easy1time = 0;
        DP0Sequence_combine[getID(temp_DP_unit.best_permutation)] = temp_DP_unit;
    }



    return 0;
}

int DP_Permutation::getID(std::vector<object> objects)
{
    int result = 0;
    for (auto vec : objects)
    {
        result += pow(2,vec.number);
    }
    return result;
}

int DP_Permutation::combine(const std::vector<object> &source, int index)
{
    int depth = 0;
    std::vector<object> temp(index,0);
    combine_inner(source, 0, source.size(), index, depth, temp);
    return 0;
}

void DP_Permutation::combine_inner(const std::vector<object> &data, int start, int n, int m, int depth,
                                   std::vector<object> temp){
    if (depth == m - 1)
    {
        //最内层循环 将temp加入result
        for (int i = start; i < n - (m - depth - 1); ++i)
        {
            temp[depth] = data[i];
            update(temp, data);
        }
    }
    else
        for (int i = start; i < n - (m - depth - 1);++i)
        {
            temp[depth] = data[i];//每层输出一个元素
            combine_inner(data,i + 1, n, m, depth+1,temp);
        }

}

int DP_Permutation::update(const std::vector<object> &temp_, const std::vector<object> &source)
{
    int id_temp = getID(temp_);
    //计算temp中，以i为结尾的最速时间
    float best_time_in_0 = 0;
    int best_miss_in_0 = 0;
    float best_time_in_1 = 0;
    int best_miss_in_1 = 0;
    std::vector<object> best_seq_in_0_arm0;
    std::vector<object> best_seq_in_0_arm1;
    std::vector<object> best_seq_in_1_arm0;
    std::vector<object> best_seq_in_1_arm1;
    for (auto vec : temp_)
    {
        int id_temp_except_vec = id_temp - (int)pow(2,vec.number);

        //计算0序列,即试图使用0抓这个物体

        //用上次的0序列更新
        float T020 = DP0Sequence_combine[id_temp_except_vec].best_time;
        int miss_020 = DP0Sequence_combine[id_temp_except_vec].best_miss;
        //用0抓
        float pnp = getPNPtime0(vec,T020);
        if (pnp > 0)
        {
            T020 += pnp;
        } else
        {
            miss_020++;
        }
        int flag020 = 0;//肯定还是属于0序列

        //用上次的1序列更新
        float T120 = DP1Sequence_combine[id_temp_except_vec].best_time;
        int miss_120 = DP1Sequence_combine[id_temp_except_vec].best_miss;
        //用0抓
        pnp = 0;
        pnp = getPNPtime0(vec,T120 + DP1Sequence_combine[id_temp_except_vec].easy0time);
        if (pnp > 0)
        {
            T120 = T120 + pnp + DP1Sequence_combine[id_temp_except_vec].easy0time;
        } else
        {
            miss_120++;
        }
        int flag120;
        if (T120 > DP1Sequence_combine[id_temp_except_vec].best_time)
        {
            flag120 = 0;//属于0序列结果
        } else
        {
            flag120 = 1;
//            T120 = DP1Sequence_combine[id_temp_except_vec].best_time;

        }

        //计算1序列,即试图使用1抓这个物体

        //用上次的1序列更新
        float T121 = DP1Sequence_combine[id_temp_except_vec].best_time;
        int miss_121 = DP1Sequence_combine[id_temp_except_vec].best_miss;
        //用1抓
        pnp = 0;
        pnp = getPNPtime1(vec,T121);
        if (pnp > 0)
        {
            T121 = T121 + pnp;
        } else
        {
            miss_121++;
        }
        int flag121 = 1;

        //用上次的0序列更新
        float T021 = DP0Sequence_combine[id_temp_except_vec].best_time;
        int miss_021 = DP0Sequence_combine[id_temp_except_vec].best_miss;
        //用1抓
        pnp = 0;
        pnp = getPNPtime1(vec,T021+ DP0Sequence_combine[id_temp_except_vec].easy1time);
        if (pnp > 0)
        {
            T021 = T021 + pnp + DP0Sequence_combine[id_temp_except_vec].easy1time;
        } else
        {
            miss_021++;
        }
        int flag021 ;
        if (T021 > DP0Sequence_combine[id_temp_except_vec].best_time)
        {
            flag021 = 1;//属于1序列结果
        } else
        {
            flag021 = 0;
//            T021 = DP0Sequence_combine[id_temp_except_vec].best_time;
        }

        DP_Sequence temp_DP_unit;


        //开始真正的比较
        //建立 0和1序列的最佳记录

        //三个0序列取最优的情况
//        if (flag021 == 0 && flag120 == 0)
//        {
//            //将121数据更新至1序列
//            temp_DP_unit.best_time = T121;
//            temp_DP_unit.best_miss = miss_121;
//            temp_DP_unit.easy1time = 0;
//            temp_DP_unit.easy0time = DP1Sequence_combine[id_temp_except_vec].easy0time + DP1Sequence_combine[id_temp_except_vec].best_time - T121;
//            DP1Sequence_combine[id_temp] = temp_DP_unit;
//
//            if (miss_020<miss_021 && miss_020<miss_120)
//            {
//                //将020的数据更新至0序列
//                temp_DP_unit.best_time = T020;
//                temp_DP_unit.best_miss = miss_020;
//                temp_DP_unit.easy0time = 0;
//                temp_DP_unit.easy1time = DP0Sequence_combine[id_temp_except_vec].easy1time + DP0Sequence_combine[id_temp_except_vec].best_time - T020;
//                DP0Sequence_combine[id_temp] = temp_DP_unit;
//            }
//            else if (miss_021<miss_020 && miss_021<miss_120)
//            {
//                //将021的数据更新至0序列
//                temp_DP_unit.best_time = DP0Sequence_combine[id_temp_except_vec].best_time;
//                temp_DP_unit.best_miss = miss_021;
//                temp_DP_unit.easy0time = 0;
//                temp_DP_unit.easy1time =  -DP0Sequence_combine[id_temp_except_vec].best_time + T021;
//                DP0Sequence_combine[id_temp] = temp_DP_unit;
//            }
//        }
    }
    return 0;
}










