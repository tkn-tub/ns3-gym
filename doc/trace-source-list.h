///
/// \ingroup TraceSourceList
/// \brief send ipv4 packet to outgoing interface
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet sent.
/// \param arg3 index of output ipv4 interface.
///
///
/// The path to this trace source is: /nodes/[0-n]/ipv4/tx.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::Ipv4L3ProtocolTraceContextElement
void TraceSinkCallback0 (const TraceContext & arg1, const Packet & arg2, uint32_t arg3);

///
/// \ingroup TraceSourceList
/// \brief receive ipv4 packet from incoming interface
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet received.
/// \param arg3 index of input ipv4 interface.
///
///
/// The path to this trace source is: /nodes/[0-n]/ipv4/rx.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::Ipv4L3ProtocolTraceContextElement
void TraceSinkCallback1 (const TraceContext & arg1, const Packet & arg2, uint32_t arg3);

///
/// \ingroup TraceSourceList
/// \brief drop ipv4 packet
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet dropped.
///
///
/// The path to this trace source is: /nodes/[0-n]/ipv4/drop.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::Ipv4L3ProtocolTraceContextElement
void TraceSinkCallback2 (const TraceContext & arg1, const Packet & arg2);

///
/// \ingroup TraceSourceList
/// \brief store packet in queue
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet queued.
///
///
/// The path to this trace source is: /nodes/[0-n]/devices/[0-n]/queue/enqueue.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::NodeNetDeviceIndex
///  - ns3::QueueTraceType
void TraceSinkCallback3 (const TraceContext & arg1, const Packet & arg2);

///
/// \ingroup TraceSourceList
/// \brief remove packet from queue
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet dequeued.
///
///
/// The path to this trace source is: /nodes/[0-n]/devices/[0-n]/queue/dequeue.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::NodeNetDeviceIndex
///  - ns3::QueueTraceType
void TraceSinkCallback4 (const TraceContext & arg1, const Packet & arg2);

///
/// \ingroup TraceSourceList
/// \brief drop packet from queue
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet dropped.
///
///
/// The path to this trace source is: /nodes/[0-n]/devices/[0-n]/queue/drop.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::NodeNetDeviceIndex
///  - ns3::QueueTraceType
void TraceSinkCallback5 (const TraceContext & arg1, const Packet & arg2);

///
/// \ingroup TraceSourceList
/// \brief receive MAC packet
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet received.
///
///
/// The path to this trace source is: /nodes/[0-n]/devices/[0-n]/rx.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::NodeNetDeviceIndex
///  - ns3::PointToPointTraceType
void TraceSinkCallback6 (const TraceContext & arg1, const Packet & arg2);

///
/// \ingroup TraceSourceList
/// \brief receive MAC packet
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet received.
///
///
/// The path to this trace source is: /nodes/[0-n]/devices/[0-n]/rx.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::NodeNetDeviceIndex
///  - ns3::CsmaTraceType
void TraceSinkCallback7 (const TraceContext & arg1, const Packet & arg2);

///
/// \ingroup TraceSourceList
/// \brief drop MAC packet
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet dropped.
///
///
/// The path to this trace source is: /nodes/[0-n]/devices/[0-n]/drop.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::NodeNetDeviceIndex
///  - ns3::CsmaTraceType
void TraceSinkCallback8 (const TraceContext & arg1, const Packet & arg2);

///
/// \ingroup TraceSourceList
/// \brief The value of the speed vector changed
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 the mobility model whose course changed.
///
///
/// The path to this trace source is: /nodes/[0-n]/$MobilityModelNotifier/course-change.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
void TraceSinkCallback9 (const TraceContext & arg1, Ptr<const MobilityModel> arg2);

///
/// \ingroup TraceSourceList
/// \brief send ipv4 packet to outgoing interface
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet sent.
/// \param arg3 index of output ipv4 interface.
///
///
/// The path to this trace source is: /nodes/[0-n]/$Ipv4L3Protocol/tx.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::Ipv4L3ProtocolTraceContextElement
void TraceSinkCallback10 (const TraceContext & arg1, const Packet & arg2, uint32_t arg3);

///
/// \ingroup TraceSourceList
/// \brief receive ipv4 packet from incoming interface
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet received.
/// \param arg3 index of input ipv4 interface.
///
///
/// The path to this trace source is: /nodes/[0-n]/$Ipv4L3Protocol/rx.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::Ipv4L3ProtocolTraceContextElement
void TraceSinkCallback11 (const TraceContext & arg1, const Packet & arg2, uint32_t arg3);

///
/// \ingroup TraceSourceList
/// \brief drop ipv4 packet
/// \param arg1 the trace context associated to the connected trace source.
/// \param arg2 packet dropped.
///
///
/// The path to this trace source is: /nodes/[0-n]/$Ipv4L3Protocol/drop.
///
/// The following classes can be extracted from \p arg1 with 
/// ns3::TraceContext::GetElement:
///  - ns3::NodeListIndex
///  - ns3::Ipv4L3ProtocolTraceContextElement
void TraceSinkCallback12 (const TraceContext & arg1, const Packet & arg2);

