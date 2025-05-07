# -- Project information -----------------------------------------------------
import sys
from pathlib import Path

project = "Erbsland Unittest"
copyright = "2023-2025, Erbsland DEV"
author = "Erbsland DEV"
release = "1.0"

# -- General configuration ---------------------------------------------------
extensions = ["sphinx_rtd_theme", "sphinx_design", "breathe"]
templates_path = ["_templates"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

# -- Options for HTML output -------------------------------------------------
html_theme = "sphinx_rtd_theme"
html_static_path = ["_static"]

# -- Options for Breathe -----------------------------------------------------
_project_dir = Path(__file__).parent.parent
_src_dir = _project_dir / "src"
breathe_projects = {"erbsland-unittest": _project_dir / "_build/breathe/doxygen/erbsland-unittest/xml"}
breathe_default_project = "erbsland-unittest"
breathe_projects_source = {"erbsland-unittest": (_src_dir, ["erbsland/unittest", "erbsland/unittest/impl/Macros.hpp"])}
breathe_doxygen_config_options = {
    "STRIP_FROM_PATH": _src_dir,
    "STRIP_FROM_INC_PATH": _src_dir,
}
