# -*- Mode:Python; -*-

import ns3 as ns


class MyModel(object):

    def Start(self):
        ns.Simulator.Schedule(ns.Seconds(10.0), self.DealWithEvent, ns.Simulator.Now().GetSeconds())

    def DealWithEvent(self, value):
        print "Member method received event at ", ns.Simulator.Now().GetSeconds(), \
            "s started at ", value, "s"

def random_function(model):
    print "random function received event at ", ns.Simulator.Now().GetSeconds(), "s"
    model.Start()


def main(dummy_argv):
    model = MyModel()
    ns.Simulator.Schedule(ns.Seconds(10.0), random_function, model)
    ns.Simulator.Run()
    ns.Simulator.Destroy()

if __name__ == '__main__':
    import sys
    main(sys.argv)
