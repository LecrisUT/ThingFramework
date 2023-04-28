import os
import subprocess
import sys

project = 'ThingFramework'
copyright = '2023, Cristian Le'
author = 'Cristian Le'

extensions = [
    "myst_parser",
    "sphinx_design",
    "sphinx_togglebutton",
    "breathe",
]

templates_path = []
exclude_patterns = [
    'build',
    '_build',
    'Thumbs.db',
    '.DS_Store',
    "README.md",
    "CMakeLists.txt",
    "doxygen",
]
source_suffix = [".md"]

pygments_style = "sphinx"
html_css_files = ["breathe.css"]

html_theme = 'furo'
html_static_path = ['_static']

myst_enable_extensions = [
    "tasklist",
    "colon_fence",
]

cmake_build_dir = os.getenv("DOXYGEN_DIR", "../cmake-build-docs/doc/xml")
breathe_projects = {"ThingFramework": cmake_build_dir}
breathe_default_project = "ThingFramework"


def generate_doxygen_xml(app):
    """Run the doxygen make commands"""

    read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'
    if read_the_docs_build:
        os.makedirs("../cmake-build-docs/doc", exist_ok=True)
        try:
            retcode = subprocess.call("doxygen", shell=True)
            if retcode < 0:
                sys.stderr.write(f"doxygen terminated by signal {-retcode}")
        except OSError as e:
            sys.stderr.write(f"doxygen execution failed: {e}")


def setup(app):
    # Add hook for building doxygen xml when needed
    app.connect("builder-inited", generate_doxygen_xml)
