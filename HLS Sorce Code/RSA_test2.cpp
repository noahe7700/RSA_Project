#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_stream.h"
#include "hls_math.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "vector"

#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_axis<32,1,1,1> AXI_VAL;
typedef int data_t;
typedef int	coef_t;
typedef int	acc_t;
typedef ap_uint<512> uint512_t;
typedef ap_uint<128> uint1024_t;


void encrypt (hls::stream<AXI_VAL>& dataIn, hls::stream<AXI_VAL>& dataOut);
void decrypt (hls::stream<AXI_VAL>& dataIn, hls::stream<AXI_VAL>& dataOut);

//void rsa_encrypt(uint1024_t n, uint1024_t e, AXI_VAL plain, AXI_VAL& cipher);
//void rsa_decrypt(uint1024_t n, uint1024_t d, AXI_VAL cipher, AXI_VAL& plain);
int main(){
	hls::stream<AXI_VAL> input, output;
	hls::stream<AXI_VAL> input2, output2;
	AXI_VAL tmp1, tmp2, tmp3, tmp4;

	coef_t charArr[13] = {2374081 ,634949 ,683399 ,683399 ,4222941 ,5390756 ,6054897 ,694407 ,4222941 ,4107156 ,683399 ,3812635 ,469162};
	// Testing for the different values that run the RSA Encryption
	AXI_VAL plain, cipher;
	/*
	AXI_VAL plain, cipher;
	plain.data = 72;
	uint1024_t n, e, d;
	generate_key(n, e, d);
	rsa_encrypt(n, e, plain, cipher);
	if (cipher.data == 2374081) {
		printf("success");
	}
	*/
	uint1024_t n, e, d;
	//plain.data = 72;
	n = 6103297;
	e = 65537;
	d = 2416153;
	//rsa_encrypt(n,e,plain,cipher);
	//printf("Cipher Val: %d \n", cipher.data);

	//rsa_decrypt(n, d, cipher, plain);
	//printf("Decrypted val: %d", plain.data);


	for (int i = 0; i <13; i++){
			tmp3.data = charArr[i];

			if (i == 12){
				tmp3.last = 1;
			} else {
				tmp3.last =0;
			}
			input2.write(tmp3);
		}
	decrypt(input2, output2);

	for (int j = 0; j < 13; j++){
		output2.read(tmp4);
		printf("%d, ", tmp4.data);
	}


}
