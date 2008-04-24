#ifndef MTAG_LIST_H
#define MTAG_LIST_H

#include <stdint.h>
#include "ns3/type-id.h"
#include "mtag-buffer.h"

namespace ns3 {

class MtagList
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
      MtagBuffer buf;
    private:
      friend class MtagList;
      Item (MtagBuffer buf);
    };
    bool HasNext (void) const;
    struct MtagList::Iterator::Item Next (void);
    uint32_t GetOffsetStart (void) const;
  private:
    friend class MtagList;
    Iterator (uint8_t *start, uint8_t *end, uint32_t offsetStart, uint32_t offsetEnd);
    void PrepareForNext (void);
    uint8_t *m_current;
    uint8_t *m_end;
    uint32_t m_offsetStart;
    uint32_t m_offsetEnd;
  };

  MtagList ();
  MtagList (const MtagList &o);
  MtagList &operator = (const MtagList &o);
  ~MtagList ();

  MtagBuffer Add (TypeId tid, uint32_t bufferSize, uint32_t start, uint32_t end);

  void Add (const MtagList &o);

  void Remove (const Iterator &i);
  void RemoveAll (void);

  MtagList::Iterator Begin (uint32_t offsetStart, uint32_t offsetEnd) const;

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
