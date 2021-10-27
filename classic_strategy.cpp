//
// Created by 10372 on 2021/10/25.
//

#include "classic_strategy.h"
#include "iostream"

classic_strategy::classic_strategy(std::vector<object> object_to_pick, int mode_):mode(mode_)
{
    objects_to_pick_ = object_to_pick;
    T = 0;
    T1 = 0;
    T0 = 0;
    miss = 0;
    workspace_update();
}

int classic_strategy::workspace_update()
{
    objects_in_workspace0.clear();
    objects_in_workspace1.clear();
    for (auto obj : objects_to_pick_)
    {
        obj.x += CONVEY_SPEED * T;
        if (obj.x >= MAX_EDGE_0)
            objects_in_workspace1.push_back(obj);
        else objects_in_workspace0.push_back(obj);
    }
    return 0;
}

int classic_strategy::pick()
{
    //先0抓
    while ( !objects_in_workspace0.empty() )
    {
        std::vector<object>::iterator item;
        if ( mode == 0 )
        {
            item = getSPT(objects_in_workspace0, T0);
        } else
        {
            item = getFIFO(objects_in_workspace0, T0);
        }
        T0 += getPNPtime0(*item,T0);
        int number = item->number;
        std::cout << "pick by 0:    " << "x y :     " << item->x << "    " << item->y << std::endl;
        objects_in_workspace0.erase(item);
        for (auto o = objects_to_pick_.begin();o!=objects_to_pick_.end();o++)
        {
            if (o->number == number)
            {
                objects_to_pick_.erase(o);
                break;
            }
        }

    }

    //再1抓
    while ( !objects_to_pick_.empty() )
    {
        std::vector<object>::iterator item;
        if ( mode == 0 )
        {
            item = getSPT(objects_to_pick_, T1);
        } else
        {
            item = getFIFO(objects_to_pick_, T1);
        }
        float pnp = getPNPtime1(*item,T1);
        if (pnp == 0)
        {
            miss++;
            std::cout << "pick by 1: miss!   " << "x y :     " << item->x << "    " << item->y << std::endl;
        } else
        {
            T1 += pnp;
            std::cout << "pick by 1:    " << "x y :     " << item->x << "    " << item->y << std::endl;
        }

        objects_to_pick_.erase(item);

    }
    std::cout << "T0T1:    "<<T0 << "   "<< T1 << std::endl;


    return 0;
}

std::vector<object>::iterator classic_strategy::getSPT(std::vector<object> &objects, float t_now)
{
    auto iterator_to_min_distance = objects.begin();
    auto iterator_temp = objects.begin();
    if (objects.size() == 1) return iterator_to_min_distance;
    float min_distance = minDistCal(*iterator_to_min_distance, t_now);
    iterator_temp++;
    while (iterator_temp != objects.end())
    {
        if (minDistCal(*iterator_temp,t_now) < min_distance)
        {
            iterator_to_min_distance = iterator_temp;
            min_distance = minDistCal(*iterator_temp,t_now);
        }
        iterator_temp++;
    }
    return iterator_to_min_distance;
}

std::vector<object>::iterator classic_strategy::getFIFO(std::vector<object> &objects, float t_now)
{

    return objects.begin();
}

float classic_strategy::minDistCal(object &object, float now)
{
    float x = object.x + now * CONVEY_SPEED;
    return std::sqrt(x*x + object.y*object.y);
    return 0;
}
