#ifndef DEFAULT_DELETER_H
#define DEFAULT_DELETER_H

namespace ns3 {

/**
 * \brief a template used to delete objects
 *        by the *RefCount<> templates when the
 *        last reference to an object they manage
 *        disappears.
 *
 * \sa ns3::SimpleRefCount
 */
template <typename T>
struct DefaultDeleter
{
  inline static void Delete (T *object) {
    delete object;
  }
};

} // namespace ns3

#endif /* DEFAULT_DELETER_H */
