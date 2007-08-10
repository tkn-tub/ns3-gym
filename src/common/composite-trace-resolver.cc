/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
#include "composite-trace-resolver.h"
#include "ns3/debug.h"

NS_DEBUG_COMPONENT_DEFINE ("CompositeTraceResolver");

namespace ns3 {

CompositeTraceResolver::CompositeTraceResolver ()
{}

CompositeTraceResolver::~CompositeTraceResolver ()
{}

void 
CompositeTraceResolver::Add (std::string name, 
                             Callback<TraceResolver *> createResolver)
{
  DoAdd (name, createResolver, TraceContext ());
}

void 
CompositeTraceResolver::DoAdd (std::string name, 
			       Callback<TraceResolver *> createResolver,
			       TraceContext const &context)
{
  struct CallbackTraceSourceItem item;
  item.name = name;
  item.createResolver = createResolver;
  item.context = context;
  m_items.push_back (item);
}

void 
CompositeTraceResolver::Connect (std::string path, CallbackBase const &cb, const TraceContext &context)
{
  NS_DEBUG ("connect path="<<path);
  DoRecursiveOperation (path, cb, context, CONNECT);
}
void 
CompositeTraceResolver::DoRecursiveOperation (std::string path, CallbackBase const &cb, 
                                              const TraceContext &context,
                                              enum Operation op)
{
  std::string id = GetElement (path);
  std::string subpath = GetSubpath (path);

  if (id == "*")
    {
      for (TraceItems::const_iterator i = m_items.begin (); i != m_items.end (); i++)
	{
          OperationOne (subpath, i, cb, context, op);
        }
      return;
    }
  std::string::size_type start, end;
  start = id.find_first_of ("(", 0);
  end = id.find_first_of (")", 0);
  if (start != 0 || end != (id.size ()-1))
    {
      for (TraceItems::const_iterator i = m_items.begin (); i != m_items.end (); i++)
	{
	  if (i->name == id)
	    {
              OperationOne (subpath, i, cb, context, op);
              return;
	    }
	}
    }
  std::list<std::string> names;
  std::string alternatives = std::string (id, start+1, end-1);
  std::string::size_type next_pos, cur_pos;
  next_pos = 0;
  cur_pos = 0;
  while (true)
    {
      std::string element;
      next_pos = alternatives.find ("|", cur_pos);
      if (next_pos == std::string::npos)
	{
	  element = std::string (alternatives, cur_pos, alternatives.size ());
	  names.push_back (element);
	  break;
	}
      element = std::string (alternatives, cur_pos, next_pos);
      names.push_back (element);
      cur_pos = next_pos + 1;
    }
  for (std::list<std::string>::const_iterator i = names.begin (); i != names.end (); i++)
    {
      for (TraceItems::const_iterator j = m_items.begin (); j != m_items.end (); j++)
	{
	  if (j->name == *i)
	    {
              OperationOne (subpath, j, cb, context, op);
	      break;
	    }
	}
    }
}

void 
CompositeTraceResolver::OperationOne (std::string subpath, 
                                      TraceItems::const_iterator i,
                                      const CallbackBase &cb,
                                      const TraceContext &context,
                                      enum Operation op)
{
  TraceResolver *resolver = i->createResolver ();
  switch (op) {
  case CONNECT: {
    NS_DEBUG ("connect to path="<<subpath<<" name="<<i->name);
    TraceContext ctx = context;
    ctx.Add (i->context);
    resolver->Connect (subpath, cb, ctx);
    } break;
  case DISCONNECT:
    resolver->Disconnect (subpath, cb);
    break;
  }
  delete resolver;
}

void 
CompositeTraceResolver::Disconnect (std::string path, CallbackBase const &cb)
{
  DoRecursiveOperation (path, cb, TraceContext (), DISCONNECT);
}

}//namespace ns3

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include "trace-context-element.h"

namespace ns3 {

class TraceSourceTest : public TraceContextElement
{
public:
  enum Sources {
    DOUBLEA,
    DOUBLEB,
    SUBRESOLVER,
  };
  static uint16_t GetUid (void) 
  {static uint16_t uid = AllocateUid<TraceSourceTest> ("TraceSourceTest"); return uid;}
  void Print (std::ostream &os)
  {os << "tracesource=";
    if (m_sources == DOUBLEA) {os << "doubleA";}
    else if (m_sources == DOUBLEB) {os << "doubleB";}
    else if (m_sources == SUBRESOLVER) {os << "subresolver";}
  }
  TraceSourceTest () : m_sources (TraceSourceTest::DOUBLEA) {}
  TraceSourceTest (enum Sources sources) :m_sources (sources) {}
  bool IsDoubleA (void) {return m_sources == TraceSourceTest::DOUBLEA;}
  bool IsDoubleB (void) {return m_sources == TraceSourceTest::DOUBLEB;}
private:
  enum TraceSourceTest::Sources m_sources;
};

class SubTraceSourceTest : public TraceContextElement
{
public:
  enum Sources {
    INT,
  };
  static uint16_t GetUid (void) 
  {static uint16_t uid = AllocateUid<SubTraceSourceTest> ("SubTraceSourceTest"); return uid;}
  void Print (std::ostream &os)
  {os << "subtracesource=int";}
  SubTraceSourceTest () : m_sources (SubTraceSourceTest::INT) {}
  SubTraceSourceTest (enum Sources sources) : m_sources (sources) {}
private:
  enum Sources m_sources;
};

class CompositeTraceResolverTest : public Test
{
public:
  CompositeTraceResolverTest ();
  virtual ~CompositeTraceResolverTest ();
  virtual bool RunTests (void);
private:
  void TraceDouble (TraceContext const &context, double v);
  void TraceInt (TraceContext const &context, int v);
  TraceResolver *CreateSubResolver ();


  bool m_gotDoubleA;
  bool m_gotDoubleB;
  CallbackTraceSource<int> m_traceInt;
  bool m_gotInt;
};

CompositeTraceResolverTest::CompositeTraceResolverTest ()
  : Test ("CompositeTraceResolver")
{}
CompositeTraceResolverTest::~CompositeTraceResolverTest ()
{}
void 
CompositeTraceResolverTest::TraceDouble (TraceContext const &context, double v)
{
  TraceSourceTest source;
  context.Get (source);
  if (source.IsDoubleA ())
    {
      m_gotDoubleA = true;
    }
  else if (source.IsDoubleB ())
    {
      m_gotDoubleB = true;
    }
  else
    {
      NS_FATAL_ERROR ("should not get any other trace source in this sink");
    }
  
}

void 
CompositeTraceResolverTest::TraceInt (TraceContext const &context, int v)
{
  m_gotInt = true;
}

TraceResolver *
CompositeTraceResolverTest::CreateSubResolver (void)
{
  CompositeTraceResolver *subresolver = new CompositeTraceResolver ();
  subresolver->Add ("trace-int", m_traceInt, 
                    SubTraceSourceTest (SubTraceSourceTest::INT));
  return subresolver;
}
bool 
CompositeTraceResolverTest::RunTests (void)
{
  bool ok = true;

  CallbackTraceSource<double> traceDoubleA;
  CallbackTraceSource<double> traceDoubleB;
  TraceContext context;

  CompositeTraceResolver resolver;

  resolver.Add ("trace-double-a", traceDoubleA, 
                TraceSourceTest (TraceSourceTest::DOUBLEA));
  resolver.Add ("trace-double-b", traceDoubleB, 
                TraceSourceTest (TraceSourceTest::DOUBLEB));

  resolver.Connect ("/*", MakeCallback (&CompositeTraceResolverTest::TraceDouble, this), TraceContext ());

  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  if (!m_gotDoubleA || m_gotDoubleB)
    {
      ok = false;
    }
  m_gotDoubleA = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (!m_gotDoubleA || !m_gotDoubleB)
    {
      ok = false;
    }
  m_gotDoubleA = false;
  m_gotDoubleB = false;

  resolver.Disconnect ("/*", MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));

  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (m_gotDoubleA || m_gotDoubleB)
    {
      ok = false;
    }

  resolver.Connect ("/trace-double-a", 
		    MakeCallback (&CompositeTraceResolverTest::TraceDouble, this), TraceContext ());
  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (!m_gotDoubleA || m_gotDoubleB)
    {
      ok = false;
    }
  resolver.Disconnect ("/trace-double-a", 
		       MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));

  resolver.Connect ("/(trace-double-a)", 
		    MakeCallback (&CompositeTraceResolverTest::TraceDouble, this), TraceContext ());
  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (!m_gotDoubleA || m_gotDoubleB)
    {
      ok = false;
    }
  resolver.Disconnect ("/trace-double-a", 
		       MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));

  resolver.Connect ("/(trace-double-a|trace-double-b)", 
		    MakeCallback (&CompositeTraceResolverTest::TraceDouble, this), TraceContext ());
  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (!m_gotDoubleA || !m_gotDoubleB)
    {
      ok = false;
    }
  resolver.Disconnect ("/trace-double-a", 
		       MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));
  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (m_gotDoubleA || !m_gotDoubleB)
    {
      ok = false;
    }


  resolver.Disconnect ("/(trace-double-a|trace-double-b)", 
		       MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));
  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (m_gotDoubleA || m_gotDoubleB)
    {
      ok = false;
    }

  resolver.Add ("subresolver", 
		MakeCallback (&CompositeTraceResolverTest::CreateSubResolver, this),
		TraceSourceTest (TraceSourceTest::SUBRESOLVER));

  resolver.Connect ("/subresolver/trace-int", 
		    MakeCallback (&CompositeTraceResolverTest::TraceInt, this), TraceContext ());
  m_gotInt = false;
  m_traceInt (1);
  if (!m_gotInt)
    {
      ok = false;
    }

  resolver.Disconnect ("/subresolver/trace-int", 
		       MakeCallback (&CompositeTraceResolverTest::TraceInt, this));
  m_gotInt = false;
  m_traceInt (1);
  if (m_gotInt)
    {
      ok = false;
    }

  resolver.Connect ("/*/trace-int", 
		    MakeCallback (&CompositeTraceResolverTest::TraceInt, this), TraceContext ());
  m_gotInt = false;
  m_traceInt (1);
  if (!m_gotInt)
    {
      ok = false;
    }

  resolver.Disconnect ("/subresolver/trace-int", 
		       MakeCallback (&CompositeTraceResolverTest::TraceInt, this));
  m_gotInt = false;
  m_traceInt (1);
  if (m_gotInt)
    {
      ok = false;
    }


  

  return ok;
}

static CompositeTraceResolverTest g_compositeTraceResolverTest;

}//namespace ns3


#endif /* RUN_SELF_TESTS */
