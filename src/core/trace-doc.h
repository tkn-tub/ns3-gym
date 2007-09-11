#ifndef TRACE_DOC_H
#define TRACE_DOC_H

#include <vector>
#include <string>

namespace ns3 {

class TraceDoc
{
  typedef std::vector<std::pair<std::string,std::string> > ArgVector;
public:
  typedef ArgVector::const_iterator Iterator;
  TraceDoc ();
  TraceDoc (std::string help);
  TraceDoc (std::string help,
	    std::string arg0Type,
	    std::string arg0Help);
  TraceDoc (std::string help,
	    std::string arg0Type,
	    std::string arg0Help,
	    std::string arg1Type,
	    std::string arg1Help);
  TraceDoc (std::string help,
	    std::string arg0Type,
	    std::string arg0Help,
	    std::string arg1Type,
	    std::string arg1Help,
	    std::string arg2Type,
	    std::string arg2Help);
  TraceDoc (std::string help,
	    std::string arg0Type,
	    std::string arg0Help,
	    std::string arg1Type,
	    std::string arg1Help,
	    std::string arg2Type,
	    std::string arg2Help,
	    std::string arg3Type,
	    std::string arg3Help);
  std::string GetHelp (void) const;
  Iterator ArgsBegin (void) const;
  Iterator ArgsEnd (void) const;
private:
  ArgVector m_argVector;
  std::string m_help;
};

} // namespace ns3

#endif /* TRACE_DOC_H */
