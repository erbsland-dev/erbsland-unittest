
Maintenance Tools
=================

All utilities in this folder are only required for active development and maintenance of this project.
They are not used if you use this library to write unit tests.

Setup and Usage
---------------

If you need to work on this library, enable the virtual environment and run the utilities as follows:

```
python3 -mvenv .venv
.venv/bin/activate
pip install -r utilities/requirements-dev.in
python3 utilities/run.py <utility_name> [<utility_arguments>]
```

Directory Structure
-------------------

- `run.py`: Script runner that 
- `requirements-dev.in`: The requirements for development and testing, not pinned to versions.
- `docs`: Includes tools for generating documentation.
- `lint`: Holds scripts for linting and formatting code.
- `test`: Contains utilities for running tests and test-related tasks.