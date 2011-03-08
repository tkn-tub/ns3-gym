import warnings
import sys
import os
import pybindgen.settings
from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers
import ns3modulegen_core_customizations

class ErrorHandler(pybindgen.settings.ErrorHandler):
    def handle_error(self, wrapper, exception, traceback_):
        warnings.warn("exception %r in wrapper %s" % (exception, wrapper))
        return True
pybindgen.settings.error_handler = ErrorHandler()




def main(argv):
    assert len(argv) == 3
    module_abs_src_path, target = argv[1], argv[2]
    module_name = os.path.basename(module_abs_src_path)

    sys.path.insert(0, os.path.join(module_abs_src_path, "bindings"))
    try:
        module_apidefs = __import__("modulegen__%s" % target)
        del sys.modules["modulegen__%s" % target]
        try:
            module_customization = __import__("modulegen_local")
            del sys.modules["modulegen_local"]
        except ImportError:
            module_customization = None
    finally:
        sys.path.pop(0)
    
    out = FileCodeSink(sys.stdout)
    root_module = module_apidefs.module_init()
    root_module.add_include('"ns3/%s-module.h"' % module_name)
    module_apidefs.register_types(root_module)

    module_apidefs.register_methods(root_module)
    module_apidefs.register_functions(root_module)
    
    ns3modulegen_core_customizations.Object_customizations(root_module)
    ns3modulegen_core_customizations.Attribute_customizations(root_module)

    if module_customization is not None:
        module_customization.customize(root_module)

    root_module.generate(out)

if __name__ == '__main__':
    import sys
    main(sys.argv)

    
