/*
 * Copyright 2021 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __STREAM2MASTER_OUTPUT_QAM_H__
#define __STREAM2MASTER_OUTPUT_QAM_H__

#include "../data_path.hpp"

void stream2master_output_QAM(hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN>& pstrmInput, ap_uint<32>* out_mem, ap_uint<32> DATA_LEN);

#endif
