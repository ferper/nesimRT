#################################################################
#                                                               #
#     ********************   NESIM-RT  ********************     #
#     -----------------------------------------------------     #
#          A Real Time Spiking Neural Network Simulator         #
#     -----------------------------------------------------     #
#                                                               #
#     Author: Daniel Jesus Rosa Gallardo                        #
#     email: dani.rosagallardo@mail.uca.es                      #
#     Date: 2019                                                #
#     Cadiz University - Spain                                  #
#                                                               #
#################################################################

import pyNN.spiNNaker as sim
import pyNN.utility.plotting as plot
import matplotlib.pyplot as plt
sim.setup(timestep=0.25)

cell_params_lif = {
		'cm': 0.25,
		'i_offset': 0,
		'tau_m': 20,
		'tau_refrac': 2,
		'tau_syn_E': 50,
		'tau_syn_I': 5,
		'v_reset': -70,
		'v_rest': -65,
		'v_thresh': -55
}

sim.set_number_of_neurons_per_core(sim.IF_curr_exp, 100)

G1_1= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G1_1")
G2_2= sim.Population(10,sim.IF_curr_exp(**cell_params_lif), label="G2_2")
GEN1_3= sim.Population(1,sim.SpikeSourceArray(spike_times=[1,2,4,6,7,9],label="GEN1_3"))
G4_4= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G4_4")
G3_5= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G3_5")
G5_6= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G5_6")
G6_7= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G6_7")

input_GEN1_3G1_1=sim.Projection(GEN1_3,G1_1, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=1.25, delay=0))
input_G2_2G1_1=sim.Projection(G2_2,G1_1, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=0.8, delay=0))
input_G4_4G1_1=sim.Projection(G4_4,G1_1, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=1.7, delay=0))
input_G3_5G1_1=sim.Projection(G3_5,G1_1, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=1.3, delay=0))
input_G5_6G1_1=sim.Projection(G5_6,G1_1, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=1, delay=0))
input_G1_1G2_2=sim.Projection(G1_1,G2_2, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=0.1, delay=0))
input_G4_4G4_4=sim.Projection(G4_4,G4_4, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=1, delay=0))
input_G1_1G6_7=sim.Projection(G1_1,G6_7, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=1, delay=0))

G1_1.record(["spikes","v","gsyn_exc"])

simtime =100
sim.run(simtime)

neo = G1_1.get_data(variables=["spikes","v"])
spikes = neo.segments[0].spiketrains
print spikes
v = neo.segments[0].filter(name='v')[0]
print v

sim.end()

plot.Figure(
# plot voltage for first ([0]) neuron
plot.Panel(v, ylabel="Membrane potential (mV)",
data_labels=[G1_1.label], yticks=True, xlim=(0, simtime)),
# plot spikes (or in this case spike)
plot.Panel(spikes, yticks=True, markersize=5, xlim=(0, simtime)),
title="Simple Example",
annotations="Simulated with {}".format(sim.name())
)

plt.show()
