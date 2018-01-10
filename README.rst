dayplan
=======

*dayplan* is a command line tool that supports task planning and work logging 
on a daily basis. It should help with planning a work day and enable the user
to always answer the three questions:

- What tasks should be accomplished today?
- What tasks were accomplished today?
- How much time was spent working on a task?

The user maintains text files containing information about tasks and work logs,
*dayplan* creates reports based on those files. This basic workflow is heavily 
inspired by `ledger <https://github.com/ledger/ledger>`_.

Quick start
-----------

*dayplan* reads all its data from plain text files. Those files consist of
lines with dates in ISO format, each followed by indented lines describing 
tasks and work logs.

We start with a file containing the date line ``2018-01-01`` followed by three
tasks. The date line must not contain any trailing spaces. Every task line must
be indented and consists of a task identifier (like ``#1``) and a task name:

.. code-block:: text

   2018-01-01
     #1    Fix segfault
     #2    Call Stanislaus
     #3    Review main.c
     08:00

Assuming the file was saved as ``work.dat``, we can let *dayplan* process it
and list all tasks in a handy format:

.. code-block:: bash

    $ dayplan -f work.dat tasks --oneline
    (open) #1 Fix segfault
    (open) #2 Call Stanislaus
    (open) #3 Review main.c
    $

This output shows all tasks with their ids, names and states. All tasks are
open - let's change this by adding a work log that closes a task:

.. code-block:: text

   2018-01-01
     #1    Fix segfault
     #2    Call Stanislaus
     #3    Review main.c
     08:00 Close #2
     08:25

Now the same command shows that the tasks with the id ``#2`` is closed:

.. code-block:: bash

    $ dayplan -f work.dat tasks --oneline
    (done) #2 Call Stanislaus
    (open) #1 Fix segfault
    (open) #3 Review main.c
    $

The command can be extended to additionally show details about the work logs:

.. code-block:: bash

    $ dayplan -f work.dat tasks --oneline
    Mon Jan  1 08:00:00 2018  0h 25m  (done) #2 Close #2
    (done) #2 Call Stanislaus
    (open) #1 Fix segfault
    (open) #3 Review main.c
    $

This output helps to answer the three questions mentioned above. One sees

- what tasks should be accomplished today,
- what tasks were accompished today and
- how much time was spent working on a task.


Building
--------

At the moment it is only possible to build *dayplan* on Linux. *dayplan* uses
CMake, so from the root directory of the source tree it can be built via the
following commands:

.. code-block:: bash

    $ mkdir build
    $ cd build
    $ cmake .. && make dayplan

If you want to run the full test suite, additionally run `make test`.

Dependencies
------------

In addition to a C compiler supporting C99, the following dependencies are
needed:

+-----------------------------+--------+
| CMake                       | >= 3.5 |
+-----------------------------+--------+
| Bison                       | >= 3.0 |
+-----------------------------+--------+
| pytest (for the test suite) | >= 2.7 |
+-----------------------------+--------+
