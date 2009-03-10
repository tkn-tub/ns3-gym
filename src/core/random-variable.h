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
// Author: Hadi Arbabi<marbabi@cs.odu.edu>
//

#ifndef __random_variable_h
#define __random_variable_h

#include <vector>
#include <algorithm>
#include <stdint.h>
#include <istream>
#include <ostream>
#include "attribute.h"
#include "attribute-helper.h"

/**
 * \ingroup core
 * \defgroup randomvariable Random Variable Distributions
 *
 */

namespace ns3{

class RandomVariableBase;

class SeedManager
{
public:
	  
  /**
   * \brief set the seed
   * it will duplicate the seed value 6 times
   * \code
   * SeedManger::SetSeed(15);
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
   static uint32_t GetSeed ();
 
   /**
    * \brief Set the run number of simulation
    *
    * \code
    * SeedManager::SetSeed(12);
    * int N = atol(argv[1]); //read in run number from command line
    * SeedManager::SetRun(N);
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
  static void SetRun (uint32_t run);
  /**
   * \returns the current run number
   * @sa SetRun
   */
  static uint32_t GetRun (void);
  
  /**
   * \brief Check if seed value is valid if wanted to be used as seed
   * \return true if valid and false if invalid
   */
  static bool CheckSeed (uint32_t seed);
};


/**
 * \brief The basic RNG for NS-3.
 * \ingroup randomvariable
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
class RandomVariable
{ 
public:
  RandomVariable();
  RandomVariable(const RandomVariable&o);
  RandomVariable &operator = (const RandomVariable &o);
  ~RandomVariable();
  
  /**
   * \brief Returns a random double from the underlying distribution
   * \return A floating point random value
   */
  double GetValue (void) const;

  /**
   * \brief Returns a random integer integer from the underlying distribution
   * \return  Integer cast of ::GetValue()
   */
  uint32_t GetInteger (void) const;

private:
  friend std::ostream &operator << (std::ostream &os, const RandomVariable &var);
  friend std::istream &operator >> (std::istream &os, RandomVariable &var);

  RandomVariableBase *m_variable;
protected:
  RandomVariable (const RandomVariableBase &variable);
  RandomVariableBase *Peek (void) const;
};

/**
 * \brief The uniform distribution RNG for NS-3.
 * \ingroup randomvariable
 *
 * This class supports the creation of objects that return random numbers
 * from a fixed uniform distribution.  It also supports the generation of 
 * single random numbers from various uniform distributions.
 *
 * The low end of the range is always included and the high end
 * of the range is always excluded.
 * \code
 * UniformVariable x(0,10);
 * x.GetValue();  //will always return numbers [0,10)
 * UniformVariable::GetSingleValue(100,1000); //returns a value [100,1000)
 * \endcode
 */
class UniformVariable : public RandomVariable 
{
public:
  /**
   * Creates a uniform random number generator in the
   * range [0.0 .. 1.0).
   */
  UniformVariable();

  /**
   * Creates a uniform random number generator with the specified range
   * \param s Low end of the range
   * \param l High end of the range
   */
  UniformVariable(double s, double l);

  /**
  * \brief call RandomVariable::GetValue
  * \return A floating point random value
  *
  * Note: we have to re-implement this method here because the method is 
  * overloaded below for the two-argument variant and the c++ name resolution
  * rules don't work well with overloads split between parent and child 
  * classes.
  */
  double GetValue (void) const;
  
  /**
  * \brief Returns a random double with the specified range
  * \param s Low end of the range
  * \param l High end of the range
  * \return A floating point random value
  */
  double GetValue(double s, double l);

  /**
   * \brief Returns a random unsigned integer from the interval [s,l] including both ends.
   * \param s Low end of the range
   * \param l High end of the range
   * \return A random unsigned integer value.
   */
  uint32_t GetInteger (uint32_t s, uint32_t l);
};

/**
 * \brief A random variable that returns a constant
 * \ingroup randomvariable
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

  /**
   * \brief Specify a new constant RNG for this generator.
   * \param c New constant value for this RNG.
   */
  void SetConstant(double c);

};

/**
 * \brief Return a sequential list of values
 * \ingroup randomvariable
 *
 * Class SequentialVariable defines a random number generator that
 * returns a sequential sequence.  The sequence monotonically
 * increases for a period, then wraps around to the low value 
 * and begins monotonicaly increasing again.
 */
class SequentialVariable : public RandomVariable 
{
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

};

/**
 * \brief Exponentially Distributed random var
 * \ingroup randomvariable
 *
 * This class supports the creation of objects that return random numbers
 * from a fixed exponential distribution.  It also supports the generation of 
 * single random numbers from various exponential distributions.
 *
 * The probability density function of an exponential variable 
 * is defined over the interval [0, +inf) as:
 * \f$ \alpha  e^{-\alpha x} \f$
 * where \f$ \alpha = \frac{1}{mean} \f$ 
 *
 * The bounded version is defined over the internal [0,+inf) as:
 * \f$ \left\{ \begin{array}{cl} \alpha  e^{-\alpha x} & x < bound \\ bound & x > bound \end{array}\right. \f$
 * 
 * \code
 * ExponentialVariable x(3.14);
 * x.GetValue();  //will always return with mean 3.14
 * ExponentialVariable::GetSingleValue(20.1); //returns with mean 20.1
 * ExponentialVariable::GetSingleValue(108); //returns with mean 108
 * \endcode
 *
 */
class ExponentialVariable : public RandomVariable 
{ 
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

};

/**
 * \brief ParetoVariable distributed random var
 * \ingroup randomvariable
 *
 * This class supports the creation of objects that return random numbers
 * from a fixed pareto distribution.  It also supports the generation of 
 * single random numbers from various pareto distributions.
 *
 * The probability density function is defined over the range [\f$x_m\f$,+inf) as:
 * \f$ k \frac{x_m^k}{x^{k+1}}\f$ where \f$x_m > 0\f$ is called the location 
 * parameter and \f$ k > 0\f$ is called the pareto index or shape.
 * 
 * The parameter \f$ x_m \f$ can be infered from the mean and the parameter \f$ k \f$
 * with the equation \f$ x_m = mean \frac{k-1}{k},  k > 1\f$.
 *
 * \code
 * ParetoVariable x(3.14);
 * x.GetValue();  //will always return with mean 3.14
 * ParetoVariable::GetSingleValue(20.1); //returns with mean 20.1
 * ParetoVariable::GetSingleValue(108); //returns with mean 108
 * \endcode
 */
class ParetoVariable : public RandomVariable
{
public:
  /**
   * Constructs a pareto random variable with a mean of 1 and a shape
   * parameter of 1.5
   */
  ParetoVariable ();

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
   *
   * Since pareto distributions can theoretically return unbounded values,
   * it is sometimes useful to specify a fixed upper limit.  Note however
   * when the upper limit is specified, the true mean of the distribution
   * is slightly smaller than the mean value specified.
   * \param m Mean value
   * \param s Shape parameter
   * \param b Upper limit on returned values
   */
  ParetoVariable(double m, double s, double b);

};

/**
 * \brief WeibullVariable distributed random var
 * \ingroup randomvariable
 *
 * This class supports the creation of objects that return random numbers
 * from a fixed weibull distribution.  It also supports the generation of 
 * single random numbers from various weibull distributions.
 *
 * The probability density function is defined over the interval [0, +inf]
 * as: \f$ \frac{k}{\lambda}\left(\frac{x}{\lambda}\right)^{k-1}e^{-\left(\frac{x}{\lambda}\right)^k} \f$
 * where \f$ k > 0\f$ is the shape parameter and \f$ \lambda > 0\f$  is the scale parameter. The
 * specified mean is related to the scale and shape parameters by the following relation:
 * \f$ mean = \lambda\Gamma\left(1+\frac{1}{k}\right) \f$ where \f$ \Gamma \f$ is the Gamma function.
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

};

/**
 * \brief Class NormalVariable defines a random variable with a
 * normal (Gaussian) distribution.
 * \ingroup randomvariable
 * 
 * This class supports the creation of objects that return random numbers
 * from a fixed normal distribution.  It also supports the generation of 
 * single random numbers from various normal distributions.
 *
 * The density probability function is defined over the interval (-inf,+inf)
 * as: \f$ \frac{1}{\sigma\sqrt{2\pi}} e^{-\frac{(x-\mu)^2}{s\sigma^2}}\f$
 * where \f$ mean = \mu \f$ and \f$ variance = \sigma^2 \f$
 *
 */
class NormalVariable : public RandomVariable
{
public:
  /**
   * Constructs an normal random variable  with a mean
   * value of 0 and variance of 1.
   */ 
  NormalVariable();

  /**
   * \brief Construct a normal random variable with specified mean and variance.
   * \param m Mean value
   * \param v Variance
   */ 
  NormalVariable(double m, double v);

  /**
   * \brief Construct a normal random variable with specified mean and variance
   * \param m Mean value
   * \param v Variance
   * \param b Bound.  The NormalVariable is bounded symetrically about the mean
   * [mean-bound,mean+bound]
   */ 
  NormalVariable(double m, double v, double b);
};

/**
 * \brief EmpiricalVariable distribution random var
 * \ingroup randomvariable
 *
 * Defines a random variable  that has a specified, empirical 
 * distribution.  The distribution is specified by a
 * series of calls to the CDF member function, specifying a
 * value and the probability that the function value is less than
 * the specified value.  When values are requested,
 * a uniform random variable is used to select a probabililty,
 * and the return value is interpreted linerarly between the
 * two appropriate points in the CDF.  The method is known
 * as inverse transform sampling:
 * (http://en.wikipedia.org/wiki/Inverse_transform_sampling).
 */
class EmpiricalVariable : public RandomVariable {
public:
  /**
   * Constructor for the EmpiricalVariable random variables.
   */
  explicit EmpiricalVariable();

  /**
   * \brief Specifies a point in the empirical distribution
   * \param v The function value for this point
   * \param c Probability that the function is less than or equal to v
   */
  void CDF(double v, double c);  // Value, prob <= Value
protected:
  EmpiricalVariable (const RandomVariableBase &variable);
};

/**
 * \brief Integer-based empirical distribution
 * \ingroup randomvariable
 *
 * Defines an empirical distribution where all values are integers.
 * Indentical to EmpiricalVariable, except that the inverse transform
 * sampling interpolation described in the EmpiricalVariable documentation
 * is modified to only return integers.
 */
class IntEmpiricalVariable : public EmpiricalVariable 
{
public:
  IntEmpiricalVariable();
};

/**
 * \brief a non-random variable
 * \ingroup randomvariable
 *
 * Defines a random variable  that has a specified, predetermined
 * sequence.  This would be useful when trying to force
 * the RNG to return a known sequence, perhaps to
 * compare NS-3 to some other simulator
 */
class DeterministicVariable : public RandomVariable
{
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
};


/**
 * \brief Log-normal Distributed random var
 * \ingroup randomvariable
 *
 * LogNormalVariable defines a random variable with log-normal
 * distribution.  If one takes the natural logarithm of random
 * variable following the log-normal distribution, the obtained values
 * follow a normal distribution.
 *  This class supports the creation of objects that return random numbers
 * from a fixed lognormal distribution.  It also supports the generation of
 * single random numbers from various lognormal distributions.
 *
 * The probability density function is defined over the interval [0,+inf) as:
 * \f$ \frac{1}{x\sigma\sqrt{2\pi}} e^{-\frac{(ln(x) - \mu)^2}{2\sigma^2}}\f$
 * where \f$ mean = e^{\mu+\frac{\sigma^2}{2}} \f$ and 
 * \f$ variance = (e^{\sigma^2}-1)e^{2\mu+\sigma^2}\f$
 *
 * The \f$ \mu \f$ and \f$ \sigma \f$ parameters can be calculated from the mean
 * and standard deviation with the following equations:
 * \f$ \mu = ln(mean) - \frac{1}{2}ln\left(1+\frac{stddev}{mean^2}\right)\f$, and,
 * \f$ \sigma = \sqrt{ln\left(1+\frac{stddev}{mean^2}\right)}\f$
 */
class LogNormalVariable : public RandomVariable 
{
public:
  /**
   * \param mu mu parameter of the lognormal distribution
   * \param sigma sigma parameter of the lognormal distribution
   */
  LogNormalVariable (double mu, double sigma);
};

/**
 * \brief Triangularly Distributed random var
 * \ingroup randomvariable
 * 
 * This distribution is a triangular distribution.  The probablility density
 * is in the shape of a triangle.
 */
class TriangularVariable : public RandomVariable 
{
public:
  /**
   * Creates a triangle distribution random number generator in the
   * range [0.0 .. 1.0), with mean of 0.5
   */
  TriangularVariable();

  /**
   * Creates a triangle distribution random number generator with the specified
   * range
   * \param s Low end of the range
   * \param l High end of the range
   * \param mean mean of the distribution
   */
  TriangularVariable(double s, double l, double mean);

};

std::ostream &operator << (std::ostream &os, const RandomVariable &var);
std::istream &operator >> (std::istream &os, RandomVariable &var);

/**
 * \class ns3::RandomVariableValue
 * \brief hold objects of type ns3::RandomVariable
 */

ATTRIBUTE_VALUE_DEFINE (RandomVariable);
ATTRIBUTE_CHECKER_DEFINE (RandomVariable);
ATTRIBUTE_ACCESSOR_DEFINE (RandomVariable);

}//namespace ns3


#endif
