#ifndef MESSAGES_H
#define MESSAGES_H
#include <QObject>
// #:C:field1;field2;field3;...field8;
const QString header="#"; //message's header
const QString sep_operation="~";
const QString separator=";"; //Separator for the differents fields
// #:CS:filed1;field2;filed3;field4;field5;field6;field7; - impS;impT;port;type;Wvalue;fx;idGlobalSynapse
const QString CREATE_SYNAPSYS="CS";

// #:CREATESMOTHER:source;target;port;typeSynapse;w;valueFxI;fx;typeNeuron;idGlobal
const QString CREATE_SYNAPSE_INTO_MOTHER="CREATESMOTHER";
// #:CREATESNEURON:impS;impT;port;type;Wvalue;valueFxI;fx;typeNeuron;idGlobalSynapse
const QString CREATE_SYNAPSE_INTO_NEURON="CREATESNEURON";
// #:CREATESGRAPHIC:impS;impT;port;type;Wvalue;valueFxI;fx;typeNeuron;idGlobalSynapse
const QString CREATE_SYNAPSE_INTO_GRAPHIC="CREATESGRAPHIC";

//#:ESGM:field1;field2; - ipmSource;true/false
const QString ENABLE_SEND_GENERAL_MONITOR="ESGM";
//#:RN:field1;field2,field3,field4; - typeNeuron,ipmSource;idNeuron;AmountGroup
const QString REGISTER_NEURON="RN";
//#:GGIDS:field1; - ipmSource
const QString GET_GLOBAL_IDSYNAPSE="GGIDS";
//#:UVSFNTOMOTHER:field1;field2;field3;field4;field5;field6; - ipmS;idGlobalSynapse;w;fx_numberTxt;fx_unitMeasure;typeSynapse
const QString UPDATE_VALUES_SYNAPSE_FROM_NEURONWIDGET_TO_MOTHER="UVSFNTOMOTHER";
//#:UVSFNTOMOTHER:field1;field2;field3;field4;field5;field6; - ipmS;idGlobalSynapse;w;fx_numberTxt;fx_unitMeasure;typeSynapse
const QString UPDATE_VALUES_SYNAPSE_FROM_MOTHER_TO_NEURON="UVSFMTONEURON";

const QString UPDATE_MODEL_VALUES_PARAMETERS_FROM_WIDGET_TO_MOTHER="UMVPFWTOMOTHER";

const QString UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER="UMVPFMTOMOTHER";
const QString  UPDATE_MODEL_VALUES_PARAMETERS_FROM_MOTHER_TO_NEURON="UMVPFMFROMTONEURON";


const QString  REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER="RSFROMGRAPHICTOMOTHER";
const QString  REMOVE_SYNAPSE_FROM_MOTHER_TO_NEURON="RSFROMMOTHERTONEURON";
const QString  REMOVE_SYNAPSE_FROM_MOTHER_TO_GRAPHICS="RSFROMMOTHERTOGRAPHICS";
const QString  REMOVE_ALL_SYNAPSYS_THE_OTHER_TOME_FROM_NEURON_TO_MOTHER="RALLSTHEOTHERFROMNEURONTOMOTHER";
const QString REMOVE_NEURON_FROM_MOTHER_TO_NEURON="RNFROMMOTHERTONEURON";
const QString REMOVE_NEURON_FROM_MOTHER_TO_GRAPHIC="RNFROMMOTHERTOGRAPHIC";
const QString REMOVE_NEURON_FROM_GRAPHIC_TO_MOTHER="RNFROMGRAPHICTOMOTHER";

const QString REMOVE_ALL_NEURON_FROM_GRAPHICS_TO_MOTHER_LOADSCENE="RALLNFROMGRAPHICSTOMOTHERLOADSCENE";
const QString REMOVE_ALL_NEURON_FROM_MOTHER_TO_NEURON_LOADSCENE="RALLNFROMMOTHERTONEURONLOADSCENE";
const QString REMOVE_ALL_NEURON_FROM_GRAPHICS_TO_MOTHER="RALLNFROMGRAPHICSTOMOTHER";
const QString REMOVE_ALL_NEURON_FROM_MOTHER_TO_NEURON="RALLNFROMMOTHERTONEURON";
const QString LOADSCENE_FROM_MOTHER_TO_GRAPHIC="LOADSCENE_FROMMOTHERTOGRAPHICS";
//#:UNGN:field1;field2; - ipmS;N
const QString UPDATE_NUMBER_GROUP_NEURON="UNGN";
//#:UNGN:field1;field2; - ipmS;R
const QString UPDATE_RESISTOR_NEURON="URESISTORN";

//#:RSM:field1;field2; - ipms,idGlobalSynapse
const QString REMOVE_SYNAPSE_MOTHER="RSM";
//#:REQIPDHCP:field1;field2; - MAC;typeofNode -> NormalNeuron, Generator...
const QString REQ_IP_DHCP="REQIPDHCP";
//#:RECIPDHCP:field1;field2; - MAC;ip
const QString ACK_REQ_IP_DHCP="ACKREQIPDHCP";




// Message sent by a Neuron to Mother, when it is create
//#:CREATEA_N:id;label;amountOfNeurons;ipmSource;typeNeuron;w;fx;param1..param26
const QString CREATE_NEURON_FROM_NEURON_TO_MOTHER="CREATE_N";

// Message sent by Mother Neuron and received by Graphics
//#:CGRAPHICN:ip;id;label;typeNeuron;amountOfNeurons;w_default;fx;param1..param26;
const QString CREATE_GRAPHIC_NEURON_FROM_MOTHER_TO_GRAPHICS="CGRAPHICN";




#endif // MESSAGES_H
