#ifndef SYSTEM_PATH
#define SYSTEM_PATH

#include <string>
#include <list>

namespace ns3 {

/**
 * \brief Encapsulate OS-specific functions to manipulate file and directory paths.
 *
 * The functions provided here are used mostly to implement the ns-3 test framework.
 */
namespace SystemPath {

  /**
   * \return the directory in which the currently-executing binary is located
   */
  std::string FindSelfDirectory (void);
  
  /**
   * \param left a path element
   * \param right a path element
   * \return a concatenation of the two input paths
   */
  std::string Append (std::string left, std::string right);

  /**
   * \param path a path
   * \return a list of path elements that can be joined together again with
   *         the Join function.
   * \sa ns3::SystemPath::Join
   */
  std::list<std::string> Split (std::string path);

  /**
   * \param begin iterator to first element to join
   * \param end iterator to last element to join
   * \return a path that is a concatenation of all the input elements.
   */
  std::string Join (std::list<std::string>::const_iterator begin,
		    std::list<std::string>::const_iterator end);
  
  /**
   * \param path a path which identifies a directory
   * \return a list of the filenames which are located in the input directory
   */
  std::list<std::string> ReadFiles (std::string path);

  /**
   * \return a path which identifies a temporary directory.
   *
   * The returned path identifies a directory which does not exist yet
   * Call ns3::SystemPath::MakeDirectories to create it. Yes, there is a
   * well-known security race in this API but we don't care in ns-3.
   */
  std::string MakeTemporaryDirectoryName (void);

  /**
   * \param path a path to a directory
   *
   * Create all the directories leading to path.
   */
  void MakeDirectories (std::string path);

} // namespace SystemPath


} // namespace ns3


#endif /* SYSTEM_PATH */
