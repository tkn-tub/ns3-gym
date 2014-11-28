/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Lawrence Livermore National Laboratory
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
 * Author: Peter D. Barnes, Jr. <pdbarnes@llnl.gov>
 */

#include <iostream>
#include <iomanip>
#include <string>

#include "ns3/core-module.h"


using namespace ns3;


std::string g_cbArg = "cbArg default";

bool SetCbArg (std::string val)
{
  g_cbArg = val;
  return true;
}


int main (int argc, char *argv[])
{

  int         intArg  = 1;
  bool        boolArg = false;
  std::string strArg  = "strArg default";
  // Attribute path
  const std::string attrClass = "ns3::RandomVariableStream";
  const std::string attrName  = "Antithetic";
  const std::string attrPath  = attrClass + "::" + attrName;
 
  // Cache the initial values.  Normally one wouldn't do this,
  // but we want to demonstrate that CommandLine has changed them.
  const int intDef = intArg;
  const bool boolDef = boolArg;
  const std::string strDef = strArg;
  const std::string cbDef  = g_cbArg;
  // Look up default value for attribute
  const TypeId tid = TypeId::LookupByName (attrClass);
  std::string attrDef;
  {
    struct TypeId::AttributeInformation info;
    tid.LookupAttributeByName (attrName, &info);
    attrDef = info.originalInitialValue->SerializeToString (info.checker);
  }
  
  
  CommandLine cmd;
  cmd.Usage ("CommandLine example program.\n"
             "\n"
             "This little program demonstrates how to use CommandLine.");
  cmd.AddValue ("intArg",  "an int argument",       intArg);
  cmd.AddValue ("boolArg", "a bool argument",       boolArg);
  cmd.AddValue ("strArg",  "a string argument",     strArg);
  cmd.AddValue ("anti",    attrPath);
  cmd.AddValue ("cbArg",   "a string via callback", MakeCallback (SetCbArg));
  cmd.Parse (argc, argv);

  // Show initial values:
  std::cout << std::endl;
  std::cout << cmd.GetName () << ":" << std::endl;
  std::cout << "Initial values:" << std::endl;
  
  std::cout << std::left << std::setw (10) << "intArg:"
            <<                    intDef
            << std::endl;
  std::cout << std::setw (10)              << "boolArg:"
            << std::boolalpha  << boolDef  << std::noboolalpha
            << std::endl;
  
  std::cout << std::setw (10)              << "strArg:"
            << "\""            << strDef   << "\""
            << std::endl;
  std::cout << std::setw (10)              << "anti:"
            << "\""            << attrDef  << "\""
            << std::endl;
  std::cout << std::setw (10)              << "cbArg:"
            << "\""            << cbDef    << "\""
            << std::endl;
  std::cout << std::endl;


  // Show final values
  std::cout << "Final values:" << std::endl;
  std::cout << std::left << std::setw (10) << "intArg:"
            <<                    intArg
            << std::endl;
  std::cout << std::setw (10)              << "boolArg:"
            << std::boolalpha  << boolArg
            << std::noboolalpha
            << std::endl;
  
  std::cout << std::setw (10)              << "strArg:"
            << "\""            << strArg   << "\""
            << std::endl;

  // Look up new default value for attribute
  {
    struct TypeId::AttributeInformation info;
    tid.LookupAttributeByName (attrName, &info);
  
    std::cout << std::setw (10)            << "anti:"
              << "\""
              << info.initialValue->SerializeToString (info.checker)
              << "\""
              << std::endl;
  }
  std::cout << std::setw (10)              << "cbArg:"
            << "\""            << g_cbArg  << "\""
            << std::endl;

  return 0;
}
