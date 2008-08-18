
LOCAL_MODULES = [
    #'my_extra_api_definitions',
    ]



import sys
import os

from pybindgen import FileCodeSink, write_preamble
from pybindgen.module import MultiSectionFactory
import pybindgen.settings

from ns3modulegen_generated import module_init, register_types, register_methods, register_functions
import ns3modulegen_core_customizations
import callbacks_list

this_script_dir = os.path.dirname(os.path.abspath(sys.argv[0]))

class ErrorHandler(pybindgen.settings.ErrorHandler):
    def handle_error(self, wrapper, exception, traceback_):
        try:
            stack = wrapper.stack_where_defined
        except AttributeError:
            print >> sys.stderr, "??:??: %s / %r" % (wrapper, exception)
        else:
            stack = list(stack)
            stack.reverse()
            for (filename, line_number, function_name, text) in stack:
                file_dir = os.path.dirname(os.path.abspath(filename))
                if file_dir == this_script_dir:
                    print >> sys.stderr, "%s:%i: %r" % (os.path.join("..", "bindings", "python", os.path.basename(filename)),
                                                        line_number, exception)
                    break
        return True
pybindgen.settings.error_handler = ErrorHandler()


class MyMultiSectionFactory(MultiSectionFactory):

    def __init__(self, main_file_name, modules):
        super(MyMultiSectionFactory, self).__init__()
        self.main_file_name = main_file_name
        self.main_sink = FileCodeSink(open(main_file_name, "wt"))
        self.header_name = "ns3module.h"
        header_file_name = os.path.join(os.path.dirname(self.main_file_name), self.header_name)
        self.header_sink = FileCodeSink(open(header_file_name, "wt"))
        self.section_sinks = {'__main__': self.main_sink}

        for module in modules:
            section_name = 'ns3_module_%s' % module.replace('-', '_')
            file_name = os.path.join(os.path.dirname(self.main_file_name), "%s.cc" % section_name)
            sink = FileCodeSink(open(file_name, "wt"))
            self.section_sinks[section_name] = sink            

    def get_section_code_sink(self, section_name):
        return self.section_sinks[section_name]

    def get_main_code_sink(self):
        return self.main_sink

    def get_common_header_code_sink(self):
        return self.header_sink

    def get_common_header_include(self):
        return '"%s"' % self.header_name

    def close(self):
        self.header_sink.file.close()
        self.main_sink.file.close()
        for sink in self.section_sinks.itervalues():
            sink.file.close()



def main():
    out = MyMultiSectionFactory(sys.argv[1], sys.argv[2:])
    root_module = module_init()
    root_module.add_include('"everything.h"')

    register_types(root_module)

    ns3modulegen_core_customizations.Simulator_customizations(root_module)
    ns3modulegen_core_customizations.CommandLine_customizations(root_module)


    for local_module in LOCAL_MODULES:
        mod = __import__(local_module)
        mod.register_types(root_module)

    ns3modulegen_core_customizations.generate_callback_classes(root_module.after_forward_declarations,
                                                               callbacks_list.callback_classes)


    register_methods(root_module)

    for local_module in LOCAL_MODULES:
        mod = __import__(local_module)
        mod.register_methods(root_module)

    ns3modulegen_core_customizations.Object_customizations(root_module)
    ns3modulegen_core_customizations.Attribute_customizations(root_module)

    register_functions(root_module)

    for local_module in LOCAL_MODULES:
        mod = __import__(local_module)
        mod.register_functions(root_module)


    # if GtkConfigStore support is disabled, disable the class wrapper
    if 'DISABLE_GTK_CONFIG_STORE' in os.environ:
        root_module.classes.remove(root_module['ns3::GtkConfigStore'])

    root_module.generate(out, '_ns3')

    out.close()

if __name__ == '__main__':
    main()

