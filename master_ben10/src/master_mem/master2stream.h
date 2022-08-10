#ifndef __MASTER2STREAM_H__
#define __MASTER2STREAM_H__

#include "../data_path.hpp"

void master2stream(ap_uint<32> *in_mem, hls::stream<ap_uint<pixl_bit>>& pstrmOutput, ap_uint<32> DATA_LEN);

#endif
