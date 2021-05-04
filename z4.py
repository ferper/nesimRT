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

GEN1_1= sim.Population(1,simSpikeSourceArray(spike_times=[0,8], label="GEN1_1")
G1_2= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G1_2")
G3_4= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G3_4")
G4_5= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G4_5")
G2_6= sim.Population(1,sim.IF_curr_exp(**cell_params_lif), label="G2_6")

input_GEN1_1G1_2=sim.Projection(GEN1_1,G1_2, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=1.25, delay=0))
input_G4_5G1_2=sim.Projection(G4_5,G1_2, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=0.8, delay=0))
input_G4_5G3_4=sim.Projection(G4_5,G3_4, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=0.9, delay=0))
input_G2_6G4_5=sim.Projection(G2_6,G4_5, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=1, delay=0))
input_G1_2G2_6=sim.Projection(G1_2,G2_6, sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight=1.6, delay=0))

GEN1_1.record(["spikes","v","gsyn_exc"])

simtime =50
sim.run(simtime)

neo = GEN1_1.get_data(variables=["spikes","v","gsync_exc"])
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
data_labels=[GEN1_1.label], yticks=True, xlim=(0, simtime)),
# plot spikes (or in this case spike)
plot.Panel(spikes, yticks=True, markersize=5, xlim=(0, simtime)),
title="Simple Example",
annotations="Simulated with {}".format(sim.name())
)

plt.show()
