#include <knapsack.hpp>
#include <random.hpp>
#include <algorithm>
#include <cmath>

std::random_device rand_gen;

ks_pair::ks_pair() :
    m_weight(0),
    m_profit(0) {}

ks_pair::ks_pair(const uint64_t &_weight, const uint64_t &_profit) :
    m_weight(_weight),
    m_profit(_profit) {}

uint64_t &ks_pair::weight()
{
    return m_weight;
}

const uint64_t &ks_pair::const_weight() const
{
    return m_weight;
}

uint64_t &ks_pair::profit()
{
    return m_profit;
}

const uint64_t &ks_pair::const_profit() const
{
    return m_profit;
}

ks_problem::ks_problem(std::istream &_istream)
{
    size_t pair_count;

    _istream >> pair_count;

    m_ks_pairs.resize(pair_count);

    for(size_t i = 0; i < pair_count; ++i)
    {
        _istream >> m_ks_pairs[i].profit();
    }

    for(size_t i = 0; i < pair_count; ++i)
    {
        _istream >> m_ks_pairs[i].weight();
    }

    _istream >> m_capacity;
}

const std::vector<ks_pair> &ks_problem::const_ks_pairs() const
{
    return m_ks_pairs;
}

const size_t &ks_problem::const_capacity() const
{
    return m_capacity;
}

ks_particle::ks_particle(const ks_problem *_ks_problem_ptr) :
    m_ks_problem_ptr(_ks_problem_ptr) {}

ks_particle::ks_particle(const ks_problem *_ks_problem_ptr, const std::vector<bool> &_position,
    const std::vector<double> &_velocity) :
    m_ks_problem_ptr(_ks_problem_ptr),
    particle(_position, _velocity) {}

void ks_particle::update_fitness()
{
    const ks_problem &problem = *m_ks_problem_ptr;
    uint64_t weight_sum = 0;
    uint64_t profit_sum = 0;

    for(size_t i = 0; i < problem.const_ks_pairs().size(); ++i)
    {
        if(m_position[i])
        {
            weight_sum += problem.const_ks_pairs()[i].const_weight();
            profit_sum += problem.const_ks_pairs()[i].const_profit();
        }
    }

    if(weight_sum <= problem.const_capacity())
    {
        if(!m_best_fitness.has_value() || profit_sum > m_best_fitness)
        {
            m_best_fitness = profit_sum;
            m_best_position = m_position;
        }
    }
}

double ks_swarm::sigmoid(const double &x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

ks_swarm::ks_swarm() :
    m_vmax(6.0),
    swarm() {}

ks_swarm::ks_swarm(const std::vector<ks_particle *> &_particle_ptrs) :
    m_vmax(6.0)
{
    for(size_t i = 0; i < _particle_ptrs.size(); ++i)
    {
        m_particle_ptrs.push_back(_particle_ptrs[i]);
    }
}

double &ks_swarm::vmax()
{
    return m_vmax;
}

const double &ks_swarm::vmax() const
{
    return m_vmax;
}

void ks_swarm::update_position()
{
    const double c0 = 2.0f;
    const double c1 = 2.0f;

    for(particle<bool, double, uint64_t> *item : m_particle_ptrs)
    {
        std::vector<bool> &position = item->position();
        std::vector<bool> &best_position = item->best_position().value();
        std::vector<double> &velocity = item->velocity();
        const double r0 = random::get_double();
        const double r1 = random::get_double();
        const size_t dimension = velocity.size();

        for(size_t i = 0; i < dimension; ++i)
        {
            velocity[i] += c0 * r0 * (best_position[i] - position[i])
                + c1 * r1 * (m_best_position.value()[i] - position[i]);
            velocity[i] = std::clamp(velocity[i], -m_vmax, m_vmax);
            velocity[i] = sigmoid(velocity[i]);
            double rand_value = random::get_double();
            
            if(rand_value < velocity[i])
            {
                position[i] = true;
            }
            else
            {
                position[i] = false;
            }
        }
    }
}

ks_bpso::ks_bpso(ks_swarm *_swarm_ptr) :
    m_generation_count(100) ,
    pso(_swarm_ptr) {}

ks_bpso::ks_bpso(const size_t &_generation_count, ks_swarm *_swarm_ptr) :
    m_generation_count(_generation_count) ,
    pso(_swarm_ptr) {}

size_t &ks_bpso::generation_count()
{
    return m_generation_count;
}

const size_t &ks_bpso::const_generation_count() const
{
    return m_generation_count;
}

void ks_bpso::run()
{
    for(size_t i = 0; i < m_generation_count; ++i)
    {
        m_swarm_ptr->update_position();
        m_swarm_ptr->update_fitness();
    }
}

ks_tvbpso::ks_tvbpso(ks_swarm *_swarm_ptr) :
    m_max_iteration(10),
    m_vlow(2.0),
    m_vhigh(4.0),
    ks_bpso(_swarm_ptr) {}

ks_tvbpso::ks_tvbpso(const size_t &_generation_count, const size_t &_max_iteration,
    ks_swarm *_swarm_ptr) :
    m_max_iteration(_max_iteration),
    m_vlow(2.0),
    m_vhigh(4.0),
    ks_bpso(_generation_count, _swarm_ptr) {}

size_t &ks_tvbpso::max_iteration()
{
    return m_max_iteration;
}

const size_t &ks_tvbpso::const_max_iteration() const
{
    return m_max_iteration;
}

double &ks_tvbpso::vlow()
{
    return m_vlow;
}

const double &ks_tvbpso::const_vlow() const
{
    return m_vlow;
}

double &ks_tvbpso::vhigh()
{
    return m_vhigh;
}

const double &ks_tvbpso::const_vhigh() const
{
    return m_vhigh;
}

void ks_tvbpso::run()
{
    for(size_t i = 0; i < m_max_iteration; ++i)
    {
        ((ks_swarm *)m_swarm_ptr)->vmax() = m_vlow + (i * (m_vhigh - m_vlow)) / m_max_iteration;

        for(size_t j = 0; j < m_generation_count; ++j)
        {
            m_swarm_ptr->update_position();
            m_swarm_ptr->update_fitness();
        }
    }
}