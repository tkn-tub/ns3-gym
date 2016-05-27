# -*- Mode:Python; -*-
# /*
#  * Copyright (c) 2010 INRIA
#  *
#  * This program is free software; you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License version 2 as
#  * published by the Free Software Foundation;
#  *
#  * This program is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with this program; if not, write to the Free Software
#  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#  *
#  * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
#  */
# 
# Python version of sample-simulator.cc

import ns.core

class MyModel(object):
    """Simple model object to illustrate event handling."""

    ## \returns None.
    def Start(self):
        """Start model execution by scheduling a HandleEvent."""
        ns.core.Simulator.Schedule(ns.core.Seconds(10.0), self.HandleEvent, ns.core.Simulator.Now().GetSeconds())

    ## \param [in] value Event argument.
    ## \return None.
    def HandleEvent(self, value):
        """Simple event handler."""
        print "Member method received event at", ns.core.Simulator.Now().GetSeconds(), \
            "s started at", value, "s"

def ExampleFunction(model):
    print "ExampleFunction received event at", ns.core.Simulator.Now().GetSeconds(), "s"
    model.Start()

def RandomFunction(model):
    print "RandomFunction received event at", ns.core.Simulator.Now().GetSeconds(), "s"

def CancelledEvent():
    print "I should never be called... "

def main(dummy_argv):
    ns.core.CommandLine().Parse(dummy_argv)
    
    model = MyModel()
    v = ns.core.UniformRandomVariable()
    v.SetAttribute("Min", ns.core.DoubleValue (10))
    v.SetAttribute("Max", ns.core.DoubleValue (20))

    ns.core.Simulator.Schedule(ns.core.Seconds(10.0), ExampleFunction, model)

    ns.core.Simulator.Schedule(ns.core.Seconds(v.GetValue()), RandomFunction, model)

    id = ns.core.Simulator.Schedule(ns.core.Seconds(30.0), CancelledEvent)
    ns.core.Simulator.Cancel(id)

    ns.core.Simulator.Run()

    ns.core.Simulator.Destroy()

if __name__ == '__main__':
    import sys
    main(sys.argv)
