#ifndef _LIST_HPP
#define _LIST_HPP
#include "../h/print.hpp"

template<typename T>
class List
{
private:
    struct Elem
    {
        T *data;
        Elem *next;
        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head;


public:
    List() : head(nullptr), num(0) {}
    int num;
    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        Elem *elem =(Elem*) MemoryAllocator::instance().mem_alloc(sizeof(Elem));
        elem->data=data;
        elem->next= nullptr;
        if(num==0){
            head= elem;
        }
        else {
            elem->next= head;
            head = elem;
        }
        num++;
    }
    bool isEmpty(){
        return num==0;
    }

    T* removeFirst()
    {
        if(num==0)return nullptr;
        Elem *elem = head;
        head = head->next;
        T *ret = elem->data;
        num--;
        if(num==0)head= nullptr;
        delete elem;
        return ret;
    }




};

#endif
