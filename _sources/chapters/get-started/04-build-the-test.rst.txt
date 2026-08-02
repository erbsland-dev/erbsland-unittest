
.. _build-the-test:

Build the Test!
===============

Compiling your unit test requires generating build files with CMake and running the build process. Follow these steps:

Generate Build Files with CMake
-------------------------------

1. Open a terminal and navigate to your home directory:

   .. code-block:: bash

       cd ~
       mkdir cmake-build
       cd cmake-build

2. Generate the build files for your project using CMake:

   .. code-block:: bash

       cmake ~/erbsland-unittest-example

3. If everything is configured correctly, you will see output similar to the following:

   .. code-block:: none

       -- The C compiler identification is (...)
       -- The CXX compiler identification is (...)
       -- Detecting C compiler ABI info
       (...)
       -- Found Python3: (...) (found suitable version "3.11.2", minimum required is "3.11") found components: Interpreter
       -- Configuring done
       -- Generating done
       -- Build files have been written to: ~/erbsland-unittest-example

This output confirms that CMake has successfully created the build files for your project.

Compile the Unit Test
---------------------

4. Compile the test executable by running the `make` command in the build directory:

   .. code-block:: bash

       make

5. If the build process completes successfully, you will see an output like this:

   .. code-block:: none

       (...)
       [100%] Linking CXX executable unittest
       [100%] Built target unittest

You’ve successfully built your unit test executable! Next, let’s run the test to verify everything works as expected.

.. button-ref:: run-the-test
    :color: success
    :align: center
    :expand:
    :class: sd-fs-5 sd-font-weight-bold sd-p-2

    Run the Test! →

.. card:: Build Troubleshooting Tips

    - **Missing Dependencies**: Ensure all required libraries, such as `erbsland-unittest`, are initialized as submodules and included in the project.
    - **CMake Version**: Verify that your installed CMake version meets the minimum required version (3.23 or newer).
    - **Compiler Issues**: Check that your C++ compiler is correctly configured and supports C++20.
