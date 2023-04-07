import os

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
