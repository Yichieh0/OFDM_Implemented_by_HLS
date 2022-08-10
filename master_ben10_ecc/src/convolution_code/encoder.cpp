#include <iostream>
#include "encoder.h"

//void encoder(hls::stream<ap_uint<pixl_bit>>& input, hls::stream<ap_uint<pixl_bit>>& x1, hls::stream<ap_uint<pixl_bit>>& x2){
void encoder(hls::stream<ap_uint<pixl_bit>, FFT_LEN>& input, hls::stream<ap_uint<QAM_unsigned_bit>, FFT_LEN>& x, hls::stream<ap_uint<6>>& s_out, ap_uint<32> pilot_width){
	static ap_uint<6> s = 0;
//#pragma HLS ARRAY_PARTITION variable=s type=complete
	ap_uint<pixl_bit> in;
	ap_uint<QAM_unsigned_bit> x_temp;

	ap_uint<pixl_bit> temp;
#pragma HLS dataflow
	for(int i = 0; i <((FFT_LEN-(FFT_LEN/pilot_width))) ; i++){
		if((i%4)==0){
			in = input.read();
      for(int j=0 ; j<pixl_bit/QAM_unsigned_bit ; j++){
				x_temp[j*2] = in[(i%4)*2+j]^s[1]^s[2]^s[4]^s[5];
				x_temp[j*2+1] = in[(i%4)*2+j]^s[0]^s[1]^s[2]^s[5];
				s =  s << 1;
				s[0] = in[(i%4)*2+j];
			}
			x.write(x_temp);
      
      
		}
		else if((i%4)==1){
			//temp = input.read();//-----------------------------------//
			for(int j=0 ; j<pixl_bit/QAM_unsigned_bit ; j++){
				x_temp[j*2] = in[(i%4)*2+j]^s[1]^s[2]^s[4]^s[5];
				x_temp[j*2+1] = in[(i%4)*2+j]^s[0]^s[1]^s[2]^s[5];
				s =  s << 1;
				s[0] = in[(i%4)*2+j];
			}
			x.write(x_temp);
		}
		else if((i%4)==2){
			for(int j=0 ; j<pixl_bit/QAM_unsigned_bit ; j++){
				x_temp[j*2] = in[(i%4)*2+j]^s[1]^s[2]^s[4]^s[5];
				x_temp[j*2+1] = in[(i%4)*2+j]^s[0]^s[1]^s[2]^s[5];
				s =  s << 1;
				s[0] = in[(i%4)*2+j];
			}
			x.write(x_temp);
		}
		else{
			for(int j=0 ; j<pixl_bit/QAM_unsigned_bit ; j++){
				x_temp[j*2] = in[(i%4)*2+j]^s[1]^s[2]^s[4]^s[5];
				x_temp[j*2+1] = in[(i%4)*2+j]^s[0]^s[1]^s[2]^s[5];
				s =  s << 1;
				s[0] = in[(i%4)*2+j];
			}
			x.write(x_temp);
		}
	}
	//printf("s = %d\n",(int)s);
	s_out.write(s);
  //std::cout << "s_push" << std::endl;
	return;
}
