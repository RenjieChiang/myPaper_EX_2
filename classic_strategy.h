//
// Created by 10372 on 2021/10/25.
//

#ifndef MYPAPER_EX_2_CLASSIC_STRATEGY_H
#define MYPAPER_EX_2_CLASSIC_STRATEGY_H

#include "vector"
#include "Convey_2arm.h"

class classic_strategy {
public:
    float T0, T1, T;//T 为 二者最小,也即是每次考虑抓哪个的时间
    int mode ; // 0 is SPT , 1 is FIFO
    int miss;
    std::vector<object> objects_to_pick_;
    std::vector<object> objects_in_workspace0;//0号机工作视野内的物体
    std::vector<object> objects_in_workspace1;

    explicit classic_strategy(std::vector<object> object_to_pick, int mode_ = 0);

    std::vector<object>::iterator getSPT(std::vector<object> &objects, float t_now);

    std::vector<object>::iterator getFIFO(std::vector<object> &objects, float t_now);


    int workspace_update();
    int pick();


    float minDistCal(object &object, float now);
};


#endif //MYPAPER_EX_2_CLASSIC_STRATEGY_H
