import math
import ns3
import goocanvas
from visualizer.base import Link, transform_distance_canvas_to_simulation

class WifiLink(Link):
    def __init__(self, parent_canvas_item, sta, dev):
        self.node1 = sta
        self.dev = dev
        self.node2 = None # ap
        self.canvas_item = goocanvas.Group(parent=parent_canvas_item)
        self.invisible_line = goocanvas.Polyline(parent=self.canvas_item,
                                                 line_width=25.0,
                                                 visibility=goocanvas.ITEM_HIDDEN)
        self.visible_line = goocanvas.Polyline(parent=self.canvas_item,
                                              line_width=1.0,
                                              stroke_color_rgba=0xC00000FF,
                                              line_dash=goocanvas.LineDash([2.0, 2.0 ]))
        self.invisible_line.props.pointer_events = (goocanvas.EVENTS_STROKE_MASK
                                                    |goocanvas.EVENTS_FILL_MASK
                                                    |goocanvas.EVENTS_PAINTED_MASK)
        self.canvas_item.set_data("pyviz-object", self)
        self.canvas_item.lower(None)
        self.set_ap(None)

    def set_ap(self, ap):
        if ap is self.node2:
            return
        if self.node2 is not None:
            self.node2.remove_link(self)
        self.node2 = ap
        if self.node2 is None:
            self.canvas_item.set_property("visibility", goocanvas.ITEM_HIDDEN)
        else:
            self.node2.add_link(self)
            self.canvas_item.set_property("visibility", goocanvas.ITEM_VISIBLE)
        self.update_points()

    def update_points(self):
        if self.node2 is None:
            return
        pos1_x, pos1_y = self.node1.get_position()
        pos2_x, pos2_y = self.node2.get_position()
        points = goocanvas.Points([(pos1_x, pos1_y), (pos2_x, pos2_y)])
        self.visible_line.set_property("points", points)
        self.invisible_line.set_property("points", points)
        
    def destroy(self):
        self.canvas_item.destroy()
        self.node1 = None
        self.node2 = None

    def tooltip_query(self, tooltip):
        pos1_x, pos1_y = self.node1.get_position()
        pos2_x, pos2_y = self.node2.get_position()
        dx = pos2_x - pos1_x
        dy = pos2_y - pos1_y
        d = transform_distance_canvas_to_simulation(math.sqrt(dx*dx + dy*dy))
        mac = self.dev.GetMac()
        tooltip.set_text(("WiFi link between STA Node %i and AP Node %i; distance=%.2f m.\n"
                          "SSID: %s\n"
                          "BSSID: %s")
                         % (self.node1.node_index, self.node2.node_index, d,
                            mac.GetSsid(), mac.GetBssid()))


class WifiLinkMonitor(object):
    def __init__(self, dummy_viz):
        self.access_points = {} # bssid -> node
        self.stations = [] # list of (sta_netdevice, viz_node, wifi_link)

    def scan_nodes(self, viz):
        for (sta_netdevice, viz_node, wifi_link) in self.stations:
            wifi_link.destroy()

        self.access_points = {}
        self.stations = []

        for node in viz.nodes.itervalues():
            ns3_node = ns3.NodeList.GetNode(node.node_index)
            for devI in range(ns3_node.GetNDevices()):
                dev = ns3_node.GetDevice(devI)
                if not isinstance(dev, ns3.WifiNetDevice):
                    continue
                wifi_mac = dev.GetMac()
                if isinstance(wifi_mac, ns3.StaWifiMac):
                    wifi_link = WifiLink(viz.links_group, node, dev)
                    self.stations.append((dev, node, wifi_link))
                elif isinstance(wifi_mac, ns3.ApWifiMac):
                    bssid = ns3.Mac48Address.ConvertFrom(dev.GetAddress())
                    self.access_points[str(bssid)] = node
        #print "APs: ", self.access_points
        #print "STAs: ", self.stations

    def simulation_periodic_update(self, viz):
        for (sta_netdevice, viz_node, wifi_link) in self.stations:
            if not sta_netdevice.IsLinkUp():
                wifi_link.set_ap(None)
                continue
            bssid = str(sta_netdevice.GetMac().GetBssid())
            if bssid == '00:00:00:00:00:00':
                wifi_link.set_ap(None)
                continue
            ap = self.access_points[bssid]
            wifi_link.set_ap(ap)

    def update_view(self, viz):
        for (dummy_sta_netdevice, dummy_viz_node, wifi_link) in self.stations:
            if wifi_link is not None:
                wifi_link.update_points()


def register(viz):
    link_monitor = WifiLinkMonitor(viz)
    viz.connect("simulation-periodic-update", link_monitor.simulation_periodic_update)
    viz.connect("update-view", link_monitor.update_view)
    viz.connect("topology-scanned", link_monitor.scan_nodes)
