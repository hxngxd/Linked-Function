#include "LinkedFunction.h"

std::set<LinkedFunction *> LinkedFunction::Functions;

FunctionNode::FunctionNode(std::function<bool()> func, Uint32 delay = 0, Uint32 repeat = 1)
{
    this->func = func;
    this->delay = delay;
    this->duration = duration;
    this->repeat = repeat;
    this->next = nullptr;
}

LinkedFunction::LinkedFunction(std::function<bool()> func, Uint32 delay, Uint32 duration, Uint32 repeat)
{
    head = tail = nullptr;
    Next(func, delay, duration, repeat);
}

LinkedFunction::LinkedFunction(FunctionNode *fn)
{
    head = tail = nullptr;
    Next(fn);
}

LinkedFunction::~LinkedFunction()
{
}

void LinkedFunction::Next(std::function<bool()> func, Uint32 delay, Uint32 duration, Uint32 repeat)
{
    FunctionNode *fn = new FunctionNode(func, delay, duration, repeat);
    Next(fn);
}

void LinkedFunction::Next(FunctionNode *fn)
{
    if (!fn)
        return;

    if (!head)
    {
        head = tail = fn;
    }
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
        if (!lf->head)
        {
            Functions.erase(it++);
            continue;
        }

        FunctionNode *fn = lf->head;
        if (fn->duration)
        {
            if (GetMS() - lf->start <= fn->duration)
            {
                fn->func();
            }
            else
            {
                delete fn;
                lf->head = lf->head->next;
            }
        }
        // else
        // {
        //     if (GetMS() - fn->start < fn->delay)
        //         continue;
        // }
        // if (GetMS() - fn->start >= fn->delay)
        // {
        //     if (fn->func())
        //     {
        //         if (fn->repeat == 0)
        //         {
        //             delete fn;
        //             lf->head = lf->head->next;
        //         }
        //         else
        //         {
        //             if (fn->repeat > 0)
        //                 --fn->repeat;
        //             fn->start = GetMS();
        //         }
        //     }
        //     else
        //     {
        //         delete fn;
        //         lf->head = lf->head->next;
        //     }
        // }

        // if (!lf->head)
        //     Functions.erase(it++);
        // else
        //     ++it;
    }
}