# System imports
import logging
import yaml
import json
import os.path as path
from jinja2 import Template
from sys import argv, exit

# Local imports
import generator_functions

logging.basicConfig(level=logging.INFO)

def _parse_config(filename):
    with open(filename, "r") as f:
        try:
            return yaml.safe_load(f)
        except Exception as e:
            logging.error("Could not load file {}: {}".format(filename, e))
            return None


def _read_source(config_path, config):
    if 'name' in config:
        source_filename = path.join(path.dirname(config_path), config['name'])
    else:
        source_filename = config_filename.replace('.yml', '.c')

    with open(source_filename, 'r') as source_file:
        return source_file.read()


def _generate_phase_source_from_template(source, config):
    template = Template(source)

    # Build a dictionary that maps all function names in generator_functions to
    # the actual functions, e.g. 'simple_int_range': simple_int_range. All of
    # these functions will then be available in the phase templates.
    functions = {}
    for name in dir(generator_functions):
        attr = getattr(generator_functions, name)
        if callable(attr) and attr.__module__ == 'generator_functions':
            functions[name] = attr

    return template.render(functions)


def generate_phase(config_path):
    config = _parse_config(config_path)
    if config is None:
        exit(1)

    source_template = _read_source(config_path, config)

    return _generate_phase_source_from_template(source_template, config)


def main():
    logging.info("Starting phase generation")
    if len(argv) == 3:
        generator_functions.static_value_map = json.loads(argv[2])
    elif len(argv) != 2:
        print("Invalid arguments. Please specify a phase configuration file")
        exit(1)


    config_path = argv[1]
    print(generate_phase(config_path))

if __name__ == '__main__':
    main()
