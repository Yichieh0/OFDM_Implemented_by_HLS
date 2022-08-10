#ifndef __ENCODER_H__
#define __ENCODER_H__
#include "../data_path.hpp"

//void encoder(hls::stream<ap_uint<1>>& input, hls::stream<ap_uint<1>>& x1, hls::stream<ap_uint<1>>& x2);
//void encoder(hls::stream<ap_uint<pixl_bit>>& input, hls::stream<ap_uint<pixl_bit*2>>& x);
void encoder(hls::stream<ap_uint<pixl_bit>, FFT_LEN>& input, hls::stream<ap_uint<QAM_unsigned_bit>, FFT_LEN>& x, hls::stream<ap_uint<6>>& s_out, ap_uint<32> pilot_width);
#endif
