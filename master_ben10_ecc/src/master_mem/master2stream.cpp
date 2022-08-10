#include "master2stream.h"

void master2stream(ap_uint<32>* in_mem, hls::stream<ap_uint<pixl_bit>>& pstrmOutput, ap_uint<32> DATA_LEN)
{	
#pragma HLS INTERFACE ap_fifo depth=DATA_LEN port=pstrmOutput
#pragma HLS DATAFLOW

  ap_uint<32> in_temp;
  ap_uint<pixl_bit> out_temp;
  static int i = 0;
  int remain_num;
  
	//while(in_mem.last != 1){
  for(int i = 0; i<DATA_LEN; i++){
//#pragma HLS pipeline
    in_temp = in_mem[i];
	  out_temp = (ap_uint<8>)in_temp;
		pstrmOutput.write(out_temp); 
  }
	return;
}
