#include "LinkedFunction.h"
#include <iostream>

uint32_t GetMS()
{
    static const auto start = std::chrono::high_resolution_clock::now();
    const auto now = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    return duration.count();
}

std::set<LinkedFunction *> LinkedFunction::Functions;

FunctionNode::FunctionNode(std::function<bool()> work, uint8_t type, uint32_t delay, uint32_t t) : type(type), delay(delay), work(work)
{
    if (type)
        this->repeat = t;
}

LinkedFunction::LinkedFunction()
{
    head = tail = nullptr;
}

LinkedFunction::~LinkedFunction()
{
    std::cout << "finished" << std::endl;
}

void LinkedFunction::Next(FunctionNode *fn)
{
    if (!fn)
        return;

    if (!head)
        head = tail = fn;
    else
    {
        tail->next = fn;
        tail = tail->next;
    }
}

void LinkedFunction::Run()
{
    if (!head)
        return;

    start = GetMS();
    Functions.insert(this);
}

void LinkedFunction::Update()
{
    if (Functions.empty())
        return;

    for (auto it = Functions.begin(); it != Functions.end();)
    {
        LinkedFunction *lf = *it;

        if (!lf || !lf->head)
        {
            it = Functions.erase(it);
            continue;
        }

        FunctionNode *fn = lf->head;

        bool done = false;
        if (GetMS() - lf->start >= fn->delay)
        {
            if (!fn->type)
            {
                if (!fn->work())
                    done = true;
            }
            else if (fn->type == 1)
            {
                if (fn->repeat > 0)
                {
                    fn->work();
                    fn->repeat--;
                }
                else
                    done = true;
            }
            else if (fn->type == 2)
            {
                if (GetMS() - lf->start <= fn->repeat + fn->delay)
                    fn->work();
                else
                    done = true;
            }
        }

        if (done)
        {
            lf->start = GetMS();
            lf->head = lf->head->next;
            delete fn;
            fn = nullptr;
        }

        if (!lf->head)
        {
            delete lf;
            lf = nullptr;
            it = Functions.erase(it);
        }
        else
            it++;
    }
}