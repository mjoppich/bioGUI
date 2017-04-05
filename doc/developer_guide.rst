.. _biogui_developer_guide:

********************
Developer Guide
********************

It is highly recommend to read the mechanism section :ref:`bioGUI_working_mechanism` first.
For more information about visual/execution node behaviour, check section :ref:`bioGUI_nodes_descr`.

Extending bioGUI
================

The creation of nodes for both the visual model as well as the execution model is organised by factories.

Window component factory
------------------------

In order to add new visual components, one must register the constructor for a new visual elements in the WindowComponentFactory.

Each constructor for new components must extend the WindowWidgetNode class to return ``CreatedElement`` objects.
This class contains the retriever function for the created element (which fetches values from nodes in *bioGUI*) as well as a WidgetFunctionNode.
The ``WidgetFunctionNode`` contains a pointer to the widget, as well as attribute setters.
These attribute setters can update specific attributes/properties of the widget, for instance the image to be displayed (:ref:`bioGUI_nodes_execution_update`).

Execution node factory
----------------------

The execution model has no special node factory, but the ``XMLParserExecution`` serves as such (this may change soon).
Here, nodes must be registered.
Execution nodes must extend the ``ExecutionNode`` class.
The most important function to implement is the ``std::string evaluate(...)`` function.
This function has three parameters, namely ``std::map< std::string, ExecutionNode*>* pID2Node``, ``std::map<std::string, std::string>* pInputID2Value`` and ``std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget``.

``pID2Node`` is a map which contains a pointer to the *ExecutionNode* for a given *id*.
``pInputID2Value`` is a map which maps any visual element (by id) to its value (determined by the retriever function).
Finally ``pInputID2FunctionWidget`` is a map from any visual element (by id) to its WidgetFunctionNode. This map is essential for updating visual elements (e.g. changing the image being displayed).
