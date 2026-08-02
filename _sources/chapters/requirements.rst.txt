************
Requirements
************

This page lists the software and tool requirements for using, building, and maintaining the library.  
Most users only need the basic development requirements to write and run unit tests.

Usage
=====

Code-Level Requirements
-----------------------

To use the library in your project, you need the following:

* A C++ compiler with support for **C++20** or newer.
* A standard C++ library implementation that supports the required C++20 features.
* For comparison macros such as ``REQUIRE_EQUAL``, your standard library must provide support for ``std::format``.

Build and Metadata System Requirements
--------------------------------------

The following tools are recommended for a smooth integration with the provided CMake build system:

* **CMake** version 3.25 or newer.
* **Python** version 3.11 or newer for metadata compilation and related build utilities.

Library Development and Maintenance
===================================

If you only want to write and run unit tests, you can safely ignore the requirements in this section.

Running the Maintenance Utilities
---------------------------------

The maintenance utilities are used for tasks such as formatting, validation, and pre-commit checks.

Required tools:

* **Python** 3.14 or newer.
* **black** for Python code formatting.
* **erbsland-elcl** for parsing Erbsland configuration files in Python.

Building the Documentation
--------------------------

To build the full project documentation locally, install the following tools:

* **doxygen** for extracting API documentation from the source code.
* **Sphinx** for generating the documentation site.
* **breathe** to integrate the generated Doxygen documentation into Sphinx.
* **sphinx-rtd-theme** as the documentation theme.
* **sphinx_design** for additional layout and design elements used throughout the documentation.
