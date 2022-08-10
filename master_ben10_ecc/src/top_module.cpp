/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

*/
//================================== End Lic =================================================
#include <iostream>
#include "top_module.hpp"
#include "data_path.hpp"

//void top_module(hls::stream<ap_uint<pixl_bit>, DATA_LENGTH>& in,hls::stream<ap_axiu<pixl_bit,1,1,1>, DATA_LENGTH>& pic_out){
//void top_module(hls::stream<ap_uint<pixl_bit>, DATA_LENGTH>& in,hls::stream<ap_uint<pixl_bit>, DATA_LENGTH>& pic_out){
void top_module(hls::stream<ap_uint<pixl_bit>, FFT_LEN>& in,hls::stream<ap_uint<pixl_bit>, FFT_LEN>& pic_out, hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN>& deQAM_output_real, hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN>& deQAM_output_imag, ap_uint<32> DATA_LEN, ap_uint<32>SNR, ap_uint<32> TAPS_NUM, ap_uint<32> pilot_width){


#pragma HLS dataflow
  ap_fixed<IN_WL,IN_IL> real_temp;
  ap_fixed<IN_WL,IN_IL> imag_temp;
  hls::stream<ap_fixed<IN_WL,IN_IL>, (CP_length+FFT_LEN)> channel_in_real;
  hls::stream<ap_fixed<IN_WL,IN_IL>, (CP_length+FFT_LEN)> channel_in_imag;
  hls::stream<T_in, ((CP_length+FFT_LEN)/SSR)> FFT_in[SSR];
  const ap_fixed<IN_WL,IN_IL> sqrt_No = (ap_fixed<IN_WL,IN_IL>)(1/sqrt(pow(10.0,SNR/10.0)));
  
  hls::stream<ap_uint<6>> s_out;
  ap_uint<6> s_out_temp;
  
  for(int w = 0; w<(int)(DATA_LEN/((FFT_LEN-(FFT_LEN/pilot_width))/SSR))*2; w++){
    Tx(in, channel_in_real, channel_in_imag, s_out, pilot_width);
    if(w < (DATA_LEN/((FFT_LEN-(FFT_LEN/pilot_width))/2))*2-1){
			s_out_temp = s_out.read();
      //std::cout << "s_pull" << std::endl;
		}
    AWGN(channel_in_real, channel_in_imag, FFT_in, sqrt_No, TAPS_NUM, pilot_width);    
    Rx(FFT_in, pic_out, deQAM_output_real, deQAM_output_imag, s_out, DATA_LEN, pilot_width, TAPS_NUM);
  }
	return;
}
