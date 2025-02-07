.. _cmake-configuration:

The CMake Configuration
=======================

Configuring the Root Project CMake File
---------------------------------------

To link all necessary components and prepare your project for building, create the root CMake configuration file. Follow these steps:

1. Open a terminal and create the file:

   .. code-block:: bash

       nano CMakeLists.txt

2. Add the following content to configure the project:

   .. code-block:: cmake

       cmake_minimum_required(VERSION 3.20)
       project(ExampleUnitTest)
       add_subdirectory(erbsland-unittest)
       add_subdirectory(example-lib)
       add_subdirectory(unittest)

This CMake configuration:

* **Defines the Minimum CMake Version**: Ensures compatibility with CMake 3.20 or newer.
* **Sets the Project Name**: Defines the project as ``ExampleUnitTest``.
* **Includes Subdirectories**:

  - :file:`erbsland-unittest`: The Erbsland UnitTest framework.
  - :file:`example-lib`: The application or library being tested.
  - :file:`unittest`: The unit test binaries and supporting files.

With this setup, your project is structured and ready to build and run unit tests seamlessly.

Setting Up the Unit Test CMake File
-----------------------------------

To configure a unit test executable within the recommended project structure, follow these steps:

1. Create a directory for your unit test files:

    .. code-block:: bash

        mkdir unittest
        cd unittest
        nano CMakeLists.txt

2. Use the following content for your ``CMakeLists.txt`` file:

    .. code-block:: cmake

        cmake_minimum_required(VERSION 3.20)
        project(unittest)
        add_executable(unittest
               src/main.cpp)
        target_compile_features(unittest PRIVATE cxx_std_17)
        target_link_libraries(unittest PRIVATE erbsland-unittest-example-lib)
        target_include_directories(unittest PRIVATE ../example-lib/src)
        erbsland_unittest(TARGET unittest)

Explanation of the Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The unit test :file:`CMakeLists.txt` file accomplishes the following:

1. **Defines the Minimum CMake Version**:

    - Ensures compatibility with CMake 3.20 or newer.

2. **Creates the Unit Test Executable**:

    - The target executable is named :file:`unittest`.
    - Includes :file:`src/main.cpp` as the entry point for the unit test.

3. **Specifies the C++ Standard**:

    - Configures the project to use C++17 with ``target_compile_features``.

4. **Links the Unit Test Library**:

    - Connects the :file:`unittest` executable to the example library (:file:`erbsland-unittest-example-lib`) using ``target_link_libraries``.

5. **Adds Include Directories**:

    - Ensures the unit test can access header files from the library's source directory using ``target_include_directories``.

6. **Integrates Erbsland UnitTest**:

    - The ``erbsland_unittest`` function configures the target to link the required unit test library and automatically sets up the necessary include paths.
    - This function also creates an auxiliary target, ``unittestMetaGen``, to handle additional code generation required for the unit test.

With these configurations, your unit test project is ready to build and execute, leveraging the full functionality of the Erbsland UnitTest framework.

.. button-ref:: write-the-test
    :color: success
    :align: center
    :expand:
    :class: sd-fs-5 sd-font-weight-bold sd-p-2

    Write the Test! →

