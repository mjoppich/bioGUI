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
