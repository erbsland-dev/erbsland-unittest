.. _project-structure:

The Project Structure
=====================

Organizing Your Project for Success
-----------------------------------

To streamline integration and maintainability, Erbsland Unit Test is designed to be included as a **Git submodule** within your project. We recommend the following directory structure for a sample project called ``erbsland-unittest-example``:

.. code-block:: none

    erbsland-unittest-example
        ├── <libraries>         # All dependent libraries for the `Example` project (submodules)
        ├── erbsland-unittest   # The Erbsland Unit Test library added as a submodule
        ├── example-lib         # The "Example" project being tested (also added as a submodule)
        │   ├── src (...)
        │   └── CMakeLists.txt
        ├── unittest            # The actual unit tests, containing one or more test binaries
        │   ├── src             # Source files for the unit tests
        │   │   ├── main.cpp           # Main entry point for the unit test executable
        │   │   ├── BasicTest.cpp      # Each test set resides in a file ending with ...Test.cpp
        │   │   ├── ContextTest.cpp    # Each file contains a test class matching the file name
        │   │   └── LongTest.cpp
        │   ├── data            # Supporting data for unit tests
        │   └── CMakeLists.txt
        └── CMakeLists.txt      # The root project's CMake configuration file

This structure ensures your project is:

* **Well-Organized**: Clear separation of libraries, application code, and tests.
* **Easy to Navigate**: Simplifies locating files and understanding dependencies.
* **Straightforward to Manage**: Facilitates efficient integration and testing of libraries and applications.

Create the Structure
--------------------

To get started, create the recommended directory structure and initialize the necessary submodules:

.. code-block:: bash

    cd ~
    mkdir erbsland-unittest-example
    cd erbsland-unittest-example
    git init
    git submodule add https://github.com/erbsland-dev/erbsland-unittest.git erbsland-unittest
    git submodule init erbsland-unittest

Next, add the application or library you wish to test, along with any dependencies, as submodules to the project:

.. code-block:: bash

    git submodule add https://github.com/erbsland-dev/erbsland-unittest-example-lib.git example-lib
    git submodule init example-lib

By including each component as a submodule, you ensure that everything is version-controlled, organized, and manageable within a single repository.

.. card:: Why Use This Structure?

    - **Modularity**: Each part of your project is modular, promoting clean and scalable development.
    - **Version Control**: Submodules allow you to track specific versions of dependencies, ensuring consistency.
    - **Ease of Maintenance**: Makes it easy to update libraries and track changes over time.

.. button-ref:: cmake-configuration
    :color: success
    :align: center
    :expand:
    :class: sd-fs-5 sd-font-weight-bold sd-p-2

    Proceed to the CMake Configuration →

.. card:: About the Command-Line Examples

    Throughout this guide, command-line examples demonstrate essential setup tasks such as:

    - Adding submodules using the ``git`` command.
    - Creating directories with ``mkdir``.
    - Editing files using a text editor like ``nano``.

    While these examples use the command-line interface for accessibility and clarity, you are free to use your preferred tools and workflows to achieve the same results.
