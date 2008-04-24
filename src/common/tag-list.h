#ifndef MTAG_LIST_H
#define MTAG_LIST_H

#include <stdint.h>
#include "ns3/type-id.h"
#include "tag-buffer.h"

namespace ns3 {

class TagList
{
public:

  class Iterator
  {
  public:
    struct Item 
    {
      TypeId tid;
      uint32_t size;
      uint32_t start;
      uint32_t end;
      TagBuffer buf;
    private:
      friend class TagList;
      Item (TagBuffer buf);
    };
    bool HasNext (void) const;
    struct TagList::Iterator::Item Next (void);
    uint32_t GetOffsetStart (void) const;
  private:
    friend class TagList;
    Iterator (uint8_t *start, uint8_t *end, uint32_t offsetStart, uint32_t offsetEnd);
    void PrepareForNext (void);
    uint8_t *m_current;
    uint8_t *m_end;
    uint32_t m_offsetStart;
    uint32_t m_offsetEnd;
  };

  TagList ();
  TagList (const TagList &o);
  TagList &operator = (const TagList &o);
  ~TagList ();

  TagBuffer Add (TypeId tid, uint32_t bufferSize, uint32_t start, uint32_t end);

  void Add (const TagList &o);

  void Remove (const Iterator &i);
  void RemoveAll (void);

  TagList::Iterator Begin (uint32_t offsetStart, uint32_t offsetEnd) const;

  void AddAtEnd (int32_t adjustment, uint32_t appendOffset);
  void AddAtStart (int32_t adjustment, uint32_t prependOffset);

private:
  bool IsDirtyAtEnd (uint32_t appendOffset);
  bool IsDirtyAtStart (uint32_t prependOffset);
  uint8_t *m_buffer;
  uint32_t m_size;
};

} // namespace ns3

#endif /* MTAG_LIST_H */
