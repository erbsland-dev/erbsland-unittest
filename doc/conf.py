# -- Project information -----------------------------------------------------
from pathlib import Path

project = 'Erbsland Unittest'
copyright = '2023-2024, Erbsland DEV'
author = 'Erbsland DEV'
release = '1.0'

# -- General configuration ---------------------------------------------------
extensions = ['sphinx_rtd_theme', 'breathe']
templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------
html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

# -- Options for Breathe -----------------------------------------------------
_project_dir = Path(__file__).parent.parent
_src_dir = _project_dir / 'src'
breathe_projects = {"erbsland-unittest": str(_project_dir / '_build/breathe/doxygen/erbsland-unittest/xml')}
breathe_default_project = "erbsland-unittest"
breathe_projects_source = {
    "erbsland-unittest": (str(_src_dir), ['erbsland/unittest', 'erbsland/unittest/impl/macros.hpp'])
}
breathe_doxygen_config_options = {
    'STRIP_FROM_PATH': str(_src_dir),
    'STRIP_FROM_INC_PATH': str(_src_dir),
}

