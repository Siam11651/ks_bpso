#include <pso.hpp>

template <typename P, typename V, typename F>
pso<P, V, F>::pso(swarm<P, V, F> *_swarm_ptr) :
    m_swarm_ptr(_swarm_ptr) {}

template class pso<bool, double, uint64_t>;