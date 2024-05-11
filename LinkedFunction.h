#pragma once
#include <chrono>
#include <functional>
#include <set>
typedef unsigned int Uint32;

// Get the number of milliseconds since the program started
Uint32 GetMS()
{
    static const auto start = std::chrono::high_resolution_clock::now();
    const auto now = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    return duration.count();
}

class FunctionNode
{
public:
    Uint32 duration;
    Uint32 repeat;
    Uint32 delay;

    std::function<bool()> func;

    FunctionNode *next;

    FunctionNode(std::function<bool()> func, Uint32 delay = 0, Uint32 repeat = 1);

    FunctionNode(std::function<bool()> func, Uint32 delay = 0, Uint32 duration = 1);
};

class LinkedFunction
{
public:
    static std::set<LinkedFunction *> Functions;

    FunctionNode *head;
    FunctionNode *tail;
    Uint32 start;
    Uint32 id;

    LinkedFunction(std::function<bool()> func, Uint32 delay = 0, Uint32 duration = 0, Uint32 repeat = 1);
    LinkedFunction(FunctionNode *fn);
    ~LinkedFunction();

    void Next(std::function<bool()> func, Uint32 delay = 0, Uint32 duration = 0, Uint32 repeat = 1);
    void Next(FunctionNode *fn);

    void Run();

    static void Update();
};