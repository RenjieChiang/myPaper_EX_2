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
//        temp_DP_unit.best_permutation.push_back(vec);
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
        temp_DP_unit.best_permutation_0.push_back(vec);
        DP0Sequence_combine[getID(temp_DP_unit.best_permutation_0)] = temp_DP_unit;

        temp_DP_unit.best_permutation_0.clear();
        //1序列
        temp_DP_unit.best_time = T1;
        temp_DP_unit.best_miss = miss_1;
        temp_DP_unit.easy0time = miss_1 == 0? 0-T1 : 0;
        temp_DP_unit.easy1time = 0;
        temp_DP_unit.best_permutation_1.push_back(vec);
        DP1Sequence_combine[getID(temp_DP_unit.best_permutation_1)] = temp_DP_unit;
        temp_DP_unit.best_permutation_1.clear();
    }
    for(int i = 2;i < permutation_vector.size()+1; i++)
    {
        combine(permutation_vector,i);
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
    //最后更新序列0物体顺序所需要的信息
    int best_seq_0_i_exp = 0;//以i为抓取最速
    int best_seq_0_exp_from_ = 0;//记录最优序列是从0/1发展而来的
    int best_seq_0_exp_last_cat = 0;//记录最优序列是哪台机械臂抓取的目标物体
    float best_seq_0_exp_easy1time = 0;//更新该序列1的空闲时间

    float best_time_in_1 = 0;
    int best_miss_in_1 = 0;
    //最后更新序列1物体顺序所需要的信息
    int best_seq_1_i_exp = 0;//以i为抓取最速
    int best_seq_1_exp_from_ = 0;//记录最优序列是从0/1发展而来的
    int best_seq_1_exp_last_cat = 0;//记录最优序列是哪台机械臂抓取的目标物体
    float best_seq_1_exp_easy0time = 0;//更新该序列0的空闲时间

    int i = 0;

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
        if (best_time_in_0 == 0)//若是第一次更新本组合的best 0 time
        {
            best_time_in_0 = T020;
            best_miss_in_0 = miss_020;
            best_seq_0_i_exp = i;
            best_seq_0_exp_from_ = 0;
            best_seq_0_exp_last_cat = 0;
            best_seq_0_exp_easy1time = DP0Sequence_combine[id_temp_except_vec].easy1time - pnp;
        } else
        {
            if (miss_020 == best_miss_in_0)
            {
                if (T020 < best_time_in_0)
                {
                    best_time_in_0 = T020;
                    best_seq_0_i_exp = i;
                    best_seq_0_exp_from_ = 0;
                    best_seq_0_exp_last_cat = 0;
                    best_seq_0_exp_easy1time = DP0Sequence_combine[id_temp_except_vec].easy1time - pnp;
                }
            } else if (miss_020 < best_miss_in_0)
            {
                best_time_in_0 = T020;
                best_miss_in_0 = miss_020;
                best_seq_0_i_exp = i;
                best_seq_0_exp_from_ = 0;
                best_seq_0_exp_last_cat = 0;
                best_seq_0_exp_easy1time = DP0Sequence_combine[id_temp_except_vec].easy1time - pnp;
            }
        }


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
        if (best_time_in_1 == 0)//若是第一次更新本组合的best 1 time
        {
            best_time_in_1 = T121;
            best_miss_in_1 = miss_121;
            best_seq_1_i_exp = i;
            best_seq_1_exp_from_ = 1;
            best_seq_1_exp_last_cat = 1;
            best_seq_1_exp_easy0time = DP1Sequence_combine[id_temp_except_vec].easy0time - pnp;
        } else
        {
            if (miss_121 == best_miss_in_1)
            {
                if (T121 < best_time_in_1)
                {
                    best_time_in_1 = T121;
                    best_seq_1_i_exp = i;
                    best_seq_1_exp_from_ = 1;
                    best_seq_1_exp_last_cat = 1;
                    best_seq_1_exp_easy0time = DP1Sequence_combine[id_temp_except_vec].easy0time - pnp;
                }
            } else if (miss_121 < best_miss_in_1)
            {
                best_time_in_1 = T121;
                best_miss_in_1 = miss_121;
                best_seq_1_i_exp = i;
                best_seq_1_exp_from_ = 1;
                best_seq_1_exp_last_cat = 1;
                best_seq_1_exp_easy0time = DP1Sequence_combine[id_temp_except_vec].easy0time - pnp;
            }
        }



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

            if (miss_120 == best_miss_in_0)
            {
                if (T120 < best_time_in_0)
                {
                    best_time_in_0 = T120;
                    best_seq_0_i_exp = i;
                    best_seq_0_exp_from_ = 1;
                    best_seq_0_exp_last_cat = 0;
                    best_seq_0_exp_easy1time = -T120 + DP1Sequence_combine[id_temp_except_vec].best_time;
                }
            } else if (miss_120 < best_miss_in_0)
            {
                best_time_in_0 = T120;
                best_miss_in_0 = miss_120;
                best_seq_0_i_exp = i;
                best_seq_0_exp_from_ = 1;
                best_seq_0_exp_last_cat = 0;
                best_seq_0_exp_easy1time = -T120 + DP1Sequence_combine[id_temp_except_vec].best_time;
            }
        } else
        {
            flag120 = 1;//属于序列1，即总时间没变，只是用了easy0time或者是miss了
//            T120 = DP1Sequence_combine[id_temp_except_vec].best_time;
            if (miss_120 == best_miss_in_1)
            {
                if (DP1Sequence_combine[id_temp_except_vec].best_time < best_time_in_1)
                {
                    best_time_in_1 = DP1Sequence_combine[id_temp_except_vec].best_time;
                    best_seq_1_i_exp = i;
                    best_seq_1_exp_from_ = 1;
                    best_seq_1_exp_last_cat = 0;
                    best_seq_1_exp_easy0time = T120 - DP1Sequence_combine[id_temp_except_vec].best_time ;
                }
            } else if (miss_120 < best_miss_in_1)
            {
                best_time_in_1 = DP1Sequence_combine[id_temp_except_vec].best_time ;
                best_miss_in_1 = miss_120;
                best_seq_1_i_exp = i;
                best_seq_1_exp_from_ = 1;
                best_seq_1_exp_last_cat = 0;
                best_seq_1_exp_easy0time = T120 - DP1Sequence_combine[id_temp_except_vec].best_time;
            }
        }

        //计算1序列,即试图使用1抓这个物体



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
            if (miss_021 == best_miss_in_1)
            {
                if (T021 < best_time_in_1)
                {
                    best_time_in_1 = T021;
                    best_seq_1_i_exp = i;
                    best_seq_1_exp_from_ = 0;
                    best_seq_1_exp_last_cat = 1;
                    best_seq_1_exp_easy0time = -T021 + DP0Sequence_combine[id_temp_except_vec].best_time;
                }
            } else if (miss_021 < best_miss_in_1)
            {
                best_miss_in_1 = miss_021;
                best_time_in_1 = T021;
                best_seq_1_i_exp = i;
                best_seq_1_exp_from_ = 0;
                best_seq_1_exp_last_cat = 1;
                best_seq_1_exp_easy0time = -T021 + DP0Sequence_combine[id_temp_except_vec].best_time;
            }

        } else
        {
            flag021 = 0;
//            T021 = DP0Sequence_combine[id_temp_except_vec].best_time;
            if (miss_021 == best_miss_in_0)
            {
                if (DP0Sequence_combine[id_temp_except_vec].best_time < best_time_in_1)
                {
                    best_time_in_0 = DP0Sequence_combine[id_temp_except_vec].best_time;
                    best_seq_0_i_exp = i;
                    best_seq_0_exp_from_ = 0;
                    best_seq_0_exp_last_cat = 1;
                    best_seq_0_exp_easy1time = T021 - DP0Sequence_combine[id_temp_except_vec].best_time ;
                }
            } else if (miss_021 < best_miss_in_0)
            {
                best_miss_in_0 = miss_021;
                best_time_in_0 = DP0Sequence_combine[id_temp_except_vec].best_time;
                best_seq_0_i_exp = i;
                best_seq_0_exp_from_ = 0;
                best_seq_0_exp_last_cat = 1;
                best_seq_0_exp_easy1time = T021 - DP0Sequence_combine[id_temp_except_vec].best_time ;
            }
        }


    i++;
    }

    //循环结束，开始更新该组合最速序列

    //更新0序列
    int id_from = id_temp - pow(2, temp_[best_seq_0_i_exp].number);
    DP_Sequence temp_seq;
    temp_seq.best_time = best_time_in_0;
    temp_seq.best_miss = best_miss_in_0;
    temp_seq.number_objects = temp_.size();
    temp_seq.easy1time = best_seq_0_exp_easy1time;
    if (best_seq_0_exp_from_ == 1)
    {
        temp_seq.best_permutation_0 = DP1Sequence_combine[id_from].best_permutation_0;
        temp_seq.best_permutation_1 = DP1Sequence_combine[id_from].best_permutation_1;
        temp_seq.best_permutation_0.push_back(temp_[best_seq_0_i_exp]);
    } else
    {
        temp_seq.best_permutation_0 = DP0Sequence_combine[id_from].best_permutation_0;
        temp_seq.best_permutation_1 = DP0Sequence_combine[id_from].best_permutation_1;
        if (best_seq_0_exp_last_cat == 0)
        {
            temp_seq.best_permutation_0.push_back(temp_[best_seq_0_i_exp]);
        } else
        {
            temp_seq.best_permutation_1.push_back(temp_[best_seq_0_i_exp]);
        }
    }

    // 更新1序列
    id_from = id_temp - pow(2, temp_[best_seq_1_i_exp].number);
    DP_Sequence temp_seq1;
    temp_seq1.best_time = best_time_in_1;
    temp_seq1.best_miss = best_miss_in_1;
    temp_seq1.number_objects = temp_.size();
    temp_seq1.easy1time = best_seq_1_exp_easy0time;
    if (best_seq_1_exp_from_ == 0)
    {
        temp_seq1.best_permutation_0 = DP0Sequence_combine[id_from].best_permutation_0;
        temp_seq1.best_permutation_1 = DP0Sequence_combine[id_from].best_permutation_1;
        temp_seq1.best_permutation_1.push_back(temp_[best_seq_1_i_exp]);
    } else
    {
        temp_seq1.best_permutation_0 = DP1Sequence_combine[id_from].best_permutation_0;
        temp_seq1.best_permutation_1 = DP1Sequence_combine[id_from].best_permutation_1;
        if (best_seq_1_exp_last_cat == 0)
        {
            temp_seq1.best_permutation_0.push_back(temp_[best_seq_1_i_exp]);
        } else
        {
            temp_seq1.best_permutation_1.push_back(temp_[best_seq_1_i_exp]);
        }
    }


    return 0;
}










