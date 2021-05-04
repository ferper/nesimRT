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
GEN1_3= sim.Population(1,simSpikeSourceArray(spike_times=[0,8], label="GEN1_3")
G4_4= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G4_4")
Dani_5= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="Dani_5")
G5_6= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G5_6")
G6_7= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G6_7")

input_G1_1GEN1_3=sim.Projection(G1_1,GEN1_3, sim.OneToOneConnector(), synapse_type.StaticSynapse(weight=1.25, delay=1))
input_G1_1G2_2=sim.Projection(G1_1,G2_2, sim.OneToOneConnector(), synapse_type.StaticSynapse(weight=0.8, delay=1))
input_G1_1G4_4=sim.Projection(G1_1,G4_4, sim.OneToOneConnector(), synapse_type.StaticSynapse(weight=1.7, delay=1))
input_G1_1Dani_5=sim.Projection(G1_1,Dani_5, sim.OneToOneConnector(), synapse_type.StaticSynapse(weight=1.3, delay=1))
input_G1_1G5_6=sim.Projection(G1_1,G5_6, sim.OneToOneConnector(), synapse_type.StaticSynapse(weight=1, delay=1))
input_G2_2G1_1=sim.Projection(G2_2,G1_1, sim.OneToOneConnector(), synapse_type.StaticSynapse(weight=0.1, delay=1))
input_G4_4G4_4=sim.Projection(G4_4,G4_4, sim.OneToOneConnector(), synapse_type.StaticSynapse(weight=1, delay=1))
input_G6_7G1_1=sim.Projection(G6_7,G1_1, sim.OneToOneConnector(), synapse_type.StaticSynapse(weight=1, delay=1))

G1_1.record(["spikes","v","gsyn_exc"])

simtime =50
sim.run(simtime)

neo = G1_1.get_data(variables=["spikes","v","gsync_exc"])
spikes = neo.segments[0].spiketrains
print spikes
v = neo.segments[0].filter(name='v')[0]
print v
gsync = neo.segments[0].filter(name='gsync_exc')[0]
print gsync

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
