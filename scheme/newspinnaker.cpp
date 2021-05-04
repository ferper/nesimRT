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
    cm=0.25;
    i_offset=0.0;
    tau_m=20.0;
    tau_refrac=2.0;
    tau_syn_E=50.0;
    tau_syn_I=5.0;
    v_reset=-70.0;
    v_rest=-65.0;
    v_thresh=-55.0;
    timeStep=0.25;
    delay=0;
    timeSimulation=50;

    ui->lineEdit_cm->setText(QString::number(cm));
    ui->lineEdit_i_offset->setText(QString::number(i_offset));
    ui->lineEdit_tau_m->setText(QString::number(tau_m));
    ui->lineEdit_tau_refrac->setText(QString::number(tau_refrac));
    ui->lineEdit_tau_syn_E->setText(QString::number(tau_syn_E));
    ui->lineEdit_tau_syn_I->setText(QString::number(tau_syn_I));
    ui->lineEdit_v_reset->setText(QString::number(v_reset));
    ui->lineEdit_v_rest->setText(QString::number(v_rest));
    ui->lineEdit_v_thresh->setText(QString::number(v_thresh));
    ui->lineEdit_timeStep->setText(QString::number(timeStep));
    ui->lineEdit_delay->setText(QString::number(delay));
    ui->lineEdit_timeSimulation->setText(QString::number(timeSimulation));

    fillAllNeurons();
    connect(ui->lineEdit_Neuron, SIGNAL(textChanged(const QString &)), this, SLOT(neuronFilterSearch()));

    QGraphicsScene *sceneTmp = new QGraphicsScene();
    QPixmap p0("scheme/graphics/neuron.png");

    sceneTmp->addPixmap(p0);
    ui->graphicsView->setScene(sceneTmp);
    sceneTmp = new QGraphicsScene();
    QPixmap p1("scheme/graphics/equation.png");
    sceneTmp->addPixmap(p1);

    ui->graphicsView_2->setScene(sceneTmp);

    this->setWindowTitle("Export to SpiNNaker");
    ui->lineEdit_cm->setFocus();

    QPixmap pixmap_ok("scheme/graphics/createNeuron.png");
    QIcon ButtonIcon(pixmap_ok);
    ui->pushButton->setIcon(ButtonIcon);
    QSize s0(ui->pushButton->rect().size().width()-10,ui->pushButton->rect().size().height()-10);
    ui->pushButton->setIconSize(s0);
    ui->pushButton->setFixedSize(ui->pushButton->rect().size());
    ui->pushButton->setFlat(true);

    QPixmap pixmap_cancel("scheme/graphics/cancel.png");
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
       case Qt::Key_Escape: // si se pulsa ESCAPE se cierra la ventana
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

    cm=ui->lineEdit_cm->text().toFloat(&ok);
    if (!ok) {
       QMessageBox::warning(this, "Error","El valor para el campo cm no es correcto.");
       ui->lineEdit_cm->setFocus();
       error=true;
    }
    if (!error) {
        i_offset=ui->lineEdit_i_offset->text().toFloat(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo i_offset no es correcto.");
            ui->lineEdit_i_offset->setFocus();
            error=true;
        }
    }
    if (!error) {
        tau_m=ui->lineEdit_tau_m->text().toFloat(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo tau_m no es correcto.");
            ui->lineEdit_tau_m->setFocus();
            error=true;
        }
    }
    if (!error) {
        tau_refrac=ui->lineEdit_tau_refrac->text().toFloat(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo tau_refrac no es correcto.");
            ui->lineEdit_tau_refrac->setFocus();
            error=true;
        }
    }
    if (!error) {
        tau_syn_E=ui->lineEdit_tau_syn_E->text().toFloat(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo tau_syn_E no es correcto.");
            ui->lineEdit_tau_syn_E->setFocus();
            error=true;
        }
    }

    if (!error) {
        tau_syn_I=ui->lineEdit_tau_syn_I->text().toFloat(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo tau_syn_I no es correcto.");
            ui->lineEdit_tau_syn_I->setFocus();
            error=true;
        }
    }

    if (!error) {
        v_reset=ui->lineEdit_v_reset->text().toFloat(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo v_reset no es correcto.");
            ui->lineEdit_v_reset->setFocus();
            error=true;
        }
    }

    if (!error) {
        v_rest=ui->lineEdit_v_rest->text().toFloat(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo v_rest no es correcto.");
            ui->lineEdit_v_rest->setFocus();
            error=true;
        }
    }

    if (!error) {
        v_thresh=ui->lineEdit_v_thresh->text().toFloat(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo v_thresh no es correcto.");
            ui->lineEdit_v_thresh->setFocus();
            error=true;
        }
    }
    if (!error) {
         timeStep=ui->lineEdit_timeStep->text().toFloat(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo timeStep no es correcto.");
            ui->lineEdit_timeStep->setFocus();
            error=true;
        }
    }

    if (!error) {
         delay=ui->lineEdit_delay->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo delay no es correcto.");
            ui->lineEdit_delay->setFocus();
            error=true;
        }
    }
    if (!error) {
         timeSimulation=ui->lineEdit_timeSimulation->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error","El valor para el campo Time Simulation no es correcto.");
            ui->lineEdit_timeSimulation->setFocus();
            error=true;
        }
    }
    if (!error) {
       ok=true;
       for (int i=0; i<ui->tableWidget_times->rowCount() && ok ;i++)
          int t=ui->tableWidget_times->item(i,0)->text().toInt(&ok);
       if (!ok) {
          QMessageBox::warning(this, "Error","Hay un valor icorrecto en la tabla de spikes time.");
          error=true;
       }
    }

    return !error;

}
void NewSpiNNaker::exportToSpiNNaker(){

   if (fileName.length()) {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
              return;
            QTextStream out(&file);
            out << "#################################################################"<<"\n";
            out << "#                                                               #"<<"\n";
            out << "#     ********************   NESIM-RT  ********************     #"<<"\n";
            out << "#     -----------------------------------------------------     #"<<"\n";
            out << "#          A Real Time Spiking Neural Network Simulator         #"<<"\n";
            out << "#     -----------------------------------------------------     #"<<"\n";
            out << "#                                                               #"<<"\n";
            out << "#     Author: Daniel Jesus Rosa Gallardo                        #"<<"\n";
            out << "#     email: dani.rosagallardo@mail.uca.es                      #"<<"\n";
            out << "#     Date: 2019                                                #"<<"\n";
            out << "#     Cadiz University - Spain                                  #"<<"\n";
            out << "#                                                               #"<<"\n";
            out << "#################################################################"<<"\n";
            out << "\n";
            out << "import pyNN.spiNNaker as sim"<< "\n";
            out << "import pyNN.utility.plotting as plot"<< "\n";
            out << "import matplotlib.pyplot as plt"<< "\n";
            out << "sim.setup(timestep="<< QString::number(timeStep)<<")"<<"\n";
            out << "\n";
            out << "cell_params_lif = {"<<"\n";
            out << "\t\t'cm': "<<QString::number(cm)<<",\n";
            out << "\t\t'i_offset': "<<QString::number(i_offset)<<",\n";
            out << "\t\t'tau_m': "<<QString::number(tau_m)<<",\n";
            out << "\t\t'tau_refrac': "<<QString::number(tau_refrac)<<",\n";
            out << "\t\t'tau_syn_E': "<<QString::number(tau_syn_E)<<",\n";
            out << "\t\t'tau_syn_I': "<<QString::number(tau_syn_I)<<",\n";
            out << "\t\t'v_reset': "<<QString::number(v_reset)<<",\n";
            out << "\t\t'v_rest': "<<QString::number(v_rest)<<",\n";
            out << "\t\t'v_thresh': "<<QString::number(v_thresh)<<"\n";
            out << "}" << "\n\n";

            out <<"sim.set_number_of_neurons_per_core(sim.IF_curr_exp, 100)"<<"\n";

            out <<"\n";
            /* Exportamos todas las neuronas incluido los Generadores*/
            Node *nodeSource;
            for (int i=0; i<vectorGraphicsNodes->size();i++ ) {
                        nodeSource=vectorGraphicsNodes->at(i);
               QString labelNeuron= nodeSource->label+"_"+nodeSource->id;
               if (nodeSource->typeNode==TYPENEURON_GENERATOR) {
                   out<< labelNeuron<<"= sim.Population(1,sim.SpikeSourceArray(spike_times=[";
                    int s=ui->tableWidget_times->rowCount();
                   for (int j=0; j<ui->tableWidget_times->rowCount();j++) {
                      out<<ui->tableWidget_times->item(j,0)->text();
                      if (j!=ui->tableWidget_times->rowCount()-1)
                          out <<",";
                    }
                   out<<"],label=\""<<labelNeuron<<"\"))"<<"\n";
               }
               else if (nodeSource->typeNode==TYPENEURON_NORMAL) {
                   out<< labelNeuron<<"= sim.Population("<<nodeSource->amountOfNeurons<<",sim.IF_curr_exp(**cell_params_lif), label=\""<<labelNeuron<<"\")"<<"\n";
               }

            }
            out << "\n";
            /* Exportamos todas las synapsis */
            Node *nodeTarget;
            for (int i=0; i<vectorGraphicsNodes->size();i++ ) {
                nodeSource=vectorGraphicsNodes->at(i);
                QString NeuronS=nodeSource->label+"_"+nodeSource->id;
                for (int j=0; j<nodeSource->synapsys.size();j++) {
                   nodeTarget= findNode("ip",nodeSource->synapsys.at(j).ipmTarget);
                   QString NeuronT=nodeTarget->label+"_"+nodeTarget->id;
                     out<<"input_"<<NeuronT<<NeuronS<<"=sim.Projection("<<NeuronT<<","<<NeuronS<<", sim.OneToOneConnector(), synapse_type=sim.StaticSynapse(weight="<<nodeSource->synapsys.at(j).w<<", delay="<<delay<<"))"<<"\n";
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
            out<< "print spikes"<<"\n";
            out<< "v = neo.segments[0].filter(name='v')[0]"<<"\n";
            out<< "print v"<<"\n";
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

            QMessageBox::information(this, "Informacion","La exportación se ha realizado satisfactoriamente.");

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
        ui->tableWidget_Neurons->setColumnWidth(2,80); //Es 0 para que no se muestre

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
                QString msg="No hay ninguna neurona seleccionada para monitorizar.\n¿Desea continuar con la exportación?";
                reply=QMessageBox::question(this, "Atención",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
                if (reply==QMessageBox::Yes)
                    exportToSpiNNaker();
            }
            else
               exportToSpiNNaker();
        }
    }
}

void NewSpiNNaker::on_pushButton_3_clicked()
{
   QFileDialog fileDialog(this, tr("Export to SpiNNaker"), QDir::currentPath(),tr("SpiNNaker Files (*.py)"));
   fileDialog.setOption(QFileDialog::DontUseNativeDialog,true); //Very important for refresh
   fileDialog.setAcceptMode(QFileDialog::AcceptSave);
   if (QDialog::Accepted != fileDialog.exec())
      return ;

   fileDialog.window()->setFocus();
   fileName=fileDialog.selectedFiles().at(0);

   if (fileName.isEmpty())
      return;
   fileName=fileName+".py";
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


