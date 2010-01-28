from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## average.h: ns3::Average<double> [class]
    module.add_class('Average', template_parameters=['double'])
    ## delay-jitter-estimation.h: ns3::DelayJitterEstimation [class]
    module.add_class('DelayJitterEstimation')
    ## event-garbage-collector.h: ns3::EventGarbageCollector [class]
    module.add_class('EventGarbageCollector')
    ## file-config.h: ns3::FileConfig [class]
    module.add_class('FileConfig', allow_subclassing=True)
    ## gnuplot.h: ns3::Gnuplot [class]
    module.add_class('Gnuplot')
    ## gnuplot.h: ns3::GnuplotCollection [class]
    module.add_class('GnuplotCollection')
    ## gnuplot.h: ns3::GnuplotDataset [class]
    module.add_class('GnuplotDataset')
    ## gtk-config-store.h: ns3::GtkConfigStore [class]
    module.add_class('GtkConfigStore')
    ## file-config.h: ns3::NoneFileConfig [class]
    module.add_class('NoneFileConfig', parent=root_module['ns3::FileConfig'])
