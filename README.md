# bioGUI: a universal GUI for CLI applications

With bioGUI developers can easily build a graphical-user-interface (GUI) for their command-line only application.
Such a GUI then enables many more people to use their software.
Especially in biology, the domain experts are often computer novices and have difficulties using tools written for them by bioinformaticiens/computer scientists.

## Who is bioGUI for?

With bioGUI domain experts, which have no clue about the command line, should be enabled to use highly sophisticated tools written for them.
bioGUI specifically aims at Windows users, as Microsoft just introduced the Windows Subsystem for Linux (WSL) with its _Bash on Ubuntu on Windows_.
This system allows the usage of said sophisticated tools on a regular Windows computer as most people have.
With bioGUI one also does not need any knowledge about the command line, because the task of executing a given tools becomes a point & click solution.

### What is bioGUI not?

It is not about generating a GUI by its own.

# About using/building bioGUI
## How to obtain bioGUI?

Currently the only way is to clone this repository and build bioGUI.
It is planned to also distribute full installers/archives to download with binaries built for Windows.

### How to build bioGUI?

Before building bioGUI check that the following dependencies are installed:

	* cmake
	* Qt 5.5+
	* libssl-dev
	* liblua5.3-dev
	
then create a new folder `build` within the git root, cd into it and execute `cmake .. && make && make install`

## How to see which templates exist and how to install new templates?

The _Download Templates_ button in bioGUI lists all available templates and allows also for filtering.
You can also explore all templates at [link](https://services.bio.ifi.lmu.de/biogui).
This is also where you can submit new/modified templates.

After clicking on _Download Templates_, you can select one or multiple templates to download.
These will then be downloaded and saved for you.
Upon exiting the _Download Templates_ dialogue, you will see the templates in bioGUI.
Install templates require the Template Installation Module to be downloaded before.

_README to be continued_

# About Templates

## How to create a new template?

## Which visual elements exist?

## Which execution nodes exist?
