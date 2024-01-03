#ifndef PSO_H
#define PSO_H

#include <swarm.hpp>

template <typename P, typename V, typename F>
class pso
{
protected:
    swarm<P, V, F> *m_swarm_ptr;

public:
    pso(swarm<P, V, F> *_swarm_ptr) :
        m_swarm_ptr(_swarm_ptr) {};
    
    virtual void run() = 0;
};

#endif