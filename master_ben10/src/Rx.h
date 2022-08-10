#ifndef __RX_H__
#define __RX_H__
#include "data_path.hpp"

//void Rx(hls::stream<T_in> FFT_in[SSR], hls::stream<ap_uint<pixl_bit>>& pic_out, ap_uint<32> pilot_width);
void Rx(hls::stream<T_in> FFT_in[SSR], hls::stream<ap_uint<pixl_bit>>& pic_out, hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN>& deQAM_output_real, hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN>& deQAM_output_imag, ap_uint<32> pilot_width, ap_uint<32> TAPS_NUM);

#endif
