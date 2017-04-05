##############################
bioGUI Install Modules Example
##############################

===============
Example: hisat2
===============

***********************
Dependency/Install Part
***********************

This is already covered in section :ref:`biogui_install_modules`.

*************
Template Part
*************

The template description and title is shown in the left, template selection window of *bioGUI*.
The title attribute of the ``window`` tag is shown as application window title.

.. code:: XML

    <template description="hisat2 2.0.5 aligner" title="hisat2 2.0.5">
    <window title="hisat2 2.0.5 aligner">


All following elements are placed in a vertical layout (therefore from top to bottom).
The following group collects the hisat2 index as well as the output file via a file dialog window.
Remember to specify whether a file to be selected is input our output, a directory or whether multiple files can be selected (and how they are delimited).

If you know your application can run in WSL/Bash on Ubuntu on Windows, include a checkbox with the ``selectonwindows="true"`` attribute.

.. code:: XML

    <vgroup>

    <group title="Alignment Information">
        <checkbox id="WSLsel" value="true" selectonwindows="true">run in WSL?</checkbox>
        <label>hisat2 Index</label> <filedialog id="hisat_index" output="false" folder="false">Select hisat2 index</filedialog>
        <label>output file</label> <filedialog id="hisat_output" output="true" folder="false">Select output file</filedialog>
    </group>
    
Further input is collected. Not here, that you can either activate *paried end* or *single end* read data. The ``exclusive="true"`` signals *bioGUI* to only allow one ``group`` child to be checked.

.. code:: XML

    <group title="Input Data" exclusive="true">
        <hgroup>
        <group id="pairedend" selected="false" title="Paired End" checkable="true">
            <filedialog id="hisat_paired_m1" location="1" multiple="true">Mate 1 files</filedialog>
            <filedialog id="hisat_paired_m2" location="2" multiple="true">Mate 2 files</filedialog>
        </group>
        <group id="singleend" selected="true" title="Unpaired reads" checkable="true">
            <filedialog id="hisat_unpaired_reads" location="">Input File 1</filedialog>
        </group>
        </hgroup>
    </group>
    

The following group contains several input options where a checkbox is sufficient.
For more program control the phred-/report-checkboxes could also be placed inside an *exclusive* group.

.. code:: XML

    <group title="Input Options">
        <checkbox id="hisat_input_f">Reads are FASTA files</checkbox>
        <checkbox id="hisat_input_r">Reads are files with one input sequence per line</checkbox>
        <checkbox id="hisat_input_trim5">Trim bases from 5p</checkbox><input id="hisat_input_trim5_bases" type="int"></input>
        <checkbox id="hisat_input_trim3">Trim bases from 3p</checkbox><input id="hisat_input_trim3_bases" type="int"></input>
        
        <checkbox id="hisat_input_phred33">Qualities are phred+33</checkbox>
        <checkbox id="hisat_input_phred64">Qualities are phred+64</checkbox>

        <checkbox id="hisat_input_max_reports">Search at most x distinct, primary alignments for each read</checkbox><input id="hisat_input_max_reports_num" type="int"></input>
    </group>
    
    <group title="Alignment Options">
        <checkbox id="hisat_align_nofw">Do not attempt to align unpaired reads to the forward reference strand</checkbox>
        <checkbox id="hisat_align_norc">Do not attempt to align unpaired reads to the reverse reference strand</checkbox>
        
        <checkbox id="hisat_align_no-softclip">Disable soft-clipping</checkbox>
        <checkbox id="hisat_align_no-spliced">Disable spliced alignment</checkbox>
        <checkbox id="hisat_align_no-mixed">Disable mixed alignments (if no conc/disc)</checkbox>
        <checkbox id="hisat_align_no-discordant">Do not look for discordant alignments if no concordant are available</checkbox>
        
        <combobox id="hisat_align_mate_orientations">
        <comboitem value="--fr">forward/reverse</comboitem>
        <comboitem value="--rf">reverse/forward</comboitem>
        <comboitem value="--ff">forward/forward</comboitem>
        </combobox>
        
    </group>
    
    <group title="Output Options">
        <checkbox id="hisat_align_no-unaligned">Disable output of unaligned reads</checkbox>
        <checkbox id="hisat_align_reorder">Guarantees output to be in order of input</checkbox>
        <checkbox id="hisat_transcriptome_assembly_stringtie">Report alignments tailored for transcript assemblers including StringTie</checkbox>
        <checkbox id="hisat_transcriptome_assembly_cufflinks">Report alignments tailored specifically for Cufflinks</checkbox>
    </group>
    
    <group title="Performance Options">
        <label>Number of threads</label><input id="hisat_perf_threads" type="int">1</input>
        <checkbox id="hisat_perf_reorder">Maintain order of reads</checkbox>
    </group>

HISAT allows many option. Sometimes not all are documented or some are used so seldom, that it is not worth to include them as visual checkboxes.
A simple input element can serve as container for user-defined command line arguments.
Note that this ``action`` button has the program attribute set!

.. code:: XML
       
    <group title="Advanced">
        <label>Additional command-line flags</label><input id="hisat_adv_specific_options"></input>
    </group>
    
    <action program="hisat2">Align!</action>

A very important preprocessing step for HISAT is to build the index. Here (the minimal) needed input for building the index is collected.
Also the ``action`` button will only launch the program to build an index.

.. code:: XML

    <group title="build index">
            <label>Input Reference</label> <filedialog id="hisat_build_inref" output="false" folder="false">Select hisat2 index</filedialog>		
            <label>hisat2 Index</label> <filedialog id="hisat_build_idx_out" output="true" folder="false">Select hisat2 index</filedialog>
            <action program="hisat2index">Build Index</action>
    </group>
    
Finally we need a streambox to collect any (command line) output.

.. code:: XML

    <streambox id="output1">
        <stream id="outputstream1">std out</stream>
        <stream id="outputstream2">err out</stream>
    </streambox>

    </vgroup>
    </window>

As the visual part has been closed, we need to start the execution part.
First a const-node containing the location of the binary is created. This is filled from the install template (note the unescaped ``$``).

Then nodes needed for launching the hisat2index program are defined.
Since this program is WSL-enabled, any folder/file must be relocated from the windows path to the WSL path.
This is done using relocate nodes.
Then the command line arguments are assembled using the add node. With the ``sep`` attribute, the delimiter can be set.

Finally the program is executed and output is redirected to the outputstream nodes.

.. code:: XML

    <execution>
        <const id="bindir">${PROGDIR}/${PROG}/</const>

        <relocate id="hisat_build_inref_rel" from="\${hisat_build_inref}" wsl="\${WSLsel}"/>
        <relocate id="hisat_build_idx_out_rel" from="\${hisat_build_idx_out}" wsl="\${WSLsel}"/>

        <add id="cl_idx" sep=" ">
            <value from="hisat_build_inref_rel"/>
            <value from="hisat_build_idx_out_rel"/>
        </add>

        <execute program="hisat2index" param="\${cl_idx}" location="\${bindir}" exec="hisat2-build" wsl="WSLsel">
            <output type="COUT" color="green" to="outputstream1" />
            <output type="CERR" color="red" to="outputstream2" />
        </execute>

The actual HISAT exeuction more input files are needed, thus more relocations are needed.
Note that we also use a *LUA* script here to crop the file extensions from the HISAT index.
For a detailled description of the script node, see :ref:`biogui_nodes_script`.

.. code:: XML

    <relocate id="hisat_paired_m1_rel" wsl="\${WSLsel}" from="\${hisat_paired_m1}"/>
    <relocate id="hisat_paired_m2_rel" wsl="\${WSLsel}" from="\${hisat_paired_m2}"/>
    <relocate id="hisat_unpaired_reads_rel" wsl="\${WSLsel}" from="\${hisat_unpaired_reads}"/>
    <relocate id="hisat_output_rel" wsl="\${WSLsel}" from="\${hisat_output}"/>

    <relocate id="hisat_index_rel_raw" from="\${hisat_index}" wsl="\${WSLsel}"/>

    <script id="hisat_index_rel" argv="\${hisat_index_rel_raw}">
    <![CDATA[

    function evaluate(arg1)
        
        if (string.match(arg1, ".%d.ht2$")) then
            return(string.sub(arg1, 0, arg1:find(".%d.ht2$")-1))
        end

        return(arg1)
    end


    ]]>
    </script>

Depending on whether *pairedend* or *singleend* data is being used, the input file arguments are assembled:

.. code:: XML


    <if id="input_files" sep=" " comp="EQUALS" value1="pairedend" value2="true">
        <const>-1</const>
        <value from="hisat_paired_m1_rel"/>
        <const>-2</const>
        <value from="hisat_paired_m2_rel"/>
    <else>
        <const>-U</const>
        <value from="hisat_unpaired_reads_rel"/>
    </else>
    </if>

Then all command line arguments are combined in the ``cl`` add node.
Here checkboxes are masked using if nodes. However, setting the attribute ``unchecked_value=""`` in the checkbox would have the same effect.
Finally the execute node with the program attribute for HISAT is placed.

It is important to remember that only when the ``action`` button is pressed, this node is activated.
Only then the ``cl`` node is evaluated and all the referenced input nodes are collected and evaluated!

.. code:: XML

    <add id="cl" sep=" ">
        <value from="hisat_adv_specific_options"/>
        <const>-x</const>
        <value from="hisat_index_rel"/>

        <value from="input_files"/>
        <const>-S</const>
        <value from="hisat_output_rel"/>

        <if comp="IS_SET" value1="hisat_input_f" >-f</if>
        <if comp="IS_SET" value1="hisat_input_r" >-r</if>

        <if comp="IS_SET" value1="hisat_input_trim5" ><value from="hisat_input_trim5_bases"/></if>
        <if comp="IS_SET" value1="hisat_input_trim3" ><value from="hisat_input_trim3_bases"/></if>

        <if comp="IS_SET" value1="hisat_input_phred33" >--phred33</if>
        <if comp="IS_SET" value1="hisat_input_phred64" >--phred64</if>

        <if comp="IS_SET" value1="hisat_align_nofw" >--nofw</if>
        <if comp="IS_SET" value1="hisat_align_norc" >--norc</if>

        <if comp="IS_SET" value1="hisat_transcriptome_assembly_stringtie">--downstream-transcriptome-assembly</if>
        <if comp="IS_SET" value1="hisat_transcriptome_assembly_cufflinks" >--dta-cufflinks</if>

        <if comp="IS_SET" value1="hisat_input_max_reports" ><value from="hisat_input_max_reports_num"/></if>

        <if comp="IS_SET" value1="hisat_align_no-softclip" >--no-softclip</if>
        <if comp="IS_SET" value1="hisat_align_no-spliced" >--no-spliced</if>
        <if comp="IS_SET" value1="hisat_align_no-mixed" >--no-mixed</if>
        <if comp="IS_SET" value1="hisat_align_no-discordant" >--no-discordant</if>

        <value from="hisat_align_mate_orientations"/>
    </add>

    <execute program="hisat2" param="\${cl}" location="\${bindir}" exec="hisat2" wsl="WSLsel">
        <output type="COUT" color="green" to="outputstream1" />
        <output type="CERR" color="red" to="outputstream2" />
    </execute>

    </execution>

    </template>

..
    =================
    Example: poreSTAT
    =================


    ***********************
    Dependency/Install Part
    ***********************



    *************
    Template Part
    *************