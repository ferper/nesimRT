#-------------------------------------------------
#
# Project created by QtCreator 2018-12-04T06:38:52
#
#-------------------------------------------------
QT       += core gui widgets
QT       += network widgets
QT       += xml
requires(qtConfig(udpsocket))

# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nesimRT
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG+=wwwidgets


SOURCES += \
        main.cpp \
    models/neuron_adexlif.cpp \
    models/neuron_cubalif.cpp \
    models/neuron_izhikevich.cpp \
    spikeGenerator.cpp \
    neuron.cpp \
    synapse.cpp \
    parameters.cpp \
    Widget.cpp \
    TickerGraph.cpp \
    WidgetGeneralMonitor.cpp \
    motherNeuron.cpp \
    encodeDecodeMsg.cpp \
    scheme/newneuronGraphic.cpp \
    scheme/newsynapseGraphic.cpp \
    scheme/node.cpp \
    scheme/mainGraphics.cpp \
    mainScreen.cpp \
    scheme/newgeneratorGraphic.cpp \
    scheme/model_adexlif.cpp \
    scheme/removeitem.cpp \
    publicmsg.cpp \
    about.cpp \
    scheme/newspinnaker.cpp \
    scheme/junctionsDraw.cpp

HEADERS += \
    models/neuron_adexlif.h \
    models/neuron_cubalif.h \
    models/neuron_izhikevich.h \
    spikeGenerator.h \
    neuron.h \
    synapse.h \
    parameters.h \
    Widget.h \
    TickerGraph.h \
    WidgetGeneralMonitor.h \
    messages.h \
    config.h \
    motherNeuron.h \
    motherNeuron.h \
    encodeDecodeMsg.h \
    typedata.h \
    scheme/newneuronGraphic.h \
    scheme/newsynapseGraphic.h \
    scheme/node.h \
    scheme/mainGraphics.h \
    mainScreen.h \
    scheme/newgeneratorGraphic.h \
    scheme/model_adexlif.h \
    scheme/removeitem.h \
    publicmsg.h \
    about.h \
    scheme/newspinnaker.h \
    scheme/junctionsDraw.h

FORMS += \
        mainwindow.ui \
    Widget.ui \
    generalmonitor.ui \
    scheme/newneuronform.ui \
    scheme/newsynapseform.ui \
    scheme/newgeneratorform.ui \
    scheme/model_adexlif.ui \
    scheme/removeitem.ui \
    Formabout.ui \
    scheme/newspinnaker.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    scheme/NeuronsScheme.pro

DISTFILES += \
    scheme/graphics/Neuron.png \
    scheme/graphics/equation_IZHIKEVICH.png

RESOURCES += \
    scheme/graphics/graphics.qrc
