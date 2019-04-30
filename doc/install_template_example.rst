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

====================
Example: Trimmomatic
====================

***********************
Dependency/Install Part
***********************

For the install module, we start with the shebang which gives the module its name:

.. code:: bash

    #! Install Trimmomatic 0.36

The text after the shebang is shown as title in the *Install Trimmomatic 0.36*.
For the install part we remember that this is essentially a bash script. Thus any bash commands will work here.

We first have to ensure that all dependencies are installed. Trimmomatic has only java as dependency.
We can install java (openJDK) on Ubuntu and in WSL, however, will ask the user to install JAVA on Mac OS:


.. code:: bash

    if [ ! "$2" = "" ]; then

        if [ "$(uname)" == "Darwin" ]; then

            echo "PLEASE INSTALL JAVA PRIOR USING TRIMMOMATIC!"

        else

            echo "Installing dependencies: openjdk 9"
            echo $2 | sudo -S apt-get update
            echo $2 | sudo -S apt-get -y install openjdk-9-jre
        fi

    else
        echo "No sudo password, not installing dependencies"
    fi


Following the dependencies we should set multiple variables.
It has shown useful to have the program name in a variable (once for processing, and once for displaying to the user):

.. code:: bash

    ## set all variables
    PROG=trimmomatic_0_36
    PROGNICE=Trimmomatic-0.36
    
Apart from the program name we also fetch the install dir as the first parameter given to the install module:



.. code:: bash

    PROGDIR=$1

    if [ -z "${PROGDIR}" ]; then
        PROGDIR=~/bioGUI/progs/
    fi

    PROGDIRESC=$(echo $PROGDIR | sed 's/ /\\ /g')
    APPBINARYDIR=${PROGDIRESC}/${PROG}/

If the PROGDIR (install dir) has not been given (is empty), we set it to a default value.
We create an escaped version of the install dir (PROGDIRESC) and save the program directory where we expect the program to reside (APPBINARYDIR).
In the following we create the progam's install directory, download the application and move everything into place:

.. code:: bash

    ## create progdir
    mkdir -p "$PROGDIR"

    #download and unzip
    echo "Downloading Trimmomatic"
    wget http://www.usadellab.org/cms/uploads/supplementary/Trimmomatic/$PROGNICE.zip -O "$PROGDIR/$PROG.zip"

    unzip -o "$PROGDIR/$PROG.zip" -d "$PROGDIR/$PROG"
    mv "$PROGDIR/$PROG/"*/* "$PROGDIR/$PROG"


The user can select to add the program to the system's PATH variable.
If this should be done, the third parameter supplied to the install module is a `1`.
We add the escaped path to the PATH variable:

.. code:: bash

    if [ $# -gt 2 ]; then

        if [ "$3" = "1" ]; then

            if ! grep -q "$PROGDIR/$PROG" ~/.bashrc; then
                echo "export PATH=\"$PROGDIRESC/$PROG:\$PATH\" " >> ~/.bashrc;
            fi
        fi

    fi

We are almost done. We now want to verify whether the installation has been successful. We determine this by checking whether the install directory is not empty and whether the executable is available at the thought place.
If this is not the case we return the content of all affected paths for better debugging possibilities.
Additionally we give the user an ERROR message with possible actions.

.. code:: bash

    APPBINARYESC="${PROGDIRESC}/${PROG}/"
    APPBINARY="${PROGDIRESC}/${PROG}/trimmomatic-0.36.jar"
    if [ -z "$(ls -A ${APPBINARYESC})" ] || [ ! -f ${APPBINARY} ]; then
    (>&2 echo ${APPBINARYESC})
    (>&2 ls ${APPBINARYESC})

    (>&2 echo " \n \n \n")

    (>&2 echo ${PROGDIRESC})
    (>&2 ls ${PROGDIRESC})

        (>&2 echo " \n \n \n")

    (>&2 echo ${APPBINARY})
    (>&2 ls ${APPBINARY})
    
    (>&2 echo " \n \n \n")

    (>&2 echo "ERROR: The application directory is empty after installation.")
    (>&2 echo "ERROR: If you experience problems please re-install the software and create an issue on https://github.com/mjoppich/bioGUI.")
    (>&2 echo "ERROR: For creating the issue, please upload the log.txt file of your installation attempt.")

        (>&2 echo " \n \n \n")

    else

        echo "${PROG} has been installed into ${APPBINARYESC}"
        echo "Binary location ${APPBINARY}"

    fi

Now we need to transfer the template to bioGUI. This is done via a TCP connection (because std-out and std-err are already used by the install module).
The user's IP address is given as fourth parameter to the install module, the corresponding port as the fifth parameter.
Unfortunately netcat behaves differently on virtually every operating system, we must call netcat differently on Mac OS and Ubuntu, to ensure that the connection is closed upon sending the EOF.

.. code:: bash

    IP=$4
    PORT=$5

    NCCMD=""

    if [ "$(uname)" == "Darwin" ]; then
        NCCMD="nc -c $IP $PORT"
    else
        NCCMD="nc -q 0 $IP $PORT"
    fi

Finally the template is sent via netcat and the bash EOF feature:

.. code:: bash

    $NCCMD <<EOF

    <template ...>
       ...
    </template>

    EOF

The following section explains the template

*************
Template Part
*************

The template description and title is shown in the left, template selection window of *bioGUI*.
The title attribute of the ``window`` tag is shown as application window title.
We must remember that this template is sent via netcat and the EOF feature of bash. Thus any bash variable (indicated by a leading dollar sign) are replaced. If we need a bioGUI variable in the template (e.g. in the execution network), the dollar sign must be escaped!

.. code:: XML

    <template description="Trimming short RNA-Seq Reads" title="$PROGNICE">
        <window title="$PROGNICE">


All following elements are placed in a vertical layout (therefore from top to bottom).
The following group collects the input files. If the user selected single-end mode, only one input and output file is needed.
For paired-end sequencing data, 2 input files and 4 output files are needed.


If your application should be run in WSL/Bash on Ubuntu on Windows, include a checkbox with the ``selectonwindows="true"`` attribute.
The ``exclusive="true"`` signals *bioGUI* to only allow one ``group`` child to be checked.

.. code:: XML

    <checkbox id="WSLsel" value="true" selectonwindows="true">Run in WSL?</checkbox>
    <group title="Method" exclusive="true">
        <hgroup>
            <group id="pairedend" selected="false" title="Paired End" checkable="true">
                <filedialog id="paired_if1" location="1">Input File 1</filedialog>
                <filedialog id="paired_if2" location="2">Input File 2</filedialog>
                <group title="Options">
                    <checkbox>Validate Pairs</checkbox>
                </group>
                <filedialog id="paired_of1p" location="" output="true">Output 1P</filedialog>
                <filedialog id="paired_of1u" location="" output="true">Output 1U</filedialog>
                <filedialog id="paired_of2p" location="" output="true">Output 2P</filedialog>
                <filedialog id="paired_of2u" location="" output="true">Output 2U</filedialog>
            </group>
            <group id="singleend" selected="true" title="Single End" checkable="true">
                <filedialog id="single_if1" location="">Input File 1</filedialog>
                <filedialog id="single_of1" location="" output="true">Output</filedialog>
            </group>
        </hgroup>
    </group>
    
Further options are now collected.
Trimmomatic is a special case, because the order of the parameters can alter the result. We thus need an ordered group (ordered=true).
The order can then later, in the execution network, be retrieved.
For a better visual appearance, elements are arranged in a 3x3 grid.

.. code:: XML

    <group ordered="true" id="orderedgroup" title="Step options">
        <grid rows="3" cols="3">
            <group id="adapters_sel" title="Adapters" checkable="true">
                <filelist id="illuminaclip_auto_file" allowempty="true" path="${APPBINARYDIR}/adapters/" ext="*.fa"/>
                <filedialog id="illuminaclip_man_file" location="">Adapter Sequence</filedialog>
                <label>Seed Mismatches</label>
                <input type="int" id="illuminaclip_seed">2</input>
                <label>Palindrome Clip Threshold</label>
                <input type="int" id="illuminaclip_palin">30</input>
                <label>Simple Clip Threshold</label>
                <input type="int" id="illuminaclip_simple">10</input>
            </group>
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
            <group id="trailingwindow_opt" selected="false" title="Trailing Window" checkable="true">
                <hgroup>
                    <label>Quality</label>
                    <input id="trailingwindow_quality" type="int"/>
                </hgroup>
            </group>
            <group id="crop_opt" selected="false" title="Crop" checkable="true">
                <hgroup>
                    <label>Length</label>
                    <input id="crop_length" type="int"/>
                </hgroup>
            </group>
            <group id="headcrop_opt" selected="false" title="Headcrop" checkable="true">
                <hgroup>
                    <label>Length</label>
                    <input id="headcrop_length" type="int"/>
                </hgroup>
            </group>
            <group id="minlen_opt" selected="false" title="Min Len" checkable="true">
                <hgroup>
                    <label>Length</label>
                    <input id="minlen_length" type="int"/>
                </hgroup>
            </group>
            <group title="PHRED base">
                <combobox id="phred" selected="phred64">
                    <comboitem>phred33</comboitem>
                    <comboitem>phred64</comboitem>
                </combobox>
            </group>
        </grid>
    </group>
    <group title="Options">
        <checkbox>Quiet Mode</checkbox>
    </group>
    <group title="System Settings">
        <label title="example 2">Threads</label>
        <input id="threads_opt" hint="Amount of Threads">2</input>
    </group>

The remaining part of the GUI template are output options.
The streambox captures stdout and stderr output from the launched processes and shows this to the user.
The action element is a button which starts the execution of the execution network (or a specific program if specified).

.. code:: XML

        <streambox id="output1">
            <stream id="outputstream1">OS1</stream>
            <stream id="outputstream2">OS2</stream>
        </streambox>

        <action id="runprog" hint="click here to run program">run program</action>
    </window>


We have now finished the visual part (closed window element) and need to start the execution part.
This part Unfortunately is relatively bulky, because of the many input/output files.

.. code:: XML

    <execution>
        <if id="illuminaclip_file_tmp" value1="illuminaclip_auto_file" comp="is_set">
            <value from="illuminaclip_man_file"/>
            <else>
                <value from="illuminaclip_man_file"/>
            </else>
        </if>
        <if id="illuminaclip_file" comp="EQUALS" value1="WSLsel" value2="true">
            <relocate from="\${illuminaclip_file_tmp}" wsl="true"/>
            <else>
                <value from="illuminaclip_file_tmp"/>
            </else>
        </if>

        <relocate id="paired_if1_rel" from="\${paired_if1}" sep=" " wsl="\${WSLsel}"/>
        <relocate id="paired_if2_rel" from="\${paired_if2}" sep=" " wsl="\${WSLsel}"/>
        <relocate id="paired_of1p_rel" from="\${paired_of1p}" sep=" " wsl="\${WSLsel}"/>
        <relocate id="paired_of1u_rel" from="\${paired_of1u}" sep=" " wsl="\${WSLsel}"/>
        <relocate id="paired_of2p_rel" from="\${paired_of2p}" sep=" " wsl="\${WSLsel}"/>
        <relocate id="paired_of2u_rel" from="\${paired_of2u}" sep=" " wsl="\${WSLsel}"/>
        <relocate id="single_if1_rel" from="\${single_if1}" sep=" " wsl="\${WSLsel}"/>
        <relocate id="single_of1_rel" from="\${single_of1}" sep=" " wsl="\${WSLsel}"/>

The relocate nodes are only used within WSL, when the Windows file path must be translated into the Unix one.
For all other Operating Systems the input path is maintained and no changes are made.

.. code:: XML

    <add sep=":" id="illuminaclip">
        <const>ILLUMINACLIP</const>
        <value from="illuminaclip_file"/>
        <value from="illuminaclip_seed"/>
        <value from="illuminaclip_palin"/>
        <value from="illuminaclip_simple"/>
    </add>
    <add sep=":" id="slidingwindow">
        <const>SLIDINGWINDOW</const>
        <value from="slidingwindow_size"/>
        <value from="slidingwindow_quality"/>
    </add>
    <add sep=":" id="leading">
        <const>LEADING</const>
        <value from="leadingwindow_quality"/>
    </add>
    <add sep=":" id="trailing">
        <const>TRAILING</const>
        <value from="trailingwindow_quality"/>
    </add>
    <add sep=":" id="crop">
        <const>CROP</const>
        <value from="crop_length"/>
    </add>
    <add sep=":" id="headcrop">
        <const>HEADCROP</const>
        <value from="headcrop_length"/>
    </add>
    <add sep=":" id="minlen">
        <const>MINLEN</const>
        <value from="minlen_length"/>
    </add>

Using the above add-nodes all options are assembled. For each possible option, one add-node fetches the user supplied parameters.

Using the orderedadd-node we can assemble these parameters in the order specified by the User in the GUI.
Using the selected attribute, only selected options are added together.
Note that the for-attribute in the value nodes is the ID within the ordered group of the GUI template.

.. code:: XML

    <orderedadd id="steps" from="orderedgroup" selected="true">
        <value from="illuminaclip" for="adapters_sel"/>
        <value from="slidingwindow" for="slidingwindow_opt"/>
        <value from="leading" for="leadingwindow_opt"/>
        <value from="trailing" for="trailingwindow_opt"/>
        <value from="crop" for="crop_opt"/>
        <value from="headcrop" for="headcrop_opt"/>
        <value from="minlen" for="minlen_opt"/>
    </orderedadd>

Finally the input files are added together, as well as the remaining general settings.

.. code:: XML

    <add sep=" " id="files_pe">
        <const>PE</const>
        <value from="paired_if1_rel"/>
        <value from="paired_if2_rel"/>
        <value from="paired_of1p_rel"/>
        <value from="paired_of1u_rel"/>
        <value from="paired_of2p_rel"/>
        <value from="paired_of2u_rel"/>
    </add>
    <add sep=" " id="files_se">
        <const>SE</const>
        <value from="single_if1_rel"/>
        <value from="single_of1_rel"/>
    </add>
    <if id="files" value1="pairedend" comp="EQUALS" value2="true">
        <value from="files_pe"/>
        <else>
            <value from="files_se"/>
        </else>
    </if>
    <if id="threads" value1="threads_opt" comp="is_set">
        <value from="threads_opt"/>
        <else>
            <const>1</const>
        </else>
    </if>

The remaining work to be done is to assemble all input arguments: files, settings and the steps to be executed.
This outcome can be accessed via the add node with id cl.

.. code:: XML

    <add sep=" " id="cl">
        <value from="files"/>
        <add sep="">
            <const>-threads </const>
            <value from="threads"/>
        </add>
        <add sep="">
            <const>-</const>
            <value from="phred"/>
        </add>
        <value from="steps"/>
    </add>

Finally the execute node defines what program is executed. The location contains the path to the exec-utable, to which the param-eters are passed, which are here referenced from the node with id cl.
The output nodes within the execute environment define where the STDOUT and STDERR output from the process should be displayed (which is the outputstream1/2 here, as defined in the GUI template).
Developers can also add deferred nodes here, which are executed as soon as the program has terminated. This might be useful to move files, open/show files, etc.

.. code:: XML

            <const id="bindir">${PROGDIR}/${PROG}/</const>
            <execute location="" exec="java" param="-jar &quot;\${bindir}/trimmomatic-0.36.jar&quot; \${cl}" wsl="WSLsel">
                <output type="COUT" color="green" to="outputstream1" />
                <output type="CERR" color="red" to="outputstream2" />
            </execute>

        </execution>

    </template>
