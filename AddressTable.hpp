/*
 *   Copyright (C) Tue Jun 25 12:44:21 2013 XiaoWei
 *
 *                       handsomexiaowei@gmail.com
 */

#ifndef _ADDRESSTABLE_H_
#define _ADDRESSTABLE_H_

#include <vector>
#include <list>
#include <windows.h>

using namespace std;

template <class T>
class AddressTable
{
public:
  typedef struct _space_
  {
    ULONG_PTR begin;
    ULONG_PTR end;
    T data;
    _space_():begin(0),end(0) { memset(&data,0,sizeof(data)); }
    ~_space_(){}
  }space;

  struct space_sort_cmp 
  { 
    bool operator () (const typename AddressTable<T>::space &a, const typename AddressTable<T>::space &b) 
    { 
      return a.begin < b.begin;
    }
  };

  AddressTable(long _base,unsigned long _size,bool _sign)
    :base(_base),size(_size),alignment(_size),sign(_sign)
  {
  }
  
  virtual ~AddressTable()
  {
  }
  
  ULONG_PTR base;
  unsigned long size;
  unsigned long alignment;
  bool sign;              //正增长还是负增长
  list <space> addr_used;  //已经使用的空间

  ULONG_PTR assign_address(unsigned long _size);
  ULONG_PTR assign_address(unsigned long _size,const T &_data);
  ULONG_PTR assign_address(unsigned long _size,const T &_data,bool _sign);

  void set_sign(bool _sign) { sign = _sign; }
  bool get_sign() { return sign; }

  void addr_used_sort()
  {
      addr_used.sort(AddressTable<T>::space_sort_cmp());
  } 
};

typedef struct _useful_space
{
  ULONG_PTR begin;
  ULONG_PTR end;
}useful_space;


template <class T>
ULONG_PTR AddressTable<T>::assign_address(unsigned long _size,const T  & _data,bool _sign)
{
    addr_used_sort();
    sign = _sign;
    vector <useful_space> useful_vec;
    useful_space t;
    t.begin = base;
    for (typename list<AddressTable<T>::space>::iterator iter = addr_used.begin(); iter != addr_used.end(); ++iter)
    {
      t.end = 0;
      if (t.begin != iter->begin)
      {
        t.end = iter->begin;
        useful_vec.push_back(t);
      }
      t.begin = iter->end;
    }
    t.end = base + size;
    if (t.begin != t.end)
      useful_vec.push_back(t);    //筛选出没有使用的空间
    long count = 0;  //符合要求的有多少个
    for (vector<useful_space>::iterator iter = useful_vec.begin(); iter != useful_vec.end(); ++iter)
    {
      //if (iter->begin - iter->end >= _size)
      if (iter->end - iter->begin >= _size)
      {
        //printf("空闲地址开始:%x\r\n",iter->begin);
        //printf("空闲地址结束:%x\r\n",iter->end);
        //printf("符合要求的空间:%x-%x,大小为:%d\r\n",iter->begin,iter->end,iter->end-iter->begin); 
        count++;
      }
    }
    if (!count)
    {
      size += alignment;
      //return assign_address(_size);
      return assign_address(_size,_data,_sign);
    }
    long r = rand()%count; //如果有多个就随机抽取一个
    count = 0;
    
    for (vector<useful_space>::iterator iter = useful_vec.begin(); iter != useful_vec.end(); ++iter)
    {                                  
      if (iter->end - iter->begin >= _size)
      {
        if (count == r)
        {
          space p;
          ////注释掉这句就只会在某些概率下随机了
          //不能和0取模
          int space_rand = 0;
          if (iter->end - iter->begin - _size)
          {
             space_rand = rand() % (iter->end - iter->begin - _size); 
          }
          p.begin = space_rand + iter->begin;        
          ////
          //p.begin = iter->begin;
          //p.end = iter->begin + _size;//iter->end;
          p.end = p.begin + _size;
          p.data = _data;
          addr_used.push_back(p);
          if (sign)
            return p.begin;// iter->begin;
          else 
            return p.end;
        }
        count++;
      }
    }  
}


template <class T>
ULONG_PTR AddressTable<T>::assign_address(unsigned long _size,const T  & _data)
{
    addr_used_sort();
    vector <useful_space> useful_vec;
    useful_space t;
    t.begin = base;
    for (typename list<AddressTable<T>::space>::iterator iter = addr_used.begin(); iter != addr_used.end(); ++iter)
    {
      t.end = 0;
      if (t.begin != iter->begin)
      {
        t.end = iter->begin;
        useful_vec.push_back(t);
      }
      t.begin = iter->end;
    }
    t.end = base + size;
    if (t.begin != t.end)
      useful_vec.push_back(t);    //筛选出没有使用的空间
    long count = 0;  //符合要求的有多少个
    for (vector<useful_space>::iterator iter = useful_vec.begin(); iter != useful_vec.end(); ++iter)
    {
      //if (iter->begin - iter->end >= _size)
      if (iter->end - iter->begin >= _size)
      {
        //printf("符合要求的空间:%x-%x,大小为:%d\r\n",iter->begin,iter->end,iter->end-iter->begin);  
        //printf("空闲地址开始:%x\r\n",iter->begin);
        //printf("空闲地址结束:%x\r\n",iter->end);
        count++;
      }
    }
    if (!count)
    {
      size += alignment;
      //return assign_address(_size);
      return assign_address(_size,_data);
    }
    long r = rand()%count; //如果有多个就随机抽取一个
    count = 0;
    
    for (vector<useful_space>::iterator iter = useful_vec.begin(); iter != useful_vec.end(); ++iter)
    {
      if (iter->end - iter->begin >= _size)
      {
        if (count == r)
        {
          space p;
          ////注释掉这句就只会在某些概率下随机了
          //不能和0取模
          unsigned long space_rand = 0;
          if (iter->end - iter->begin - _size)
          {
             space_rand = rand() % (unsigned long)(iter->end - iter->begin - _size); 
          }
          p.begin = space_rand + iter->begin;
          ////
          //p.begin = iter->begin;
          //p.end = iter->begin + _size;//iter->end;
          p.end = p.begin + _size;
          p.data = _data;
          addr_used.push_back(p);
          if (get_sign())
            return p.begin;
          else
            return p.end;
        }
        count++;
      }
    }
    return 0;
}

template <class T>
ULONG_PTR AddressTable<T>::assign_address(unsigned long _size)
{
    addr_used_sort();

    vector <useful_space> useful_vec;
    useful_space t;
    t.begin = base;

    for (typename list<AddressTable<T>::space>::iterator iter = addr_used.begin();
        iter != addr_used.end();
        ++iter)
    {
        t.end = 0;
        if (t.begin != iter->begin)
        {
            t.end = iter->begin;
            useful_vec.push_back(t);
        }
        t.begin = iter->end;
    }

    t.end = base + size;
    if (t.begin != t.end)
        useful_vec.push_back(t);    //筛选出没有使用的空间

    long count = 0;  //符合要求的有多少个
    for (vector<useful_space>::iterator iter = useful_vec.begin();
        iter != useful_vec.end();
        ++iter)
    {
        if (iter->end - iter->begin >= _size)
        {
            printf("符合要求的空间:%x-%x,大小为:%d\n",iter->begin,iter->end,iter->end-iter->begin);
            count++;
        }
    }

    if (!count)
    {
        size += alignment;
        return assign_address(_size);
    }

    long r = rand()%count; //如果有多个就随机抽取一个
    count = 0;

    for (vector<useful_space>::iterator iter = useful_vec.begin();
        iter != useful_vec.end();
        ++iter)
    {
        if (iter->end - iter->begin >= _size)
        {
            if (count == r)
            {
                space p;
                ////注释掉这句就只会在某些概率下随机了
                //不能和0取模
                int space_rand = 0;
                if (iter->end - iter->begin - _size)
                {
                    space_rand = rand() % (int)(iter->end - iter->begin - _size); 
                }

                p.begin = space_rand + iter->begin;         
                p.end = p.begin + _size;
                //memset(&(p.data),0,sizeof(p.data));//清空数据

                //printf("%s\naddress:%x","正增长" ,&p);

                addr_used.push_back(p);
                if (get_sign())
                    return p.begin;
                else
                    return p.end;
            }
            count++;
        }
    }
    return 0;
}

#endif /* _ADDRESSTABLE_H_ */
