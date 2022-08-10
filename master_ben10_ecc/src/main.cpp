#include "master_top.h"
//#include "top_module.hpp"
#define DATA_LENGTH 384

using namespace std;

int main(int argc, char *argv[]){
	int i;
	ap_uint<pixl_bit> out;
	ap_uint<32> data_in[DATA_LENGTH];
	ap_uint<32> data_out[DATA_LENGTH];
  ap_uint<32> data_out_deQAM_real[DATA_LENGTH*4];
  ap_uint<32> data_out_deQAM_imag[DATA_LENGTH*4];
	hls::stream<ap_uint<pixl_bit>,DATA_LENGTH> data_in_s;
	hls::stream<ap_uint<pixl_bit>,DATA_LENGTH> data_out_s;
	int pixl_out[DATA_LENGTH];
	int data[7168];

	cout << ">> Test Start" << endl;
	cout << ">> Load Input" << endl;
	cout << ">> Read Data" << endl;
	FILE * pFile;
	char path[] = "pic_data.txt";
	pFile = fopen( path,"r");
	i = 0;
	while(fscanf (pFile, "%d", &data[i]) != EOF){
		i++;
	}
	FILE * outfp;
	outfp=fopen("../../../../src/output_result/out.txt","w");
	cout << ">> Start Calculation" << endl;

	for(int j=0; j<DATA_LENGTH; j++){
		data_in[j]=(ap_uint<32>)data[j];
	}

	master_top(data_in, data_out, data_out_deQAM_real, data_out_deQAM_imag, DATA_LENGTH, 10, 1, 4);

	for(int t=0;t<DATA_LENGTH;t++){
		pixl_out[t] = (int)data_out[t];
	}
  
  //for(int t=0;t<DATA_LENGTH*2;t++){
		//cout << "data_out_deQAM_real = " << (int)data_out_deQAM_real[t] << " data_out_deQAM_imag = " << (int)data_out_deQAM_imag[t] << endl;
	//}

	fclose (pFile);
	fclose (outfp); 
	cout << ">> Calculation Finished!" << endl;
	cout << "-----------------------------" << endl;
	cout << ">> Pixl_Error_Rate" << endl;

	int Pixl_ERROR = 0;
	int PixlER;
	for (PixlER = 0 ; PixlER < DATA_LENGTH ; PixlER++){
		if(data[PixlER] != pixl_out[PixlER]){
			Pixl_ERROR++;
		}
	}
	cout << (float)Pixl_ERROR*100.0/(float)DATA_LENGTH << "%" <<endl;

	cout << ">> Symbol_Error_Rate" << endl;
	int Symbol_ERROR = 0;
	int SER;
	for (SER = 0 ; SER < DATA_LENGTH ; SER++){
		if((data[SER]/16) != (pixl_out[SER]/16)){
			Symbol_ERROR++;
		}
		if((data[SER]%16) != (pixl_out[SER]%16)){
			Symbol_ERROR++;
		}
	}
	cout << (float)Symbol_ERROR*100.0/(float)(DATA_LENGTH*2) << "%" <<endl;

	cout << ">> Bit_Error_Rate" << endl;
	int Bit_ERROR = 0;
	int BER;
	for (BER = 0 ; BER < DATA_LENGTH ; BER++){
		if((data[BER]/128) != (pixl_out[BER]/128)){
			Bit_ERROR++;
		}
		if(((data[BER]/64)%2) != ((pixl_out[BER]/64)%2)){
			Bit_ERROR++;
		}
		if(((data[BER]/32)%2) != ((pixl_out[BER]/32)%2)){
			Bit_ERROR++;
		}
		if(((data[BER]/16)%2) != ((pixl_out[BER]/16)%2)){
			Bit_ERROR++;
		}
		if(((data[BER]/8)%2) != ((pixl_out[BER]/8)%2)){
			Bit_ERROR++;
		}
		if(((data[BER]/4)%2) != ((pixl_out[BER]/4)%2)){
			Bit_ERROR++;
		}
		if(((data[BER]/2)%2) != ((pixl_out[BER]/2)%2)){
			Bit_ERROR++;
		}
		if((data[BER]%2) != (pixl_out[BER]%2)){
			Bit_ERROR++;
		}
	}
	cout << (float)Bit_ERROR*100.0/(float)(DATA_LENGTH*8) << "%" <<endl;
	return 0;
}
