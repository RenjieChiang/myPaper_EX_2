//
// Created by 10372 on 2021/9/25.
//

#include "Convey_2arm.h"
#include "random"

Convey_2arm::Convey_2arm(int mode_)
{
    switch (mode_)
    {
        case 1:
        {
//             指定的位置-传送带分布
            object temp{};
            float temp_x[5] = {3, 1.5, 0, -1.5, -3};
            float temp_y[5] = {1.5, 0.5, 1.5, 0.5, 1.5};
            for (int i = 0; i < 5; ++i)
            {
                temp.x = temp_x[i];
                temp.y = temp_y[i];
                temp.number =i;
                convey_object.push_back(temp);
            }
            break;
        }

        case 2:
        {
            // POISSON_DISTRIBUTION OM CONVEY 泊松分布 x方向等间隔
            std::random_device rd;
            std::mt19937 gen(rd());
            std::poisson_distribution<> d(10);
            object temp_poisson{};
            for (int i = 0; i < 15; ++i)
            {
                temp_poisson.x = 1 - i*0.5+ d(gen)/100.0;//x方向等距+随机泊松量
                temp_poisson.y = d(gen)/10.0;
                temp_poisson.number = i;
                convey_object.push_back(temp_poisson);
            }
        }

    }
}


float getPNPtime0(object ready_to_pick, float time_now, float convey_speed)
{
    float pick_x = ready_to_pick.x + time_now * convey_speed;
    if (pick_x < MAX_EDGE_0 && pick_x > MIN_EDGE_0)
    {
        pick_x = pick_x - ARM_POSITION_0;
        float time_need = std::sqrt(pick_x * pick_x + ready_to_pick.y*ready_to_pick.y) / ARM_SPEED;
        return time_need;
    } else
        return 0;
}

// 如果物体是在min边缘外则可以等待其到达边缘处抓取
float getPNPtime1(object ready_to_pick, float time_now, float convey_speed)
{
    float pick_x = ready_to_pick.x + time_now * convey_speed;
    if (pick_x < MAX_EDGE_1 && pick_x >= MIN_EDGE_1)
    {
        pick_x = pick_x - ARM_POSITION_1;
        float time_need = std::sqrt(pick_x * pick_x + ready_to_pick.y*ready_to_pick.y) / ARM_SPEED;
        return time_need;
    } else if (pick_x < MIN_EDGE_1)
    {
        float time_need = (MIN_EDGE_1 - pick_x) / convey_speed;
        pick_x = MIN_EDGE_1- ARM_POSITION_1;
        time_need += std::sqrt(pick_x * pick_x + ready_to_pick.y*ready_to_pick.y) / ARM_SPEED;
        return time_need;
    } else return 0;// missing!
}
