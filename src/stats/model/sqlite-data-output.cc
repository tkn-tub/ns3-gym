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

#include <sstream>

#include <sqlite3.h>

#include "ns3/log.h"
#include "ns3/nstime.h"

#include "data-collector.h"
#include "data-calculator.h"
#include "sqlite-data-output.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SqliteDataOutput");

//--------------------------------------------------------------
//----------------------------------------------
SqliteDataOutput::SqliteDataOutput()
{
  NS_LOG_FUNCTION (this);

  m_filePrefix = "data";
}
SqliteDataOutput::~SqliteDataOutput()
{
  NS_LOG_FUNCTION (this);
}
void
SqliteDataOutput::DoDispose ()
{
  NS_LOG_FUNCTION (this);

  DataOutputInterface::DoDispose ();
  // end SqliteDataOutput::DoDispose
}

int
SqliteDataOutput::Exec (std::string exe) {
  NS_LOG_FUNCTION (this << exe);

  int res;
  char **result;
  int nrows, ncols;
  char *errMsg = 0;

  NS_LOG_INFO ("executing '" << exe << "'");

  res = sqlite3_get_table (m_db,
                           exe.c_str (),
                           &result, &nrows, &ncols,
                           &errMsg);

  if (res != SQLITE_OK) {
      NS_LOG_ERROR ("sqlite3 error: \"" << errMsg << "\"");
      /*
      } else {
        // std::cout << "nrows " << nrows << " ncols " << ncols << std::endl;

        if (nrows > 0) {
          for (int i = 0; i < ncols; i++) {
            std::cout << "  " << result[i];
          }
          std::cout << std::endl;

          for (int r = 1; r <= nrows; r++) {
            for (int c = 0; c < ncols; c++) {
              std::cout << "  " << result[(r*ncols)+c];
            }
            std::cout << std::endl;
          }
          std::cout << std::endl;
        }
      */
    }

  sqlite3_free_table (result);
  return res;

  // end SqliteDataOutput::Exec
}

//----------------------------------------------
void
SqliteDataOutput::Output (DataCollector &dc)
{
  NS_LOG_FUNCTION (this << &dc);

  std::string m_dbFile = m_filePrefix + ".db";

  if (sqlite3_open (m_dbFile.c_str (), &m_db)) {
      NS_LOG_ERROR ("Could not open sqlite3 database \"" << m_dbFile << "\"");
      NS_LOG_ERROR ("sqlite3 error \"" << sqlite3_errmsg (m_db) << "\"");
      sqlite3_close (m_db);
      /// \todo Better error reporting, management!
      return;
    }

  std::string run = dc.GetRunLabel ();

  Exec ("create table if not exists Experiments (run, experiment, strategy, input, description text)");
  Exec ("insert into Experiments (run,experiment,strategy,input,description) values ('" +
        run + "', '" +
        dc.GetExperimentLabel () + "', '" +
        dc.GetStrategyLabel () + "', '" +
        dc.GetInputLabel () + "', '" +
        dc.GetDescription () + "')");

  Exec ("create table if not exists Metadata ( run text, key text, value)");

  for (MetadataList::iterator i = dc.MetadataBegin ();
       i != dc.MetadataEnd (); i++) {
      std::pair<std::string, std::string> blob = (*i);
      Exec ("insert into Metadata (run,key,value) values ('" +
            run + "', '" +
            blob.first + "', '" +
            blob.second + "')");
    }

  Exec ("BEGIN");
  SqliteOutputCallback callback (this, run);
  for (DataCalculatorList::iterator i = dc.DataCalculatorBegin ();
       i != dc.DataCalculatorEnd (); i++) {
      (*i)->Output (callback);
    }
  Exec ("COMMIT");

  sqlite3_close (m_db);

  // end SqliteDataOutput::Output
}

SqliteDataOutput::SqliteOutputCallback::SqliteOutputCallback
  (Ptr<SqliteDataOutput> owner, std::string run) :
  m_owner (owner),
  m_runLabel (run)
{
  NS_LOG_FUNCTION (this << owner << run);

  m_owner->Exec ("create table if not exists Singletons ( run text, name text, variable text, value )");

  // end SqliteDataOutput::SqliteOutputCallback::SqliteOutputCallback
}

void
SqliteDataOutput::SqliteOutputCallback::OutputStatistic (std::string key,
                                                         std::string variable,
                                                         const StatisticalSummary *statSum)
{
  NS_LOG_FUNCTION (this << key << variable << statSum);

  OutputSingleton (key,variable+"-count", (double)statSum->getCount ());
  if (!isNaN (statSum->getSum ()))
    OutputSingleton (key,variable+"-total", statSum->getSum ());
  if (!isNaN (statSum->getMax ()))
    OutputSingleton (key,variable+"-max", statSum->getMax ());
  if (!isNaN (statSum->getMin ()))
    OutputSingleton (key,variable+"-min", statSum->getMin ());
  if (!isNaN (statSum->getSqrSum ()))
    OutputSingleton (key,variable+"-sqrsum", statSum->getSqrSum ());
  if (!isNaN (statSum->getStddev ()))
    OutputSingleton (key,variable+"-stddev", statSum->getStddev ());
}


void
SqliteDataOutput::SqliteOutputCallback::OutputSingleton (std::string key,
                                                         std::string variable,
                                                         int val)
{
  NS_LOG_FUNCTION (this << key << variable << val);

  std::stringstream sstr;
  sstr << "insert into Singletons (run,name,variable,value) values ('" <<
  m_runLabel << "', '" <<
  key << "', '" <<
  variable << "', " <<
  val << ")";
  m_owner->Exec (sstr.str ());

  // end SqliteDataOutput::SqliteOutputCallback::OutputSingleton
}
void
SqliteDataOutput::SqliteOutputCallback::OutputSingleton (std::string key,
                                                         std::string variable,
                                                         uint32_t val)
{
  NS_LOG_FUNCTION (this << key << variable << val);

  std::stringstream sstr;
  sstr << "insert into Singletons (run,name,variable,value) values ('" <<
  m_runLabel << "', '" <<
  key << "', '" <<
  variable << "', " <<
  val << ")";
  m_owner->Exec (sstr.str ());
  // end SqliteDataOutput::SqliteOutputCallback::OutputSingleton
}
void
SqliteDataOutput::SqliteOutputCallback::OutputSingleton (std::string key,
                                                         std::string variable,
                                                         double val)
{
  NS_LOG_FUNCTION (this << key << variable << val);

  std::stringstream sstr;
  sstr << "insert into Singletons (run,name,variable,value) values ('" <<
  m_runLabel << "', '" <<
  key << "', '" <<
  variable << "', " <<
  val << ")";
  m_owner->Exec (sstr.str ());
  // end SqliteDataOutput::SqliteOutputCallback::OutputSingleton
}
void
SqliteDataOutput::SqliteOutputCallback::OutputSingleton (std::string key,
                                                         std::string variable,
                                                         std::string val)
{
  NS_LOG_FUNCTION (this << key << variable << val);

  std::stringstream sstr;
  sstr << "insert into Singletons (run,name,variable,value) values ('" <<
  m_runLabel << "', '" <<
  key << "', '" <<
  variable << "', '" <<
  val << "')";
  m_owner->Exec (sstr.str ());
  // end SqliteDataOutput::SqliteOutputCallback::OutputSingleton
}
void
SqliteDataOutput::SqliteOutputCallback::OutputSingleton (std::string key,
                                                         std::string variable,
                                                         Time val)
{
  NS_LOG_FUNCTION (this << key << variable << val);

  std::stringstream sstr;
  sstr << "insert into Singletons (run,name,variable,value) values ('" <<
  m_runLabel << "', '" <<
  key << "', '" <<
  variable << "', " <<
  val.GetTimeStep () << ")";
  m_owner->Exec (sstr.str ());
  // end SqliteDataOutput::SqliteOutputCallback::OutputSingleton
}
