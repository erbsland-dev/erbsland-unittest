
.. index::
    !single: Namespaces
    !single: erbsland
    !single: erbsland::unittest
    !single: el (Shortcut)
    single: Namespaces; erbsland
    single: Namespaces; erbsland::unittest
    single: Namespaces; el (Shortcut)
    single: Namespace Shortcuts

Namespaces
==========

As this library is part of a large framework, it's nesting of namespaces may look unnecessary complicated. If using in context of larger applications, these additional levels of namespaces provide flexibility and prevent naming conflicts.

The *Erbsland Unittest* interface is placed in the namespace ``erbsland::unittest``:

.. doxygennamespace:: erbsland
    :desc-only:

.. doxygennamespace:: erbsland::unittest
    :desc-only:

Shortcuts
---------

To simplify your code, a shortcut for the base namespace is automatically created by default.

.. code-block:: cpp

    namespace el = erbsland;

We recommended that you use either use this shortcut to name the types from the library, or write ``using`` statements, importing the types you use into the namespace of your application.

.. code-block:: cpp
    :caption: Using the namespace shortcut ``el`` to use the ``UnitTest`` class.

    #include <erbsland/unittest/UnitTest.hpp>

    class ExampleTest : public el::UnitTest {
        // ...
    }

.. code-block:: cpp
    :caption: Importing the ``UnitTest`` class, with the ``using`` statement.

    #include <erbsland/unittest/UnitTest.hpp>

    using el::UnitTest; // or using erbsland::UnitTest

    class ExampleTest : public UnitTest {
        // ...
    }

Prevent or Redefine the Shortcuts
---------------------------------

As the shortcuts can conflict with existing codes, there are several macro definitions that allow you to disable the shortcuts or redefine them. You will have to set these definition on the compile level for the whole project.

.. c:macro:: ERBSLAND_NO_SHORT_NAMESPACE

    If you set this preprocessor directive, no namespace shortcuts are created by default. You have to set this directive on the compile level for the whole project.

.. c:macro:: ERBSLAND_SHORT_NAMESPACE

    Set this preprocessor directive to overwrite the name that is used for the shortcut for the ``erbsland`` namespace. You have to set this directive on the compile level for the whole project.

