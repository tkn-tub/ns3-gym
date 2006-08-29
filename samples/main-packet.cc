/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
#include "ns3/packet.h"
#include "ns3/chunk.h"
#include <iostream>

using namespace yans;

/* A sample Chunk implementation
 */
class MyChunk : public Chunk {
public:
	MyChunk ();
	virtual ~MyChunk ();

	void set_data (uint16_t data);
	uint16_t get_data (void) const;
private:
	virtual void print (std::ostream *os) const;
	virtual void add_to (Buffer *buffer) const;
	virtual void peek_from (Buffer const *buffer);
	virtual void remove_from (Buffer *buffer);

	uint16_t m_data;
};

MyChunk::MyChunk ()
{}
MyChunk::~MyChunk ()
{}
void 
MyChunk::print (std::ostream *os) const
{
	*os << "MyChunk data=" << m_data << std::endl;
}
void 
MyChunk::add_to (Buffer *buffer) const
{
	// reserve 2 bytes at head of buffer
	buffer->add_at_start (2);
	Buffer::Iterator i = buffer->begin ();
	// serialize in head of buffer
	i.write_hton_u16 (m_data);
}
void 
MyChunk::peek_from (Buffer const *buffer)
{
	Buffer::Iterator i = buffer->begin ();
	// deserialize from head of buffer
	m_data = i.read_ntoh_u16 ();
}
void 
MyChunk::remove_from (Buffer *buffer)
{
	// remove deserialized data
	buffer->remove_at_start (2);
}

void 
MyChunk::set_data (uint16_t data)
{
	m_data = data;
}
uint16_t 
MyChunk::get_data (void) const
{
	return m_data;
}

/* A sample Tag implementation
 */
struct MyTag {
	uint16_t m_stream_id;
};


static void
receive (Packet p)
{
	MyChunk my;
	p.peek (&my);
	p.remove (&my);
	std::cout << "received data=" << my.get_data () << std::endl;
	struct MyTag my_tag;
	p.peek_tag (&my_tag);
}


int main (int argc, char *argv[])
{
	Packet p;
	MyChunk my;
	my.set_data (2);
	std::cout << "send data=2" << std::endl;
	p.add (&my);
	struct MyTag my_tag;
	my_tag.m_stream_id = 5;
	p.add_tag (&my_tag);
	receive (p);
	return 0;
}
