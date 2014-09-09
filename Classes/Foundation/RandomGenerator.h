/****************************************************************************
  Copyright (c) 2014-2015 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_RandomGenerator_h
#define Foundation_RandomGenerator_h

#include <random>
namespace Foundation{
class RandomGenerator
{
public:

    // return a random boolean.
    inline bool   randBoolean()
    {
        return random<double>() > 0.5;
    }

    template< class FP >
    inline FP random(typename std::enable_if< std::is_floating_point<FP>::value >::type* = 0)
    {
        return random(0.0, 1.0);
    }

    template< class FP >
    inline FP random(FP x, FP y,
        typename std::enable_if< std::is_floating_point<FP>::value >::type* = 0)
    {
        std::uniform_real_distribution<FP> randomResult(x, y);
        return randomResult(m_randomEngine);
    }

    template< class FP >
    inline FP random(FP x, FP y,
        typename std::enable_if< std::is_integral<FP>::value >::type* = 0)
    {
        std::uniform_int_distribution<> randomResult(x, y);
        return randomResult(m_randomEngine);
    }

    //returns a random double in the range -1 < n < 1
    inline double randClamped()
    {
        return random<double>() - random<double>();
    }

public:
    RandomGenerator():
        m_randomEngine(m_randomDevice())
    {

    }
    ~RandomGenerator(){}
private:
    std::random_device          m_randomDevice;
    std::default_random_engine  m_randomEngine;
};

} //namespace Foundation

#endif // Foundation_RandomGenerator_h


