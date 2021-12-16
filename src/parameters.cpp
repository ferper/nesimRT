#include "parameters.h"

Parameters::Parameters(double V,double Iexc,  double Iinh,double tau_e,double tau_i, double tau_v,
               double R, double Vr, double Vrh, double Vth, double At) {

    this->V=V;
    this->Iexc=Iexc;
    this->Iinh=Iinh;
    this->tau_e=tau_e;
    this->tau_i=tau_i;
    this->tau_v=tau_v;
    this->R=R;
    this->Vr=Vr;
    this->Vrh=Vrh;
    this->Vth=Vth;
    this->At=At;
}

Parameters& Parameters::operator = (const Parameters &p){

    if(this!=&p){ // It is verified that an object is not trying to match itself

        this->Iexc=p.Iexc;
        this->Iinh=p.Iinh;
        this->tau_e=p.tau_e;
        this->tau_i=p.tau_i;
        this->tau_v=p.tau_v;
        this->R=p.R;
        this->Vr=p.Vr;
        this->Vrh=p.Vrh;
        this->Vth=p.Vth;
        this->At=p.At;
        this->V=p.V;
    }
    return *this;
}

