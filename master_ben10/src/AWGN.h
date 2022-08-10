#include "data_path.hpp"

void AWGN(hls::stream<ap_fixed<IN_WL,IN_IL>>&channel_in_real, hls::stream<ap_fixed<IN_WL,IN_IL>>&channel_in_imag, hls::stream<T_in> FFT_in[SSR], ap_fixed<IN_WL,IN_IL> sqrt_No, ap_uint<32> TAPS_NUM, ap_uint<32> pilot_width);

//(FFT_LEN+CP_length)/SSR>& FFT_in);
