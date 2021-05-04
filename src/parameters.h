#ifndef PARAMETERS_H
#define PARAMETERS_H


class Parameters
{
public:
    explicit Parameters(double V=0, double Iexc=0, double Iinh=0,double tau_e=0,double tau_i=0, double tau_v=0,
               double R=0, double Vr=0, double Vrh=0, double Vth=0, double At=0);
    Parameters& operator = (const Parameters &p);

    double V;
    double Iexc;
    double Iinh;
    double tau_e;
    double tau_i;
    double tau_v;
    double R;
    double Vr;
    double Vrh;
    double Vth;
    double At;
};

#endif // PARAMETERS_H
