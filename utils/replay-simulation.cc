/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/simulator.h"
#include "ns3/scheduler-list.h"
#include "ns3/scheduler-map.h"
#include "ns3/scheduler-heap.h"
#include "ns3/nstime.h"
#include "ns3/system-wall-clock-ms.h"
#include <vector>
#include <deque>
#include <fstream>
#include <iostream>

using namespace ns3;

class LogReader {
public:
  void ReadFromFilename (char const *filename);
  void Run (void);
  void PrintStats (void);
private:
  struct Command {
      enum {
          REMOVE = 100,
          INSERT,
          INSERT_LATER,
          INSERT_REMOVE
      } m_type;
      // uid at which this command is supposed to be executed.
      uint32_t m_uid;
      union {
          struct {
            // time at which the event is supposed to expire
            uint64_t m_evNs;
            uint32_t m_evUid;
          } insert;
          struct {
            // location in the array of events to remove where
            // to insert this event once it is inserted in 
            // the scheduler.
            uint32_t m_evLoc; 
            // time at which the event is supposed to expire
            uint64_t m_evNs;
          } insertRemove;
      };
  };
  void ExecuteLogCommands (uint32_t uid);

  typedef std::vector<struct Command> Commands;
  typedef std::vector<struct Command>::iterator CommandsI;
  typedef std::vector<EventId> RemoveEvents;
  typedef std::vector<EventId>::iterator RemoveEventsI;
  

  Commands m_commands;
  CommandsI m_command;
  RemoveEvents m_removeEvents;
  uint32_t m_uid;
};

typedef std::vector<std::pair<uint32_t, uint32_t> > Removes;
typedef std::vector<std::pair<uint32_t, uint32_t> >::iterator RemovesI;

void
LogReader::ReadFromFilename (char const *filename)
{
  std::ifstream log;
  std::cout << "read log..." << std::endl;
  Removes removes;
  log.open (filename);
  while (!log.eof ()) 
    {
      std::string type;
      log >> type;
      if (type == "i") 
        {
          uint32_t nowUid, evUid;
          uint64_t nowNs, evNs;
          log >> nowUid >> nowNs >> evUid >> evNs;
          struct Command cmd;
          cmd.m_type = Command::INSERT;
          cmd.m_uid = nowUid;
          cmd.insert.m_evNs = evNs;
          cmd.insert.m_evUid = evUid;
          m_commands.push_back (cmd);
        } 
      else if (type == "r") 
        {
          uint32_t nowUid, evUid;
          uint64_t nowNs, evNs;
          log >> nowUid >> nowNs >> evUid >> evNs;
          struct Command cmd;
          cmd.m_type = Command::REMOVE;
          cmd.m_uid = nowUid;
          m_commands.push_back (cmd);
          removes.push_back (std::make_pair (nowUid, evUid));
        } 
      else if (type == "il") 
        {
          uint32_t nowUid, evUid;
          uint64_t nowNs, evNs;
          log >> nowUid >> nowNs >> evUid >> evNs;
          struct Command cmd;
          cmd.m_type = Command::INSERT_LATER;
          cmd.m_uid = nowUid;
          m_commands.push_back (cmd);
        }
    }
  log.close ();

  std::cout << "gather insert/removes, commands="<<m_commands.size () 
            << ", removes=" << removes.size () << "..." << std::endl;
  for (CommandsI i = m_commands.begin (); i != m_commands.end (); i++) 
    {
      if (i->m_type == Command::INSERT) 
        {
          for (RemovesI j = removes.begin (); j != removes.end (); j++) 
            {
              if (j->second == i->insert.m_evUid) 
                {
                  // this insert will be removed later.
                  uint64_t ns = i->insert.m_evNs;
                  uint32_t uid = i->m_uid;
                  i->m_type = Command::INSERT_REMOVE;
                  i->m_uid = uid;
                  i->insertRemove.m_evNs = ns;
                  i->insertRemove.m_evLoc = j->first;
                  break;
                }
            }
        }
    }
  std::cout << "calculate remove locations..." << std::endl;
  // calculate the final insert/remove location.
  for (CommandsI i = m_commands.begin (); i != m_commands.end (); i++) 
    {
      if (i->m_type == Command::INSERT_REMOVE) 
        {
          uint32_t loc = 0;
          for (CommandsI tmp = i; tmp != m_commands.end (); tmp++) 
            {
              if (tmp->m_type == Command::REMOVE) 
                {
                  if (tmp->m_uid == i->insertRemove.m_evLoc) 
                    {
                      i->insertRemove.m_evLoc = loc;
                      break;
                    }
                  loc++;
                }
            }
        }
    }
}
void
LogReader::ExecuteLogCommands (uint32_t uid)
{
  if (m_command == m_commands.end ()) 
    {
      return;
    }
  //std::cout << "one event, uid=" <<m_uid<< std::endl;
  struct Command cmd = *m_command;
  //std::cout << "cmd uid=" <<cmd.m_uid<< std::endl;
  while (cmd.m_uid == uid) 
    {
      m_command++;
      switch (cmd.m_type) {
      case Command::INSERT:
        //std::Cout << "exec insert now=" << Simulator::Now ().GetNanoSeconds ()
        //<< ", time=" << cmd.insert.m_evNs << std::endl;
        Simulator::Schedule (NanoSeconds (cmd.insert.m_evNs) - Now (), 
                             &LogReader::ExecuteLogCommands, this, m_uid);
        m_uid++;
        break;
      case Command::INSERT_LATER:
          //std::cout << "exec insert later" << std::endl;
          Simulator::ScheduleNow (&LogReader::ExecuteLogCommands, this, m_uid);
          m_uid++;
          break;
      case Command::REMOVE: 
        {
          //std::cout << "exec remove" << std::endl;
          EventId id = m_removeEvents.back ();
          m_removeEvents.pop_back ();
          Simulator::Remove (id);
        } break;
      case Command::INSERT_REMOVE: 
        {
          //std::cout << "exec insert remove" << std::endl;
          EventId id = Simulator::Schedule (NanoSeconds (cmd.insertRemove.m_evNs) - Now (),
                                            &LogReader::ExecuteLogCommands, this, m_uid);
          NS_ASSERT (id.GetUid () == m_uid);
          if (cmd.insertRemove.m_evLoc + 1 > m_removeEvents.size ())
            {
              uint32_t missing = cmd.insertRemove.m_evLoc + 1 - m_removeEvents.size ();
              m_removeEvents.insert (m_removeEvents.begin (),
                                     missing, id);
            }
          uint32_t index = m_removeEvents.size () - cmd.insertRemove.m_evLoc - 1;
          m_removeEvents[index] = id;
          m_uid++;
        } break;
      default:
        NS_ASSERT (false);
        break;
      }
      cmd = *m_command;
  }
}

void
LogReader::PrintStats (void)
{
  uint32_t nInserts = 0;
  uint32_t nRemoves = 0;
  for (CommandsI i = m_commands.begin (); i != m_commands.end (); i++) 
    {
      switch (i->m_type) {
      case Command::INSERT:
          nInserts++;
          break;
      case Command::INSERT_LATER:
          nInserts++;
          break;
      case Command::INSERT_REMOVE:
          nInserts++;
          break;
      case Command::REMOVE:
          nRemoves++;
          break;
      }
    }
  std::cout << "inserts="<<nInserts<<", removes="<<nRemoves<<std::endl;
  std::cout << "run simulation..."<<std::endl;
}

void
LogReader::Run (void)
{
  m_uid = 1;
  SystemWallClockMs time;
  time.Start ();
  m_command = m_commands.begin ();
  ExecuteLogCommands (0);
  Simulator::Run ();
  unsigned long long delta = time.End ();
  double delay = ((double)delta)/1000;
  std::cout << "runtime="<<delay<<"s"<<std::endl;
}


int main (int argc, char *argv[])
{
  char const *input = 0;
  uint32_t n = 1;
  bool is_map = false;
  bool is_list = false;
  bool is_heap = false;
  while (argc > 0) 
    {
      if (strcmp ("--list", argv[0]) == 0) 
        {
          is_list = true;
        } 
      else if (strcmp ("--heap", argv[0]) == 0) 
        {
          is_heap = true;
        } 
      else if (strcmp ("--map", argv[0]) == 0) 
        {
          is_map = true;
        } 
      else if (strncmp ("--n=", argv[0], strlen("--n=")) == 0) 
        {
          n = atoi (argv[0]+strlen ("--n="));
        } 
      else if (strncmp ("--input=", argv[0],strlen ("--input=")) == 0) 
        {
          input = argv[0] + strlen ("--input=");
        } 
      else if (strncmp ("--log=", argv[0],strlen ("--log=")) == 0) 
        {
          char const *filename = argv[0] + strlen ("--log=");
          Simulator::EnableLogTo (filename);
        }
      argc--;
      argv++;
    }
  if (input == 0) 
    {
      std::cerr << "need --input=[filename] option" << std::endl;
      return 1;
    }
  LogReader log;
  log.ReadFromFilename (input);
  std::cout << "start runs..." << std::endl;
  for (uint32_t i = 0; i < n; i++) 
    {
      if (is_map)
        {
          Simulator::SetScheduler (CreateObject<SchedulerMap> ());
        }
      else if (is_list)
        {
          Simulator::SetScheduler (CreateObject<SchedulerList> ());
        }
      else if (is_heap)
        {
          Simulator::SetScheduler (CreateObject<SchedulerHeap> ());
        }
      log.Run ();
      Simulator::Destroy ();
    }
}
