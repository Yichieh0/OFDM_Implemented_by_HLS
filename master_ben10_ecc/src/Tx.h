#include "data_path.hpp"


void Tx(hls::stream<ap_uint<pixl_bit>, FFT_LEN>& pic_in, hls::stream<ap_fixed<IN_WL,IN_IL>>& data_out_real, hls::stream<ap_fixed<IN_WL,IN_IL>>& data_out_imag, hls::stream<ap_uint<6>>& s_out, ap_uint<32> pilot_width);
