# &lt;bioGUI: a universal GUI for CLI applications

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
You can also explore all templates at [our website](https://services.bio.ifi.lmu.de/biogui).
This is also where you can submit new/modified templates.

After clicking on _Download Templates_, you can select one or multiple templates to download.
These will then be downloaded and saved for you.
Upon exiting the _Download Templates_ dialogue, you will see the templates in bioGUI.
Install templates require the Template Installation Module to be downloaded before.

_README to be continued_

# About Templates

Templates are structued _XML_ files.

Each template is defined as a root tag with the following structure:
`&lt;template description="a descriptive text" title="a title">`

Where the description is shown in the description line in bioGUI and the title is shown in the title line in bioGUI.

If the child element `window` does not have a title set, the template title is also used as window title.

Within the template tag two child elements are expected:

	* `&lt;window width="..." height="..." title="...">`
	* `&lt;execution>`

The window may have additional attributes such as _width_ and _height_ defining the minimal width/height of the window.
The title is shown in the title of the window.

The _window_ tag (and its children) define the **visual** appearance of a template while the _execution_ part defines the **logical** part.

Once starting a program, the _execution_ part can be used to assemble the command line arguments for the application to start.
For this a _network_-like structure is used.
Each element in the execution part can be seen as a node.
A node can refer its value from another node, but it can also manipulate or extend values (e.g. by combining multiple nodes in an `&lt;add>`-node).

For highest flexibility, `&lt;script>` nodes can refer to or contain **LUA** code. For instance

``` XML
&lt;const id="node1">some_file.tex&lt;/const>

&lt;script argv="${node1},pdf">
&lt;![CDATA[

function evaluate(arg1, arg2)
	return(string.sub(arg1, 0, -3) .. arg2)
end


]]>
&lt;/script>
```

would first split all supplied arguments from the script _argv_ attribute and resolve those, which refer to another node (indicated by `${nodeid}`).
In this case, the node with id _node1_ is a constant value of _some\_file.tex_. The second argument is also constant text (_pdf_).
Therefore the inline script would be called as `evaluate(some\_file.tex, pdf)`.
The return value is thus _some\_file.pdf_.

## How to create a new template?

In order to create a new template, simply setup a new text file with _gui_ extension (or _igui_ if it's an install template).

Create and set up the visual part of the GUI and write the logic part to assemble the command line arguments.

If you want to share your template, feel free to upload to [our website](https://services.bio.ifi.lmu.de/biogui). 

## Which visual elements exist?

Visual elements including their possible attributes are given below.
It is distinguished between Layouts and Widgets.
Layouts tell how the widgets are rendered/displayed (e.g. in which order).
There currently exist 3 layouts (grid, horizontal and vertical).

|node name|allowed attributes|
|---|---|
|&lt;GRID>|	[cols, rows]|
|&lt;HGROUP>|	[]|
|&lt;VGROUP>|	[]|
|---|---|
|&lt;ACTION>|	[program]|
|&lt;CHECKBOX>|	[selected, selectonwindows, value]|
|&lt;COMBOBOX>|	[selected]|
|&lt;COMBOITEM>|	[value]|
|&lt;FILEDIALOG>|	[filter, folder, location, multiples, multiples_delim, output]|
|&lt;FILELIST>|	[height, title, width]|
|&lt;FILESELECTBOX>|	[delim, filter, location]|
|&lt;GROUP>|	[height, title, width]|
|&lt;GROUPBOX>|	[multi]|
|&lt;IMAGE>|	[height, src, width]|
|&lt;INPUT>|	[multi, type {string, int, float, password}]|
|&lt;LABEL>|	[link]|
|&lt;RADIOBUTTON>|	[value]|
|&lt;SLIDER>|	[max, min, step]|
|&lt;SLIDERITEM>|	[display, value]|
|&lt;STREAM>|	[height, title, width]|
|&lt;STREAMBOX>|	[]|
|&lt;WINDOW>|	[height, title, width]|


## Which execution nodes exist?

Execution nodes define the logic and how the command line arguments are assembled.
Most likely the most important nodes are the add, const, value and if (and else) nodes.

However, it must be stressed, that the __script__ node is of high power as it enables the execution of _lua_ scripts (even inline written in the template).
Program arguments are provided by the _argv_ attribute (see the above example).

|node name|allowed attributes|
|---|---|
|&lt;add>|	[ID, TYPE, sep]|
|&lt;const>|	[ID, TYPE]|
|&lt;else>|	[]|
|&lt;env>|	[GET, ID, TYPE]|
|&lt;execute>|	[EXEC, ID, PROGRAM, TYPE, location, param, program, wsl]|
|&lt;file>|	[FROM, ID, SEP, TO, TYPE]|
|&lt;httpexecute>|	[CL_TO_POST, DELIM, ID, PORT, PROGRAM, TYPE]|
|&lt;if>|	[COMP, ID, SEP, TYPE, VALUE1, VALUE2]|
|&lt;math>|	[ID, OP, TYPE]|
|&lt;orderedadd>|	[FROM, ID, SELECTED, TYPE]|
|&lt;output>|	[COLOR, DEFERRED, FROM, HOST, ID, LOCATION, PORT, TO, TYPE, TYPE]|
|&lt;relocate>|	[FROM, ID, PREPEND, TO, TYPE, UNIX, WSL]|
|&lt;replace>|	[ID, REPLACE, REPLACE_WITH, TYPE]|
|&lt;script>|	[ARGV, ID, SCRIPT, TYPE]|
|&lt;value>|	[FOR, FROM, ID, TYPE]|

