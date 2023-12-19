#include <swarm.hpp>

template <typename P, typename V, typename F>
swarm<P, V, F>::swarm() {}

template <typename P, typename V, typename F>
swarm<P, V, F>::swarm(const std::vector<particle<P, V, F> *> &_particle_ptrs) :
    m_particle_ptrs(_particle_ptrs) {}

template <typename P, typename V, typename F>
void swarm<P, V, F>::update_fitness()
{
    for(particle<P, V, F> *item : m_particle_ptrs)
    {
        item->update_fitness();

        if(!m_best_fitness.has_value() || item->const_best_fitness() > m_best_fitness)
        {
            m_best_fitness = std::max(m_best_fitness, item->const_best_fitness());
            m_best_position = item->const_best_position();
        }
    }
}

template class swarm<bool, double, uint64_t>;