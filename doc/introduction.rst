.. _biogui_introduction:

==============================================
bioGUI: a universal GUI for command line tools
==============================================

Bioinformatics is a highly interdisciplinary field providing tools for researchers from many disciplines.
Nonetheless, most methods are implemented with a command line Interface only.
Non-computer affine colleagues may well interpret results from such tools, but installing and starting tools on the command-Line often is a problem.
Providing a Graphical User-Interface (GUI) for bioinformatics tools is a step towards routinely applying these command line-only tools, and, thus a more effective interdisciplinary work.

*bioGUI* is a universal GUI for command line tools making use of Window's newest feature: WSL (Windows Subsystem for Linux), which provides a *native* Ubuntu bash on Windows.
*bioGUI* templates are easily scriptable and render a GUI for user input from defined visual components elements.
Install modules can install a tool and its template with few clicks from our \emph{bioGUI} repository.
 
Using *bioGUI*, former command line-only tools can be started from a GUI, making them available to a broader scientific community.

Who is bioGUI for?
------------------

With bioGUI, domain experts, who don't want to be bothered with the command Line, are enabled to use high standard bioinformatics tools.
bioGUI specifically aims at Windows users, as Microsoft just introduced the Windows Subsystem for Linux (WSL) with its *Bash on Ubuntu on Windows*.
This system allows the usage of said sophisticated tools on a regular Windows computer as most people have.
With bioGUI one also does not need any knowledge about the command line, because the task of executing a given tools becomes a point & click solution.

What is bioGUI *currently* not?
-------------------------------

It is not about generating a GUI by its own.
*Currently* generating template files is a manual business, which is best performed by a tool's developer.
For the future, integration of automatic command line Interface to *bioGUI* converters are thought of.
However, this could only be realised for a limited number of argument parsers, such as *argparse* for *python*.