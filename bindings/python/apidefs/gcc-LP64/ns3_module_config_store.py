from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## file-config.h: ns3::FileConfig [class]
    module.add_class('FileConfig', allow_subclassing=True)
    ## gtk-config-store.h: ns3::GtkConfigStore [class]
    module.add_class('GtkConfigStore')
    ## file-config.h: ns3::NoneFileConfig [class]
    module.add_class('NoneFileConfig', parent=root_module['ns3::FileConfig'])
    ## config-store.h: ns3::ConfigStore [class]
    module.add_class('ConfigStore', parent=root_module['ns3::ObjectBase'])
    ## config-store.h: ns3::ConfigStore::Mode [enumeration]
    module.add_enum('Mode', ['LOAD', 'SAVE', 'NONE'], outer_class=root_module['ns3::ConfigStore'])
    ## config-store.h: ns3::ConfigStore::FileFormat [enumeration]
    module.add_enum('FileFormat', ['XML', 'RAW_TEXT'], outer_class=root_module['ns3::ConfigStore'])
    
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
    register_Ns3FileConfig_methods(root_module, root_module['ns3::FileConfig'])
    register_Ns3GtkConfigStore_methods(root_module, root_module['ns3::GtkConfigStore'])
    register_Ns3NoneFileConfig_methods(root_module, root_module['ns3::NoneFileConfig'])
    register_Ns3ConfigStore_methods(root_module, root_module['ns3::ConfigStore'])
    return

def register_Ns3FileConfig_methods(root_module, cls):
    ## file-config.h: ns3::FileConfig::FileConfig() [constructor]
    cls.add_constructor([])
    ## file-config.h: ns3::FileConfig::FileConfig(ns3::FileConfig const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FileConfig const &', 'arg0')])
    ## file-config.h: void ns3::FileConfig::Attributes() [member function]
    cls.add_method('Attributes', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## file-config.h: void ns3::FileConfig::Default() [member function]
    cls.add_method('Default', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## file-config.h: void ns3::FileConfig::Global() [member function]
    cls.add_method('Global', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## file-config.h: void ns3::FileConfig::SetFilename(std::string filename) [member function]
    cls.add_method('SetFilename', 
                   'void', 
                   [param('std::string', 'filename')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3GtkConfigStore_methods(root_module, cls):
    ## gtk-config-store.h: ns3::GtkConfigStore::GtkConfigStore(ns3::GtkConfigStore const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GtkConfigStore const &', 'arg0')])
    ## gtk-config-store.h: ns3::GtkConfigStore::GtkConfigStore() [constructor]
    cls.add_constructor([])
    ## gtk-config-store.h: void ns3::GtkConfigStore::ConfigureAttributes() [member function]
    cls.add_method('ConfigureAttributes', 
                   'void', 
                   [])
    ## gtk-config-store.h: void ns3::GtkConfigStore::ConfigureDefaults() [member function]
    cls.add_method('ConfigureDefaults', 
                   'void', 
                   [])
    return

def register_Ns3NoneFileConfig_methods(root_module, cls):
    ## file-config.h: ns3::NoneFileConfig::NoneFileConfig(ns3::NoneFileConfig const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NoneFileConfig const &', 'arg0')])
    ## file-config.h: ns3::NoneFileConfig::NoneFileConfig() [constructor]
    cls.add_constructor([])
    ## file-config.h: void ns3::NoneFileConfig::Attributes() [member function]
    cls.add_method('Attributes', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## file-config.h: void ns3::NoneFileConfig::Default() [member function]
    cls.add_method('Default', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## file-config.h: void ns3::NoneFileConfig::Global() [member function]
    cls.add_method('Global', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## file-config.h: void ns3::NoneFileConfig::SetFilename(std::string filename) [member function]
    cls.add_method('SetFilename', 
                   'void', 
                   [param('std::string', 'filename')], 
                   is_virtual=True)
    return

def register_Ns3ConfigStore_methods(root_module, cls):
    ## config-store.h: ns3::ConfigStore::ConfigStore(ns3::ConfigStore const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConfigStore const &', 'arg0')])
    ## config-store.h: ns3::ConfigStore::ConfigStore() [constructor]
    cls.add_constructor([])
    ## config-store.h: void ns3::ConfigStore::ConfigureAttributes() [member function]
    cls.add_method('ConfigureAttributes', 
                   'void', 
                   [])
    ## config-store.h: void ns3::ConfigStore::ConfigureDefaults() [member function]
    cls.add_method('ConfigureDefaults', 
                   'void', 
                   [])
    ## config-store.h: ns3::TypeId ns3::ConfigStore::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## config-store.h: static ns3::TypeId ns3::ConfigStore::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## config-store.h: void ns3::ConfigStore::SetFileFormat(ns3::ConfigStore::FileFormat format) [member function]
    cls.add_method('SetFileFormat', 
                   'void', 
                   [param('ns3::ConfigStore::FileFormat', 'format')])
    ## config-store.h: void ns3::ConfigStore::SetFilename(std::string filename) [member function]
    cls.add_method('SetFilename', 
                   'void', 
                   [param('std::string', 'filename')])
    ## config-store.h: void ns3::ConfigStore::SetMode(ns3::ConfigStore::Mode mode) [member function]
    cls.add_method('SetMode', 
                   'void', 
                   [param('ns3::ConfigStore::Mode', 'mode')])
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

