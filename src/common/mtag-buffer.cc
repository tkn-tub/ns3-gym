#include "mtag-buffer.h"
#include "ns3/assert.h"

namespace ns3 {

void 
MtagBuffer::WriteU8 (uint8_t v)
{
  NS_ASSERT (m_current + 1 <= m_end);
  *m_current = v;
  m_current++;
}
void 
MtagBuffer::WriteU16 (uint16_t data)
{
  WriteU8 ((data >> 0) & 0xff);
  WriteU8 ((data >> 8) & 0xff);
}
void 
MtagBuffer::WriteU32 (uint32_t data)
{
  WriteU8 ((data >> 0) & 0xff);
  WriteU8 ((data >> 8) & 0xff);
  WriteU8 ((data >> 16) & 0xff);
  WriteU8 ((data >> 24) & 0xff);
}
void 
MtagBuffer::WriteU64 (uint64_t data)
{
  WriteU8 ((data >> 0) & 0xff);
  WriteU8 ((data >> 8) & 0xff);
  WriteU8 ((data >> 16) & 0xff);
  WriteU8 ((data >> 24) & 0xff);
  WriteU8 ((data >> 32) & 0xff);
  WriteU8 ((data >> 40) & 0xff);
  WriteU8 ((data >> 48) & 0xff);
  WriteU8 ((data >> 54) & 0xff);
}
void
MtagBuffer::WriteDouble (double v)
{
  uint8_t *buf = (uint8_t *)&v;
  for (uint32_t i = 0; i < sizeof (double); ++i, ++buf)
    {
      WriteU8 (*buf);
    }
}
void 
MtagBuffer::Write (const uint8_t *buffer, uint32_t size)
{
  for (uint32_t i = 0; i < size; ++i, ++buffer)
    {
      WriteU8 (*buffer);
    }
}
uint8_t  
MtagBuffer::ReadU8 (void)
{
  NS_ASSERT (m_current + 1 <= m_end);
  uint8_t v;
  v = *m_current;
  m_current++;
  return v;
}
uint16_t 
MtagBuffer::ReadU16 (void)
{
  uint8_t byte0 = ReadU8 ();
  uint8_t byte1 = ReadU8 ();
  uint16_t data = byte1;
  data <<= 8;
  data |= byte0;
  return data;
}
uint32_t 
MtagBuffer::ReadU32 (void)
{
  uint8_t byte0 = ReadU8 ();
  uint8_t byte1 = ReadU8 ();
  uint8_t byte2 = ReadU8 ();
  uint8_t byte3 = ReadU8 ();
  uint32_t data = byte3;
  data <<= 8;
  data |= byte2;
  data <<= 8;
  data |= byte1;
  data <<= 8;
  data |= byte0;
  return data;
}
uint64_t 
MtagBuffer::ReadU64 (void)
{
  uint8_t byte0 = ReadU8 ();
  uint8_t byte1 = ReadU8 ();
  uint8_t byte2 = ReadU8 ();
  uint8_t byte3 = ReadU8 ();
  uint8_t byte4 = ReadU8 ();
  uint8_t byte5 = ReadU8 ();
  uint8_t byte6 = ReadU8 ();
  uint8_t byte7 = ReadU8 ();
  uint32_t data = byte7;
  data <<= 8;
  data |= byte6;
  data <<= 8;
  data |= byte5;
  data <<= 8;
  data |= byte4;
  data <<= 8;
  data |= byte3;
  data <<= 8;
  data |= byte2;
  data <<= 8;
  data |= byte1;
  data <<= 8;
  data |= byte0;

  return data;
}
double
MtagBuffer::ReadDouble (void)
{
  double v;
  uint8_t *buf = (uint8_t *)&v;
  for (uint32_t i = 0; i < sizeof (double); ++i, ++buf)
    {
      *buf = ReadU8 ();
    }
  return v;
}
void 
MtagBuffer::Read (uint8_t *buffer, uint32_t size)
{
  for (uint32_t i = 0; i < size; ++i, ++buffer)
    {
      *buffer = ReadU8 ();
    }
}
MtagBuffer::MtagBuffer (uint8_t *start, uint8_t *end)
  : m_current (start),
    m_end (end)
{}

void 
MtagBuffer::TrimAtEnd (uint32_t trim)
{
  NS_ASSERT (m_current <= (m_end - trim));
  m_end -= trim;
}

void 
MtagBuffer::CopyFrom (MtagBuffer o)
{
  uint32_t size = o.m_end - o.m_current;
  NS_ASSERT (size <= m_end - m_current);
  memcpy (m_current, o.m_current, size);
  m_current += size;
}

} // namespace ns3
