#pragma once
#include <chrono>
#include <functional>
#include <set>

uint32_t GetMS();

class FunctionNode
{
public:
    uint8_t type;
    uint32_t delay;
    uint32_t repeat;
    std::function<bool()> work;

    FunctionNode *next;
    FunctionNode(std::function<bool()> work, uint8_t type, uint32_t delay = 0, uint32_t t = 1);
};

class LinkedFunction
{
public:
    static std::set<LinkedFunction *> Functions;

    FunctionNode *head;
    FunctionNode *tail;
    uint32_t start;

    LinkedFunction();
    ~LinkedFunction();

    void Next(FunctionNode *fn);
    void Run();

    static void Update();
};