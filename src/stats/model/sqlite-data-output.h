/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 Drexel University
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
 * Author: Joe Kopena (tjkopena@cs.drexel.edu)
 */

#ifndef SQLITE_DATA_OUTPUT_H
#define SQLITE_DATA_OUTPUT_H

#include "ns3/nstime.h"

#include "data-output-interface.h"

#define STATS_HAS_SQLITE3

struct sqlite3;

namespace ns3 {

//------------------------------------------------------------
//--------------------------------------------
/**
 * \ingroup stats
 *
 */
class SqliteDataOutput : public DataOutputInterface {
public:
  SqliteDataOutput();
  virtual ~SqliteDataOutput();

  virtual void Output (DataCollector &dc);

protected:
  virtual void DoDispose ();

private:
  class SqliteOutputCallback : public DataOutputCallback {
public:
    SqliteOutputCallback(Ptr<SqliteDataOutput> owner, std::string run);

    void OutputStatistic (std::string key,
                          std::string variable,
                          const StatisticalSummary *statSum);

    void OutputSingleton (std::string key,
                          std::string variable,
                          int val);

    void OutputSingleton (std::string key,
                          std::string variable,
                          uint32_t val);

    void OutputSingleton (std::string key,
                          std::string variable,
                          double val);

    void OutputSingleton (std::string key,
                          std::string variable,
                          std::string val);

    void OutputSingleton (std::string key,
                          std::string variable,
                          Time val);

private:
    Ptr<SqliteDataOutput> m_owner;
    std::string m_runLabel;

    // end class SqliteOutputCallback
  };


  sqlite3 *m_db;
  int Exec (std::string exe);

  // end class SqliteDataOutput
};

// end namespace ns3
};


#endif /* SQLITE_DATA_OUTPUT_H */
