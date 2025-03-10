# ns3-gym change history
## 1.1.0 - 2024-09-01 :: by [rogerio-silva](https://github.com/rogerio-silva)
- Added support for ns-3.42
    * ‘ns3::TestSuite::UNIT’ is deprecated: Use ‘Type::UNIT’ instead
    * ‘ns3::TestCase::QUICK’ is deprecated: Use ‘Duration::QUICK’ instead
    * ‘class ns3::InetSocketAddress’ has no member named ‘SetTos’: Refactored the code by adding a socket and setting the TOS field using the SetIpTos method
      * Refatored the `linear-mesh` and `linear-mesh-2` examples code in the following way:
        ```c++
        Ptr<Socket> socket = Socket::CreateSocket (srcNode, UdpSocketFactory::GetTypeId ());
        socket->SetIpTos (0x70); // AC_BE
        # ...
        source.SetAttribute ("Socket", PointerValue (socket));
        ```
## [1.0.0] - 2021-06-01
- Custom implementation