*********
Changelog
*********

Version 1.8.0
=============

*   Changed the license from LGPL-3.0 to the less restrictive Apache-2.0.
*   Updated the code style to align with the latest Erbsland Code guidelines.
*   Hardened the build process and GitHub workflows.
*   Introduced build utilities with a ``pre_commit`` action.
*   Minor fixes, such as explicitly converting ``std::ptrdiff_t`` to ``std::size_t``.
*   Removed unused dependencies from the documentation build.
*   Reorganized the directory structure, moved Cmake related files to a new `cmake` directory.
*   Removed `tools`, it is replaced by `utilities` and `cmake`.
*   Updated all dependencies to the latest versions.
