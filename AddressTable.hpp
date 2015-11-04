/*
 *   Copyright (C) Tue Jun 25 12:44:21 2013 XiaoWei
 *
 *                       handsomexiaowei@gmail.com
 */

#ifndef _ADDRESSTABLE_H_
#define _ADDRESSTABLE_H_

#include <vector>
#include <list>

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
  bool sign;              //���������Ǹ�����
  list <space> addr_used;  //�Ѿ�ʹ�õĿռ�

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
      useful_vec.push_back(t);    //ɸѡ��û��ʹ�õĿռ�
    long count = 0;  //����Ҫ����ж��ٸ�
    for (vector<useful_space>::iterator iter = useful_vec.begin(); iter != useful_vec.end(); ++iter)
    {
      //if (iter->begin - iter->end >= _size)
      if (iter->end - iter->begin >= _size)
      {
        //printf("���е�ַ��ʼ:%x\r\n",iter->begin);
        //printf("���е�ַ����:%x\r\n",iter->end);
        //printf("����Ҫ��Ŀռ�:%x-%x,��СΪ:%d\r\n",iter->begin,iter->end,iter->end-iter->begin); 
        count++;
      }
    }
    if (!count)
    {
      size += alignment;
      //return assign_address(_size);
      return assign_address(_size,_data,_sign);
    }
    long r = rand()%count; //����ж���������ȡһ��
    count = 0;
    
    for (vector<useful_space>::iterator iter = useful_vec.begin(); iter != useful_vec.end(); ++iter)
    {                                  
      if (iter->end - iter->begin >= _size)
      {
        if (count == r)
        {
          space p;
          ////ע�͵�����ֻ����ĳЩ�����������
          //���ܺ�0ȡģ
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
      useful_vec.push_back(t);    //ɸѡ��û��ʹ�õĿռ�
    long count = 0;  //����Ҫ����ж��ٸ�
    for (vector<useful_space>::iterator iter = useful_vec.begin(); iter != useful_vec.end(); ++iter)
    {
      //if (iter->begin - iter->end >= _size)
      if (iter->end - iter->begin >= _size)
      {
        //printf("����Ҫ��Ŀռ�:%x-%x,��СΪ:%d\r\n",iter->begin,iter->end,iter->end-iter->begin);  
        //printf("���е�ַ��ʼ:%x\r\n",iter->begin);
        //printf("���е�ַ����:%x\r\n",iter->end);
        count++;
      }
    }
    if (!count)
    {
      size += alignment;
      //return assign_address(_size);
      return assign_address(_size,_data);
    }
    long r = rand()%count; //����ж���������ȡһ��
    count = 0;
    
    for (vector<useful_space>::iterator iter = useful_vec.begin(); iter != useful_vec.end(); ++iter)
    {
      if (iter->end - iter->begin >= _size)
      {
        if (count == r)
        {
          space p;
          ////ע�͵�����ֻ����ĳЩ�����������
          //���ܺ�0ȡģ
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
        useful_vec.push_back(t);    //ɸѡ��û��ʹ�õĿռ�

    long count = 0;  //����Ҫ����ж��ٸ�
    for (vector<useful_space>::iterator iter = useful_vec.begin();
        iter != useful_vec.end();
        ++iter)
    {
        if (iter->end - iter->begin >= _size)
        {
            printf("����Ҫ��Ŀռ�:%x-%x,��СΪ:%d\n",iter->begin,iter->end,iter->end-iter->begin);
            count++;
        }
    }

    if (!count)
    {
        size += alignment;
        return assign_address(_size);
    }

    long r = rand()%count; //����ж���������ȡһ��
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
                ////ע�͵�����ֻ����ĳЩ�����������
                //���ܺ�0ȡģ
                int space_rand = 0;
                if (iter->end - iter->begin - _size)
                {
                    space_rand = rand() % (int)(iter->end - iter->begin - _size); 
                }

                p.begin = space_rand + iter->begin;         
                p.end = p.begin + _size;
                //memset(&(p.data),0,sizeof(p.data));//�������

                //printf("%s\naddress:%x","������" ,&p);

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
