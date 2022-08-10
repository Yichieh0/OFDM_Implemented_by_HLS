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

#include "stream2master_output_QAM.h"

void stream2master_output_QAM(hls::stream<ap_fixed<IN_WL,IN_IL>, FFT_LEN>& pstrmInput, ap_uint<32>* out_mem, ap_uint<32> DATA_LEN){
#pragma HLS dataflow
  ap_uint<32> temp_shift;
  ap_uint<32> temp;
  ap_uint<32> in_temp;
  ap_fixed<IN_WL,IN_IL> out_temp;
  
  for(int i = 0; i<DATA_LEN*4;i++){
//#pragma HLS pipeline
     out_temp = pstrmInput.read();  
     temp_shift = pow(2,IN_IL);
     temp = out_temp*temp_shift;
     in_temp = (ap_uint<32>)temp;
     out_mem[i] = in_temp;
  }

  return;
}
