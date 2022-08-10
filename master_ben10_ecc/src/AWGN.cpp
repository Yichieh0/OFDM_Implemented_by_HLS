#include "AWGN.h"
#include "top_module.hpp"

void AWGN(hls::stream<ap_fixed<IN_WL,IN_IL>>& channel_in_real, hls::stream<ap_fixed<IN_WL,IN_IL>>& channel_in_imag, hls::stream<T_in> FFT_in[SSR], ap_fixed<IN_WL,IN_IL> sqrt_No, ap_uint<32> TAPS_NUM, ap_uint<32> pilot_width){
#pragma HLS dataflow

  T_in FFT_in_temp;
  ap_fixed<IN_WL,IN_IL> temp;
  ap_fixed<IN_WL,IN_IL> random_num_value;
  ap_fixed<IN_WL,IN_IL> fft_in_real;
  ap_fixed<IN_WL,IN_IL> fft_in_imag;
  hls::stream<ap_fixed<IN_WL,IN_IL>, (FFT_LEN+CP_length)> random_num;
  hls::stream<ap_fixed<IN_WL,IN_IL>, (FFT_LEN+CP_length)> channel_out_real;
  hls::stream<ap_fixed<IN_WL,IN_IL>, (FFT_LEN+CP_length)> channel_out_imag;
  

  for (int t = 0; t < ((FFT_LEN+CP_length)/SSR); ++t) {
    for(int p = 0; p < SSR; p++){
      channel_gen(channel_in_real, channel_in_imag, channel_out_real, channel_out_imag, TAPS_NUM, pilot_width);      
      
      rand(random_num);
      random_num_value = random_num.read()*sqrt_No/sqrt_2;
      temp = channel_out_real.read()+random_num_value;
      FFT_in_temp.real(temp);
      
      rand(random_num);
      
      random_num_value = random_num.read()*sqrt_No/sqrt_2;
      temp = channel_out_imag.read()+random_num_value;
      FFT_in_temp.imag(temp);
      
      fft_in_real = FFT_in_temp.real();
      fft_in_imag = FFT_in_temp.imag();   

      FFT_in[p].write(T_in(fft_in_real,fft_in_imag)); 
    }
  }
  return;
}
