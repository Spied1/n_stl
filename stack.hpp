#pragma once
#include <vector>

template <class T>
class stack {

public:
    // Create free container
    Stack()
        : stackVec()
    {}

    // Constructor for initialization list
    Stack(std::initializer_list<T> el) :Stack(new T[el.size()]{})
    {
        int j = 0;

        for (auto i : el)
        {
            stackVec.push_back(i);
        }
    };

    // Create new container and copy other 
    Stack(const Stack& other) :stackVec(other.stack) {};

    // Rewrites stack with other
    Stack& operator=(const Stack& other)
    {
        stackVec = other.stackvec;
    }

    // Returns size of stack
    std::size_t size() { return stackVec.size(); };

    // Checks if container is empty
    bool empty()
    {
        return stackVec.empty();
    };

    // Add element for the top of stack.
    void push(const T& x)
    {
        stackVec.push_back(x);
    };

    // Gets top element 
    T& top()
    {
        if (stackVec.size() == 0)
        {
            throw
                std::runtime_error{ "nothing on top" };
        }
        else
        {
            return stackVec[stackVec.size() - 1];
        }
    };

    // Delets last added element of stack
    T pop()
    {
        if (stackVec.size() >= 0)
        {
            T value;
            value = stackVec[stackVec.size() - 1];
            stackVec.pop_back();
        }
        else
        {
            throw
                std::runtime_error{ "error,no elements in vec to pop" };
        }
    };

    // Swaps elements with other stack
    void swap(/*const*/ Stack& other) {
        stackVec.swap(other.stackVec);
    };

    //reverse stack 
    Stack reverse()
    {
        Stack stackReversed;
        for (auto i = 0; i != stackVec.size(); i++)
        {
            stackReversed[i] = stackVec[stackVec - i];

        }
        return stackReversed;
    }
    ~Stack() {};

private:
    std::vector<T>  stackVec;
};