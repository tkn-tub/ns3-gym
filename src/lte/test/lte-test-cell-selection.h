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

#ifndef LTE_TEST_CELL_SELECTION_H
#define LTE_TEST_CELL_SELECTION_H

#include <ns3/test.h>
#include <ns3/nstime.h>
#include <ns3/node-container.h>
#include <ns3/vector.h>
#include <ns3/lte-ue-rrc.h>
#include <vector>

namespace ns3 {


/**
 * \brief Test suite for executing the cell selection test cases in without-EPC
 *        and with-EPC scenarios.
 *
 * \sa ns3::LteCellSelectionTestCase
 */
class LteCellSelectionTestSuite : public TestSuite
{
public:
  LteCellSelectionTestSuite ();
};



/**
 * \brief Testing the initial cell selection procedure by UE at IDLE state in
 *        the beginning of simulation.
 */
class LteCellSelectionTestCase : public TestCase
{
public:
  /**
   * \brief A set of input parameters for setting up a UE in the simulation.
   */
  struct UeSetup_t
  {
    Vector position; ///< The position where the UE will be spawned in the simulation.
    uint32_t plmnIdentity; ///< Public Land Mobile Network identity of the UE.
    uint32_t csgIdentity; ///< Closed Subscriber Group identity of the UE.
    uint16_t expectedCellId; ///< The cell ID that the UE is expected to attach to (0 means that the UE should not attach to any cell).
    UeSetup_t (Vector position, uint32_t plmnIdentity, uint32_t csgIdentity,
               uint16_t expectedCellId);
  };

  /**
   * \brief Creates an instance of the initial cell selection test case.
   * \param name name of this test
   * \param isEpcMode set to true for setting up simulation with EPC enabled
   * \param ueSetupList an array of UE setup parameters
   * \param duration length of simulation
   */
  LteCellSelectionTestCase (std::string name, bool isEpcMode,
                            bool hasPlmnDiversity, bool hasCsgDiversity,
                            std::vector<UeSetup_t> ueSetupList, Time duration);

  virtual ~LteCellSelectionTestCase ();

private:
  /**
   * \brief Setup the simulation according to the configuration set by the
   *        class constructor, run it, and verify the result.
   */
  virtual void DoRun ();

  void MibReceivedCallback (std::string context, uint64_t imsi,
                            uint16_t cellId, uint16_t rnti,
                            uint16_t sourceCellId);
  void Sib1ReceivedCallback (std::string context, uint64_t imsi,
                             uint16_t cellId, uint16_t rnti,
                             uint16_t sourceCellId);
  void StateTransitionCallback (std::string context, uint64_t imsi,
                                uint16_t cellId, uint16_t rnti,
                                LteUeRrc::State oldState, LteUeRrc::State newState);
  void InitialCellSelectionEndOkCallback (std::string context, uint64_t imsi,
                                          uint16_t cellId);
  void InitialCellSelectionEndErrorCallback (std::string context, uint64_t imsi,
                                             uint16_t cellId);

  /**
   * \brief If true, then the simulation should be set up with EPC enabled.
   */
  bool m_isEpcMode;

  /**
   * \brief If true, then the north and south cells will be on their own PLMN.
   */
  bool m_hasPlmnDiversity;

  /**
   * \brief If true, then the west cells in the simulation will be CSG cell,
   *        while the east cells will be non-CSG cells.
   */
  bool m_hasCsgDiversity;

  /**
   * \brief The list of UE setups to be used during the test execution.
   */
  std::vector<UeSetup_t> m_ueSetupList;

  /**
   * \brief The length of the simulation.
   *
   * The shortest possible simulation length for testing initial cell selection
   * is 206 milliseconds. If RRC_CONNECTED state is required, then the length
   * should be extended to 261 milliseconds in ideal RRC protocol, or at least
   * 277 milliseconds in real RRC protocol. Moreover, scenarios which expect
   * failure in initial cell selection procedure might want to extend this even
   * further to give the UE the chance to retry the procedure.
   */
  Time m_duration;

  /// The current UE RRC state.
  LteUeRrc::State m_lastState;

}; // end of class LteCellSelectionTestCase


} // end of namespace ns3


#endif /* LTE_TEST_CELL_SELECTION_H */
