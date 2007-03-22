/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Rajib Bhattacharjea<raj.b@gatech.edu>
//

#ifndef __random_variable_h__
#define __random_variable_h__

#include <vector>
#include <algorithm>


#define INFINITE_VALUE 1e307
namespace ns3{

class RngStream;

/**
 * \brief Pure virtual base class for RNG seeds
 */
class Seed {
  // Seed is used to seed the random number generator(s)
  // This is a base class for RandomSeed and ConstantSeed
public:
  virtual ~Seed();
  virtual bool IsRandom() const = 0;
};

/**
 * \brief random RNG seeds
 */
class RandomSeed : public Seed {
public:
  RandomSeed();
  ~RandomSeed();
  bool IsRandom() const;
};

/**
 * \brief constant RNG seeds
 */
class ConstantSeed : public Seed 
{
public:
  ConstantSeed(uint32_t); // Use six copies of the specified value
  ConstantSeed(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);  // Six seeds
  bool IsRandom() const;
  ~ConstantSeed();
public:
  uint32_t seeds[6];
};

/**
 * \brief The basic RNG for NS-3.
 *
 * Note: The underlying random number generation method used
 * by NS-3 is the RngStream code by Pierre L'Ecuyer at
 * the University of Montreal.
 * 
 * NS-3 has a rich set of  random number generators.
 * Class RandomVariable defines the base class functionalty
 * required for all random number generators.  By default, the underlying
 * generator is seeded with the time of day, and then deterministically
 * creates a sequence of seeds for each subsequent generator that is created.
 * The rest of the documentation outlines how to change this behavior.
 */
class RandomVariable { 

public:
  /**
   * \brief Constructor for a random number generator with a random seed.
   */
  RandomVariable();
  
  /**
   * \brief Copy constructor
   */  
  RandomVariable(const RandomVariable&);
  
  /**
   * \brief Destructor for a random number generator with a random seed.
   */
  virtual ~RandomVariable();
  
  /**
   * \brief Returns a random double from the underlying distribution
   * \return A floating point random value
   */
  virtual double  GetValue() = 0;     

  /**
   * \brief Returns a random integer integer from the underlying distribution
   * \return  Integer cast of ::GetValue()
   */
  virtual uint32_t GetIntValue();

  /**
   * \return A copy of this object
   */  
  virtual RandomVariable*   Copy() const = 0;

  /**
   * \brief Use a private seed and private stream for this generator.
   * 
   * This function explicitly overrides all other seeding behavior for
   * this object.  For example, even if another seeding method has been
   * used (either by default or calls to UseDevRandom, UseGlobalSeed, etc.)
   * a call to SetSeed explicitly re-seeds this generator. Example:
   * \code
   * UniformVariable x(0,10);//with no other code, defaults to time of day seed
   * x.SetSeed(...); //overrides time of day seed
   * \endcode
   * \param s Seed to use.  Can either be a RandomSeed or ConstantSeed.
   * \return true if valid seed.
   */
  bool SetSeed(const Seed& s);
  
  /**
   * \brief Set seeding behavior
   * 
   * Specify whether the POSIX device /dev/random is to
   * be used for seeding.  When this is used, the underlying
   * generator is seeded with data from /dev/random instead of
   * being seeded based upon the time of day.  For this to be effective,
   * it must be called before the creation of the first instance of a 
   * RandomVariable or subclass.  Example:
   * \code
   * RandomVariable::UseDevRandom();
   * UniformVariable x(2,3);  //these are seeded randomly
   * ExponentialVariable y(120); //etc
   * \endcode
   * \param udr True if /dev/random desired.
   */
  static  void UseDevRandom(bool udr = true);

   /**
   * \brief Use the global seed to force precisely reproducible results.
   *
   * It is often desirable to create a simulation that uses random
   * numbers, while at the same time is completely reproducible.
   * Specifying this set of six random seeds initializes the
   * random number generator with the specified seed.
   * Once this is set, all generators will produce fixed output
   * from run to run.  This is because each time a new generator is created,
   * the underlying RngStream deterministically creates a new seed based upon
   * the old one, hence a "stream" of RNGs.  Example:
   * \code
   * RandomVariable::UseGlobalSeed(...);
   * UniformVariable x(2,3);     //these will give the same output everytime
   * ExponentialVariable y(120); //as long as the seed stays the same
   * \endcode
   * \param s
   * \return True if seed is valid.
   */ 
  static void UseGlobalSeed(const Seed& s);

private:
  static bool initialized;     // True if package seed is set 
  static void Initialize();    // Initialize  the RNG system
  static void GetRandomSeeds(uint32_t seeds[6]);
private:
  static bool useDevRandom;    // True if using /dev/random desired
  static bool globalSeedSet;   // True if global seed has been specified
  static int  devRandom;       // File handle for /dev/random
  static uint32_t globalSeed[6]; // The global seed to use
protected:
  static unsigned long heuristic_sequence;
  RngStream* m_generator;  //underlying generator being wrapped
};


/**
 * \brief The uniform distribution RNG for NS-3.
 */
class UniformVariable : public RandomVariable {
public:
  /**
   * Creates a uniform random number generator in the
   * range [0.0 .. 1.0)
   */
  UniformVariable();

  /**
   * Creates a uniform random number generator with the specified range
   * \param s Low end of the range
   * \param l High end of the range
   */
  UniformVariable(double s, double l);

  UniformVariable(const UniformVariable& c);
  
  /**
   * \return A value between low and high values specified by the constructor
   */
  virtual double GetValue();
  virtual RandomVariable*  Copy() const;
private:
  double m_min;
  double m_max;
};

/**
 * \brief A random variable that returns a constant
 *
 * Class ConstantVariable defines a random number generator that
 * returns the same value every sample.
 */
class ConstantVariable : public RandomVariable { 

public:
  /**
   * Construct a ConstantVariable RNG that returns zero every sample
   */
  ConstantVariable();
  
  /**
   * Construct a ConstantVariable RNG that returns the specified value
   * every sample.
   * \param c Unchanging value for this RNG.
   */
  ConstantVariable(double c);


  ConstantVariable(const ConstantVariable& c) ;

  /**
   * \brief Specify a new constant RNG for this generator.
   * \param c New constant value for this RNG.
   */
  void    NewConstant(double c);

  /**
   * \return The constant value specified
   */
  virtual double  GetValue();
  virtual uint32_t GetIntValue();
  virtual RandomVariable*   Copy() const;
private:
  double m_const;
};

/**
 * \brief Return a sequential list of values
 *
 * Class SequentialVariable defines a random number generator that
 * returns a sequential sequence.  The sequence monotonically
 * increases for a period, then wraps around to the low value 
 * and begins monotonicaly increasing again.
 */
class SequentialVariable : public RandomVariable {

public:
  /**
   * \brief Constructor for the SequentialVariable RNG.
   *
   * The four parameters define the sequence.  For example
   * SequentialVariable(0,5,1,2) creates a RNG that has the sequence
   * 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 0, 0 ...
   * \param f First value of the sequence.
   * \param l One more than the last value of the sequence.
   * \param i Increment between sequence values
   * \param c Number of times each member of the sequence is repeated
   */
  SequentialVariable(double f, double l, double i = 1, uint32_t c = 1);

  /**
   * \brief Constructor for the SequentialVariable RNG.
   *
   * Differs from the first only in that the increment parameter is a
   * random variable
   * \param f First value of the sequence.
   * \param l One more than the last value of the sequence.
   * \param i Reference to a RandomVariable for the sequence increment
   * \param c Number of times each member of the sequence is repeated
   */
  SequentialVariable(double f, double l, const RandomVariable& i, uint32_t c = 1);

  SequentialVariable(const SequentialVariable& c);
  
  ~SequentialVariable();
  /**
   * \return The next value in the Sequence
   */
  virtual double GetValue();
  virtual RandomVariable*  Copy() const;
private:
  double m_min;
  double m_max;
  RandomVariable*  m_increment;
  uint32_t  m_consecutive;
  double m_current;
  uint32_t  m_currentConsecutive;
};

/**
 * \brief Exponentially Distributed random var
 *
 * ExponentialVariable defines a random variable with an exponential distribution
 */
class ExponentialVariable : public RandomVariable { 
public:
  /**
   * Constructs an exponential random variable  with a mean
   * value of 1.0.
   */
  ExponentialVariable();

  /**
   * \brief Constructs an exponential random variable with a specified mean
   * \param m Mean value for the random variable
   */
  explicit ExponentialVariable(double m);

  /**
   * \brief Constructs an exponential random variable with spefified
   * \brief mean and upper limit.
   *
   * Since exponential distributions can theoretically return unbounded values,
   * it is sometimes useful to specify a fixed upper limit.  Note however when
   * the upper limit is specified, the true mean of the distribution is 
   * slightly smaller than the mean value specified.
   * \param m Mean value of the random variable
   * \param b Upper bound on returned values
   */
  ExponentialVariable(double m, double b);

  ExponentialVariable(const ExponentialVariable& c);
  
  /**
   * \return A random value from this exponential distribution
   */
  virtual double GetValue();
  virtual RandomVariable* Copy() const;
private:
  double m_mean;  // Mean value of RV
  double m_bound; // Upper bound on value (if non-zero)
};

/**
 * \brief ParetoVariable distributed random var
 */
class ParetoVariable : public RandomVariable { // 
public:
  /**
   * Constructs a pareto random variable with a mean of 1 and a shape
   * parameter of 1.5
   */
  ParetoVariable();

  /**
   * Constructs a pareto random variable with specified mean and shape
   * parameter of 1.5
   * \param m Mean value of the distribution
   */
  explicit ParetoVariable(double m);

  /**
   * Constructs a pareto random variable with the specified mean value and
   * shape parameter.
   * \param m Mean value of the distribution
   * \param s Shape parameter for the distribution
   */
  ParetoVariable(double m, double s);

  /**
   * \brief Constructs a pareto random variable with the specified mean
   * \brief value, shape (alpha), and upper bound.
   * Since pareto distributions can theoretically return unbounded values,
   * it is sometimes useful to specify a fixed upper limit.  Note however
   * when the upper limit is specified, the true mean of the distribution
   * is slightly smaller than the mean value specified.
   * \param m Mean value
   * \param s Shape parameter
   * \param b Upper limit on returned values
   */
  ParetoVariable(double m, double s, double b);

  ParetoVariable(const ParetoVariable& c);
  
  /**
   * \return A random value from this Pareto distribution
   */
  virtual double GetValue();
  virtual RandomVariable* Copy() const;
private:
  double m_mean;  // Mean value of RV
  double m_shape; // Shape parameter
  double m_bound; // Upper bound on value (if non-zero)
};

/**
 * \brief WeibullVariable distributed random var
 */
class WeibullVariable : public RandomVariable {
public:
  /**
   * Constructs a weibull random variable  with a mean
   * value of 1.0 and a shape (alpha) parameter of 1
   */
  WeibullVariable();


  /**
   * Constructs a weibull random variable with the specified mean
   * value and a shape (alpha) parameter of 1.5.
   * \param m mean value of the distribution
   */
   WeibullVariable(double m) ;

  /**
   * Constructs a weibull random variable with the specified mean
   * value and a shape (alpha).
   * \param m Mean value for the distribution.
   * \param s Shape (alpha) parameter for the distribution.
   */
  WeibullVariable(double m, double s);

   /**
   * \brief Constructs a weibull random variable with the specified mean
   * \brief value, shape (alpha), and upper bound.
   * Since WeibullVariable distributions can theoretically return unbounded values,
   * it is sometimes usefull to specify a fixed upper limit.  Note however
   * that when the upper limit is specified, the true mean of the distribution
   * is slightly smaller than the mean value specified.
   * \param m Mean value for the distribution.
   * \param s Shape (alpha) parameter for the distribution.
   * \param b Upper limit on returned values
   */
  WeibullVariable(double m, double s, double b);

  WeibullVariable(const WeibullVariable& c);
  
  /**
   * \return A random value from this Weibull distribution
   */
  virtual double GetValue();
  virtual RandomVariable* Copy() const;
private:
  double m_mean;  // Mean value of RV
  double m_alpha; // Shape parameter
  double m_bound; // Upper bound on value (if non-zero)
};

/**
 * Class NormalVariable defines a random variable with a
 * normal (Gaussian) distribution.
 */
class NormalVariable : public RandomVariable { // Normally Distributed random var

public:
  /**
   * Constructs an normal random variable  with a mean
   * value of 0 and variance of 1.
   */ 
  NormalVariable();


  /**
   * \brief Construct a normal random variable with specified mean and variance
   * \param m Mean value
   * \param v Variance
   * \param b Bound.  The NormalVariable is bounded within +-bound.
   */ 
  NormalVariable(double m, double v, double b = INFINITE_VALUE);

  NormalVariable(const NormalVariable& c);
  
  /**
   * \return A value from this normal distribution
   */
  virtual double GetValue();
  virtual RandomVariable* Copy() const;
private:
  double m_mean;      // Mean value of RV
  double m_variance;  // Mean value of RV
  double m_bound;     // Bound on value (absolute value)
  bool     m_nextValid; // True if next valid
  double m_next;      // The algorithm produces two values at a time
};

// Value/CDF pair class for Emiprical Distributions
//Doc:ClassXRef
class ValueCDF {
public:
  ValueCDF();
  ValueCDF(double v, double c);
  ValueCDF(const ValueCDF& c);
  double value;
  double    cdf;
};

/**
 * \brief EmpiricalVariable distribution random var
 *
 * Defines a random variable  that has a specified, empirical 
 * distribution.  The distribution is specified by a
 * series of calls the the CDF member function, specifying a
 * value and the probability that the function value is less than
 * the specified value.  When values are requested,
 * a uniform random variable is used to select a probabililty,
 * and the return value is interpreted linerarly between the
 * two appropriate points in the CDF
 */
class EmpiricalVariable : public RandomVariable {
public:
  /**
   * Constructor for the EmpiricalVariable random variables.
   */
  explicit EmpiricalVariable();

  virtual ~EmpiricalVariable();
  EmpiricalVariable(const EmpiricalVariable& c);
  /**
   * \return A value from this empirical distribution
   */
  virtual double GetValue();
  virtual RandomVariable* Copy() const;
  /**
   * \brief Specifies a point in the empirical distribution
   * \param v The function value for this point
   * \param c Probability that the function is less than or equal to v
   */
  virtual void CDF(double v, double c);  // Value, prob <= Value

private:
  virtual void Validate();  // Insure non-decreasing emiprical values
  virtual double Interpolate(double, double, double, double, double);
  bool validated; // True if non-decreasing validated
  std::vector<ValueCDF> emp;       // Empicical CDF
};

/**
 * Defines an empirical distribution where all values are integers.
 * Indentical to EmpiricalVariable, but with slightly different
 * interpolation between points.
 */
class IntEmpiricalVariable : public EmpiricalVariable {
public:

  IntEmpiricalVariable();
  
  virtual RandomVariable* Copy() const;
  /**
   * \return An integer value from this empirical distribution
   */
  virtual uint32_t GetIntValue();
  virtual double Interpolate(double, double, double, double, double);
};

/**
  * Defines a random variable  that has a specified, predetermined
  * sequence.  This would be useful when trying to force
  * the RNG to return a known sequence, perhaps to
  * compare NS-3 to some other simulator
  */
class DeterministicVariable : public RandomVariable {

public:
  /**
   * \brief Constructor
   *
   * Creates a generator that returns successive elements of the d array
   * on successive calls to ::Value().  Note that the d pointer is copied
   * for use by the generator (shallow-copy), not its contents, so the 
   * contents of the array d points to have to remain unchanged for the use 
   * of DeterministicVariable to be meaningful.
   * \param d Pointer to array of random values to return in sequence
   * \param c Number of values in the array
   */
  explicit DeterministicVariable(double* d, uint32_t c);

  virtual ~DeterministicVariable();
  /**
   * \return The next value in the deterministic sequence
   */
  virtual double GetValue();
  virtual RandomVariable* Copy() const;
private:
  uint32_t   count;
  uint32_t   next;
  double* data;
};

}//namespace ns3
#endif
