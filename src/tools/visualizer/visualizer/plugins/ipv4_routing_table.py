import gtk
import ns3
from visualizer.base import InformationWindow

class ShowIpv4RoutingTable(InformationWindow):
    (
        COLUMN_DESTINATION,
        COLUMN_NEXT_HOP,
        COLUMN_INTERFACE,
        ) = range(3)

    def __init__(self, visualizer, node_index):
        InformationWindow.__init__(self)
        self.win = gtk.Dialog(parent=visualizer.window,
                              flags=gtk.DIALOG_DESTROY_WITH_PARENT|gtk.DIALOG_NO_SEPARATOR,
                              buttons=(gtk.STOCK_CLOSE, gtk.RESPONSE_CLOSE))
        self.win.connect("response", self._response_cb)
        self.win.set_title("IPv4 routing table for node %i" % node_index) 
        self.visualizer = visualizer
        self.node_index = node_index

        self.table_model = gtk.ListStore(str, str, str)

        treeview = gtk.TreeView(self.table_model)
        treeview.show()
        self.win.vbox.add(treeview)
        
        # Dest.
        column = gtk.TreeViewColumn('Destination', gtk.CellRendererText(),
                                    text=self.COLUMN_DESTINATION)
        treeview.append_column(column)

        # Next hop
        column = gtk.TreeViewColumn('Next hop', gtk.CellRendererText(),
                                    text=self.COLUMN_NEXT_HOP)
        treeview.append_column(column)

        # Interface
        column = gtk.TreeViewColumn('Interface', gtk.CellRendererText(),
                                    text=self.COLUMN_INTERFACE)
        treeview.append_column(column)

        self.visualizer.add_information_window(self)
        self.win.show()

    def _response_cb(self, win, response):
        self.win.destroy()
        self.visualizer.remove_information_window(self)
    
    def update(self):
        node = ns3.NodeList.GetNode(self.node_index)
        ipv4 = node.GetObject(ns3.Ipv4.GetTypeId())
        routing = ipv4.GetRoutingProtocol()
        if routing is None:
            return
        if isinstance(routing, ns3.Ipv4StaticRouting):
            ipv4_routing = routing
        elif isinstance(routing, ns3.Ipv4ListRouting):
            for rI in range(routing.GetNRoutingProtocols()):
                ipv4_routing, prio = routing.GetRoutingProtocol(rI)
                if isinstance(ipv4_routing, ns3.Ipv4StaticRouting):
                    break
            else:
                return
        else:
            return
        self.table_model.clear()
        for routeI in range(ipv4_routing.GetNRoutes()):
            route = ipv4_routing.GetRoute(routeI)
            tree_iter = self.table_model.append()
            netdevice = ipv4.GetNetDevice(route.GetInterface())
            if netdevice is None:
                interface_name = 'lo'
            else:
                interface_name = ns3.Names.FindName(netdevice)
                if not interface_name:
                    interface_name = "(interface %i)" % route.GetInterface()
            self.table_model.set(tree_iter,
                                 self.COLUMN_DESTINATION, str(route.GetDest()),
                                 self.COLUMN_NEXT_HOP, str(route.GetGateway()),
                                 self.COLUMN_INTERFACE, interface_name)


def populate_node_menu(viz, node, menu):
    menu_item = gtk.MenuItem("Show IPv4 Routing Table")
    menu_item.show()

    def _show_ipv4_routing_table(dummy_menu_item):
        ShowIpv4RoutingTable(viz, node.node_index)

    menu_item.connect("activate", _show_ipv4_routing_table)
    menu.add(menu_item)

def register(viz):
    viz.connect("populate-node-menu", populate_node_menu)
