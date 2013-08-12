/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of Jyvaskyla
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Budiarto Herman <buherman@student.jyu.fi>
 *
 */

#ifndef LTE_ANR_H
#define LTE_ANR_H

#include <ns3/object.h>
#include <ns3/lte-rrc-sap.h>
#include <ns3/lte-anr-sap.h>
#include <map>

namespace ns3 {


class LteAnrSapProvider;
class LteAnrSapUser;
class LteNeighbourRelation;

/**
 * \brief Automatic Neighbour Relation function.
 *
 * Based on Section 22.3.2a and 22.3.3 of 3GPP TS 36.300.
 */
class LteAnr : public Object
{
public:
  LteAnr ();
  virtual ~LteAnr ();

  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  /**
   * \brief Provide an advance information about a related neighbouring cell
   *        and add it as a new Neighbour Relation entry.
   *
   * This function simulates the Neighbour Relation addition operation by
   * network operations and maintenance, as depicted in Section 22.3.2a of
   * 3GPP TS 36.300.
   *
   * An entry added by this function will have NoRemove flag set to TRUE and
   * NoHo flag set to TRUE. Hence, the cell may not act as the targel cell of a
   * handover, unless a measurement report of the cell is received, which will
   * update the NoHo flag to FALSE.
   */
  void AddNeighbourRelation (uint16_t cellId);

  /**
   * \brief Remove an existing Neighbour Relation entry.
   *
   * This function simulates the Neighbour Relation removal operation by
   * network operations and maintenance, as depicted in Section 22.3.2a of
   * 3GPP TS 36.300.
   */
  void RemoveNeighbourRelation (uint16_t cellId);

  /**
   * \brief Set the user part of the LteAnrSap that this ANR instance will
   *        interact with. Normally this part of the SAP is exported by the
   *        eNodeB RRC.
   * \param s
   */
  virtual void SetLteAnrSapUser (LteAnrSapUser* s);

  /**
   *
   * \return the Provider part of the LteAnrSap provided by the ANR instance
   */
  virtual LteAnrSapProvider* GetLteAnrSapProvider ();

  friend class MemberLteAnrSapProvider<LteAnr>;

protected:
  // inherited from Object
  virtual void DoInitialize ();

private:
  // ANR SAP provider implementation
  void DoReportUeMeas (LteRrcSap::MeasResults measResults);
  void DoAddNeighbourRelation (uint16_t cellId);
  bool DoGetNoRemove (uint16_t cellId);
  bool DoGetNoHo (uint16_t cellId);
  bool DoGetNoX2 (uint16_t cellId);

  // ANR SAPs
  LteAnrSapUser* m_anrSapUser;
  LteAnrSapProvider* m_anrSapProvider;

  // Class Attributes
  uint8_t m_threshold;

  /**
   * \brief Neighbour Relation between two eNodeBs (serving eNodeB and neighbour
   *        eNodeB).
   */
  struct NeighbourRelation_t
  {
    bool noRemove;
    bool noHo;
    bool noX2;
    bool detectedAsNeighbour;
  };

  //               cellId
  typedef std::map<uint16_t, NeighbourRelation_t> NeighbourRelationTable_t;

  NeighbourRelationTable_t m_neighbourRelationTable;

  // internal methods
  NeighbourRelation_t* Find (uint16_t cellId);

  // The expected measurement identity
  uint8_t m_measId;

}; // end of class LteAnr


} // end of namespace ns3


#endif /* LTE_ANR_H */
