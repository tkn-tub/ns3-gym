from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## topology-reader-helper.h: ns3::TopologyReaderHelper [class]
    module.add_class('TopologyReaderHelper')
    ## topology-reader.h: ns3::TopologyReader [class]
    module.add_class('TopologyReader', parent=root_module['ns3::Object'])
    ## topology-reader.h: ns3::TopologyReader::Link [class]
    module.add_class('Link', outer_class=root_module['ns3::TopologyReader'])
    ## inet-topology-reader.h: ns3::InetTopologyReader [class]
    module.add_class('InetTopologyReader', parent=root_module['ns3::TopologyReader'])
    ## orbis-topology-reader.h: ns3::OrbisTopologyReader [class]
    module.add_class('OrbisTopologyReader', parent=root_module['ns3::TopologyReader'])
    ## rocketfuel-topology-reader.h: ns3::RocketfuelTopologyReader [class]
    module.add_class('RocketfuelTopologyReader', parent=root_module['ns3::TopologyReader'])
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace FatalImpl
    
    nested_module = module.add_cpp_namespace('FatalImpl')
    register_types_ns3_FatalImpl(nested_module)
    
    
    ## Register a nested module for the namespace addressUtils
    
    nested_module = module.add_cpp_namespace('addressUtils')
    register_types_ns3_addressUtils(nested_module)
    
    
    ## Register a nested module for the namespace aodv
    
    nested_module = module.add_cpp_namespace('aodv')
    register_types_ns3_aodv(nested_module)
    
    
    ## Register a nested module for the namespace dot11s
    
    nested_module = module.add_cpp_namespace('dot11s')
    register_types_ns3_dot11s(nested_module)
    
    
    ## Register a nested module for the namespace dsdv
    
    nested_module = module.add_cpp_namespace('dsdv')
    register_types_ns3_dsdv(nested_module)
    
    
    ## Register a nested module for the namespace flame
    
    nested_module = module.add_cpp_namespace('flame')
    register_types_ns3_flame(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_addressUtils(module):
    root_module = module.get_root()
    

def register_types_ns3_aodv(module):
    root_module = module.get_root()
    

def register_types_ns3_dot11s(module):
    root_module = module.get_root()
    

def register_types_ns3_dsdv(module):
    root_module = module.get_root()
    

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3TopologyReaderHelper_methods(root_module, root_module['ns3::TopologyReaderHelper'])
    register_Ns3TopologyReader_methods(root_module, root_module['ns3::TopologyReader'])
    register_Ns3TopologyReaderLink_methods(root_module, root_module['ns3::TopologyReader::Link'])
    register_Ns3InetTopologyReader_methods(root_module, root_module['ns3::InetTopologyReader'])
    register_Ns3OrbisTopologyReader_methods(root_module, root_module['ns3::OrbisTopologyReader'])
    register_Ns3RocketfuelTopologyReader_methods(root_module, root_module['ns3::RocketfuelTopologyReader'])
    return

def register_Ns3TopologyReaderHelper_methods(root_module, cls):
    ## topology-reader-helper.h: ns3::TopologyReaderHelper::TopologyReaderHelper(ns3::TopologyReaderHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TopologyReaderHelper const &', 'arg0')])
    ## topology-reader-helper.h: ns3::TopologyReaderHelper::TopologyReaderHelper() [constructor]
    cls.add_constructor([])
    ## topology-reader-helper.h: ns3::Ptr<ns3::TopologyReader> ns3::TopologyReaderHelper::GetTopologyReader() [member function]
    cls.add_method('GetTopologyReader', 
                   'ns3::Ptr< ns3::TopologyReader >', 
                   [])
    ## topology-reader-helper.h: void ns3::TopologyReaderHelper::SetFileName(std::string const fileName) [member function]
    cls.add_method('SetFileName', 
                   'void', 
                   [param('std::string const', 'fileName')])
    ## topology-reader-helper.h: void ns3::TopologyReaderHelper::SetFileType(std::string const fileType) [member function]
    cls.add_method('SetFileType', 
                   'void', 
                   [param('std::string const', 'fileType')])
    return

def register_Ns3TopologyReader_methods(root_module, cls):
    ## topology-reader.h: ns3::TopologyReader::TopologyReader() [constructor]
    cls.add_constructor([])
    ## topology-reader.h: void ns3::TopologyReader::AddLink(ns3::TopologyReader::Link link) [member function]
    cls.add_method('AddLink', 
                   'void', 
                   [param('ns3::TopologyReader::Link', 'link')])
    ## topology-reader.h: std::string ns3::TopologyReader::GetFileName() const [member function]
    cls.add_method('GetFileName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## topology-reader.h: static ns3::TypeId ns3::TopologyReader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## topology-reader.h: std::_List_const_iterator<ns3::TopologyReader::Link> ns3::TopologyReader::LinksBegin() const [member function]
    cls.add_method('LinksBegin', 
                   'std::_List_const_iterator< ns3::TopologyReader::Link >', 
                   [], 
                   is_const=True)
    ## topology-reader.h: bool ns3::TopologyReader::LinksEmpty() const [member function]
    cls.add_method('LinksEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## topology-reader.h: std::_List_const_iterator<ns3::TopologyReader::Link> ns3::TopologyReader::LinksEnd() const [member function]
    cls.add_method('LinksEnd', 
                   'std::_List_const_iterator< ns3::TopologyReader::Link >', 
                   [], 
                   is_const=True)
    ## topology-reader.h: int ns3::TopologyReader::LinksSize() const [member function]
    cls.add_method('LinksSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## topology-reader.h: ns3::NodeContainer ns3::TopologyReader::Read() [member function]
    cls.add_method('Read', 
                   'ns3::NodeContainer', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## topology-reader.h: void ns3::TopologyReader::SetFileName(std::string const fileName) [member function]
    cls.add_method('SetFileName', 
                   'void', 
                   [param('std::string const', 'fileName')])
    return

def register_Ns3TopologyReaderLink_methods(root_module, cls):
    ## topology-reader.h: ns3::TopologyReader::Link::Link(ns3::TopologyReader::Link const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TopologyReader::Link const &', 'arg0')])
    ## topology-reader.h: ns3::TopologyReader::Link::Link(ns3::Ptr<ns3::Node> fromPtr, std::string fromName, ns3::Ptr<ns3::Node> toPtr, std::string toName) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'fromPtr'), param('std::string', 'fromName'), param('ns3::Ptr< ns3::Node >', 'toPtr'), param('std::string', 'toName')])
    ## topology-reader.h: std::_Rb_tree_const_iterator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > ns3::TopologyReader::Link::AttributesBegin() [member function]
    cls.add_method('AttributesBegin', 
                   'std::_Rb_tree_const_iterator< std::pair< std::basic_string< char, std::char_traits< char >, std::allocator< char > > const, std::basic_string< char, std::char_traits< char >, std::allocator< char > > > >', 
                   [])
    ## topology-reader.h: std::_Rb_tree_const_iterator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > ns3::TopologyReader::Link::AttributesEnd() [member function]
    cls.add_method('AttributesEnd', 
                   'std::_Rb_tree_const_iterator< std::pair< std::basic_string< char, std::char_traits< char >, std::allocator< char > > const, std::basic_string< char, std::char_traits< char >, std::allocator< char > > > >', 
                   [])
    ## topology-reader.h: std::string ns3::TopologyReader::Link::GetAttribute(std::string name) [member function]
    cls.add_method('GetAttribute', 
                   'std::string', 
                   [param('std::string', 'name')])
    ## topology-reader.h: bool ns3::TopologyReader::Link::GetAttributeFailSafe(std::string name, std::string & value) [member function]
    cls.add_method('GetAttributeFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('std::string &', 'value')])
    ## topology-reader.h: ns3::Ptr<ns3::Node> ns3::TopologyReader::Link::GetFromNode() const [member function]
    cls.add_method('GetFromNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## topology-reader.h: std::string ns3::TopologyReader::Link::GetFromNodeName() const [member function]
    cls.add_method('GetFromNodeName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## topology-reader.h: ns3::Ptr<ns3::Node> ns3::TopologyReader::Link::GetToNode() const [member function]
    cls.add_method('GetToNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## topology-reader.h: std::string ns3::TopologyReader::Link::GetToNodeName() const [member function]
    cls.add_method('GetToNodeName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## topology-reader.h: void ns3::TopologyReader::Link::SetAttribute(std::string name, std::string & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('std::string &', 'value')])
    return

def register_Ns3InetTopologyReader_methods(root_module, cls):
    ## inet-topology-reader.h: static ns3::TypeId ns3::InetTopologyReader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## inet-topology-reader.h: ns3::InetTopologyReader::InetTopologyReader() [constructor]
    cls.add_constructor([])
    ## inet-topology-reader.h: ns3::NodeContainer ns3::InetTopologyReader::Read() [member function]
    cls.add_method('Read', 
                   'ns3::NodeContainer', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3OrbisTopologyReader_methods(root_module, cls):
    ## orbis-topology-reader.h: static ns3::TypeId ns3::OrbisTopologyReader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## orbis-topology-reader.h: ns3::OrbisTopologyReader::OrbisTopologyReader() [constructor]
    cls.add_constructor([])
    ## orbis-topology-reader.h: ns3::NodeContainer ns3::OrbisTopologyReader::Read() [member function]
    cls.add_method('Read', 
                   'ns3::NodeContainer', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3RocketfuelTopologyReader_methods(root_module, cls):
    ## rocketfuel-topology-reader.h: static ns3::TypeId ns3::RocketfuelTopologyReader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## rocketfuel-topology-reader.h: ns3::RocketfuelTopologyReader::RocketfuelTopologyReader() [constructor]
    cls.add_constructor([])
    ## rocketfuel-topology-reader.h: ns3::NodeContainer ns3::RocketfuelTopologyReader::Read() [member function]
    cls.add_method('Read', 
                   'ns3::NodeContainer', 
                   [], 
                   is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_aodv(module.get_submodule('aodv'), root_module)
    register_functions_ns3_dot11s(module.get_submodule('dot11s'), root_module)
    register_functions_ns3_dsdv(module.get_submodule('dsdv'), root_module)
    register_functions_ns3_flame(module.get_submodule('flame'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_addressUtils(module, root_module):
    return

def register_functions_ns3_aodv(module, root_module):
    return

def register_functions_ns3_dot11s(module, root_module):
    return

def register_functions_ns3_dsdv(module, root_module):
    return

def register_functions_ns3_flame(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

