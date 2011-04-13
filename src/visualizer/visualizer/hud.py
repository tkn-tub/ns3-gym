import goocanvas
import core
import math
import pango
import gtk


class Axes(object):
    def __init__(self, viz):
        self.viz = viz
        self.color = 0x8080C0FF
        self.hlines = goocanvas.Path(parent=viz.canvas.get_root_item(), stroke_color_rgba=self.color)
        self.hlines.lower(None)
        self.vlines = goocanvas.Path(parent=viz.canvas.get_root_item(), stroke_color_rgba=self.color)
        self.vlines.lower(None)
        self.labels = []
        hadj = self.viz.get_hadjustment()
        vadj = self.viz.get_vadjustment()
        def update(adj):
            if self.visible:
                self.update_view()
        hadj.connect("value-changed", update)
        vadj.connect("value-changed", update)
        hadj.connect("changed", update)
        vadj.connect("changed", update)
        self.visible = True
        self.update_view()

    def set_visible(self, visible):
        self.visible = visible
        if self.visible:
            self.hlines.props.visibility = goocanvas.ITEM_VISIBLE
            self.vlines.props.visibility = goocanvas.ITEM_VISIBLE
        else:
            self.hlines.props.visibility = goocanvas.ITEM_HIDDEN
            self.vlines.props.visibility = goocanvas.ITEM_HIDDEN
            for label in self.labels:
                label.props.visibility = goocanvas.ITEM_HIDDEN
            
    def _compute_divisions(self, xi, xf):
        assert xf > xi
        dx = xf - xi
        size = dx
        ndiv = 5
        text_width = dx/ndiv/2

        def rint(x):
            return math.floor(x+0.5)
        
        dx_over_ndiv = dx / ndiv
        for n in range(5): # iterate 5 times to find optimum division size
            #/* div: length of each division */
            tbe = math.log10(dx_over_ndiv)#;   /* looking for approx. 'ndiv' divisions in a length 'dx' */
            div = pow(10, rint(tbe))#;	/* div: power of 10 closest to dx/ndiv */
            if math.fabs(div/2 - dx_over_ndiv) < math.fabs(div - dx_over_ndiv): #/* test if div/2 is closer to dx/ndiv */
                div /= 2
            elif math.fabs(div*2 - dx_over_ndiv) < math.fabs(div - dx_over_ndiv):
                div *= 2 #			/* test if div*2 is closer to dx/ndiv */
            x0 = div*math.ceil(xi / div) - div
            if n > 1:
                ndiv = rint(size / text_width)
        return x0, div
        
        
    def update_view(self):
        if self.viz.zoom is None:
            return

        unused_labels = self.labels
        self.labels = []
        for label in unused_labels:
            label.set_property("visibility", goocanvas.ITEM_HIDDEN)
        def get_label():
            try:
                label = unused_labels.pop(0)
            except IndexError:
                label = goocanvas.Text(parent=self.viz.canvas.get_root_item(), stroke_color_rgba=self.color)
            else:
                label.set_property("visibility", goocanvas.ITEM_VISIBLE)
                label.lower(None)
            self.labels.append(label)
            return label

        hadj = self.viz.get_hadjustment()
        vadj = self.viz.get_vadjustment()
        zoom = self.viz.zoom.value
        offset = 10/zoom

        x1, y1 = self.viz.canvas.convert_from_pixels(hadj.value, vadj.value)
        x2, y2 = self.viz.canvas.convert_from_pixels(hadj.value + hadj.page_size, vadj.value + vadj.page_size)
        line_width = 5.0/self.viz.zoom.value

        # draw the horizontal axis
        self.hlines.set_property("line-width", line_width)
        yc = y2 - line_width/2

        sim_x1 = x1/core.PIXELS_PER_METER
        sim_x2 = x2/core.PIXELS_PER_METER
        x0, xdiv = self._compute_divisions(sim_x1, sim_x2)
        path = ["M %r %r L %r %r" % (x1, yc, x2, yc)]
        x = x0
        while x < sim_x2:
            path.append("M %r %r L %r %r" % (core.PIXELS_PER_METER*x, yc - offset, core.PIXELS_PER_METER*x, yc))
            label = get_label()
            label.set_properties(font=("Sans Serif %f" % int(12/zoom)),
                                 text=("%G" % x),
                                 fill_color_rgba=self.color,
                                 alignment=pango.ALIGN_CENTER,
                                 anchor=gtk.ANCHOR_S,
                                 x=core.PIXELS_PER_METER*x,
                                 y=(yc - offset))
            x += xdiv
        del x

        self.hlines.set_property("data", " ".join(path))

        # draw the vertical axis
        self.vlines.set_property("line-width", line_width)
        xc = x1 + line_width/2

        sim_y1 = y1/core.PIXELS_PER_METER
        sim_y2 = y2/core.PIXELS_PER_METER


        y0, ydiv = self._compute_divisions(sim_y1, sim_y2)
        path = ["M %r %r L %r %r" % (xc, y1, xc, y2)]
        y = y0
        while y < sim_y2:
            path.append("M %r %r L %r %r" % (xc, core.PIXELS_PER_METER*y, xc + offset, core.PIXELS_PER_METER*y))
            label = get_label()
            label.set_properties(font=("Sans Serif %f" % int(12/zoom)),
                                 text=("%G" % y),
                                 fill_color_rgba=self.color,
                                 alignment=pango.ALIGN_LEFT,
                                 anchor=gtk.ANCHOR_W,
                                 x=xc + offset,
                                 y=core.PIXELS_PER_METER*y)
            y += ydiv

        self.vlines.set_property("data", " ".join(path))



        self.labels.extend(unused_labels)
