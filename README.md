## NESIM-RT


<p align="justify">
NESIM-RT is an open-source cross-platform software tool for simulating spiking neural networks in real time. A friendly user interface allows users to design and modify the network, together with visualizing its output in real time. NESIM-RT outperforms other widely-used simulators in the neuromorphic engineering field in terms of latencies and network scalability. NESIM-RT also allows exporting the SNN model directly to SpiNNaker, enabling an immediate transition between software simulation and hardware implementation.

</p>
</p>



<h2>Table of contents</h2>
<p align="justify">
<ul>
<li><a href="#Installing">Installing</a></li>
<li><a href="#GettingStarted">Getting started</a></li>
<li><a href="#SoftwarePackage">Software package</a></li>
<li><a href="#Usage">Usage</a></li>
<li><a href="#Contributing">Contributing</a></li>
<li><a href="#License">License</a></li>
</ul>
</p>


<h2 name="Installing">Installing</h2>

<p align="justify">
........................

</p>





<h2 name="GettingStarted">Getting started</h2>

<p align="justify">
.............................
</p>



<h2 name="Usage">Usage</h2>

<p align="justify">

</p> The following buttoms are shown at the main view of the simulator: 
<ul>
<li> 'Neural Scheme' to access to the Neural Scheme screen. </li> 
<li> 'General monitor' where you can see all the current active signals. </li>
<li> 'About us' where you can get to know more about the team. </li>
<li> 'Exit' to close the simulator. </li>
</ul>

<p> Once you are in the Neural Scheme screen which is the main one of the simulator, the column to the left shows the options that are available. Firstly, the standard options to handle the simulation file are show:
<ul>
<li> 'Load': use this option if you want to load a previous networks that you have been working on.  </li> 
<li> 'New Scenary': use this option if you want to create a new network simulation.  </li>
<li> 'Save': use this option to save the ongoing simulation whitout changing the name. </li>
<li> 'Save as...' use this option to save the ongoing simulation whit a change on the name of the file. </li>
</ul>

<p> Secondly, the options to deal with the simulation are shown: 
<ul>
<li> 'New Neuron': use this option to create a new neuron. Once you click, a pop-up window will appear where you can define the electrical parameters and the visualization position of the neuron . Eventually, clic on 'Create' to allocate the neuron. </li> 
<li> 'New Generator': use this option if you want to create a stimulus. Once you click, a pop-up window will appear where you can define the firing rate, the amount and the position of the genertor within the grid. Eventually, clic on 'Create' to allocate the generator. </li>
<li> 'New Synapse': use this option to connect inter connect neurons, to create self excitation or to connect neurons to the generator. Once you click, a pop-up window will appear where you can define all the features of the neuron: source, target, type and weight value. Eventually, clic on 'Create' to include the synapse in the network.  </li>
<li> 'Remove neuron' use this option to remove a neuron, generator or synapse. Once you click, a pop-up window will appear where you can select which item you want to remove. Clic on the approppriate remove buttom to proceed. </li>
<li> 'SpiNNaker': use this option if you want to export your network simulation to SpiNNaker code. Once selected, you can modify the neuron parameters, edit the spike timming and eventually, set the path for the file to be generated. </li>
</ul>

<p> Once the network is defined, to run the simulation, the first step is to start the generator. To do it, docuble click on the generator to visualize the running options. You can change the firing rate and you can start ir by clicking on 'Start'.
  
<p> Now, the simulation is running! You can double click on any neuron to see the options available: 
 <ul>
<li> 'Local Monitor': A new window will appear where you can see all the data related to that neuron. The visual data can be updated, a screenshot can be taken and neuron and synapses parameters can be modified on real-time. If the neuron parameters are changed, you have to click on 'Change Data' buttom to update the real-time visualization. The synapse parameters can also be updated but, to update the effects on the visualization panel, you have to click on 'Change synapse'. </li> 
<li> 'General Monitor': on the first access, the message will be 'General Monitor: OFF'. Click on it to activate. Then, you could go back to the main window to select the option 'General Monitor' to explore all the signals of your simulation. From that window you can select what signals (membrane potential, input currents or spike generated) from which neuron you want to visualize and the color coding. Once selected, if you click on Start, the real-time simulation info will be shown on the visualization panel. You can update the range, take a screenshoot or leave the General Monitor.  </li>
<li> 'Kill me': to restart the neuron. </li>
<li> 'Quit': to close the pop-up window. 
</ul> 
 



<h2 name="SoftwarePackage">Software package</h2>

<p align="justify">
The code has been documented .....
</p>
<p align="justify">
To see all the information for the different Python modules, classes, methods and all the possible configuration options, read ...
</p>






<h2>Contributing</h2>

<p align="justify">
New functionalities or improvements to the existing project are welcome. To contribute to this project please follow these guidelines:
<ol align="justify">
<li> Search previous suggestions before making a new one, as yours may be a duplicate.</li>
<li> Fork the project.</li>
<li> Create a branch.</li>
<li> Commit your changes to improve the project.</li>
<li> Push this branch to your GitHub project.</li>
<li> Open a <a href="https://github.com/ferper/nesimRT/pulls">Pull Request</a>.</li>
<li> Discuss, and optionally continue committing.</li>
<li> Wait untill the project owner merges or closes the Pull Request.</li>
</ol>
If it is a new feature request (e.g., a new functionality), post an issue to discuss this new feature before you start coding. If the project owner approves it, assign the issue to yourself and then do the steps above.
</p>
<p align="justify">
Thank you for contributing to NESIM-RT.
</p>



<h2>License</h2>
<p align="justify">
This project is licensed under the GPL License - see the <a href="https://github.com/ferper/nesimRT/blob/main/LICENSE">LICENSE.md file</a> for details.
</p>

<p align="justify">
Copyright © 2022 .....
<a href="mailto:.....@.....com">...........</a>
</p>

<p align="center">
<img align="center" src="https://github.com/ferper/nesimRT/blob/main/src/scheme/graphics/neuron_background.jpg">
</p>


