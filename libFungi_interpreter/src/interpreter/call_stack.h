#pragma once
#include <stack>
#include "activation_record.h"

struct CallStack{
    std::stack<ActivationRecord> stack;
    CallStack():stack(std::stack<ActivationRecord>()){} 
    void push(ActivationRecord r){
        stack.push(r);
    }
    ActivationRecord pop(){
        ActivationRecord top = stack.top();
        stack.pop();
        return top;
    }
    ActivationRecord& peek(){
        return stack.top();
    }
};