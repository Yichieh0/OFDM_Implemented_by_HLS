#include "channel_gen.h"

void channel_gen(hls::stream<ap_fixed<IN_WL,IN_IL>>& in_x_r, hls::stream<ap_fixed<IN_WL,IN_IL>>& in_x_i, hls::stream<ap_fixed<IN_WL,IN_IL>>& channel_out_real, hls::stream<ap_fixed<IN_WL,IN_IL>>& channel_out_imag, ap_uint<32> TAPS_NUM, ap_uint<32> pilot_width)
{

#pragma HLS DATAFLOW

	static int index_count = 0;
	ap_uint<4> i;
  ap_fixed<IN_WL,IN_IL> x_out_r = 0.0;
	ap_fixed<IN_WL,IN_IL> x_out_i = 0.0;
	static ap_fixed<IN_WL,IN_IL> x_real_1taps;
	static ap_fixed<IN_WL,IN_IL> x_imag_1taps; 
  static ap_fixed<IN_WL,IN_IL> n_1taps;
  static ap_fixed<IN_WL,IN_IL> x_real_2taps[2] = {0.0};
	static ap_fixed<IN_WL,IN_IL> x_imag_2taps[2] = {0.0}; 
  static ap_fixed<IN_WL,IN_IL> n_2taps[2];
  ap_fixed<IN_WL,IN_IL> weight_2taps[2] = {0.5, 0.31};
  static ap_fixed<IN_WL,IN_IL> x_real_3taps[3] = {0.0};
	static ap_fixed<IN_WL,IN_IL> x_imag_3taps[3] = {0.0}; 
  static ap_fixed<IN_WL,IN_IL> n_3taps[3];
  ap_fixed<IN_WL,IN_IL> weight_3taps[3] = {0.5, 0.31, 0.19};
  static ap_fixed<IN_WL,IN_IL> x_real_6taps[6] = {0.0};
	static ap_fixed<IN_WL,IN_IL> x_imag_6taps[6] = {0.0}; 
  static ap_fixed<IN_WL,IN_IL> n_6taps[6];
  ap_fixed<IN_WL,IN_IL> weight_6taps[6] = {0.7943, 0.3981, 0.3162, 0.2512, 0.1778, 0.1259};
  static ap_fixed<IN_WL,IN_IL> x_real_9taps[9] = {0.0};
	static ap_fixed<IN_WL,IN_IL> x_imag_9taps[9] = {0.0};
	static ap_fixed<IN_WL,IN_IL> n_9taps[9];
  ap_fixed<IN_WL,IN_IL> weight_9taps[9] = {0.5, 0.31, 0.19, 0.06, 0.03, 0.02, 0.008, 0.004, 0.002};
	hls::stream<ap_fixed<IN_WL,IN_IL>> random_num;

	if(TAPS_NUM==1){
	  x_real_1taps = in_x_r.read();
	  x_imag_1taps = in_x_i.read();
	  //if(index_count==0){
		//	  rand(random_num);
		//	  n_1taps = random_num.read();
	  //}
	  //x_out_r = (ap_fixed<IN_WL,IN_IL>)n_1taps*x_real_1taps;
	  //x_out_i = (ap_fixed<IN_WL,IN_IL>)n_1taps*x_imag_1taps;
    //channel_out_real.write(x_out_r);
  	//channel_out_imag.write(x_out_i);
    channel_out_real.write(x_real_1taps);
  	channel_out_imag.write(x_imag_1taps);
	}
 
	else if(TAPS_NUM==2){
    for (i = TAPS_NUM-1 ;i > 0; i--){
		  x_real_2taps[i] = x_real_2taps[i-1];
		  x_imag_2taps[i] = x_imag_2taps[i-1];
	  }
	  x_real_2taps[0] = in_x_r.read();
	  x_imag_2taps[0] = in_x_i.read();
	  if(index_count==0){
	    for(i = 0; i < TAPS_NUM; i++){
			  rand(random_num);
			  n_2taps[i] = random_num.read();
		  }
	  }
		for(i = 0; i < TAPS_NUM; i++){
			x_out_r = x_out_r + (ap_fixed<IN_WL,IN_IL>)(weight_2taps[i]*n_2taps[i])*x_real_2taps[i];
			x_out_i = x_out_i + (ap_fixed<IN_WL,IN_IL>)(weight_2taps[i]*n_2taps[i])*x_imag_2taps[i];
		}
    channel_out_real.write(x_out_r);
  	channel_out_imag.write(x_out_i);
	}
 
	else if(TAPS_NUM==3){
    for (i = TAPS_NUM-1 ;i > 0; i--){
		  x_real_3taps[i] = x_real_3taps[i-1];
		  x_imag_3taps[i] = x_imag_3taps[i-1];
	  }
	  x_real_3taps[0] = in_x_r.read();
	  x_imag_3taps[0] = in_x_i.read();
	  if(index_count==0){
		  for(i = 0; i < TAPS_NUM; i++){
			  rand(random_num);
			  n_3taps[i] = random_num.read();
		  }
	  }
		for(i = 0; i < TAPS_NUM; i++){
			x_out_r = x_out_r + (ap_fixed<IN_WL,IN_IL>)(weight_3taps[i]*n_3taps[i])*x_real_3taps[i];
			x_out_i = x_out_i + (ap_fixed<IN_WL,IN_IL>)(weight_3taps[i]*n_3taps[i])*x_imag_3taps[i];
		}
    channel_out_real.write(x_out_r);
  	channel_out_imag.write(x_out_i);
	}
 
	else if(TAPS_NUM==6){
    for (i = TAPS_NUM-1 ;i > 0; i--){
		  x_real_6taps[i] = x_real_6taps[i-1];
		  x_imag_6taps[i] = x_imag_6taps[i-1];
	  }
	  x_real_6taps[0] = in_x_r.read();
	  x_imag_6taps[0] = in_x_i.read();
	  if(index_count==0){
		  for(i = 0; i < TAPS_NUM; i++){
			  rand(random_num);
			  n_6taps[i] = random_num.read();
		  }
	  }
		for(i = 0; i < TAPS_NUM; i++){
			x_out_r = x_out_r + (ap_fixed<IN_WL,IN_IL>)(weight_6taps[i]*n_6taps[i])*x_real_6taps[i];
			x_out_i = x_out_i + (ap_fixed<IN_WL,IN_IL>)(weight_6taps[i]*n_6taps[i])*x_imag_6taps[i];
		}
    channel_out_real.write(x_out_r);
  	channel_out_imag.write(x_out_i);
	}
	
  else{
    for (i = TAPS_NUM-1 ;i > 0; i--){
		  x_real_9taps[i] = x_real_9taps[i-1];
		  x_imag_9taps[i] = x_imag_9taps[i-1];
	  }
	  x_real_9taps[0] = in_x_r.read();
	  x_imag_9taps[0] = in_x_i.read();
	  if(index_count==0){
		  for(i = 0; i < TAPS_NUM; i++){
			  rand(random_num);
			  n_9taps[i] = random_num.read();
		  }
	  }
	  for(i = 0; i < TAPS_NUM; i++){
			x_out_r = x_out_r + (ap_fixed<IN_WL,IN_IL>)(weight_9taps[i]+n_9taps[i])*x_real_9taps[i];
			x_out_i = x_out_i + (ap_fixed<IN_WL,IN_IL>)(weight_9taps[i]+n_9taps[i])*x_imag_9taps[i];
		}
    channel_out_real.write(x_out_r);
  	channel_out_imag.write(x_out_i);
	}

	//sum of multi-path
	index_count++;
	if(index_count==(FFT_LEN+CP_length)){
		index_count = 0;
	}


	return;
}
