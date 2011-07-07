/*
 * simple-device-energy-model.h
 *
 *  Created on: 09/ago/2010
 *      Author: socket
 */

#ifndef SIMPLE_DEVICE_ENERGY_MODEL_H
#define SIMPLE_DEVICE_ENERGY_MODEL_H

#include "ns3/device-energy-model.h"
#include "ns3/nstime.h"
#include "ns3/traced-value.h"

namespace ns3 {

/**
 * \ingroup energy
 *
 * A simple device energy model where current drain can be set by the user.
 *
 * It is supposed to be used as a testing model for energy sources.
 *
 */
class SimpleDeviceEnergyModel : public DeviceEnergyModel
{
public:
  static TypeId GetTypeId (void);
  SimpleDeviceEnergyModel ();
  virtual ~SimpleDeviceEnergyModel ();

  /**
   * \brief Sets pointer to node.
   *
   * \param node Pointer to node.
   *
   * Implements DeviceEnergyModel::SetNode.
   */
  virtual void SetNode (Ptr<Node> node);

  /**
   * \brief Gets pointer to node.
   *
   * \returns Pointer to node.
   *
   * Implements DeviceEnergyModel::GetNode.
   */
  virtual Ptr<Node> GetNode (void) const;

  /**
   * \brief Sets pointer to EnergySouce installed on node.
   *
   * \param source Pointer to EnergySource installed on node.
   *
   * Implements DeviceEnergyModel::SetEnergySource.
   */
  virtual void SetEnergySource (Ptr<EnergySource> source);

  /**
   * \returns Total energy consumption of the vehicle.
   *
   * Implements DeviceEnergyModel::GetTotalEnergyConsumption.
   */
  virtual double GetTotalEnergyConsumption (void) const;

  /**
   * \param newState New state the device is in.
   *
   * Not implemented
   */
  virtual void ChangeState (int newState)
  {
  }

  /**
   * \brief Handles energy depletion.
   *
   * Not implemented
   */
  virtual void HandleEnergyDepletion (void)
  {
  }

  /**
   * \param current the current draw of device.
   *
   * Set the actual current draw of the device.
   */
  void SetCurrentA (double current);
private:
  void DoDispose (void);

  /**
   * \returns Current draw of device, at current state.
   *
   * Implements DeviceEnergyModel::GetCurrentA.
   */
  virtual double DoGetCurrentA (void) const;

  Time m_lastUpdateTime;
  double m_actualCurrentA;
  Ptr<EnergySource> m_source;
  Ptr<Node> m_node;
  TracedValue<double> m_totalEnergyConsumption;
};

} // namespace ns3

#endif /* SIMPLE_DEVICE_ENERGY_MODEL_H */
