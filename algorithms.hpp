/*
*   Copyright (C) Thu Jun 20 15:58:03 2013 XiaoWei
*
*                       handsomexiaowei@gmail.com
*/

#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

template <class T>
void RandList(T *list, unsigned long count)  //把数据乱序后返回
{
    T t;

    while (count)
    {
        unsigned long rd = rand() % count;
        count--;

        t = list[rd];
        list[rd] = list[count];
        list[count] = t;
    }
}

#endif /* _ALGORITHMS_H_ */
