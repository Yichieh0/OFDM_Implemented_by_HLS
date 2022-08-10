#include "master_top.h"

void master_top(ap_uint<32> *in_mem, ap_uint<32> *out_mem, ap_uint<32> *out_mem_deQAM_real, ap_uint<32> *out_mem_deQAM_imag, ap_uint<32> DATA_LEN, ap_uint<32> SNR, ap_uint<32> TAPS_NUM, ap_uint<32> pilot_width)
{	
#pragma HLS INTERFACE m_axi depth=384 port=in_mem bundle=gmem0
#pragma HLS INTERFACE m_axi depth=384 port=out_mem bundle=gmem0
#pragma HLS INTERFACE m_axi depth=768 port=out_mem_deQAM_real bundle=gmem1
#pragma HLS INTERFACE m_axi depth=768 port=out_mem_deQAM_imag bundle=gmem2
#pragma HLS INTERFACE s_axilite port = in_mem bundle = control
#pragma HLS INTERFACE s_axilite port = out_mem bundle = control
#pragma HLS INTERFACE s_axilite port = out_mem_deQAM_real bundle = control
#pragma HLS INTERFACE s_axilite port = out_mem_deQAM_imag bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control
#pragma HLS INTERFACE s_axilite port = DATA_LEN bundle = control
#pragma HLS INTERFACE s_axilite port = SNR bundle = control
#pragma HLS INTERFACE s_axilite port = TAPS_NUM bundle = control
#pragma HLS INTERFACE s_axilite port = pilot_width bundle = control
#pragma HLS dataflow

	hls::stream<ap_uint<pixl_bit>> temp_in;
	hls::stream<ap_uint<pixl_bit>> temp_out;
  ap_uint<pixl_bit> temp;
  
  hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> deQAM_output_real;
  hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> deQAM_output_imag;
  
	master2stream(in_mem, temp_in, DATA_LEN);
	top_module(temp_in, temp_out, deQAM_output_real, deQAM_output_imag, DATA_LEN, SNR, TAPS_NUM, pilot_width);
  stream2master(temp_out, out_mem, DATA_LEN);
  
  //-----------------------------------output_QAM------------------------//
  stream2master_output_QAM(deQAM_output_real, out_mem_deQAM_real, DATA_LEN);
  stream2master_output_QAM(deQAM_output_imag, out_mem_deQAM_imag, DATA_LEN);
  //---------------------------------------------------------------------//

	return;
}
