#pragma once
#include <vector>

struct AbstractNode{
    virtual void update()=0;
};

template<typename SAMPLE_TYPE, typename S >
struct BufferedNode:AbstractNode{
    S signal;
    size_t currentBlockSize;
    std::vector<SAMPLE_TYPE> buffer = std::vector<SAMPLE_TYPE>();
    std::vector<AbstractNode*> dependants;
    
    BufferedNode(S signal):signal(signal){}
    void update(size_t blockSize){
        // buffer.clear();
        currentBlockSize = blockSize;
        buffer.resize(blockSize);
        for(size_t idx; idx<blockSize; idx++){
            buffer.push_back(signal(idx));
        }
    }
    SAMPLE_TYPE operator()(int t){
        return buffer[t%currentBlockSize];
    }

};

template<typename S >
struct Node:AbstractNode{
    S signal;
    Node(){}
    void update(){}
};

template<typename SAMPLE_TYPE, typename S >
BufferedNode<SAMPLE_TYPE,S> wrap(S signal){
    return BufferedNode<SAMPLE_TYPE, S>(signal);
};