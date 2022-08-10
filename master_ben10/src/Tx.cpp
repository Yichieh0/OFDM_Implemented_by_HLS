#include "Tx.h"
#include "top_module.hpp"

void Tx(hls::stream<ap_uint<pixl_bit>>& pic_in, hls::stream<ap_fixed<IN_WL,IN_IL>>& data_out_real, hls::stream<ap_fixed<IN_WL,IN_IL>>& data_out_imag, ap_uint<32> pilot_width){
#pragma HLS dataflow
	ap_fixed<IN_WL,IN_IL> ifft_in_real;
	ap_fixed<IN_WL,IN_IL> ifft_in_imag;
	hls::stream<ap_uint<QAM_unsigned_bit>, FFT_LEN> symbol;
	hls::stream<T_in_int, FFT_LEN> out;
	hls::stream<T_in, FFT_LEN/SSR> IFFT_in[SSR];
	hls::stream<T_out0, FFT_LEN/SSR> IFFT_out[SSR];
	hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> pilot_in_real;
	hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> pilot_in_imag;
	hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> pilot_temp_real;
	hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> pilot_temp_imag;
	hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> cp_in_real;
	hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN> cp_in_imag;
	hls::stream<ap_fixed<IN_WL,IN_IL>, (FFT_LEN+CP_length)> cp_out_real;
	hls::stream<ap_fixed<IN_WL,IN_IL>, (FFT_LEN+CP_length)> cp_out_imag;
	T_in IFFT_in_temp;
	T_in_int data_temp_int;

	pixl_to_symbol(pic_in, symbol, pilot_width);
	QAM(symbol, out, pilot_width);

	for (int t = 0; t < ((FFT_LEN-(FFT_LEN/pilot_width))); ++t) {
			data_temp_int = out.read();
			pilot_in_real.write((ap_fixed<IN_WL,IN_IL>)data_temp_int.real());
			pilot_in_imag.write((ap_fixed<IN_WL,IN_IL>)data_temp_int.imag());
	}

	pilot_insertion(pilot_in_real, pilot_in_imag, pilot_temp_real, pilot_temp_imag, pilot_width);

	for (int t = 0; t < (FFT_LEN/SSR); t++) {
		for(int i=0;i<SSR;i++){
			ifft_in_real = (pilot_temp_real.read());
			ifft_in_imag = (pilot_temp_imag.read());
			IFFT_in[i].write(T_in(ifft_in_real,ifft_in_imag));
		}
	}

	fft_top0(IFFT_in, IFFT_out);

	for (int t = 0; t < (FFT_LEN / SSR); ++t) {
		for(int i=0;i<SSR;i++){
			IFFT_in_temp = (T_in)(IFFT_out[i].read());
			cp_in_real.write(IFFT_in_temp.real());
			cp_in_imag.write(IFFT_in_temp.imag());
		}
	}

	Cp_insertion(cp_in_real, cp_in_imag, cp_out_real, cp_out_imag);

	for (int t = 0; t < (FFT_LEN+CP_length); t++){
		data_out_real.write(cp_out_real.read()*sqrt_FFT_LENGTH);
		data_out_imag.write(cp_out_imag.read()*sqrt_FFT_LENGTH);
	}

	return;
}
