#include "../data_path.hpp"

void equalizer(hls::stream<ap_fixed<IN_WL, IN_IL>> in[SSR], hls::stream<ap_fixed<IN_WL, IN_IL>> out[SSR], ap_uint<32> pilot_width) ;
