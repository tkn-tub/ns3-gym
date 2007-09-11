#ifndef TRACE_DOC_H
#define TRACE_DOC_H

#include <vector>
#include <string>

namespace ns3 {

/**
 * \brief describe the signature of a trace source
 */
class TraceDoc
{
  typedef std::vector<std::pair<std::string,std::string> > ArgVector;
public:
  typedef ArgVector::const_iterator Iterator;
  TraceDoc ();
  /**
   * \param help a description of the purpose of the trace source
   */
  TraceDoc (std::string help);
  /**
   * \param help a description of the purpose of the trace source
   * \param arg1Type the fully-qualified typename of the first argument of the trace source
   * \param arg1Help the purpose of the first argument of the trace source
   */
  TraceDoc (std::string help,
	    std::string arg1Type,
	    std::string arg1Help);
  /**
   * \param help a description of the purpose of the trace source
   * \param arg1Type the fully-qualified typename of the first argument of the trace source
   * \param arg1Help the purpose of the first argument of the trace source
   * \param arg2Type the fully-qualified typename of the second argument of the trace source
   * \param arg2Help the purpose of the second argument of the trace source
   */
  TraceDoc (std::string help,
	    std::string arg1Type,
	    std::string arg1Help,
	    std::string arg2Type,
	    std::string arg2Help);
  /**
   * \param help a description of the purpose of the trace source
   * \param arg1Type the fully-qualified typename of the first argument of the trace source
   * \param arg1Help the purpose of the first argument of the trace source
   * \param arg2Type the fully-qualified typename of the second argument of the trace source
   * \param arg2Help the purpose of the second argument of the trace source
   * \param arg3Type the fully-qualified typename of the third argument of the trace source
   * \param arg3Help the purpose of the third argument of the trace source
   */
  TraceDoc (std::string help,
	    std::string arg1Type,
	    std::string arg1Help,
	    std::string arg2Type,
	    std::string arg2Help,
	    std::string arg3Type,
	    std::string arg3Help);
  /**
   * \param help a description of the purpose of the trace source
   * \param arg1Type the fully-qualified typename of the first argument of the trace source
   * \param arg1Help the purpose of the first argument of the trace source
   * \param arg2Type the fully-qualified typename of the second argument of the trace source
   * \param arg2Help the purpose of the second argument of the trace source
   * \param arg3Type the fully-qualified typename of the third argument of the trace source
   * \param arg3Help the purpose of the third argument of the trace source
   * \param arg4Type the fully-qualified typename of the fourth argument of the trace source
   * \param arg4Help the purpose of the fourth argument of the trace source
   */
  TraceDoc (std::string help,
	    std::string arg1Type,
	    std::string arg1Help,
	    std::string arg2Type,
	    std::string arg2Help,
	    std::string arg3Type,
	    std::string arg3Help,
	    std::string arg4Type,
	    std::string arg4Help);
  /**
   * \returns the help string associated to this trace source
   */
  std::string GetHelp (void) const;
  /**
   * \returns an iterator which points to the first descriptor of the trace source.
   *
   * Each descriptor is a pair of strings. The first one describes the type of the argument
   * while the second one describeds the purpose of the argument.
   */
  Iterator ArgsBegin (void) const;
  /**
   * \returns an iterator which points to the last descriptor of the trace source.
   *
   * Each descriptor is a pair of strings. The first one describes the type of the argument
   * while the second one describeds the purpose of the argument.
   */
  Iterator ArgsEnd (void) const;
private:
  ArgVector m_argVector;
  std::string m_help;
};

} // namespace ns3

#endif /* TRACE_DOC_H */
