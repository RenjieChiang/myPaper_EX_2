//
// Created by 10372 on 2021/9/25.
//

#ifndef MYPAPER_EX_2_CONVEY_2ARM_H
#define MYPAPER_EX_2_CONVEY_2ARM_H

#include "algorithm"
#include "list"
#include "cmath"
#include "vector"

#define CONVEY_SPEED 1
#define ARM_SPEED  1

#define MAX_EDGE_0  0
#define MIN_EDGE_0  -6
#define MAX_EDGE_1  6
#define MIN_EDGE_1  0

#define ARM_POSITION_0 -3
#define ARM_POSITION_1 3


struct object
{
    float x;
    float y;
    object(float a = 0 ):x(0),y(0) {};
};

float getPNPtime0(object ready_to_pick, float time_now, float convey_speed = CONVEY_SPEED);
float getPNPtime1(object ready_to_pick, float time_now, float convey_speed = CONVEY_SPEED);


class Convey_2arm {
public:

    std::vector<object> convey_object;

     explicit Convey_2arm(int mode_ = 1);

};


#endif //MYPAPER_EX_2_CONVEY_2ARM_H
