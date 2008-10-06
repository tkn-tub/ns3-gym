import unittest
import ns3

class TestSimulator(unittest.TestCase):

    def testScheduleNow(self):
        def callback(args):
            self._args_received = args
            self._cb_time = ns3.Simulator.Now()
        ns3.Simulator.Destroy()
        self._args_received = None
        self._cb_time = None
        ns3.Simulator.ScheduleNow(callback, "args")
        ns3.Simulator.Run()
        self.assertEqual(self._args_received, "args")
        self.assertEqual(self._cb_time.GetSeconds(), 0.0)

    def testSchedule(self):
        def callback(args):
            self._args_received = args
            self._cb_time = ns3.Simulator.Now()
        ns3.Simulator.Destroy()
        self._args_received = None
        self._cb_time = None
        ns3.Simulator.Schedule(ns3.Seconds(123), callback, "args")
        ns3.Simulator.Run()
        self.assertEqual(self._args_received, "args")
        self.assertEqual(self._cb_time.GetSeconds(), 123.0)

    def testScheduleDestroy(self):
        def callback(args):
            self._args_received = args
            self._cb_time = ns3.Simulator.Now()
        ns3.Simulator.Destroy()
        self._args_received = None
        self._cb_time = None
        def null(): pass
        ns3.Simulator.Schedule(ns3.Seconds(123), null)
        ns3.Simulator.ScheduleDestroy(callback, "args")
        ns3.Simulator.Run()
        ns3.Simulator.Destroy()
        self.assertEqual(self._args_received, "args")
        self.assertEqual(self._cb_time.GetSeconds(), 123.0)

    def testTimeComparison(self):
        self.assert_(ns3.Seconds(123) == ns3.Seconds(123))
        self.assert_(ns3.Seconds(123) >= ns3.Seconds(123))
        self.assert_(ns3.Seconds(123) <= ns3.Seconds(123))
        self.assert_(ns3.Seconds(124) > ns3.Seconds(123))
        self.assert_(ns3.Seconds(123) < ns3.Seconds(124))

    def testTimeNumericOperations(self):
        self.assertEqual(ns3.Seconds(10) + ns3.Seconds(5), ns3.Seconds(15))
        self.assertEqual(ns3.Seconds(10) - ns3.Seconds(5), ns3.Seconds(5))

        v1 = ns3.Scalar(5)*ns3.Seconds(10)
        self.assertEqual(v1, ns3.Seconds(50))

    def testConfig(self):
        ns3.Config.Set("ns3::OnOffApplication::PacketSize", ns3.UintegerValue(123))
        ns3.Config.SetDefault("ns3::OnOffApplication::PacketSize", ns3.UintegerValue(123))
        # hm.. no Config.Get?

    def testSocket(self):
        node = ns3.Node()
        ns3.AddInternetStack(node)
        self._received_packet = None

        def rx_callback(socket):
            assert self._received_packet is None
            self._received_packet = socket.Recv()

        sink = ns3.Socket.CreateSocket(node, ns3.TypeId.LookupByName("ns3::UdpSocketFactory"))
        sink.Bind(ns3.InetSocketAddress(ns3.Ipv4Address.GetAny(), 80))
        sink.SetRecvCallback(rx_callback)

        source = ns3.Socket.CreateSocket(node, ns3.TypeId.LookupByName("ns3::UdpSocketFactory"))
        source.SendTo(ns3.Packet(19), 0, ns3.InetSocketAddress(ns3.Ipv4Address("127.0.0.1"), 80))

        ns3.Simulator.Run()
        self.assert_(self._received_packet is not None)
        self.assertEqual(self._received_packet.GetSize(), 19)


    def testAttributes(self):
        ##
        ## Yes, I know, the GetAttribute interface for Python is
        ## horrible, we should fix this soon, I hope.
        ##
        queue = ns3.DropTailQueue()

        queue.SetAttribute("MaxPackets", ns3.UintegerValue(123456))

        limit = ns3.UintegerValue()
        queue.GetAttribute("MaxPackets", limit)
        self.assertEqual(limit.Get(), 123456)

        ## -- object pointer values
        mobility = ns3.RandomWaypointMobilityModel()
        ptr = ns3.PointerValue()
        mobility.GetAttribute("Position", ptr)
        self.assertEqual(ptr.GetObject(), None)
        
        pos = ns3.ListPositionAllocator()
        mobility.SetAttribute("Position", ns3.PointerValue(pos))

        ptr = ns3.PointerValue()
        mobility.GetAttribute("Position", ptr)
        self.assert_(ptr.GetObject() is not None)

    def testIdentity(self):
        csma = ns3.CsmaNetDevice()
        channel = ns3.CsmaChannel()
        csma.Attach(channel)
        
        c1 = csma.GetChannel()
        c2 = csma.GetChannel()

        self.assert_(c1 is c2)

if __name__ == '__main__':
    unittest.main()
