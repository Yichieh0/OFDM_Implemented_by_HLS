#include <iostream>
#include <hls_stream.h>
#include <ap_int.h>
#include <ap_fixed.h>
#include "decoder.h"
using namespace std;
#define truncate 33////////////////////////////////////////////////////////////////defined truncate length

void decoder_main(hls::stream<ap_uint<QAM_unsigned_bit>>& QAM_in,hls::stream<ap_uint<pixl_bit>, FFT_LEN>& output_pixl, hls::stream<ap_uint<6>>& s_out, ap_uint<32> DATA_LEN){
	static ap_uint<2> t = 0;
	static int count = 0;
	ap_uint<QAM_unsigned_bit> QAM_in_temp;
	static ap_uint<pixl_bit> output_pixl_temp;
	hls::stream<bool> y0;
	hls::stream<bool> y1;
	hls::stream<bool> output;
	static ap_uint<6> s_out_temp = 0;
 
 static int c = 0;
#pragma HLS dataflow
	if(t==0){
		QAM_in_temp = QAM_in.read();
   //c=c+1;
		y0.write((bool)QAM_in_temp[0]);
		y1.write((bool)QAM_in_temp[1]);
		decoder(y0,y1,0,output);
		output_pixl_temp[0] = (ap_uint<1>)output.read();

		y0.write((bool)QAM_in_temp[2]);
		y1.write((bool)QAM_in_temp[3]);
		decoder(y0,y1,0,output);
		output_pixl_temp[1] = (ap_uint<1>)output.read();
		count = count + 2;
	}
	else if(t==1){
		QAM_in_temp = QAM_in.read();
   //c=c+1;
		y0.write((bool)QAM_in_temp[0]);
		y1.write((bool)QAM_in_temp[1]);
		decoder(y0,y1,0,output);
		output_pixl_temp[2] = (ap_uint<1>)output.read();

		y0.write((bool)QAM_in_temp[2]);
		y1.write((bool)QAM_in_temp[3]);
		decoder(y0,y1,0,output);
		output_pixl_temp[3] = (ap_uint<1>)output.read();
		count = count + 2;
		//output_pixl.write(0);//--------------------------------------------//
	}
	else if(t==2){
		QAM_in_temp = QAM_in.read();
   //c=c+1;
		y0.write((bool)QAM_in_temp[0]);
		y1.write((bool)QAM_in_temp[1]);
		decoder(y0,y1,0,output);
		output_pixl_temp[4] = (ap_uint<1>)output.read();

		y0.write((bool)QAM_in_temp[2]);
		y1.write((bool)QAM_in_temp[3]);
		decoder(y0,y1,0,output);
		output_pixl_temp[5] = (ap_uint<1>)output.read();
		count = count + 2;
	}
	else{
		QAM_in_temp = QAM_in.read();
   //c=c+1;
		y0.write((bool)QAM_in_temp[0]);
		y1.write((bool)QAM_in_temp[1]);
		decoder(y0,y1,0,output);
		output_pixl_temp[6] = (ap_uint<1>)output.read();

		y0.write((bool)QAM_in_temp[2]);
		y1.write((bool)QAM_in_temp[3]);
		decoder(y0,y1,0,output);
		output_pixl_temp[7] = (ap_uint<1>)output.read();
		count = count + 2;
		if(count > truncate-1){
			//cout << "count = " << count << endl;
      output_pixl.write(output_pixl_temp);
      //cout << "out_pull" << endl;
		}else{
      //cout << "pass" << endl;
    }
		if(count >= DATA_LEN*8){
			s_out_temp = s_out.read();
      //cout << "s_pull" << endl;
			//cout << "s_out_temp = " << s_out_temp << endl;
			for(int i=1 ; i<truncate ; i++){
				y0.write(0^s_out_temp[1]^s_out_temp[2]^s_out_temp[4]^s_out_temp[5]);
				y1.write(0^s_out_temp[0]^s_out_temp[1]^s_out_temp[2]^s_out_temp[5]);
				s_out_temp =  s_out_temp << 1;
				s_out_temp[0] = 0;
				decoder(y0,y1,0,output);
				output_pixl_temp[(i-1)%8] = (ap_uint<1>)output.read();
				if((i%8)==0){
					output_pixl.write(output_pixl_temp);
          
				}
			}
		}
	}
	t = t+1;
 
}









void decoder(hls::stream<bool>& y0_in, hls::stream<bool>& y1_in , int mode , hls::stream<bool>& output){
	static bool trellis_table[64][2][2];     // ns // 0:up 1:down // 0-bit 1-bit
	//static bool trellis_survivor[64][truncate] = {0};
	//#pragma HLS ARRAY_PARTITION variable=trellis_survivor type=complete

	static ap_uint<truncate> trellis_survivor[64]={0};
#pragma HLS ARRAY_PARTITION variable=trellis_survivor type=complete
	ap_uint<truncate> temp_trellis_survivor[64]={0};
	static int trellis_metric[64];
	int temp_trellis_metric[64];
	int temp;
	static bool t = 0;

	int ns_1, ns_0;

	//bool output_reg[64];
	int up , down;

	int best_metric,best_branch;
	int temp_best_metric;
	bool normal_flag = 0;

	bool y0_d,y1_d;
#pragma HLS dataflow
	///////////////////////////////////////////////////////initial///////////////////////////////
	if(t==0){
		for(int i=0 ; i<64 ; i++){  // cs = i
			if(i<32){
				ns_1 = ((i<<1)+1)&63;                                                                       //input = 1   next state
				trellis_table[ns_1][0][0] = 1^((i&2)>>1)^((i&4)>>2)^((i&16)>>4)^((i&32)>>5);                //input = 1   output0
				trellis_table[ns_1][0][1] = 1^((i&1))^((i&2)>>1)^((i&4)>>2)^((i&32)>>5);                    //input = 1   output1

				ns_0 = (i<<1)&63;                                                                           //input = 0   next state
				trellis_table[ns_0][0][0] = 0^((i&2)>>1)^((i&4)>>2)^((i&16)>>4)^((i&32)>>5);                //input = 0   output0
				trellis_table[ns_0][0][1] = 0^((i&1))^((i&2)>>1)^((i&4)>>2)^((i&32)>>5);                    //input = 0   output1
				//printf("%d     %d,%d,%d  %d,%d,%d\n",i,ns_1,trellis_table[ns_1][0][0],trellis_table[ns_1][0][1],ns_0,trellis_table[ns_0][0][0],trellis_table[ns_0][0][1]);
			}else{
				ns_1 = ((i<<1)+1)&63;                                                                       //input = 1   next state
				trellis_table[ns_1][1][0] = 1^((i&2)>>1)^((i&4)>>2)^((i&16)>>4)^((i&32)>>5);                //input = 1   output0
				trellis_table[ns_1][1][1] = 1^((i&1))^((i&2)>>1)^((i&4)>>2)^((i&32)>>5);                    //input = 1   output1

				ns_0 = (i<<1)&63;                                                                           //input = 0   next state
				trellis_table[ns_0][1][0] = 0^((i&2)>>1)^((i&4)>>2)^((i&16)>>4)^((i&32)>>5);                //input = 0   output0
				trellis_table[ns_0][1][1] = 0^((i&1))^((i&2)>>1)^((i&4)>>2)^((i&32)>>5);                    //input = 0   output1
				//printf("%d     %d,%d,%d  %d,%d,%d\n",i,ns_1,trellis_table[ns_1][1][0],trellis_table[ns_1][1][1],ns_0,trellis_table[ns_0][1][0],trellis_table[ns_0][1][1]);
			}
			trellis_metric[i] = 500;
		}
		trellis_metric[0] = 0;
		t = 1;
	}

	//////////////////////////////////////////////////////////initial_end/////////////////////////////
	y0_d = y0_in.read();
	y1_d = y1_in.read();

	for(int i=0 ; i<64 ; i++){                             //{(i-1)/2} = cs      i = ns
		if(i%2){//////////////////////////////////////////////////////////////////////////////////////////////////////////////ns odd
			up = trellis_metric[(i-1)/2]+(int)(y0_d^trellis_table[i][0][0])+(int)(y1_d^trellis_table[i][0][1]);
			//printf("cs = %d  ns = %d up = %d\n",(i-1)/2,i,up);
			down = trellis_metric[(i-1)/2+32]+(int)(y0_d^trellis_table[i][1][0])+(int)(y1_d^trellis_table[i][1][1]);        //up+32
			//printf("     cs = %d  ns = %d down = %d\n",(i-1)/2+32,i,down);
		}else{//////////////////////////////////////////////////////////////////////////////////////////////////////////////ns even
			up = trellis_metric[(i)/2]+(int)(y0_d^trellis_table[i][0][0])+(int)(y1_d^trellis_table[i][0][1]);
			down = trellis_metric[(i)/2+32]+(int)(y0_d^trellis_table[i][1][0])+(int)(y1_d^trellis_table[i][1][1]);        //up+32
		}

		if(up<=down){                                     //up smaller than down
			temp_trellis_metric[i] = up;
			normal_flag = (up>100000000)? 1 : normal_flag;
			//trellis_survivor[i] = (trellis_survivor[i]<<1);
			temp_trellis_survivor[i] = (trellis_survivor[i]<<1);
			trellis_survivor[i] = temp_trellis_survivor[i];

		}else{
			temp_trellis_metric[i] = down;
			normal_flag = (down>100000000)? 1 : normal_flag;
			//trellis_survivor[i] = (trellis_survivor[i]<<1)^1;
			temp_trellis_survivor[i] = (trellis_survivor[i]<<1)^1;
			trellis_survivor[i] = temp_trellis_survivor[i];
		}
	}
	best_metric = temp_trellis_metric[0];
	best_branch = 0;
	bool flag;
	int count = 1;
	if(mode==0){/////////////////////////////////////////////////////////////////////////////////////Best state
		//temp_best_metric = best_metric;
		for(int i=1 ; i<64*2-1 ; i++){
			if(i%2){
				temp = temp_trellis_metric[count];
				flag = (bool)(best_metric > temp);
			}else{
				if(flag){
					best_metric = temp;
					best_branch = count;
				}
				count = count+1;
			}
		}
		//cout << "best_branch = " << best_branch << "  best_branch_metric = " << temp_trellis_metric[best_branch] << endl;
	}else{//////////////////////////////////////////////////////////////////////////////////////////Fixed state
		best_branch = 0;
	}
	//////////////////////////////////normalization///////////////////////////////////
	if(normal_flag){
		for(int i=0 ; i<64 ; i++){
			trellis_metric[i] = temp_trellis_metric[i] - 100000000;
		}
		normal_flag = 0;
	}else{
		for(int i=0 ; i<64 ; i++){
			trellis_metric[i] = temp_trellis_metric[i];
		}
	}
	/////////////////////////////////output information/////////////////////////////////error

	for(int i=0; i<truncate-1 ; i++){
		if(best_branch%2){ ///////////////////////////////////////////odd
			if((temp_trellis_survivor[best_branch]>>i)&1){////down
				best_branch = (best_branch-1)/2+32;
			}else{////////////////////////////////////////////////up
				best_branch = (best_branch-1)/2;
			}
		}else{////////////////////////////////////////////////////////even
			if((temp_trellis_survivor[best_branch]>>i)&1){////down
				best_branch = (best_branch)/2+32;
			}else{////////////////////////////////////////////////up
				best_branch = (best_branch)/2;
			}
		}
		//cout << best_branch << " ";
	}
	//cout << endl;

	//cout << "output_branch = " << best_branch << endl;
	output.write(best_branch%2);
	//return best_branch%2;
}

