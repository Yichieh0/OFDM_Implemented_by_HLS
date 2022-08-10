#ifndef __TOP_H__
#define __TOP_H__
#include "master_mem/master2stream.h"
#include "master_mem/stream2master.h"
#include "master_mem/stream2master_output_QAM.h"
#include "top_module.hpp"

#include "data_path.hpp"

//void master_top(ap_uint<32> *in_mem, ap_uint<32> *out_mem, ap_uint<32> DATA_LEN, ap_uint<32> SNR, ap_uint<32> TAPS_NUM, ap_uint<32> pilot_width);
void master_top(ap_uint<32> *in_mem, ap_uint<32> *out_mem, ap_uint<32> *out_mem_deQAM_real, ap_uint<32> *out_mem_deQAM_imag, ap_uint<32> DATA_LEN, ap_uint<32> SNR, ap_uint<32> TAPS_NUM, ap_uint<32> pilot_width);

#endif
