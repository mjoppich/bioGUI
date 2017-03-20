########################
bioGUI Nodes Description
########################

************************
Visual Model Nodes
************************

<GROUP> node
============

.. code:: XML

    <group ordered="true" id="orderedgroup" title="Step options">
    <grid rows="1" cols="2">
        
        <group id="slidingwindow_opt" selected="false" title="Sliding Window" checkable="true">
            <label>Size</label>
            <input id="slidingwindow_size" type="int"/>
            <label>Quality</label>
            <input id="slidingwindow_quality" type="int"/>
        </group>

        <group id="leadingwindow_opt" selected="false" title="Leading Window" checkable="true">
            <hgroup>
                <label>Quality</label>
                <input id="leadingwindow_quality" type="int"/>
            </hgroup>
        </group>

    </grid>
    </group>

.. code:: XML

    <orderedadd id="steps" from="orderedgroup" selected="true">
        <value from="slidingwindow" for="slidingwindow_opt"/>
        <value from="leading" for="leadingwindow_opt"/>
    </orderedadd>

Each group node can have its own layout, which must be the first and only child of a group node.
An *ordered* group node will give its children (visual model nodes) an order, so the user can select in which order something is taken.
For this to function, the values must be retrieved using the ``orderedadd`` node in the exeuction model.
The *for* attribute must be the id of an element within the ordered group. And the *from* attribute must be the attribute of an exeuction node which contains the value to be written at this position.

A group may also be *checkable*, which means that it has a checkbox. It will return "true" if checked, "false" otherwise.
Setting ``checked_value`` or ``unchecked_value``, respectively, allows custom values.
Setting ``selected="true"`` will make it checked right from the beginning.

If ``exclusive="true"`` is set as attribute, only one child may be selected at a time.

<COMBOBOX> node
===============

A combobox is *checkable*, which means that it has a checkbox. It will return "true" if checked, "false" otherwise.
Setting ``checked_value`` or ``unchecked_value``, respectively, allows custom values.
Setting ``selected="true"`` will make it checked right from the beginning. 


<STREAMBOX>/<STREAM> node
=========================

The ``STREAMBOX`` is the default output box, where standard out could be written to.
In order to function, a streambox must have at least one ``STREAM`` child with an ID.
This stream is then connected with one or multiple `<EXECUTE>/<ACTION> node`_ .

.. code:: XML

    <streambox id="output1">
        <stream id="outputstream1">std out</stream>
        <stream id="outputstream2">err out</stream>
    </streambox>


************************
Layout Nodes
************************

<HGROUP>/<VGROUP> node
======================

.. code:: XML

    <vgroup>

        <label>First:</label>
        <label>Second: under first</label>

    </vgroup>

<GRID> node
===========

The *GRID* node layouts its children visual model nodes in a grid. If only rows or columns are specified, the other value is calculated from the number of children.

.. code:: XML

    <grid rows="2" cols="2">

        <label>Top Left</label>
        <label>Top Right</label>

        <label>Bottom Left</label>
        <label>Bottom Right</label>

    </grid>


************************
Execution Model Nodes
************************

<RELOCATE> node
===============

The relocate node probably is the most useful node of all, at least for templates aiming at Windows Subsystem for Linux/Bash on Ubuntu on Windows enabled templates.
If the ``relocate`` node is used as below, the *windows_location* is transformed from a Windows path, to a UNIX path if the value of the node with ID ``WSLsel`` is ``true``.
For example, ``C:\files\sample.dat`` is transformed into ``/mnt/c/files/sample.dat``.

.. code:: XML

	<checkbox id="WSLsel" value="true" selectonwindows="true">run in WSL?</checkbox>
    <relocate id="wsl_location" wsl="${WSLsel}" from="${windows_location}"/>

Apart from the WSL use-case, the reloacte node can also be used manually, e.g. to change pathes on a remote server.


.. code:: XML

    <relocate from="" to=""/>

<IF>/<ELSE> node
================

Using ``if`` nodes, condition specific evaluation of nodes can be performed.
For instance, depending on the state of a checkbox, either one or the other file can be taken as input.
An ``if`` node accepts three *comp*arison modes: ``is_set``, ``equals`` or ``EQUALS``.
The first mode evaluates true, if the node reference in attribute *value1* evaluates to any value which is not empty.
The other two modes compare the node referance output of *value1* and *value2* and evaluate the ``if`` part if *value1* and *value2* are equal (``equals``, case sensitive) or are non-case-sensitive equal (``EQUALS``).
If the comparison does not return, the ``else`` part 

.. code:: XML

    <if value1="id_elem1" comp="is_set">
        <value from="inputfile_1"/>
    <else>
        <value from="inputfile_2"/>
    </else>
    </if>


<VALUE> node
============

The ``value`` node collects the *value* from the node with the id given in the *from* attribute. If no such node exists, the value is interpreted as *text*. However, the ``const`` node may be more suitable here.

.. code:: XML

    <value from="inputfile_1"/>


<ORDEREDADD> node
=================

See `<GROUP> node`_ .

<ENV> node
==========

The ``env`` (environment) node returns several system properties, such as IP addresses, the current OS, etc. .
If asked for a specific OS, the node may return ``true`` or ``false``.
The ``DATADIR`` returns the path to where applications are stored, such as applications installed via WSL.

.. code:: XML

    <env id="envip" get="IP"/>
    <env id="..." get="IP|IPv4|IPv6|LINUX|UNIX|MAC|WIN|DATADIR"/>


<SCRIPT> node
=============

For highest flexibility, `<script>` nodes can refer to or contain **LUA** code. For instance

.. code:: XML

    <const id="node1">some_file.tex</const>

    <script argv="${node1},pdf">
    <![CDATA[

    function evaluate(arg1, arg2)
        return(string.sub(arg1, 0, -3) .. arg2)
    end


    ]]>
    </script>


would first split all supplied arguments from the script *argv* attribute and resolve those, which refer to another node (indicated by `${nodeid}`).
In this case, the node with id *node1* is a constant value of *some\file.tex*. The second argument is also constant text (_pdf_).
Therefore the inline script would be called as ``evaluate(some_file.tex, pdf)``.
The return value is thus *some\file.pdf*.


<EXECUTE>/<ACTION> node
=======================

.. code:: XML

        <image id="statimg" src="" width="100" height="100"/>
    ...
        <action id="with_program" program="python-prog"/>
        <action id="no_program"/>

    </window>
    <execution>

    <execute program="python-prog" exec="python" param="some.py">
        <output type="COUT" color="green" to="outputstream1"/>
        <output type="CERR" color="red" to="outputstream1"/>
        <update deferred="true" target="statimg" attrib="src" value="..."/>
        <messagebox deferred="false">This is shown before program starts.</messagebox>
        <messagebox deferred="true">This is shown when program ended.</messagebox>
    </execute>

    <execute exec="cowsay" param="hello">
        <output type="COUT" color="green" to="outputstream1"/>
        <output type="CERR" color="red" to="outputstream2"/>
    </execute>

    </execution>

Execution and action node form a unit: the action button uses the exeuction network to execute a program.
For instance, the *action* node with id *with_program* has the program attribute set. Thus, only executable nodes with a program attribute set to this value will be executed.
Here, only the *python* program will be executed.

In contrast, the action node with id *no_program* has no program attribute set. Therefore, all available executable nodes will be executed. Thus, both the *python* program and the *cowsay* program will be executed.

Executable nodes may have several children. In general, these children may have a ``deferred`` attribute which means that these nodes are either activated *before* (``deferred="false"``) the executable is started, or *after* (``deferred="true"``).

<OUTPUT> node
-------------

Output nodes transfer information while a program is running.
If they are of type ``STD``, they transfer both standard console output (``COUT``) as well as standard console error (``CERR``).
The output is transported to the specified stream (`<STREAMBOX>/<STREAM> node`_) in the given color.

There also exist output nodes of type ``TCP``. These must have **host** (from where is information received) and **port** attributes set.
Additionally nodes of type ``FILE`` directly save output to a file.

<UPDATE> node
-------------

Using *update* nodes, attributes of visual elements can be updated. This could, for instance, be the location of an image to be displayed (as in the example above).

<MESSAGEBOX> node
-----------------

Using *messagebox* nodes, message boxes can be created. The text of a message box may also contain node ids (given in brackets ``${id}``).