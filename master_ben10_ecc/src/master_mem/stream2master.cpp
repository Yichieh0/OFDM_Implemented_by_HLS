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

#include "stream2master.h"

//void stream2master(hls::stream<ap_axiu<pixl_bit,1,1,1>, DATA_LENGTH>& pstrmInput, ap_uint<32>* out_mem){
void stream2master(hls::stream<ap_uint<pixl_bit>>& pstrmInput, ap_uint<32>* out_mem, ap_uint<32> DATA_LEN){
#pragma HLS dataflow

  ap_uint<pixl_bit> temp;
  ap_uint<32> in_temp;
  //ap_axiu<pixl_bit,1,1,1> out_temp;
  ap_uint<pixl_bit> out_temp;
  //static int i = 0;
  
  //while(1){
  //do{
  for(int i = 0; i<DATA_LEN; i++){
//#pragma HLS pipeline   
     
     //printf("read i = %d\n", i);
     //printf("front : %d\t", out_temp.last);
     out_temp = pstrmInput.read();
     //printf("after : %d\n", out_temp.last);
     temp = out_temp;
     in_temp = (ap_uint<32>)temp;
     out_mem[i] = in_temp;
     //i = i + 1;
     //if(out_temp.last==0) break;
  }//while(out_temp.last!=0);


  return;
}
