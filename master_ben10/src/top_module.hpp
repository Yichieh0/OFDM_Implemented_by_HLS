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

#include "data_path.hpp"

#include "QAM/qam.hpp"
#include "picture_to_symbol/pixl_to_symbol.h"
#include "deQAM_to_pixl/data2pic.h"
#include "fft/fft.hpp"
#include "channel_stream/channel_gen.h"
#include "pilot_insertion/pilot_insertion.h"
#include "CP_insertion/Cp_insertion.h"
#include "Rx.h"
#include "Tx.h"
#include "AWGN.h"
#include "equalizer/equalizer.hpp"
#include "channel_stream/channel_gen.h"

//void top_module(hls::stream<ap_uint<pixl_bit>, DATA_LENGTH>& in,hls::stream<ap_axiu<pixl_bit,1,1,1>, DATA_LENGTH>& pic_out);
//void top_module(hls::stream<ap_uint<pixl_bit>, DATA_LENGTH>& in,hls::stream<ap_uint<pixl_bit>, DATA_LENGTH>& pic_out);
//void top_module(hls::stream<ap_uint<pixl_bit>>& in,hls::stream<ap_uint<pixl_bit>>& pic_out, ap_uint<32> DATA_LEN, ap_uint<32> SNR, ap_uint<32> TAPS_NUM, ap_uint<32> pilot_width);
void top_module(hls::stream<ap_uint<pixl_bit>>& in,hls::stream<ap_uint<pixl_bit>>& pic_out, hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN>& deQAM_output_real, hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN>& deQAM_output_imag, ap_uint<32> DATA_LEN, ap_uint<32>SNR, ap_uint<32> TAPS_NUM, ap_uint<32> pilot_width);

