README file for Plato (Cognitive Engine Library)
************************************************
Version: 0.1.0.0, 24th September 2008.

Please read the NEWS file if you are upgrading from a previous release.

Please read the INSTALL file for instructions on building and installing the library.

The contents of this README file are:
1)  Overview of Plato Library.
2)  Documentation for Plato.


Overview of Plato Library
==========================
The purpose of Plato is to provide a cognitive engine library
for instilling the benefits of minds in control devices, such as a robotic room cleaner.
Mind models have a hierarchical structure and are specified in C++.
For example, see the model specified in "PrimaryMindModel.cpp".
An overview of modelling in Plato is given in the documentation.

Plato is written in C++, with only a few non-standard dependencies, such as hash_maps.

The library is split into two main parts:

Standard
--------
The classes that make up the essential characteristics of the library.

Components
----------
Extensions to the standard classes, such as models for particular kinds of cognitive features.

Models
======
Various example models are included with the Plato distribution,
which present explorations into the structure of minds based on causal systems.

HelloWorldModel
---------------
The classic example, simply writes 'Hello World' to the console. Not really a mind!

Documentation for Plato
=======================
If Plato is installed in the normal way, documentation is provided as a collection of html files.
The documentation is built separately via the Bash script: builddocs.bash.
The resulting index.html file is in the root html directory at Plato/Documentation/tmp/html.
Doxygen is required to produced the library class documentation. 
A help file compiler is required if a single help file is required, such as Microsoft's hhc.exe.

+++
