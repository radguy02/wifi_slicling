// +---------+        Cable        +---------+
// | Node 0  |=====================| Node 1  |
// +---------+                     +---------+


#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h" //using a wired cable
#include "ns3/applications-module.h"

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstNetwork");

int main(){
    
    const int SERVERPORT = 9;
    const string DATARATE = "10Mbps";
    const string DELAY = "2ms";

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    //Create Nodes
    //Node0, Node1
    NodeContainer nodes; 
    nodes.Create(2);


    PointToPointHelper pointToPoint; //Ethernet like links
    pointToPoint.SetDeviceAttribute("DataRate", StringValue(DATARATE)); //Link bandwidth 10Mbps
    pointToPoint.SetChannelAttribute("Delay", StringValue(DELAY));
    //Because signals do not travel instantly
    //Even fiber optics has propagation delay


    //creates two network cards,
    //creates one cable,
    //connects everything together                                         
    NetDeviceContainer devices;
    devices = pointToPoint.Install(nodes);
    

    //     Node
    //      ↓
    //   NetDevice
    // === Cable ===
    //   NetDevice
    //      ↓
    //     Node


    //Nodes understand networking
    //IPv4, TCP, UDP, ARP, Routing
    InternetStackHelper stack;
    stack.Install(nodes);


    //Assign IP Addresses
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    //We're creating the subnet 10.1.1.x
    

    //assigning ips to devices not to nodes
    //Node 0: 10.1.1.1
    //Node 1: 10.1.1.2
    Ipv4InterfaceContainer interfaces;
    interfaces = address.Assign(devices);


    //Installing the application
    UdpEchoServerHelper server(SERVERPORT); //port 9
    //server
    ApplicationContainer serverApps = server.Install(nodes.Get(1));


    //create the client
    UdpEchoClientHelper client(interfaces.GetAddress(1), 9);
    //the client needs to know where to send packets 
    //Configuring client
    client.SetAttribute("MaxPackets", UintegerValue(1)); //Send exactly one packet
    client.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    client.SetAttribute("PacketSize", UintegerValue(1024));

    //client
    ApplicationContainer clientApps = client.Install(nodes.Get(0));


    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));
    
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}