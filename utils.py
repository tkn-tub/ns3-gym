#! /usr/bin/env python

import os
import sys

def get_list_from_file(file_path, list_name):
    '''Looks for a Python list called list_name in the file specified
    by file_path and returns it.

    If the file or list name aren't found, this function will return
    an empty list.

    '''

    list = []

    # Read in the file if it exists.
    if os.path.exists(file_path):
        file_in = open(file_path, "r")

        # Look for the list.
        list_string = ""
        parsing_multiline_list = False
        for line in file_in:
            if list_name in line or parsing_multiline_list:
                list_string += line

                # Handle multiline lists.
                if ']' not in list_string:
                    parsing_multiline_list = True
                else:
                    # Evaluate the list once its end is reached.
                    # Make the split function only split it once.
                    list = eval(list_string.split('=', 1)[1].strip())
                    break

        # Close the file
        file_in.close()

    return list


def read_config_file():
    '''Reads the NS-3 configuration file and returns a list of enabled modules.

    This function first looks for the ns3 configuration file (.ns3rc)
    in the current working directory and then looks in the ~
    directory.

    '''

    # See if the ns3 configuration file exists in the current working
    # directory and then look for it in the ~ directory.
    dot_ns3rc_name = '.ns3rc'
    dot_ns3rc_path = dot_ns3rc_name
    if not os.path.exists(dot_ns3rc_path):
        dot_ns3rc_path = os.path.expanduser('~/') + dot_ns3rc_name
        if not os.path.exists(dot_ns3rc_path):
            # Enable all modules if the .ns3rc file can't be found.
            return ['all_modules']

    # Read in the ns3 configuration file.
    modules_enabled = get_list_from_file(dot_ns3rc_path, 'modules_enabled')
    if not modules_enabled:
        # Enable all modules if the modules_enabled line can't be found.
        return ['all_modules']

    return modules_enabled
