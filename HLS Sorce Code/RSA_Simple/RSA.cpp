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

typedef ap_axis<32,1,1,1> AXI_VAL;
//typedef int data_t;
//typedef int	coef_t;
//typedef int	acc_t;
typedef ap_uint<32> uint65_t;
typedef ap_uint<128> uint128_t;
//typedef ap_uint<512> uint512_t;
//typedef ap_uint<1024> uint1024_t;


void encrypt (hls::stream<AXI_VAL>& dataIn, hls::stream<AXI_VAL>& dataOut) {

#pragma HLS INTERFACE axis port=dataIn
#pragma HLS INTERFACE axis port=dataOut
#pragma HLS interface s_axilite port=return

	uint65_t n, e, d; // Variables used for the keys (since static, we can test with one or the other).
	AXI_VAL plain;
	AXI_VAL cipher;
	//uint128_t decrypted_plain;
	// Removed generate keys... set the keys to the right values...
	uint65_t res;
	uint65_t a;
	// generate_key(n, e, d); // Gets the values for the keys using prime numbers in the function.
	while(1){
		dataIn.read(plain); // Sets the plain to the value read

		n = 6103297;
		e = 65537;
		d = 2416153; // Private key value from PS
		//cipher.data = mod_exp(plain.data.to_int(), e, n);

		res = 1;

		a = plain.data.to_int();

		a = a%n;
		while (e > 0){
				if(e & 1)
					res = (res * a) % n;
				e = e >> 1;
				a = (a * a) % n;

			}
		cipher.data = res;
		cipher.keep = plain.keep;
		cipher.strb = plain.strb;
		cipher.user = plain.user;
		cipher.last = plain.last;
		cipher.id = plain.id;
		cipher.dest = plain.dest;
		dataOut.write(cipher); //After performing the encryption, the output stores the cipher

		if(plain.last){

			break;
		}
	}

}


void decrypt (hls::stream<AXI_VAL>& dataIn, hls::stream<AXI_VAL>& dataOut) {

#pragma HLS INTERFACE axis port=dataIn
#pragma HLS INTERFACE axis port=dataOut
#pragma HLS interface s_axilite port=return

	uint65_t n, e, d; // Variables used for the keys (since static, we can test with one or the other).
	AXI_VAL plain;
	AXI_VAL cipher;
	//uint128_t decrypted_plain;
	// Removed generate keys... set the keys to the right values...
	uint65_t res;
	uint65_t a;
	// generate_key(n, e, d); // Gets the values for the keys using prime numbers in the function.
	while(1){
		dataIn.read(plain); // Sets the plain to the value read

		n = 6103297;
		e = 65537;
		d = 2416153; // Private key value from PS
		//cipher.data = mod_exp(plain.data.to_int(), e, n);

		res = 1;

		a = plain.data.to_int();

		a = a%n;
		while (d > 0){
				if(d & 1)
					res = (res * a) % n;
				d = d >> 1;
				a = (a * a) % n;

			}
		cipher.data = res;
		cipher.keep = plain.keep;
		cipher.strb = plain.strb;
		cipher.user = plain.user;
		cipher.last = plain.last;
		cipher.id = plain.id;
		cipher.dest = plain.dest;
		dataOut.write(cipher); //After performing the encryption, the output stores the cipher

		if(plain.last){

			break;
		}
	}

}
