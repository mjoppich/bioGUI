########################
bioGUI Nodes Description
########################

************************
Visual Model Nodes
************************

<GROUP> node
============

<STREAMBOX>/<STREAM> node
=========================


************************
Layout Nodes
************************

<HGROUP>/<VGROUP> node
======================

<GRID> node
===========

************************
Execution Model Nodes
************************

<IF>/<ELSE> node
================

<VALUE> node
============

<ORDEREDADD> node
=================

<ENV> node
==========

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


<EXECUTE> node
==============

<UPDATE> node
-------------

<MESSAGEBOX> node
-----------------

<SAVEFILE> node
---------------