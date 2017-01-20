.. bioGUI documentation master file, created by
   sphinx-quickstart on Wed Jan 11 11:08:00 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

********************
bioGUI documentation
********************

bioGUI: a universal GUI for CLI applications
============================================

With bioGUI developers can easily build a graphical-user-interface (GUI) for their command-line only application.
Such a GUI then enables many more people to use their software.
Especially in biology, the domain experts are often computer novices and have difficulties using tools written for them by bioinformaticiens/computer scientists.

Who is bioGUI for?
==================

With bioGUI domain experts, which have no clue about the command line, should be enabled to use highly sophisticated tools written for them.
bioGUI specifically aims at Windows users, as Microsoft just introduced the Windows Subsystem for Linux (WSL) with its *Bash on Ubuntu on Windows*.
This system allows the usage of said sophisticated tools on a regular Windows computer as most people have.
With bioGUI one also does not need any knowledge about the command line, because the task of executing a given tools becomes a point & click solution.

What is bioGUI not?
=======================

It is not about generating a GUI by its own.

How to get bioGUI?
==================

Download bioGUI releases from the `github releases <http://github.com/mjoppich/bioGUI/>`_.
Please note additional information provided in our :ref:`user_guide`.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   user_guide.rst
   build_wsl.rst
   developer_guide.rst
