#ifndef CONFIG_H
#define CONFIG_H
#include <QObject>

#define IPM_MOTHER "231.0.0.1"
#define IPM_NEURON_PROMISCUOUS "231.0.0.2" // Any Neuron wants to send somethings to the MOTHER
#define NEURON_PROMISCUOS_PORT 50000 // Port through which all Neurons are heard.
#define IPM_GENERATOR "232.0.0.1"
#define IPM_BASE_NEURON "233.0.0.1"
#define IPM_GRAPHICS "234.0.0.1"
#define GENERATOR_PORT 47000
#define MONITOR_PORT 48000
#define MOTHER_PORT 45000 // To post messages, for example CREATE_SYNAPSE. And they are recibe by all subscribed Neurons. They discriminate.
#define INTERNEURON_PORT 46000
#define GRAPHICS_PORT 49000 // For the graphical representation of the Neuronal model.
#define TYPE_SYP_EXCITATION 1
#define TYPE_SYP_INHIBITION -1
#define TYPENEURON_NONE 0
#define TYPENEURON_MOTHER 1
#define TYPENEURON_GENERATOR 2
#define TYPENEURON_NORMAL 3
#define TTL 2 // Number of jumps. Maybe a router.
#define SIZE0_NODE 3.8 //Graphic scale for another kind of node, i.e MotherNeuron
#define SIZE1_NODE 2.8 //Graphic scale for a kind of node, i.e. Generator
#define SIZE2_NODE 1.8 //Graphic scale for another kind of node, i.e Normal
#define MODEL_ADEXLIF "LIF"
#define EDIT_STYLE_DEFAULT "QLineEdit {background-color: white;}"
#define EDIT_STYLE_MODIFIED "QLineEdit {background-color: green;}"
#define LOCAL_NEURON 1 // The neuron is created locally
#define REMOTE_NEURON 2 // The neuron is created remoted
#endif // CONFIG_H
