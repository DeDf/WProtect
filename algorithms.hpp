/*
*   Copyright (C) Thu Jun 20 15:58:03 2013 XiaoWei
*
*                       handsomexiaowei@gmail.com
*/

#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

template <class T>
void upset(T * t,int count)  //把数据乱序后返回
{
    int nowCount = count;
    T temp;

    while (nowCount)
    {
        int rd = rand() % nowCount;
        temp = t[rd];
        nowCount--;
        t[rd] = t[nowCount];
        t[nowCount] = temp;
    }
}

#include "AddressTable.hpp"

#endif /* _ALGORITHMS_H_ */
