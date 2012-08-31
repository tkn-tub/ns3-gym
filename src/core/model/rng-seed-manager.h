#ifndef RNG_SEED_MANAGER_H
#define RNG_SEED_MANAGER_H

#include <stdint.h>

namespace ns3 {

class RngSeedManager
{
public:
  /**
   * \brief set the seed
   * it will duplicate the seed value 6 times
   * \code
   * RngSeedManger::SetSeed(15);
   * UniformVariable x(2,3);     //these will give the same output everytime
   * ExponentialVariable y(120); //as long as the seed stays the same
   * \endcode
   * \param seed
   *
   * Note, while the underlying RNG takes six integer values as a seed;
   * it is sufficient to set these all to the same integer, so we provide
   * a simpler interface here that just takes one integer.
   */
  static void SetSeed (uint32_t seed);

  /**
   * \brief Get the seed value
   * \return the seed value
   *
   * Note:  returns the first of the six seed values used in the underlying RNG
   */
  static uint32_t GetSeed (void);

  /**
   * \brief Set the run number of simulation
   *
   * \code
   * RngSeedManager::SetSeed(12);
   * int N = atol(argv[1]); //read in run number from command line
   * RngSeedManager::SetRun(N);
   * UniformVariable x(0,10);
   * ExponentialVariable y(2902);
   * \endcode
   * In this example, N could successivly be equal to 1,2,3, etc. and the user
   * would continue to get independent runs out of the single simulation.  For
   * this simple example, the following might work:
   * \code
   * ./simulation 0
   * ...Results for run 0:...
   *
   * ./simulation 1
   * ...Results for run 1:...
   * \endcode
   */
  static void SetRun (uint64_t run);
  /**
   * \returns the current run number
   * @sa SetRun
   */
  static uint64_t GetRun (void);

  static uint64_t GetNextStreamIndex(void);

};

// for compatibility
typedef RngSeedManager SeedManager;

} // namespace ns3


#endif /* RNG_SEED_MANAGER_H */
