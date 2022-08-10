#include "Rx.h"
#include "top_module.hpp"

void Rx(hls::stream<T_in> FFT_in[SSR], hls::stream<ap_uint<pixl_bit>>& pic_out, hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN>& deQAM_output_real, hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN>& deQAM_output_imag, ap_uint<32> pilot_width, ap_uint<32> TAPS_NUM){
#pragma HLS dataflow
	//#pragma HLS interface ap_ctrl_none port = return

	hls::stream<T_in, (FFT_LEN/SSR)> FFT_in_temp[SSR];
	hls::stream<T_out1, (FFT_LEN/SSR)> FFT_out[SSR];
	hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> data_real;
	hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> data_imag;
	hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> Equal_in[SSR];
	hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> Equal_out[SSR];
	ap_fixed<IN_WL,IN_IL> cp_imag;
	ap_fixed<IN_WL,IN_IL> cp_real;
	T_in CP_remove;
	T_in data_temp;
	T_out1 equal_temp;

	for(int t=0 ; t<((FFT_LEN+CP_length)/SSR) ; t++){
		for(int p=0 ; p<SSR ; p++){
			if(t<(CP_length/SSR)){
				CP_remove = FFT_in[p].read();
				cp_real = CP_remove.real();
				cp_imag = CP_remove.imag();
			}
			else{
				FFT_in_temp[p].write(FFT_in[p].read());
			}
		}
	}
	fft_top1(FFT_in_temp,FFT_out);

	for (int t = 0; t < (FFT_LEN/2); t++) {
		for(int s = 0; s < SSR; s++){
			equal_temp = FFT_out[s].read();
			Equal_in[0].write((ap_fixed<IN_WL,IN_IL>)equal_temp.real()/sqrt_FFT_LENGTH);
			Equal_in[1].write((ap_fixed<IN_WL,IN_IL>)equal_temp.imag()/sqrt_FFT_LENGTH);
		}
	}
  if(TAPS_NUM==1){
    for (int t = 0; t < (FFT_LEN); t++) {
    #pragma HLS array_partition variable=Equal_in type=complete
  			Equal_out[0].write(Equal_in[0].read());
  			Equal_out[1].write(Equal_in[1].read());
  	}
  }else{
  	equalizer(Equal_in, Equal_out, pilot_width) ;
  }
  
	for (int k = 0; k < FFT_LEN ; k++) {
#pragma HLS array_partition variable=Equal_out type=complete
		data_temp.real(Equal_out[0].read());
		data_temp.imag(Equal_out[1].read());
		if((k%pilot_width)!=0){
			data_real.write(data_temp.real()*sqrt_QAM_norm);
			data_imag.write(data_temp.imag()*sqrt_QAM_norm);
      //---------------------------deQAM output---------------------//
      deQAM_output_real.write(data_temp.real()*sqrt_QAM_norm);
			deQAM_output_imag.write(data_temp.imag()*sqrt_QAM_norm);
      //------------------------------------------------------------//
			data_out_main(data_real,data_imag,pic_out);
		}
	}

	return;
}
