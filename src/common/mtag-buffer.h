#ifndef MTAG_BUFFER_H
#define MTAG_BUFFER_H

#include <stdint.h>

namespace ns3 {

class MtagBuffer
{
public:
  MtagBuffer (uint8_t *start, uint8_t *end);
  void TrimAtEnd (uint32_t trim);

  void WriteU8 (uint8_t v);
  void WriteU16 (uint16_t v);
  void WriteU32 (uint32_t v);
  void WriteU64 (uint64_t v);
  uint8_t  ReadU8 (void);
  uint16_t ReadU16 (void);
  uint32_t ReadU32 (void);
  uint64_t ReadU64 (void);

  void CopyFrom (MtagBuffer o);
private:
  
  uint8_t *m_current;
  uint8_t *m_end;
};

} // namespace ns3

#endif /* MTAG_BUFFER_H */
