#include "newspinnaker.h"
#include "ui_newspinnaker.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <iostream>


NewSpiNNaker::NewSpiNNaker(QWidget *parent,QList <Node*> *vectorGraphicsNodes,QVector <JunctionsDraw*> *vectorGraphicsEdge):
    QWidget(parent),
    ui(new Ui::NewSpiNNaker)
{
    ui->setupUi(this);

    this->vectorGraphicsNodes=vectorGraphicsNodes;


    fileName="";
    timeStep=0.25;
    delay=0;
    timeSimulation=50;

    ui->lineEdit_timeStep->setText(QString::number(timeStep));
    ui->lineEdit_delay->setText(QString::number(delay));
    ui->lineEdit_timeSimulation->setText(QString::number(timeSimulation));

    fillAllNeurons();
    connect(ui->lineEdit_Neuron, SIGNAL(textChanged(const QString &)), this, SLOT(neuronFilterSearch()));

    QGraphicsScene *sceneTmp = new QGraphicsScene();
    QPixmap p0(":graphics/neuron.png");

    sceneTmp->addPixmap(p0);
    ui->graphicsView->setScene(sceneTmp);
    sceneTmp = new QGraphicsScene();
    QPixmap p1(":graphics/equation.png");
    sceneTmp->addPixmap(p1);

    ui->graphicsView_2->setScene(sceneTmp);

    this->setWindowTitle("Export to SpiNNaker");

    QPixmap pixmap_ok(":graphics/createNeuron.png");
    QIcon ButtonIcon(pixmap_ok);
    ui->pushButton->setIcon(ButtonIcon);
    QSize s0(ui->pushButton->rect().size().width()-10,ui->pushButton->rect().size().height()-10);
    ui->pushButton->setIconSize(s0);
    ui->pushButton->setFixedSize(ui->pushButton->rect().size());
    ui->pushButton->setFlat(true);

    QPixmap pixmap_cancel(":graphics/cancel.png");
    QIcon ButtonIcon1(pixmap_cancel);
    ui->pushButton_2->setIcon(ButtonIcon1);
    QSize s1(ui->pushButton_2->rect().size().width()-10,ui->pushButton_2->rect().size().height()-10);
    ui->pushButton_2->setIconSize(s1);
    ui->pushButton_2->setFixedSize(ui->pushButton_2->rect().size());
    ui->pushButton_2->setFlat(true);

    QGraphicsDropShadowEffect* effect0 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect1 = new QGraphicsDropShadowEffect();

    effect0->setBlurRadius(100);
    effect0->setOffset(3,3);
    effect1->setBlurRadius(100);
    effect1->setOffset(3,3);

    ui->pushButton->setGraphicsEffect(effect0);
    ui->pushButton_2->setGraphicsEffect(effect1);
}

//To strecht the images into the container
void NewSpiNNaker::showEvent(QShowEvent *) {
    ui->graphicsView->fitInView(ui->graphicsView->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView_2->fitInView(ui->graphicsView_2->sceneRect(),Qt::KeepAspectRatio);

    ui->tableWidget_times->clearContents();
    ui->tableWidget_times->setColumnCount(1);
    ui->tableWidget_times->setRowCount(0);
    QStringList labels;
    labels << tr("Spikes Time");
    ui->tableWidget_times->setColumnWidth(0,100);
    ui->tableWidget_times->setHorizontalHeaderLabels(labels);
    ui->tableWidget_times->setShowGrid(true);
}

void NewSpiNNaker::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {
       case Qt::Key_Escape: // Pressing ESCAPE closes the window
          close();
          break;
    }
}


NewSpiNNaker::~NewSpiNNaker()
{
    delete ui;
}

void NewSpiNNaker::on_pushButton_2_clicked()
{
    close();
}

bool NewSpiNNaker::ParametersOk() {
    bool ok=true;
    bool error=false;

    if (!ok) {
        timeStep=ui->lineEdit_timeStep->text().toFloat(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","The value of the timeStep parameter is not valid.");
            ui->lineEdit_timeStep->setFocus();
            error=true;
        }
    }

    if (!error) {
         delay=ui->lineEdit_delay->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","The value of the delay parameter is not valid.");
            ui->lineEdit_delay->setFocus();
            error=true;
        }
    }
    if (!error) {
         timeSimulation=ui->lineEdit_timeSimulation->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","The value of the Time Simulation parameter is not valid.");
            ui->lineEdit_timeSimulation->setFocus();
            error=true;
        }
    }
    if (!error) {
       ok=true;
       for (int i=0; i<ui->tableWidget_times->rowCount() && ok ;i++)
          int t=ui->tableWidget_times->item(i,0)->text().toInt(&ok);
       if (!ok) {
          QMessageBox::warning(this, "Error","There is an incorrect value in the Spikes time table.");
          error=true;
       }
    }

    return !error;

}

void NewSpiNNaker::exportToBrian2()
{
    if (fileName.length()) {
         QFile file(fileName);
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
           return;
         QTextStream out(&file);
         out << "from brian2 import *"<< "\n";
         out << "import matplotlib.pyplot as plt"<< "\n";
         out << "\n\n";

         out << "eqs = '''\n";
         out << "dv/dt  = (-(v-Vrest) + R*(Iexc - Iinh))/taum : volt (unless refractory)\n";
         out << "dIexc/dt = -Iexc/taue : amp\n";
         out << "dIinh/dt = -Iinh/taui : amp\n";
         out << "R : ohm\n";
         out << "Vth : volt\n";
         out << "Vreset : volt\n";
         out << "Vrest : volt\n";
         out << "taum : second\n";
         out << "taue : second\n";
         out << "taui : second\n";
         out << "'''\n";

         /* Export all neurons including Generators*/
         Node *nodeSource;
         for (int i=0; i<vectorGraphicsNodes->size();i++ ) {
                     nodeSource=vectorGraphicsNodes->at(i);
            QString labelNeuron= nodeSource->label+"_"+nodeSource->id;
            if (nodeSource->typeNode==TYPENEURON_GENERATOR) { // Use frequency instead of time
                out << "times = array([";
                for (int j=0; j<ui->tableWidget_times->rowCount();j++)
                {
                    out<<ui->tableWidget_times->item(j,0)->text();
                    if (j!=ui->tableWidget_times->rowCount()-1)
                        out <<",";
                }
                out<<"])*ms\n";

                out << "indices = array([";
                for (int j=0; j<ui->tableWidget_times->rowCount();j++)
                {
                    out<<"0";
                    if (j!=ui->tableWidget_times->rowCount()-1)
                        out <<",";
                }
                out<<"])\n";
                out<< labelNeuron<<"= SpikeGeneratorGroup(1, indices, times)\n";
            }
            else if (nodeSource->typeNode==TYPENEURON_NORMAL) { // TODO: Use nodeSource->parameters to create parameter per node
                out << labelNeuron<<"= NeuronGroup("<<nodeSource->amountOfNeurons<<", eqs, threshold='v>Vth', reset='v = Vreset',method='exact')\n";
                out << labelNeuron<<".v = "<<QString::number(nodeSource->parameters->Vrh)<<"*volt\n";
                out << labelNeuron<<".Iexc = 0*nA\n";
                out << labelNeuron<<".Iinh = 0*nA\n";
                out << labelNeuron<<".R = "<<QString::number(nodeSource->parameters->R)<<"*ohm\n";
                out << labelNeuron<<".Vth = "<<QString::number(nodeSource->parameters->Vth)<<"*volt\n";
                out << labelNeuron<<".Vreset = "<<QString::number(nodeSource->parameters->Vr)<<"*volt\n";
                out << labelNeuron<<".Vrest = "<<QString::number(nodeSource->parameters->Vrh)<<"*volt\n";
                out << labelNeuron<<".taum = "<<QString::number(nodeSource->parameters->tau_v)<<"*ms\n";
                out << labelNeuron<<".taue = "<<QString::number(nodeSource->parameters->tau_e)<<"*ms\n";
                out << labelNeuron<<".taui = "<<QString::number(nodeSource->parameters->tau_i)<<"*ms\n";
            }

         }
         out << "\n";
         /* Export all synapses */
         Node *nodeTarget;
         for (int i=0; i<vectorGraphicsNodes->size();i++ ) {
             nodeTarget=vectorGraphicsNodes->at(i);
             QString NeuronT=nodeTarget->label+"_"+nodeTarget->id;
             for (int j=0; j<nodeTarget->synapsys.size();j++) {
                nodeSource= findNode("ip",nodeTarget->synapsys.at(j).ipmSource);
                QString NeuronS=nodeSource->label+"_"+nodeSource->id;
                  out<<"input_"<<NeuronS<<NeuronT<<"=Synapses("<<NeuronS<<", "<<NeuronT<<", 'w : amp', on_pre='"<<((nodeTarget->synapsys.at(j).type==1) ? "Iexc" : "Iinh")<<" += w')\n";
                  out<<"input_"<<NeuronS<<NeuronT<<".connect(True)\n";
                  out<<"input_"<<NeuronS<<NeuronT<<".w = "<<nodeTarget->synapsys.at(j).w<<"*"<<nodeTarget->synapsys.at(j).fx_unitMeasureTxt<<"\n";
             }
         }
         out <<"\n";
         QTableWidgetItem *item = ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),0);
         QString neuron_Monitorized;
         if (item) {
             neuron_Monitorized=ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),0)->text()+"_"+ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),2)->text();
             out << neuron_Monitorized << "_smon = SpikeMonitor(" << neuron_Monitorized  <<")\n";
             out << neuron_Monitorized << "_mon = StateMonitor(" << neuron_Monitorized  <<", 'v', record=0)\n";
         }
         out <<"\n";
         out << "simtime ="<<timeSimulation<<"\n";
         out << "run(simtime * second)\n";
         out <<"\n";

         if (ui->checkBox_plot->isChecked()) {
             out << "plot(" << neuron_Monitorized << "_smon.t/ms, " << neuron_Monitorized << "_smon.i, ',k')\n";
             out << "xlabel('Time (ms)')\n";
             out << "ylabel('Neuron index')\n";
             out << "show()\n";
             out<<"\n";
             out<< "plt.show()"<<"\n";
         }

         QMessageBox::information(this, "Warning","The file has been succesfully exported.");

     }
     else QMessageBox::warning(nullptr, tr("Save Error"), tr("The file name is empty."));
}

void NewSpiNNaker::exportToSpiNNaker(){

   if (fileName.length()) {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
              return;
            QTextStream out(&file);
            out << "import pyNN.spiNNaker as sim"<< "\n";
            out << "import pyNN.utility.plotting as plot"<< "\n";
            out << "import matplotlib.pyplot as plt"<< "\n";
            out << "sim.setup(timestep="<< QString::number(timeStep)<<")"<<"\n";
            out << "\n\n";

            out <<"sim.set_number_of_neurons_per_core(sim.IF_curr_exp, 100)"<<"\n";

            out <<"\n";
            /* Export all neurons including Generators*/
            Node *nodeSource;
            for (int i=0; i<vectorGraphicsNodes->size();i++ ) {
                        nodeSource=vectorGraphicsNodes->at(i);
               QString labelNeuron= nodeSource->label+"_"+nodeSource->id;
               if (nodeSource->typeNode==TYPENEURON_GENERATOR) { // Use frequency instead of time
                   out<< labelNeuron<<"= sim.Population(1,sim.SpikeSourceArray, {'spike_times': [[";
                    int s=ui->tableWidget_times->rowCount();
                   for (int j=0; j<ui->tableWidget_times->rowCount();j++) {
                      out<<ui->tableWidget_times->item(j,0)->text();
                      if (j!=ui->tableWidget_times->rowCount()-1)
                          out <<",";
                    }
                   out<<"]]},label=\""<<labelNeuron<<"\")"<<"\n";
               }
               else if (nodeSource->typeNode==TYPENEURON_NORMAL) { // TODO: Use nodeSource->parameters to create parameter per node
                   out << "cell_params_" << labelNeuron << " = {"<<"\n"; // TODO: Change
                   out << "\t\t'cm': "<<QString::number(1)<<",\n"; // Capacity of the membrane in nF
                   out << "\t\t'i_offset': "<<QString::number(0)<<",\n"; // Offset current in nA
                   out << "\t\t'tau_m': "<<QString::number(nodeSource->parameters->tau_v)<<",\n"; // Membrane time constant in ms.
                   out << "\t\t'tau_refrac': "<<QString::number(0.1)<<",\n"; //Duration of refractory period in ms.
                   out << "\t\t'tau_syn_E': "<<QString::number(nodeSource->parameters->tau_e)<<",\n"; // Decay time of excitatory synaptic current in ms.
                   out << "\t\t'tau_syn_I': "<<QString::number(nodeSource->parameters->tau_i)<<",\n"; // Decay time of inhibitory synaptic current in ms.
                   out << "\t\t'v_reset': "<<QString::number(nodeSource->parameters->Vr*1000)<<",\n"; // Reset potential after a spike in mV.
                   out << "\t\t'v_rest': "<<QString::number(nodeSource->parameters->Vrh*1000)<<",\n"; // Resting membrane potential in mV.
                   out << "\t\t'v_thresh': "<<QString::number(nodeSource->parameters->Vth*1000)<<"\n"; //Spike threshold in mV.
                   out << "}" << "\n";
                   out << labelNeuron<<"= sim.Population("<<nodeSource->amountOfNeurons<<",sim.IF_curr_exp, cell_params_" << labelNeuron << ", label=\""<<labelNeuron<<"\")"<<"\n";
                   out << labelNeuron<<".initialize(v="<<QString::number(nodeSource->parameters->Vr)<<")\n\n";
               }

            }
            out << "\n";
            /* Export all synapses */
            Node *nodeTarget;
            for (int i=0; i<vectorGraphicsNodes->size();i++ ) {
                nodeTarget=vectorGraphicsNodes->at(i);
                QString NeuronT=nodeTarget->label+"_"+nodeTarget->id;
                for (int j=0; j<nodeTarget->synapsys.size();j++) {
                   nodeSource= findNode("ip",nodeTarget->synapsys.at(j).ipmSource);
                   QString NeuronS=nodeSource->label+"_"+nodeSource->id;
                     out<<"input_"<<NeuronS<<NeuronT<<"=sim.Projection("<<NeuronS<<","<<NeuronT<<", sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight="<<nodeTarget->synapsys.at(j).w<<", delay="<<delay<<"),receptor_type= \""<< ((nodeTarget->synapsys.at(j).type==1) ? "excitatory" : "inhibitory" )<<"\" )"<<"\n";
                }
            }
            out <<"\n";
            QTableWidgetItem *item = ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),0);
            QString neuron_Monitorized;
            if (item) {
                neuron_Monitorized=ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),0)->text()+"_"+ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),2)->text();
                out << neuron_Monitorized <<".record([\"spikes\",\"v\",\"gsyn_exc\"])"<<"\n";
            }
            out <<"\n";
            out << "simtime ="<<timeSimulation<<"\n";
            out << "sim.run(simtime)"<<"\n";
            out <<"\n";
            out<< "neo = "<<neuron_Monitorized<<".get_data(variables=[\"spikes\",\"v\"])"<<"\n";
            out<< "spikes = neo.segments[0].spiketrains"<<"\n";
            out<< "print(spikes)"<<"\n";
            out<< "v = neo.segments[0].filter(name='v')[0]"<<"\n";
            out<< "print(v)"<<"\n";
            out<< "\n";
            out<< "sim.end()"<<"\n";
            out<< "\n";

            if (ui->checkBox_plot->isChecked()) {
                out<< "plot.Figure("<<"\n";
                out<< "# plot voltage for first ([0]) neuron"<<"\n";
                out<< "plot.Panel(v, ylabel=\"Membrane potential (mV)\","<<"\n";
                out<< "data_labels=["<<neuron_Monitorized<<".label], yticks=True, xlim=(0, simtime)),"<<"\n";
                out<< "# plot spikes (or in this case spike)"<<"\n";
                out<< "plot.Panel(spikes, yticks=True, markersize=5, xlim=(0, simtime)),"<<"\n";
                out<< "title=\"Simple Example\","<<"\n";
                out<< "annotations=\"Simulated with {}\".format(sim.name())"<<"\n";
                out<< ")"<<"\n";
                out<<"\n";
                out<< "plt.show()"<<"\n";
            }

            QMessageBox::information(this, "Warning","The file has been succesfully exported.");

        }
        else QMessageBox::warning(nullptr, tr("Save Error"), tr("The file name is empty."));
}
void NewSpiNNaker::fillAllNeurons(QString key, QString value) {

        ui->tableWidget_Neurons->clearContents();
        ui->tableWidget_Neurons->setColumnCount(3);
        ui->tableWidget_Neurons->setRowCount(0);
        QStringList labels;
        labels << tr("Label") << tr("IP Address") <<tr("ID") ;
        ui->tableWidget_Neurons->setColumnWidth(0,100);
        ui->tableWidget_Neurons->setColumnWidth(1,80);
        ui->tableWidget_Neurons->setColumnWidth(2,80); // It is 0 so it does not show

        ui->tableWidget_Neurons->setHorizontalHeaderLabels(labels);
        ui->tableWidget_Neurons->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget_Neurons->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableWidget_Neurons->setShowGrid(true);
        ui->tableWidget_Neurons->setEditTriggers(QAbstractItemView::NoEditTriggers);

        QString test0="";
        QString test1="";
        if (key.length()) {
            if (key=="label")
                test0=value.toUpper();
            else
                test1=value.toUpper();
        }

        for (int i=0; i<vectorGraphicsNodes->size();i++) {
           if ((vectorGraphicsNodes->at(i)->typeNode==TYPENEURON_NORMAL) || (vectorGraphicsNodes->at(i)->typeNode==TYPENEURON_GENERATOR)) {
              if ((vectorGraphicsNodes->at(i)->label.toUpper().contains(test0) && (vectorGraphicsNodes->at(i)->ip.toUpper().contains(test1)))) {
                 QTableWidgetItem *tlabel = new QTableWidgetItem();
                 QTableWidgetItem *tIp = new QTableWidgetItem();
                 QTableWidgetItem *tId = new QTableWidgetItem();

                 tlabel->setText(vectorGraphicsNodes->at(i)->label);
                 tIp->setText(vectorGraphicsNodes->at(i)->ip);
                 tId->setText(vectorGraphicsNodes->at(i)->id);

                 int row = ui->tableWidget_Neurons->rowCount();

                 ui->tableWidget_Neurons->insertRow(row);
                 ui->tableWidget_Neurons->setItem(row, 0,tlabel);
                 ui->tableWidget_Neurons->setItem(row, 1,tIp);
                 ui->tableWidget_Neurons->setItem(row, 2,tId);
              }
           }
           ui->tableWidget_Neurons->selectRow(0);
       }
}

void NewSpiNNaker::neuronFilterSearch(){

    QString key="";
    if (ui->comboBox_Neurons->currentIndex()==0)
        key="label";
    else
        key="Ip";

    fillAllNeurons(key,ui->lineEdit_Neuron->text());
}

void NewSpiNNaker::on_pushButton_clicked()
{
    if (ParametersOk()) {
        QString baseName = QFileInfo(ui->lineEdit_fileName->text()).baseName();
        QString extension= QFileInfo(fileName).completeSuffix();
        if (extension!="py") {
            extension=".py";
            fileName=fileName+extension;
        }
        if (!baseName.length())
            QMessageBox::warning(this, "Error","The filename is empty");
        else {
            fileName=ui->lineEdit_fileName->text();
            ui->lineEdit_fileName->setText(fileName);

            QTableWidgetItem *item = ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),0);
            if (!item) {
                QMessageBox::StandardButton reply;
                QString msg="There is no selected neuron to monitor.\nWould you still like to export this?";
                reply=QMessageBox::question(this, "Atención",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
                if (reply==QMessageBox::Yes)
                {
                    exportToSpiNNaker();
                }
            }
            else
            {
                exportToSpiNNaker();
            }
        }
    }
}

void NewSpiNNaker::on_pushButton_3_clicked()
{
   QFileDialog fileDialog(this, tr("Export to SpiNNaker"), QDir::currentPath(),tr("SpiNNaker Files (*.py)"));
   fileDialog.setOption(QFileDialog::DontUseNativeDialog,true); // Very important for refresh
   fileDialog.setAcceptMode(QFileDialog::AcceptSave);
   if (QDialog::Accepted != fileDialog.exec())
      return ;

   fileDialog.window()->setFocus();
   fileName=fileDialog.selectedFiles().at(0);

   if (fileName.isEmpty())
      return;
   //fileName=fileName+".py";
   ui->lineEdit_fileName->setText(fileName);
}

Node* NewSpiNNaker::findNode(QString key, QString value){
    bool founded=false;
    int idx=0;
    int total=vectorGraphicsNodes->size();
    while ((!founded) && (idx<total)) {
        if (key=="ip") {
            if (vectorGraphicsNodes->at(idx)->ip==value)
                founded=true;
            else
                idx++;
        }
        else if (key=="label") {
            if (vectorGraphicsNodes->at(idx)->label==value)
                founded=true;
            else
                idx++;

        }
        else if (key=="id") {
            if (vectorGraphicsNodes->at(idx)->id==value)
                founded=true;
            else
                idx++;
        }
    }
    if (founded)
        return vectorGraphicsNodes->at(idx);
    return (Node*) nullptr;
}

void NewSpiNNaker::on_pushButton_4_clicked()
{

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText("0");
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->tableWidget_times->setRowCount(ui->tableWidget_times->rowCount()+1);
    ui->tableWidget_times->setItem(ui->tableWidget_times->rowCount()-1,0,item);
}

void NewSpiNNaker::on_pushButton_5_clicked()
{
    QTableWidgetItem *item = ui->tableWidget_times->item(ui->tableWidget_times->currentRow(),0);
    if (item)
        ui->tableWidget_times->removeRow(ui->tableWidget_times->currentRow());
}



void NewSpiNNaker::on_bt_exportBrian2_clicked()
{
    if (ParametersOk()) {
        QString baseName = QFileInfo(ui->lineEdit_fileName->text()).baseName();
        QString extension= QFileInfo(fileName).completeSuffix();
        if (extension!="py") {
            extension=".py";
            fileName=fileName+extension;
        }
        if (!baseName.length())
            QMessageBox::warning(this, "Error","The filename is empty");
        else {
            fileName=ui->lineEdit_fileName->text();
            ui->lineEdit_fileName->setText(fileName);

            QTableWidgetItem *item = ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),0);
            if (!item) {
                QMessageBox::StandardButton reply;
                QString msg="There is no selected neuron to monitor.\nWould you still like to export this?";
                reply=QMessageBox::question(this, "Atención",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
                if (reply==QMessageBox::Yes)
                {
                    exportToBrian2();
                }
            }
            else
            {
                exportToBrian2();
            }
        }
    }
}

