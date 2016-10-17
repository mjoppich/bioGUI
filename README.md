# bioGUI: a universal GUI for CLI applications

With bioGUI developers can easily build a graphical-user-interface (GUI) for their command-line only application.
Such a GUI then enables many more people to use their software.
Especially in biology, the domain experts are often computer novices and have difficulties using tools written for them by bioinformaticiens/computer scientists.

## Who is bioGUI for?

With bioGUI domain experts, which have no clue about the command line, should be enabled to use highly sophisticated tools written for them.
bioGUI specifically aims at Windows users, as Microsoft just introduced the Windows Subsystem for Linux with its Bash on Ubuntu on Windows.
This system allows the usage of said sophisticated tools on a regular Windows computer as most people have.
With bioGUI one also does not need any knowledge about the command line, because the task of executing a given tools becomes a point & click solution.

### What is bioGUI not?

It is not about generating a GUI by its own.

## How to obtain bioGUI?

Currently the only way is to clone this repository and build bioGUI.
It is planned to also distribute full installers/archives to download with binaries built for Windows.

### How to build bioGUI?

Before building bioGUI check that the following dependencies are installed:

	* cmake
	* libssl-dev
	* Qt 5.5+
	
then create a new folder `build` within the git root, cd into it and execute `cmake .. && make && make install`

## How to create and submit new templates?

New templates can be submitted via [link](https://services.bio.ifi.lmu.de/biogui).

### How to obtain new templates?

New templates can be obtained directly from the bioGUI app by clicking the _Download Templates_ button.