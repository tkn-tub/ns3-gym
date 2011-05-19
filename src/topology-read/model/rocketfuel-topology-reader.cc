/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Hajime Tazaki
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
 * Author: Hajime Tazaki (tazaki@sfc.wide.ad.jp)
 */

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <regex.h>

#include "ns3/log.h"
#include "rocketfuel-topology-reader.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RocketfuelTopologyReader");

NS_OBJECT_ENSURE_REGISTERED (RocketfuelTopologyReader);

TypeId RocketfuelTopologyReader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RocketfuelTopologyReader")
    .SetParent<Object> ()
  ;
  return tid;
}

RocketfuelTopologyReader::RocketfuelTopologyReader ()
{
  NS_LOG_FUNCTION (this);
}

RocketfuelTopologyReader::~RocketfuelTopologyReader ()
{
  NS_LOG_FUNCTION (this);
}

/* uid @loc [+] [bb] (num_neigh) [&ext] -> <nuid-1> <nuid-2> ... {-euid} ... =name[!] rn */

#define REGMATCH_MAX 16

#define START "^"
#define END "$"
#define SPACE "[ \t]+"
#define MAYSPACE "[ \t]*"

#define ROCKETFUEL_MAPS_LINE \
  START "(-*[0-9]+)" SPACE "(@[?A-Za-z0-9,+]+)" SPACE \
  "(\\+)*" MAYSPACE "(bb)*" MAYSPACE \
  "\\(([0-9]+)\\)" SPACE "(&[0-9]+)*" MAYSPACE \
  "->" MAYSPACE "(<[0-9 \t<>]+>)*" MAYSPACE \
  "(\\{-[0-9\\{\\} \t-]+\\})*" SPACE \
  "=([A-Za-z0-9.!-]+)" SPACE "r([0-9])" \
  MAYSPACE END

#define ROCKETFUEL_WEIGHTS_LINE \
  START "([^ \t]+)" SPACE "([^ \t]+)" SPACE "([0-9.]+)" MAYSPACE END

int linksNumber = 0;
int nodesNumber = 0;
std::map<std::string, Ptr<Node> > nodeMap;

NodeContainer
RocketfuelTopologyReader::GenerateFromMapsFile (int argc, char *argv[])
{
  std::string uid;
  std::string loc;
  std::string ptr;
  std::string name;
  std::string nuid;
  bool dns = false;
  bool bb = false;
  int num_neigh_s = 0;
  unsigned int num_neigh = 0;
  int radius = 0;
  std::vector <std::string> neigh_list;
  NodeContainer nodes;

  uid = argv[0];
  loc = argv[1];

  if (argv[2])
    {
      dns = true;
    }

  if (argv[3])
    {
      bb = true;
    }

  num_neigh_s = ::atoi (argv[4]);
  if (num_neigh_s < 0)
    {
      num_neigh = 0;
      NS_LOG_WARN ("Negative number of neighbors given");
    }
  else
    {
      num_neigh = num_neigh_s;
    }

  /* neighbors */
  if (argv[6])
    {
      char *nbr;
      char *stringp = argv[6];
      while ((nbr = strsep (&stringp, " \t")) != NULL)
        {
          nbr[strlen (nbr) - 1] = '\0';
          neigh_list.push_back (nbr + 1);
        }
    }
  if (num_neigh != neigh_list.size ())
    {
      NS_LOG_WARN ("Given number of neighbors = " << num_neigh << " != size of neighbors list = " << neigh_list.size ());
    }

  /* externs */
  if (argv[7])
    {
      //      euid = argv[7];
    }

  /* name */
  if (argv[8])
    {
      name = argv[8];
    }

  radius = ::atoi (&argv[9][1]);
  if (radius > 0)
    {
      return nodes;
    }

  /* uid @loc [+] [bb] (num_neigh) [&ext] -> <nuid-1> <nuid-2> ... {-euid} ... =name[!] rn */
  NS_LOG_INFO ("Load Node[" << uid << "]: location: " << loc << " dns: " << dns
                            << " bb: " << bb << " neighbors: " << neigh_list.size ()
                            << "(" << "%d" << ") externals: \"%s\"(%d) "
                            << "name: " << name << " radius: " << radius);

  //cast bb and dns to void, to suppress variable set but not used compiler warning
  //in optimized builds
  (void) bb;
  (void) dns;

  // Create node and link
  if (!uid.empty ())
    {
      if (nodeMap[uid] == 0)
        {
          Ptr<Node> tmpNode = CreateObject<Node> ();
          nodeMap[uid] = tmpNode;
          nodes.Add (tmpNode);
          nodesNumber++;
        }

      for (uint32_t i = 0; i < neigh_list.size (); ++i)
        {
          nuid = neigh_list[i];

          if (nuid.empty ())
            {
              return nodes;
            }

          if (nodeMap[nuid] == 0)
            {
              Ptr<Node> tmpNode = CreateObject<Node> ();
              nodeMap[nuid] = tmpNode;
              nodes.Add (tmpNode);
              nodesNumber++;
            }
          NS_LOG_INFO (linksNumber << ":" << nodesNumber << " From: " << uid << " to: " << nuid);
          Link link (nodeMap[uid], uid, nodeMap[nuid], nuid);
          AddLink (link);
          linksNumber++;
        }
    }
  return nodes;
}

NodeContainer
RocketfuelTopologyReader::GenerateFromWeightsFile (int argc, char *argv[])
{
  /* uid @loc [+] [bb] (num_neigh) [&ext] -> <nuid-1> <nuid-2> ... {-euid} ... =name[!] rn */
  std::string sname;
  std::string tname;
  char *endptr;
  NodeContainer nodes;

  sname = argv[0];
  tname = argv[1];
  double v = strtod (argv[2], &endptr); // weight
  // cast v to void , to suppress 'v' set but not used compiler warning
  (void) v;
  if (*endptr != '\0')
    {
      NS_LOG_WARN ("invalid weight: " << argv[2]);
      return nodes;
    }

  // Create node and link
  if (!sname.empty () && !tname.empty ())
    {
      if (nodeMap[sname] == 0)
        {
          Ptr<Node> tmpNode = CreateObject<Node> ();
          nodeMap[sname] = tmpNode;
          nodes.Add (tmpNode);
          nodesNumber++;
        }

      if (nodeMap[tname] == 0)
        {
          Ptr<Node> tmpNode = CreateObject<Node> ();
          nodeMap[tname] = tmpNode;
          nodes.Add (tmpNode);
          nodesNumber++;
        }
      NS_LOG_INFO (linksNumber << ":" << nodesNumber << " From: " << sname << " to: " << tname);
      TopologyReader::ConstLinksIterator iter;
      bool found = false;
      for (iter = LinksBegin (); iter != LinksEnd (); iter++)
        {
          if ((iter->GetFromNode () == nodeMap[tname])
              && (iter->GetToNode () == nodeMap[sname]))
            {
              found = true;
              break;
            }
        }

      if (!found)
        {
          Link link (nodeMap[sname], sname, nodeMap[tname], tname);
          AddLink (link);
          linksNumber++;
        }
    }
  return nodes;
}

enum RocketfuelTopologyReader::RF_FileType
RocketfuelTopologyReader::GetFileType (const char *line)
{
  int ret;
  regmatch_t regmatch[REGMATCH_MAX];
  regex_t regex;
  char errbuf[512];

  // Check whether MAPS file or not
  ret = regcomp (&regex, ROCKETFUEL_MAPS_LINE, REG_EXTENDED | REG_NEWLINE);
  if (ret != 0)
    {
      regerror (ret, &regex, errbuf, sizeof (errbuf));
      return RF_UNKNOWN;
    }
  ret = regexec (&regex, line, REGMATCH_MAX, regmatch, 0);
  if (ret != REG_NOMATCH)
    {
      regfree (&regex);
      return RF_MAPS;
    }
  regfree (&regex);

  // Check whether Weights file or not
  ret = regcomp (&regex, ROCKETFUEL_WEIGHTS_LINE, REG_EXTENDED | REG_NEWLINE);
  if (ret != 0)
    {
      regerror (ret, &regex, errbuf, sizeof (errbuf));
      return RF_UNKNOWN;
    }
  ret = regexec (&regex, line, REGMATCH_MAX, regmatch, 0);
  if (ret != REG_NOMATCH)
    {
      regfree (&regex);
      return RF_WEIGHTS;
    }
  regfree (&regex);

  return RF_UNKNOWN;
}


NodeContainer
RocketfuelTopologyReader::Read (void)
{
  std::ifstream topgen;
  topgen.open (GetFileName ().c_str ());
  NodeContainer nodes;

  std::istringstream lineBuffer;
  std::string line;
  int lineNumber = 0;
  enum RF_FileType ftype = RF_UNKNOWN;
  char errbuf[512];

  if (!topgen.is_open ())
    {
      NS_LOG_WARN ("Couldn't open the file " << GetFileName ());
      return nodes;
    }

  while (!topgen.eof ())
    {
      int ret;
      int argc;
      char *argv[REGMATCH_MAX];
      char *buf;

      lineNumber++;
      line.clear ();
      lineBuffer.clear ();

      getline (topgen, line);
      buf = (char *)line.c_str ();

      if (lineNumber == 1)
        {
          ftype = GetFileType (buf);
          if (ftype == RF_UNKNOWN)
            {
              NS_LOG_INFO ("Unknown File Format (" << GetFileName () << ")");
              break;
            }
        }

      regmatch_t regmatch[REGMATCH_MAX];
      regex_t regex;

      if (ftype == RF_MAPS)
        {
          ret = regcomp (&regex, ROCKETFUEL_MAPS_LINE, REG_EXTENDED | REG_NEWLINE);
          if (ret != 0)
            {
              regerror (ret, &regex, errbuf, sizeof (errbuf));
              regfree (&regex);
              break;
            }

          ret = regexec (&regex, buf, REGMATCH_MAX, regmatch, 0);
          if (ret == REG_NOMATCH)
            {
              NS_LOG_WARN ("match failed (maps file): %s" << buf);
              regfree (&regex);
              break;
            }
        }
      else if (ftype == RF_WEIGHTS)
        {
          ret = regcomp (&regex, ROCKETFUEL_WEIGHTS_LINE, REG_EXTENDED | REG_NEWLINE);
          if (ret != 0)
            {
              regerror (ret, &regex, errbuf, sizeof (errbuf));
              regfree (&regex);
              break;
            }

          ret = regexec (&regex, buf, REGMATCH_MAX, regmatch, 0);
          if (ret == REG_NOMATCH)
            {
              NS_LOG_WARN ("match failed (weights file): %s" << buf);
              regfree (&regex);
              break;
            }
        }

      line = buf;
      argc = 0;

      /* regmatch[0] is the entire strings that matched */
      for (int i = 1; i < REGMATCH_MAX; i++)
        {
          if (regmatch[i].rm_so == -1)
            {
              argv[i - 1] = NULL;
            }
          else
            {
              line[regmatch[i].rm_eo] = '\0';
              argv[i - 1] = &line[regmatch[i].rm_so];
              argc = i;
            }
        }

      if (ftype == RF_MAPS)
        {
          nodes.Add (GenerateFromMapsFile (argc, argv));
        }
      else if (ftype == RF_WEIGHTS)
        {
          nodes.Add (GenerateFromWeightsFile (argc, argv));
        }
      else
        {
          NS_LOG_WARN ("Unsupported file format (only Maps/Weights are supported)");
        }

      regfree (&regex);
    }


  topgen.close ();

  NS_LOG_INFO ("Rocketfuel topology created with " << nodesNumber << " nodes and " << linksNumber << " links");
  return nodes;
}

} /* namespace ns3 */


