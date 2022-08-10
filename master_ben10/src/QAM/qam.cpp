/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

 */
//================================== End Lic =================================================

#include "qam.hpp"

void QAM(hls::stream<ap_uint<QAM_unsigned_bit>>& in, hls::stream<T_in_int>& out, ap_uint<32> pilot_width){

#pragma HLS dataflow

	T_in_int out_temp;
	ap_uint<QAM_unsigned_bit> in_temp;

	for(int t = 0; t < (FFT_LEN-(FFT_LEN/pilot_width)); t++){
		in_temp = in.read();

		if((in_temp%4)==0){
			out_temp.imag((ap_int<QAM_signed_bit>)-3);
		}
		else if((in_temp%4)==1){
			out_temp.imag((ap_int<QAM_signed_bit>)-1);
		}
		else if((in_temp%4)==2){
			out_temp.imag((ap_int<QAM_signed_bit>)3);
		}
		else{
			out_temp.imag((ap_int<QAM_signed_bit>)1);
		}

		if((in_temp/4)==0){
			out_temp.real((ap_int<QAM_signed_bit>)-3);
		}
		else if((in_temp/4)==1){
			out_temp.real((ap_int<QAM_signed_bit>)-1);
		}
		else if((in_temp/4)==2){
			out_temp.real((ap_int<QAM_signed_bit>)3);
		}
		else{
			out_temp.real((ap_int<QAM_signed_bit>)1);
		}
		out.write(out_temp);
	}

	return;
}
