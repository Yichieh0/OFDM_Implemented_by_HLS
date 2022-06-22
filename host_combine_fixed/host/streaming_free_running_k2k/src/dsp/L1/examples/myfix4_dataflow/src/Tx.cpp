#include "Tx.h"
#include "top_module.hpp"

void Tx(hls::stream<ap_uint<pixl_bit>>& pic_in, hls::stream<ap_fixed<IN_WL,IN_IL>>& data_out_real, hls::stream<ap_fixed<IN_WL,IN_IL>>& data_out_imag){
//#pragma HLS dataflow
//#pragma HLS INTERFACE mode= ap_ctrl_chain bundle=control
//#pragma HLS interface ap_ctrl_none port = return
//#pragma HLS PIPELINE
  ap_fixed<IN_WL,IN_IL> ifft_in_real;
  ap_fixed<IN_WL,IN_IL> ifft_in_imag;  
  hls::stream<ap_uint<QAM_unsigned_bit>> symbol;
  hls::stream<T_in_int> out;
  hls::stream<T_in,FFT_LEN/SSR> IFFT_in[SSR];
  hls::stream<T_out0,FFT_LEN/SSR> IFFT_out[SSR];
  hls::stream<ap_fixed<IN_WL,IN_IL>,FFT_LEN> pilot_in_real;
  hls::stream<ap_fixed<IN_WL,IN_IL>,FFT_LEN> pilot_in_imag;
  hls::stream<ap_fixed<IN_WL,IN_IL>,FFT_LEN> pilot_temp_real;
  hls::stream<ap_fixed<IN_WL,IN_IL>,FFT_LEN> pilot_temp_imag;
  hls::stream<ap_fixed<IN_WL,IN_IL>,FFT_LEN> cp_in_real;
  hls::stream<ap_fixed<IN_WL,IN_IL>,FFT_LEN> cp_in_imag;
  hls::stream<ap_fixed<IN_WL,IN_IL>,(FFT_LEN+CP_length)> cp_out_real;
  hls::stream<ap_fixed<IN_WL,IN_IL>,(FFT_LEN+CP_length)> cp_out_imag;
  T_in IFFT_in_temp;
  T_in_int data_temp_int;
  
  for (int t = 0; t < ((FFT_LEN-(FFT_LEN/pilot_width)) / SSR); ++t) {
    pixl_to_symbol(pic_in, symbol);
    for(int i=0;i<SSR;i++){
      QAM(symbol, out);
      data_temp_int = out.read();
      pilot_in_real.write((ap_fixed<IN_WL,IN_IL>)data_temp_int.real());
      pilot_in_imag.write((ap_fixed<IN_WL,IN_IL>)data_temp_int.imag());
      pilot_insertion(pilot_in_real, pilot_in_imag, pilot_temp_real, pilot_temp_imag);
    }
  } 
  for (int t = 0; t < (FFT_LEN / SSR); ++t) {
    for(int i=0;i<SSR;i++){
        ifft_in_real = (pilot_temp_real.read());
        ifft_in_imag = (pilot_temp_imag.read());
        IFFT_in[i].write(T_in(ifft_in_real,ifft_in_imag));
    }
  } 
  
  fft_top0(IFFT_in, IFFT_out);
  
  for (int t = 0; t < (FFT_LEN / SSR); ++t) {
    for(int i=0;i<SSR;i++){
      IFFT_in_temp = (T_in)(IFFT_out[i].read());
      cp_in_real.write(IFFT_in_temp.real());
      cp_in_imag.write(IFFT_in_temp.imag());
      Cp_insertion(cp_in_real, cp_in_imag, cp_out_real, cp_out_imag);
    }
  } 
  
  for (int t = 0; t < (FFT_LEN+CP_length); t++){
    data_out_real.write(cp_out_real.read()*sqrt_FFT_LENGTH);
    data_out_imag.write(cp_out_imag.read()*sqrt_FFT_LENGTH);
  }

  return;
}