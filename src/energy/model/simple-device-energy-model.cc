#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/energy-source.h"
#include "simple-device-energy-model.h"


namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SimpleDeviceEnergyModel);

TypeId
SimpleDeviceEnergyModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimpleDeviceEnergyModel")
    .SetParent<DeviceEnergyModel> ()
    .AddConstructor<SimpleDeviceEnergyModel> ()
    .AddTraceSource ("TotalEnergyConsumption",
                     "Total energy consumption of the radio device.",
                     MakeTraceSourceAccessor (&SimpleDeviceEnergyModel::m_totalEnergyConsumption))
  ;
  return tid;
}

SimpleDeviceEnergyModel::SimpleDeviceEnergyModel ()
{
  m_lastUpdateTime = Seconds (0.0);
  m_actualCurrentA = 0.0;
  m_source = 0;
}

SimpleDeviceEnergyModel::~SimpleDeviceEnergyModel ()
{
}

void
SimpleDeviceEnergyModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_ASSERT (source != NULL);
  m_source = source;
}

void
SimpleDeviceEnergyModel::SetNode (Ptr<Node> node)
{
  NS_ASSERT (node != NULL);
  m_node = node;
}

Ptr<Node>
SimpleDeviceEnergyModel::GetNode () const
{
  return m_node;
}

double
SimpleDeviceEnergyModel::GetTotalEnergyConsumption (void) const
{
  return m_totalEnergyConsumption;
}

void
SimpleDeviceEnergyModel::SetCurrentA (double current)
{
  Time duration = Simulator::Now () - m_lastUpdateTime;

  double energyToDecrease = 0.0;
  double supplyVoltage = m_source->GetSupplyVoltage ();
  energyToDecrease = duration.GetSeconds () * current * supplyVoltage;

  // update total energy consumption
  m_totalEnergyConsumption += energyToDecrease;
  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();
  // notify energy source
  m_source->UpdateEnergySource ();
  // update the current drain
  m_actualCurrentA = current;
}

void
SimpleDeviceEnergyModel::DoDispose (void)
{
  m_source = 0;
}

double
SimpleDeviceEnergyModel::DoGetCurrentA (void) const
{
  return m_actualCurrentA;
}

} // namespace ns3
