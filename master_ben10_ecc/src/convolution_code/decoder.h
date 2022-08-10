#ifndef __DECODER_H__
#define __DECODER_H__
#include "../data_path.hpp"

void decoder_main(hls::stream<ap_uint<QAM_unsigned_bit>>& QAM_in,hls::stream<ap_uint<pixl_bit>, FFT_LEN>& output_pixl, hls::stream<ap_uint<6>>& s_out, ap_uint<32> DATA_LEN);
void decoder(hls::stream<bool>& y0_in, hls::stream<bool>& y1_in , int mode , hls::stream<bool>& output);

#endif
