#ifndef PROPAGATION_LOSS_MODEL_H
#define PROPAGATION_LOSS_MODEL_H

#include "ns3/object.h"

namespace ns3 {

class MobilityModel;

class RandomVariable;

/**
 * \brief Modelize the propagation loss through a transmission medium
 *
 * Calculate the receive power (dbm) from a transmit power (dbm),
 * and, a mobility model for the source and destination positions.
 */
class PropagationLossModel : public Object
{
public:
  virtual ~PropagationLossModel ();
  /**
   * \param txPowerDbm the tx power
   * \param a the mobility model of the source
   * \param b the mobility model of the destination
   * \returns the receive power (dbm)
   */
  virtual double GetRxPower (double txPowerDbm,
			     Ptr<MobilityModel> a,
			     Ptr<MobilityModel> b) const = 0;

  /**
   * \returns the default propagation loss model as specified
   * by the PropagationLossModelType ns3::DefaultValue.
   */
  static Ptr<PropagationLossModel> CreateDefault (void);
};

/**
 * \brief The propagation loss is random
 */ 
class RandomPropagationLossModel : public PropagationLossModel
{
public:
  /**
   * Use the default parameters from RandomPropagation.
   */
  RandomPropagationLossModel ();
  /**
   * \param variable the RandomVariable to use for this
   * instance.
   */
  RandomPropagationLossModel (const RandomVariable &variable);
  virtual ~RandomPropagationLossModel ();

  virtual double GetRxPower (double txPowerDbm,
			     Ptr<MobilityModel> a,
			     Ptr<MobilityModel> b) const;
private:
  RandomVariable *m_variable;
};

/**
 * \brief a Friis propagation loss model
 *
 * XXX: link to model description.
 */
class FriisPropagationLossModel : public PropagationLossModel
{
public:
  /**
   * Use the default parameters from FriisPropagationLambda.
   */
  FriisPropagationLossModel ();
  /**
   * \param frequency (Hz)
   * \param speed (m/s)
   *
   * Set the main wavelength used in the Friis model 
   * calculation.
   */
  void SetLambda (double frequency, double speed);
  /**
   * \param lambda (m) the wavelength
   *
   * Set the main wavelength used in the Friis model 
   * calculation.
   */
  void SetLambda (double lambda);
  /**
   * \param systemLoss (dimension-less)
   *
   * Set the system loss used by the Friis propagation model.
   */
  void SetSystemLoss (double systemLoss);
  /**
   * \returns the current wavelength (m)
   */
  double GetLambda (void) const;
  /**
   * \returns the current system loss (dimention-less)
   */
  double GetSystemLoss (void) const;

  virtual double GetRxPower (double txPowerDbm,
			     Ptr<MobilityModel> a,
			     Ptr<MobilityModel> b) const;
private:
  double DbmToW (double dbm) const;
  double DbmFromW (double w) const;

  static const double PI;
  double m_lambda;
  double m_systemLoss;
};

/**
 * \brief a Path Loss propagation model.
 *
 * XXX: link to description model.
 */
class PathLossPropagationLossModel : public PropagationLossModel
{
public:
  /**
   * Use the default parameters from
   * PathLossPropagationExponent, and,
   * PathLossPropagationReferenceType
   * to create a new propagation loss model.
   */
  PathLossPropagationLossModel ();

  /**
   * \param n the path loss exponent.
   * Set the path loss exponent.
   */
  void SetPathLossExponent (double n);
  /** 
   * \returns the current path loss exponent.
   */
  double GetPathLossExponent (void) const;

  /**
   * \param model the reference propagation model
   */
  void SetReferenceModel (Ptr<PropagationLossModel> model);
  
  virtual double GetRxPower (double txPowerDbm,
			     Ptr<MobilityModel> a,
			     Ptr<MobilityModel> b) const;
private:
  double DbToW (double db) const;
  static Ptr<PropagationLossModel> CreateDefaultReference (void);

  double m_exponent;
  Ptr<PropagationLossModel> m_reference;
};

} // namespace ns3

#endif /* PROPAGATION_LOSS_MODEL_H */
