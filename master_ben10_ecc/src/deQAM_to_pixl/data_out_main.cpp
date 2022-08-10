#include "data2pic.h"
#include "../convolution_code/decoder.h"
void data_out_main(hls::stream<ap_fixed<IN_WL,IN_IL>>& data_real, hls::stream<ap_fixed<IN_WL,IN_IL>>& data_imag, hls::stream<ap_uint<pixl_bit>, FFT_LEN>& data_out, hls::stream<ap_uint<6>>& s_out, ap_uint<32> DATA_LEN)
{
#pragma HLS dataflow

	hls::stream<ap_uint<QAM_unsigned_bit>> QAM_out;

	deQAM(data_real, data_imag, QAM_out);
  decoder_main(QAM_out, data_out, s_out, DATA_LEN);
  
	//data2pic(QAM_out, data_out);

	return;
}
