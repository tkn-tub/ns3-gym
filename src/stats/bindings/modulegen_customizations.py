import os

def post_register_types(root_module):
    enabled_features = os.environ['NS3_ENABLED_FEATURES'].split(',')

    # if no sqlite, the class SqliteDataOutput is disabled
    if 'SqliteDataOutput' not in enabled_features:
        try:
            root_module.classes.remove(root_module['ns3::SqliteDataOutput'])
        except KeyError:
            pass

