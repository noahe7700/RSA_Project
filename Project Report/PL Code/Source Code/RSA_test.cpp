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
typedef ap_uint<1024> uint1024_t;


void generate_key(uint1024_t& n, uint1024_t& e, uint1024_t& d);
uint1024_t gcd(uint1024_t a, uint1024_t b);
uint1024_t mod_exp(uint1024_t a, uint1024_t b, uint1024_t m);
void rsa_encrypt(uint1024_t n, uint1024_t e, AXI_VAL plain, AXI_VAL& cipher);
uint1024_t inv_mod(uint1024_t a, uint1024_t m);


int main(){

	// Testing for the different values that run the RSA Encryption
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



}
