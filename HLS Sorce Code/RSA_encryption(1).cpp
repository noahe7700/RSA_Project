/*
##############################################################
---------------   RSA Encryption Simplified   ----------------
This is a similiar implementation of the primary code with the
exception that the keys are provided rather than generated.
The cause for this change was challenges generating the bit
stream in vivado. When exporting RTL and synthesizing, there
were many issues with pipelining and scheduling. My hope is
that by limiting the number of recursive logic performed, the
code will synthesize properly and generate an overlay.
##############################################################
*/


#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_stream.h"
#include "hls_math.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "vector"

using namespace std;

#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_axis<64,1,1,1> AXI_VAL;
//typedef int data_t;
//typedef int	coef_t;
//typedef int	acc_t;
typedef ap_uint<65> uint65_t;
typedef ap_uint<128> uint128_t;
typedef ap_uint<512> uint512_t;
typedef ap_uint<1024> uint1024_t;

//typedef ap_fixed<64, 32, AP_RND_CONV, AP_SAT> fixed_t;
/*
uint1024_t inv_mod(uint1024_t a, uint1024_t m){
	uint1024_t m0 = m;
	uint1024_t y =0, x = 1;

	if (m==1) {
		return 0;
	}

	while (a>1){
		uint1024_t q = a / m;
		uint1024_t t = m;

		m = a % m;
		a=t;
		t = y;
		y = x - q * y;
		x = t;


	}
	if (x<0) {
		x+=m0;
	}
	return x;
}
*/
//No longer needed for this version
/*
uint1024_t inv_mod(uint1024_t a, uint1024_t m){

    a = a % m;
    for (uint1024_t x=1; x<m; x++)
       if ((a*x) % m == 1)
          return x;

    return -1;

}
*/
/*
uint1024_t gcd(uint1024_t a, uint1024_t b){
	while(b!=0){
		uint1024_t t =b;
		b = a % b;
		a = t;
	}
	return a;
}
*/

uint128_t mod_exp(uint128_t a, uint128_t b, uint128_t m){
	uint128_t res = 1;

	a = a % m;

	while (b > 0){
		if(b & 1)
			res = (res * a) % m;
		b = b >> 1;
		a = (a * a) % m;

	}
	return res;
}

// No longer needed for this version (returned N = 6103297, e = 65537)
/*
void generate_key(uint1024_t& n, uint1024_t& e, uint1024_t& d){
	uint1024_t p, q, phi, tmp2;
	uint512_t tmp;

	p = 2027;
	q = 3011;
	n = p * q;
	phi = (p-1)*(q-1);

	for (uint1024_t i = 65537; i < phi; i+=2){
		tmp = gcd(i,phi);
		if (tmp == 1){
			e=i;
			break;
		}
	}
	tmp2 = inv_mod(e, phi);
	d = tmp2;

}
*/

void rsa_encrypt(uint1024_t n, uint1024_t e, AXI_VAL plain, AXI_VAL& cipher){
	uint1024_t plainVal = plain.data.to_int();
	cipher.data = mod_exp(plainVal, e, n);
}

void rsa_decrypt(uint1024_t n, uint1024_t d, AXI_VAL cipher, AXI_VAL& plain){
	uint1024_t cipherVal = cipher.data.to_int();
	plain.data = mod_exp(cipherVal, d, n);
}

void encrypt (hls::stream<AXI_VAL>& dataIn, hls::stream<AXI_VAL>& dataOut) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis register both port=dataIn
#pragma HLS INTERFACE axis register both port=dataOut


	uint128_t n, e, d; // Variables used for the keys (since static, we can test with one or the other).
	AXI_VAL plain;
	AXI_VAL cipher;
	//uint128_t decrypted_plain;
	// Removed generate keys... set the keys to the right values...
	n = 6103297;
	e = 65537;
	// generate_key(n, e, d); // Gets the values for the keys using prime numbers in the function.
	while(1){
		dataIn.read(plain); // Sets the plain to the value read

		n = 6103297;
		e = 65537;

		//cipher.data = mod_exp(plain.data.to_int(), e, n);
		uint128_t res = 1;
		int a, b, m;
		a = plain.data.to_int();

		a = a%n;
		while (e > 0){
				if(e & 1)
					res = (res * a) % n;
				e = e >> 1;
				a = (a * a) % n;

			}
		cipher.data = res;
		dataOut.write(cipher); //After performing the encryption, the output stores the cipher

		if(plain.last){
			break;
		}
	}

}

