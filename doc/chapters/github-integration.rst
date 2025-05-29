
.. index::
    !single: GitHub

******************
GitHub Integration
******************

Integrating *Erbsland Unit Test* with GitHub Actions allows you to automatically build and test your project on each push or pull request. This ensures that all contributions meet quality standards and pass unit tests before being merged.

Verify CTest Activation
=======================

Ensure that **CTest** is enabled in your CMake project and configured to run the `unittest` target. Add the following lines to your main `CMakeLists.txt`:

.. code-block:: cmake
    :emphasize-lines: 8-13

    cmake_minimum_required(VERSION 3.23)
    project(ExampleUnitTest)

    add_subdirectory(erbsland-unittest)
    add_subdirectory(example-lib)
    add_subdirectory(unittest)

    # Activate CTest
    enable_testing()
    add_test(
        NAME unittest
        COMMAND $<TARGET_FILE:unittest> --no-color
    )

This configuration registers a test named ``unittest``, which executes your unit test binary using ``CTest``. The ``--no-color`` flag ensures clean log output in CI environments like GitHub Actions.

Create the Workflow File
=========================

To enable this integration, create a new workflow file named ``ci.yml`` (or alternatively ``unittest.yml``) inside your repository under the path ``.github/workflows``. This file defines a GitHub Actions workflow that sets up the build environment, compiles your project, and runs the tests.

Here is a complete example of such a workflow:

.. code-block:: yaml

    name: CI

    on:
      push:
        branches: [ main ]
      pull_request:

    jobs:
      build:
        runs-on: ubuntu-latest
        env:
          BUILD_DIR: cmake-build

        steps:
          - uses: actions/checkout@v4
            with:
              submodules: recursive

          - name: Cache CMake build
            uses: actions/cache@v3
            with:
              path: ${{ env.BUILD_DIR }}
              key: ${{ runner.os }}-cmake-${{ hashFiles('**/CMakeLists.txt') }}

          - name: Install apt deps
            run: |
              sudo apt-get update
              sudo apt-get install -y build-essential cmake ninja-build

          - name: Configure
            run: cmake -S . -B ${{ env.BUILD_DIR }} -G Ninja -DCMAKE_BUILD_TYPE=Debug

          - name: Build
            run: cmake --build ${{ env.BUILD_DIR }} --config Debug

          - name: Run tests
            run: |
              cd ${{ env.BUILD_DIR }}
              ctest --output-on-failure

Step-by-Step Explanation
=========================

This workflow is designed to automatically build and test your project using GitHub-hosted runners. Let’s go through each section:

Workflow Trigger
----------------

.. code-block:: yaml

    on:
      push:
        branches: [ main ]
      pull_request:

The workflow is triggered on two events:

* When a commit is pushed to the ``main`` branch.
* When a pull request is opened or updated.

These triggers ensure that both direct changes and proposed changes are verified.

Job Definition
--------------

.. code-block:: yaml

    jobs:
      build:
        runs-on: ubuntu-latest
        env:
          BUILD_DIR: cmake-build

A single job named ``build`` is defined. It runs on the latest Ubuntu runner and uses an environment variable ``BUILD_DIR`` to store build artifacts in a dedicated subdirectory.

Checkout the Repository
-----------------------

.. code-block:: yaml

    - uses: actions/checkout@v4
      with:
        submodules: recursive

This step checks out your source code, including all Git submodules, which are often used for test frameworks or dependencies.

Cache the Build
---------------

.. code-block:: yaml

    - name: Cache CMake build
      uses: actions/cache@v3
      with:
        path: ${{ env.BUILD_DIR }}
        key: ${{ runner.os }}-cmake-${{ hashFiles('**/CMakeLists.txt') }}

To speed up repeated builds, this step caches the ``cmake-build`` directory. The cache is keyed by the operating system and the contents of all ``CMakeLists.txt`` files. If nothing changed in those files, the cached build output can be reused.

Install Dependencies
--------------------

.. code-block:: yaml

    - name: Install apt deps
      run: |
        sudo apt-get update
        sudo apt-get install -y build-essential cmake ninja-build

Installs required packages for building the project using CMake and Ninja. This is typically needed for C++ development environments on CI runners.

Configure the Build
-------------------

.. code-block:: yaml

    - name: Configure
      run: cmake -S . -B ${{ env.BUILD_DIR }} -G Ninja -DCMAKE_BUILD_TYPE=Debug

Runs CMake to configure the project using the Ninja generator. The build type is set to ``Debug`` to include debugging symbols and enable assertions in your unit tests.

Build the Project
-----------------

.. code-block:: yaml

    - name: Build
      run: cmake --build ${{ env.BUILD_DIR }} --config Debug

Builds the project using the previously configured Ninja setup. The ``--config`` flag is mostly relevant for multi-config generators, but including it keeps the setup consistent.

Run the Tests
-------------

.. code-block:: yaml

    - name: Run tests
      run: |
        cd ${{ env.BUILD_DIR }}
        ctest --output-on-failure

Navigates to the build directory and runs your test suite using ``ctest``. The ``--output-on-failure`` flag ensures that, if any test fails, the output is shown in the GitHub Actions log for easy debugging.

Final Outcome
=============

With this setup in place, your unit tests will automatically run every time:

* a new commit is pushed to the ``main`` branch, or
* a pull request is opened or updated.

This ensures continuous validation of your codebase, helping to catch regressions early and maintain high quality standards throughout development.

