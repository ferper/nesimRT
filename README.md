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
En primer lugar debemos ir a la url de qt y descargar Qt opensource. Para ello debemos ir a la url https://www.qt.io/download-open-source .
Debemos pulsar el siguiente botón y seguir las instrucciones para bajar el instalador:
(Foto 1)
Cuando tengamos descargado el instalador:
(Foto2)
Lo ejecutamos y debemos loguear nuestro correo y contraseña de Qt (si no se tiene cuenta debemos registrarnos). 
(Foto 3) 
Debemos aceptar las condiciones de usos:
(Foto 4)
Ahora debemos seguir al pie de la letra las siguientes instrucciones.
En primer lugar debemos pulsar el botón de “Instalación personalizada”
(Foto 5)
Debemos seleccionar la versión de Qt. Este proyecto se ha desarrollado en Qt 5.15 por lo que debemos seleccionar esta versión de Qt junto a sus Herramientas.
(Foto 6)
Aceptamos de nuevo los acuerdos de licencia:
(Foto 7)
Y por último instalamos.
(Foto 8)


Una vez descargado Todo he instalado abrimos Qt creator
(Foto 9)
Abrimos el proyecto descargado de GitHub:
(Foto 10)
Para cargar el proyecto abrimos la carpeta “src”:
(Foto 11)
Y dentro de esta abrimos el archivo “nesimRT.pro”:
(Foto 12)
Una vez termine de actualizar todo y cargar el compilador tendremos el proyecto ya abierto:
(Foto 13)

Para ejecutar la aplicación Primero debemos compilarla:
(Foto 14)
Cuando finalice de compilar ya solo tendremos que pulsar el botón “play”:
(Foto 15)



installation tutorial

First of all we must go to the qt url and download Qt opensource. For this we must go to the url https://www.qt.io/download-open-source .
We must press the following button and follow the instructions to download the installer:
(Photo 1)
When we have downloaded the installer:
(Photo2)
We run it and we must log in our Qt email and password (if we do not have an account we must register).
(Photo 3)
We must accept the conditions of use:
(Photo 4)
Now we must follow the following instructions to the letter.
First of all we must press the “Custom installation” button
(Photo 5)
We must select the version of Qt. This project has been developed in Qt 5.15, so we must select this version of Qt next to its Tools.
(Photo 6)
We accept the new license agreements:
(Photo 7)
And finally we installed it.


</p>





<h2 name="GettingStarted">Getting started</h2>

<p align="justify">
Once downloaded, I have installed everything, open Qt Creator
(Photo 9)
We open the project downloaded from GitHub:
(Photo 10)
To load the project we open the “src” folder:
(Photo 11)
And inside this we open the file "nesimRT.pro":
(Photo 12)
Once you finish updating everything and loading the compiler we will have the project already open:
(Photo 13)

To run the application, we must first compile it:
(Photo 14)
When it finishes compiling, we will only have to press the "play" button:
(Photo 15)


</p>



<h2 name="Usage">Usage</h2>

<p align="justify">

</p> The following buttons are shown at the main view of the simulator: 
<ul>
<li> 'Neural Scheme' to access to the Neural Scheme screen. </li> 
<li> 'General monitor' where you can see all the current active signals. </li>
<li> 'About us' where you can get to know more about the team. </li>
<li> 'Exit' to close the simulator. </li>
</ul>

<p align="center">
<img align="center" src="https://github.com/ferper/nesimRT/blob/main/imgs/main.PNG">
</p>

<p> Once you are in the Neural Scheme screen, which is the main one of the simulator, the column on the left shows the options that are available. Firstly, the standard options to handle the simulation file are show:
<ul>
<li> 'Load': use this option if you want to load a previous network that you have been working on.  </li> 
<li> 'New Scenary': use this option if you want to create a new network simulation.  </li>
<li> 'Save': use this option to save the ongoing simulation whitout changing the name of the file. </li>
<li> 'Save as...' use this option to save the current simulation in a new file. </li>
</ul>

<p> Secondly, the options to deal with the simulation are shown: 
<ul>
<li> 'New Neuron': use this option to create a new neuron. Once you click, a pop-up window will appear where you can define the electrical parameters and the visualization position of the neuron. Eventually, clic on 'Create' to allocate the neuron. </li> 
<li> 'New Generator': use this option if you want to create a stimulus. Once you click, a pop-up window will appear where you can define the firing rate, the position of the generator within the grid, among many other options. Eventually, clic on 'Create' to allocate the generator. </li>
<li> 'New Synapse': use this option to connect neurons, to create self excitation or to connect neurons to the generator. Once you click, a pop-up window will appear where you can define all the features of the synapse: source, target, type and weight value. Eventually, clic on 'Create' to include the synapse in the network.  </li>
<li> 'Remove neuron' use this option to remove a neuron, generator or synapse. Once you click, a pop-up window will appear where you can select which item you want to remove. Clic on the approppriate remove button to proceed. </li>
<li> 'SpiNNaker': use this option if you want to export your network simulation to SpiNNaker code. Once selected, you can modify the neuron parameters, edit the spike timing and eventually, set the path for the file to be generated. </li>
</ul>

<p align="center">
<img align="center" src="https://github.com/ferper/nesimRT/blob/main/imgs/neuron.PNG">
</p>

<p> Once the network is defined, to run the simulation, the first step is to start the generator. To do it, double click on the generator to visualize the running options. You can change the firing rate and you can start it by clicking on 'Start'.
  
<p> Now, the simulation is running! You can double click on any neuron to see the options available: 
 <ul>
<li> 'Local Monitor': A new window will appear where you can see all the data related to that neuron. The visual data can be updated, a screenshot can be taken and neuron and synapses parameters can be modified on real-time. If the neuron parameters are changed, you have to click on 'Change Data' button to update the real-time visualization. The synapse parameters can also be updated but, to update the effects on the visualization panel, you have to click on 'Change synapse'. </li> 
<li> 'General Monitor': on the first access, the message will be 'General Monitor: OFF'. Click on it to activate. Then, you could go back to the main window to select the option 'General Monitor' to explore all the signals of your simulation. From that window you can select what signals (membrane potential, input currents or spike generated) from which neuron you want to visualize and the color coding. Once selected, if you click on Start, the real-time simulation info will be shown on the visualization panel. You can update the range, take a screenshoot or leave the General Monitor.  </li>
<li> 'Kill me': to restart the neuron. </li>
<li> 'Quit': to close the pop-up window. 
</ul> 

<p align="center">
<img align="center" src="https://github.com/ferper/nesimRT/blob/main/imgs/synapse.PNG">
</p>
<p align="center">
<img align="center" src="https://github.com/ferper/nesimRT/blob/main/imgs/neuronplot.PNG">
</p>


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
Copyright © 2022
</p>




