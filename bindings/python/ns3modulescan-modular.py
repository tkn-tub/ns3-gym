#! /usr/bin/env python

import sys
import os.path

import pybindgen.settings
from pybindgen.gccxmlparser import ModuleParser, PygenClassifier, PygenSection, WrapperWarning
from pybindgen.typehandlers.codesink import FileCodeSink
from pygccxml.declarations import templates
from pygccxml.declarations.enumeration import enumeration_t
from pygccxml.declarations.class_declaration import class_t
from pygccxml.declarations.calldef import free_function_t, member_function_t, constructor_t, calldef_t


## we need the smart pointer type transformation to be active even
## during gccxml scanning.
import ns3modulegen_core_customizations


## silence gccxmlparser errors; we only want error handling in the
## generated python script, not while scanning.
class ErrorHandler(pybindgen.settings.ErrorHandler):
    def handle_error(self, dummy_wrapper, dummy_exception, dummy_traceback_):
        return True
pybindgen.settings.error_handler = ErrorHandler()
import warnings
warnings.filterwarnings(category=WrapperWarning, action='ignore')

type_annotations = {
    '::ns3::AttributeChecker': {
        'automatic_type_narrowing': 'true',
        'allow_subclassing': 'false',
        },
    '::ns3::AttributeValue': {
        'automatic_type_narrowing': 'true',
        'allow_subclassing': 'false',
        },

    '::ns3::CommandLine': {
        'allow_subclassing': 'true', # needed so that AddValue is able to set attributes on the object
        },

    '::ns3::NscTcpL4Protocol': {
        'ignore': 'true', # this class is implementation detail
        },


    'ns3::RandomVariable::RandomVariable(ns3::RandomVariableBase const & variable) [constructor]': {
        'ignore': None,
        },
    'ns3::RandomVariableBase * ns3::RandomVariable::Peek() const [member function]': {
        'ignore': None,
        },
    'void ns3::RandomVariable::GetSeed(uint32_t * seed) const [member function]': {
        'params': {'seed':{'direction':'out',
                           'array_length':'6'}}
        },
    'bool ns3::TypeId::LookupAttributeByName(std::string name, ns3::TypeId::AttributeInfo * info) const [member function]': {
        'params': {'info':{'transfer_ownership': 'false'}}
        },
    'static bool ns3::TypeId::LookupByNameFailSafe(std::string name, ns3::TypeId * tid) [member function]': {
        'ignore': None, # manually wrapped in 
        },
    'bool ns3::TraceSourceAccessor::ConnectWithoutContext(ns3::ObjectBase * obj, ns3::CallbackBase const & cb) const [member function]': {
        'params': {'obj': {'transfer_ownership':'false'}}
        },
    'bool ns3::TraceSourceAccessor::Connect(ns3::ObjectBase * obj, std::string context, ns3::CallbackBase const & cb) const [member function]': {
        'params': {'obj': {'transfer_ownership':'false'}}
        },
    'bool ns3::TraceSourceAccessor::DisconnectWithoutContext(ns3::ObjectBase * obj, ns3::CallbackBase const & cb) const [member function]': {
        'params': {'obj': {'transfer_ownership':'false'}}
        },
    'bool ns3::TraceSourceAccessor::Disconnect(ns3::ObjectBase * obj, std::string context, ns3::CallbackBase const & cb) const [member function]': {
        'params': {'obj': {'transfer_ownership':'false'}}
        },
    'bool ns3::AttributeAccessor::Set(ns3::ObjectBase * object, ns3::AttributeValue const & value) const [member function]': {
        'params': {'object': {'transfer_ownership':'false'}}
        },
    'ns3::EmpiricalVariable::EmpiricalVariable(ns3::RandomVariableBase const & variable) [constructor]': {
        'ignore': None
        },
    'static ns3::AttributeList * ns3::AttributeList::GetGlobal() [member function]': {
        'caller_owns_return': 'false'
        },
    'void ns3::CommandLine::Parse(int argc, char * * argv) const [member function]': {
        'ignore': None # manually wrapped
        },
    'extern void ns3::PythonCompleteConstruct(ns3::Ptr<ns3::Object> object, ns3::TypeId typeId, ns3::AttributeList const & attributes) [free function]': {
        'ignore': None # used transparently by, should not be wrapped
        },

    'ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4ListRouting::GetRoutingProtocol(uint32_t index, int16_t & priority) const [member function]': {
        'params': {'priority':{'direction':'out'}}
        },
    'ns3::Ipv4RoutingTableEntry * ns3::GlobalRouter::GetInjectedRoute(uint32_t i) [member function]': {
        'params': {'return': { 'caller_owns_return': 'false',}},
        },
    'ns3::Ipv4RoutingTableEntry * ns3::Ipv4GlobalRouting::GetRoute(uint32_t i) [member function]': {
        'params': {'return': { 'caller_owns_return': 'false',}},
        },
    
    }

def get_ns3_relative_path(path):
    l = []
    head = path
    while head:
        head, tail = os.path.split(head)
        if tail == 'ns3':
            return os.path.join(*l)
        l.insert(0, tail)
    raise AssertionError("is the path %r inside ns3?!" % path)

class PreScanHook:

    def __init__(self, headers_map, module):
        self.headers_map = headers_map
        self.module = module

    def __call__(self, dummy_module_parser,
                 pygccxml_definition,
                 global_annotations,
                 parameter_annotations):
        ns3_header = get_ns3_relative_path(pygccxml_definition.location.file_name)
        definition_module = self.headers_map[ns3_header]

        ## Note: we don't include line numbers in the comments because
        ## those numbers are very likely to change frequently, which would
        ## cause needless changes, since the generated python files are
        ## kept under version control.

        #global_annotations['pygen_comment'] = "%s:%i: %s" % \
        #    (ns3_header, pygccxml_definition.location.line, pygccxml_definition)
        global_annotations['pygen_comment'] = "%s (module %r): %s" % \
            (ns3_header, definition_module, pygccxml_definition)


        ## handle ns3::Object::GetObject (left to its own devices,
        ## pybindgen will generate a mangled name containing the template
        ## argument type name).
        if isinstance(pygccxml_definition, member_function_t) \
                and pygccxml_definition.parent.name == 'Object' \
                and pygccxml_definition.name == 'GetObject':
            template_args = templates.args(pygccxml_definition.demangled_name)
            if template_args == ['ns3::Object']:
                global_annotations['template_instance_names'] = 'ns3::Object=>GetObject'

        ## Don't wrap Simulator::Schedule* (manually wrapped)
        if isinstance(pygccxml_definition, member_function_t) \
                and pygccxml_definition.parent.name == 'Simulator' \
                and pygccxml_definition.name.startswith('Schedule'):
            global_annotations['ignore'] = None

        # manually wrapped
        if isinstance(pygccxml_definition, member_function_t) \
                and pygccxml_definition.parent.name == 'Simulator' \
                and pygccxml_definition.name == 'Run':
            global_annotations['ignore'] = True

        ## http://www.gccxml.org/Bug/view.php?id=9915
        if isinstance(pygccxml_definition, calldef_t):
            for arg in pygccxml_definition.arguments:
                if arg.default_value is None:
                    continue
                if "ns3::MilliSeconds( )" == arg.default_value:
                    arg.default_value = "ns3::MilliSeconds(0)"

        ## classes
        if isinstance(pygccxml_definition, class_t):
            # no need for helper classes to allow subclassing in Python, I think...
            #if pygccxml_definition.name.endswith('Helper'):
            #    global_annotations['allow_subclassing'] = 'false'

            if definition_module != self.module:
                global_annotations['import_from_module'] = 'ns.%s' % definition_module

            if pygccxml_definition.decl_string.startswith('::ns3::SimpleRefCount<'):
                global_annotations['incref_method'] = 'Ref'
                global_annotations['decref_method'] = 'Unref'
                global_annotations['peekref_method'] = 'GetReferenceCount'
                global_annotations['automatic_type_narrowing'] = 'true'
                return

            if pygccxml_definition.decl_string.startswith('::ns3::Callback<'):
                # manually handled in ns3modulegen_core_customizations.py
                global_annotations['ignore'] = None
                return

            if pygccxml_definition.decl_string.startswith('::ns3::TracedCallback<'):
                global_annotations['ignore'] = None
                return

            if pygccxml_definition.decl_string.startswith('::ns3::Ptr<'):
                # handled by pybindgen "type transformation"
                global_annotations['ignore'] = None
                return

            # table driven class customization
            try:
                annotations = type_annotations[pygccxml_definition.decl_string]
            except KeyError:
                pass
            else:
                global_annotations.update(annotations)

        ## enums
        if isinstance(pygccxml_definition, enumeration_t):
            if definition_module != self.module:
                global_annotations['import_from_module'] = 'ns.%s' % definition_module

        ## free functions
        if isinstance(pygccxml_definition, free_function_t):

            if definition_module != self.module:
                global_annotations['ignore'] = None
                return

            if pygccxml_definition.name == 'PeekPointer':
                global_annotations['ignore'] = None
                return

        ## table driven methods/constructors/functions customization
        if isinstance(pygccxml_definition, (free_function_t, member_function_t, constructor_t)):
            try:
                annotations = type_annotations[str(pygccxml_definition)]
            except KeyError:
                pass
            else:
                for key,value in annotations.items():
                    if key == 'params':
                        parameter_annotations.update (value)
                        del annotations['params']
                global_annotations.update(annotations)


# def post_scan_hook(dummy_module_parser, dummy_pygccxml_definition, pybindgen_wrapper):
#     ## classes
#     if isinstance(pybindgen_wrapper, CppClass):
#         if pybindgen_wrapper.name.endswith('Checker'):
#             print >> sys.stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", pybindgen_wrapper
#             #pybindgen_wrapper.set_instance_creation_function(AttributeChecker_instance_creation_function)


def scan_callback_classes(module_parser, callback_classes_file):
    callback_classes_file.write("callback_classes = [\n")
    for cls in module_parser.module_namespace.classes(function=module_parser.location_filter,
                                                      recursive=False):
        if not cls.name.startswith("Callback<"):
            continue
        assert templates.is_instantiation(cls.decl_string), "%s is not a template instantiation" % cls
        dummy_cls_name, template_parameters = templates.split(cls.decl_string)
        callback_classes_file.write("    %r,\n" % template_parameters)
    callback_classes_file.write("]\n")


def ns3_module_scan(top_builddir, module_name, headers_map, output_file_name, cflags):
    module_parser = ModuleParser('ns.%s' % module_name, 'ns3')
    module_parser.add_pre_scan_hook(PreScanHook(headers_map, module_name))
    #module_parser.add_post_scan_hook(post_scan_hook)

    gccxml_options = dict(
        include_paths=[top_builddir],
         define_symbols={
            #'NS3_ASSERT_ENABLE': None,
            #'NS3_LOG_ENABLE': None,
            },
        cflags=('--gccxml-cxxflags %r' % (cflags,))
        )

    try:
        os.unlink(output_file_name)
    except OSError:
        pass
    try:
        os.makedirs(os.path.dirname(output_file_name))
    except OSError:
        pass
    output_file = open(output_file_name, "wt")
    output_sink = FileCodeSink(output_file)
    module_parser.parse_init([os.path.join(top_builddir, "ns3", "%s-module.h" % module_name)],
                             None, whitelist_paths=[top_builddir],
                             #includes=['"ns3/everything.h"'],
                             pygen_sink=output_sink,
                             gccxml_options=gccxml_options)
    module_parser.scan_types()

    #callback_classes_file = open(os.path.join(os.path.dirname(pygen_file_name), "callbacks_list.py"), "wt")
    #scan_callback_classes(module_parser, callback_classes_file)
    #callback_classes_file.close()


    module_parser.scan_methods()
    module_parser.scan_functions()
    module_parser.parse_finalize()

    output_file.close()
    os.chmod(output_file_name, 0400)


if __name__ == '__main__':
    if len(sys.argv) != 6:
        print "ns3modulescan2.py top_builddir module_path module_headers output_file_name cflags"
        sys.exit(1)
    ns3_module_scan(sys.argv[1], sys.argv[2], eval(sys.argv[3]), sys.argv[4], sys.argv[5])
    sys.exit(0)
