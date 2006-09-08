/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:f -*- */
#include "ns3/packet.h"
#include "ns3/chunk.h"
#include <iostream>

using namespace ns3;

/* A sample Chunk implementation
 */
class MyChunk : public Chunk {
public:
    MyChunk ();
    virtual ~MyChunk ();

    void setData (uint16_t data);
    uint16_t getData (void) const;
private:
    virtual void print (std::ostream *os) const;
    virtual void addTo (Buffer *buffer) const;
    virtual void peekFrom (Buffer const *buffer);
    virtual void removeFrom (Buffer *buffer);

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
MyChunk::addTo (Buffer *buffer) const
{
    // reserve 2 bytes at head of buffer
    buffer->addAtStart (2);
    Buffer::Iterator i = buffer->begin ();
    // serialize in head of buffer
    i.writeHtonU16 (m_data);
}
void 
MyChunk::peekFrom (Buffer const *buffer)
{
    Buffer::Iterator i = buffer->begin ();
    // deserialize from head of buffer
    m_data = i.readNtohU16 ();
}
void 
MyChunk::removeFrom (Buffer *buffer)
{
    // remove deserialized data
    buffer->removeAtStart (2);
}

void 
MyChunk::setData (uint16_t data)
{
    m_data = data;
}
uint16_t 
MyChunk::getData (void) const
{
    return m_data;
}

/* A sample Tag implementation
 */
struct MyTag {
    uint16_t m_streamId;
};


static void
receive (Packet p)
{
    MyChunk my;
    p.peek (my);
    p.remove (my);
    std::cout << "received data=" << my.getData () << std::endl;
    struct MyTag myTag;
    p.peekTag (myTag);
}


int main (int argc, char *argv[])
{
    Packet p;
    MyChunk my;
    my.setData (2);
    std::cout << "send data=2" << std::endl;
    p.add (my);
    struct MyTag myTag;
    myTag.m_streamId = 5;
    p.addTag (myTag);
    receive (p);
    return 0;
}
