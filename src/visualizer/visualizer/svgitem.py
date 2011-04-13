import gobject
import rsvg
#import cairo
import goocanvas
import os.path


class SvgItem(goocanvas.ItemSimple):
    # setup our custom properties
    __gproperties__ = {
        'x': (float,                                # property type
              'X',                                  # property nick name
              'The x coordinate of a SVG image',    # property description
              -10e6,                                    # property minimum value
              10e6,                                 # property maximum value
              0,                                    # property default value
              gobject.PARAM_READWRITE),             # property flags

        'y': (float,
              'Y',
              'The y coordinate of a SVG image',
              -10e6,
              10e6,
              0,
              gobject.PARAM_READWRITE),

        'width': (float,
                  'Width',
                  'The width of the SVG Image',
                  0,
                  10e6,
                  0,
                  gobject.PARAM_READWRITE),

        'height': (float,
                   'Height',
                   'The width of the SVG Image',
                   0,
                   10e6,
                   0,
                   gobject.PARAM_READWRITE),
        }
    
    def __init__(self, x, y, rsvg_handle, **kwargs):
        super(SvgItem, self).__init__(**kwargs)
        assert isinstance(rsvg_handle, rsvg.Handle)
        self.x = x
        self.y = y
        self.sx = 1.0
        self.sy = 1.0
        self.handle = rsvg_handle
        self.width = self.handle.props.width
        self.height = self.handle.props.height
        self.custom_width = None
        self.custom_height = None

    def do_set_property(self, pspec, value):
        if pspec.name == 'x':
            self.x = value
            
            # make sure we update the display
            self.changed(True)
        
        elif pspec.name == 'y':
            self.y = value
            
            # make sure we update the display
            self.changed(True)
        
        elif pspec.name == 'width':
            self.custom_width = value
            self._size_changed()
            
            # make sure we update the display
            self.changed(True)

        elif pspec.name == 'height':
            self.custom_height = value
            self._size_changed()
            
            # make sure we update the display
            self.changed(True)
        
        else:
            raise AttributeError, 'unknown property %s' % pspec.name

    def _size_changed(self):
        if self.custom_width is None and self.custom_height is None:
            self.width = self.handle.props.width
            self.height = self.handle.props.height
            self.sx = 1.0
            self.sy = 1.0
        elif self.custom_width is not None and self.custom_height is None:
            self.width = self.custom_width
            self.sx = self.custom_width / self.handle.props.width
            self.sy = self.sx
            self.height = self.handle.props.height*self.sy
        elif self.custom_width is None and self.custom_height is not None:
            self.height = self.custom_height
            self.sy = self.custom_height / self.handle.props.height
            self.sx  = self.sy
            self.width = self.handle.props.width*self.sx
        else:
            self.width = self.custom_width
            self.height = self.custom_height
            self.sx = self.custom_width / self.handle.props.width
            self.sy = self.custom_height / self.handle.props.height

    def do_get_property(self, pspec):
        if pspec.name == 'x':
            return self.x

        elif pspec.name == 'y':
            return self.y

        elif pspec.name == 'width':
            self.width = self.handle.props.width
            self.height = self.handle.props.height

            return self.width

        elif pspec.name == 'height':
            return self.height

        else:
            raise AttributeError, 'unknown property %s' % pspec.name
    
    def do_simple_paint(self, cr, bounds):
        cr.translate(self.x, self.y)
        cr.scale(self.sx, self.sy)
        self.handle.render_cairo(cr)

    def do_simple_update(self, cr):
        self.bounds_x1 = float(self.x)
        self.bounds_y1 = float(self.y)
        self.bounds_x2 = float(self.x + self.width)
        self.bounds_y2 = float(self.y + self.height)

    def do_simple_is_item_at(self, x, y, cr, is_pointer_event):
        if ((x < self.x) or (x > self.x + self.width)) or ((y < self.y) or (y > self.y + self.height)):
            return False
        else:    
            return True


_rsvg_cache = dict()

def rsvg_handle_factory(base_file_name):
    try:
        return _rsvg_cache[base_file_name]
    except KeyError:
        full_path = os.path.join(os.path.dirname(__file__), 'resource', base_file_name)
        rsvg_handle = rsvg.Handle(full_path)
        _rsvg_cache[base_file_name] = rsvg_handle
        return rsvg_handle

