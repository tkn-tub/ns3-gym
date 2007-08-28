#include "trace-doc.h"

namespace ns3 {

TraceDoc::TraceDoc (std::string help)
  : m_help (help)
{}
TraceDoc::TraceDoc (std::string help,
		    std::string arg0Type,
		    std::string arg0Help)
  : m_help (help)
{
  m_argVector.push_back (std::make_pair (arg0Type, arg0Help));
}
TraceDoc::TraceDoc (std::string help,
		    std::string arg0Type,
		    std::string arg0Help,
		    std::string arg1Type,
		    std::string arg1Help)
{
  m_argVector.push_back (std::make_pair (arg0Type, arg0Help));
  m_argVector.push_back (std::make_pair (arg1Type, arg1Help));
}
TraceDoc::TraceDoc (std::string help,
		    std::string arg0Type,
		    std::string arg0Help,
		    std::string arg1Type,
		    std::string arg1Help,
		    std::string arg2Type,
		    std::string arg2Help)
{
  m_argVector.push_back (std::make_pair (arg0Type, arg0Help));
  m_argVector.push_back (std::make_pair (arg1Type, arg1Help));
  m_argVector.push_back (std::make_pair (arg2Type, arg2Help));
}
TraceDoc::TraceDoc (std::string help,
		    std::string arg0Type,
		    std::string arg0Help,
		    std::string arg1Type,
		    std::string arg1Help,
		    std::string arg2Type,
		    std::string arg2Help,
		    std::string arg3Type,
		    std::string arg3Help)
{
  m_argVector.push_back (std::make_pair (arg0Type, arg0Help));
  m_argVector.push_back (std::make_pair (arg1Type, arg1Help));
  m_argVector.push_back (std::make_pair (arg2Type, arg2Help));
  m_argVector.push_back (std::make_pair (arg3Type, arg3Help));
}
std::string 
TraceDoc::GetHelp (void)
{
  return m_help;
}
TraceDoc::Iterator 
TraceDoc::ArgsBegin (void) const
{
  return m_argVector.begin ();
}
TraceDoc::Iterator 
TraceDoc::ArgsEnd (void) const
{
  return m_argVector.end ();
}

} // namespace ns3
