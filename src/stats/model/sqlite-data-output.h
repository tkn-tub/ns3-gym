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
 * \ingroup dataoutput
 * \class SqliteDataOutput
 * \brief Outputs data in a format compatible with SQLite
 */
class SqliteDataOutput : public DataOutputInterface {
public:
  SqliteDataOutput();
  virtual ~SqliteDataOutput();

  virtual void Output (DataCollector &dc);

protected:
  virtual void DoDispose ();

private:
  /**
   * \ingroup dataoutput
   *
   * \brief Class to generate OMNeT output
   */
  class SqliteOutputCallback : public DataOutputCallback {
public:
    /**
     * Constructor
     * \param owner pointer to the instance this object belongs to
     * \param run experiment descriptor
     */
    SqliteOutputCallback(Ptr<SqliteDataOutput> owner, std::string run);

    /**
     * \brief Generates data statistics
     * \param key the SQL key to use
     * \param variable the variable name
     * \param statSum the stats to print
     */
    void OutputStatistic (std::string key,
                          std::string variable,
                          const StatisticalSummary *statSum);

    /**
     * \brief Generates a single data output
     * \param key the SQL key to use
     * \param variable the variable name
     * \param val the value
     */
    void OutputSingleton (std::string key,
                          std::string variable,
                          int val);

    /**
     * \brief Generates a single data output
     * \param key the SQL key to use
     * \param variable the variable name
     * \param val the value
     */
    void OutputSingleton (std::string key,
                          std::string variable,
                          uint32_t val);

    /**
     * \brief Generates a single data output
     * \param key the SQL key to use
     * \param variable the variable name
     * \param val the value
     */
    void OutputSingleton (std::string key,
                          std::string variable,
                          double val);

    /**
     * \brief Generates a single data output
     * \param key the SQL key to use
     * \param variable the variable name
     * \param val the value
     */
    void OutputSingleton (std::string key,
                          std::string variable,
                          std::string val);

    /**
     * \brief Generates a single data output
     * \param key the SQL key to use
     * \param variable the variable name
     * \param val the value
     */
    void OutputSingleton (std::string key,
                          std::string variable,
                          Time val);

private:
    Ptr<SqliteDataOutput> m_owner; //!< the instance this object belongs to
    std::string m_runLabel; //!< Run label

    // end class SqliteOutputCallback
  };


  sqlite3 *m_db; //!< pointer to the SQL database

  /**
   * \brief Execute a sqlite3 query
   * \param exe the query to execute
   * \return sqlite return code.
   */
  int Exec (std::string exe);

  // end class SqliteDataOutput
};

// end namespace ns3
};


#endif /* SQLITE_DATA_OUTPUT_H */
