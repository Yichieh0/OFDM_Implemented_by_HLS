#include <iostream>
#include <math.h>
#include <hls_stream.h>
#include <cmath>

void Rayleigh_fading(double t_T, double fmT, double h_real , double h_imag){

	double temp = 2-cos(M_PI*fmT/2);
	double factor = 0;
	double thegma = 0;
	double WI = 0;
	double WQ = 0;
	hls::stream<double> random_num_WI;//ap_fixed<IN_WL,IN_IL>, (FFT_LEN+CP_length)
	hls::stream<double> random_num_WQ;//ap_fixed<IN_WL,IN_IL>, (FFT_LEN+CP_length)
	static double real_g = 0;
	static double imag_g = 0;
	factor = sqrt(pow(temp,2)-1);
	thegma = sqrt((1+factor)/(1-factor));
	rand(random_num_WI);
	rand(random_num_WQ);
	WI = thegma*random_num_WI;
	WQ = thegma*random_num_WI;
	h_real = factor*real_g+(1-factor)*WI;
	h_imag = factor*imag_g+(1-factor)*WQ;
	real_g = h_real;
	imag_g = h_imag;

}
