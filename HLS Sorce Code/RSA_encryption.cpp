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
typedef int data_t;
typedef int	coef_t;
typedef int	acc_t;
typedef ap_uint<32> uint65_t;
//typedef ap_uint<64> uint64_t;
typedef ap_uint<512> uint512_t;
typedef ap_uint<1024> uint1024_t;

typedef ap_fixed<64, 32, AP_RND_CONV, AP_SAT> fixed_t;
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

uint65_t inv_mod(uint65_t a, uint65_t m){

    a = a % m;
    for (uint65_t x=1; x<m; x++)
       if ((a*x) % m == 1)
          return x;

    return -1;

}

uint65_t gcd(uint65_t a, uint65_t b){
	while(b!=0){
		uint65_t t =b;
		b = a % b;
		a = t;
	}
	return a;
}

uint65_t mod_exp(uint65_t a, uint65_t b, uint65_t m){
	uint65_t res = 1;

	a = a % m;

	while (b > 0){
		if(b & 1)
			res = (res * a) % m;
		b = b >> 1;
		a = (a * a) % m;

	}
	return res;
}

void generate_key(int prime1, int prime2, uint65_t& n, uint65_t& e, uint65_t& d){
	uint65_t p, q, phi, tmp2;
	uint65_t tmp;

	p = prime1;
	q = prime2;
	n = p * q;
	phi = (p-1)*(q-1);

	for (uint65_t i = 65537; i < phi; i+=2){
		tmp = gcd(i,phi);
		if (tmp == 1){
			e=i;
			break;
		}
	}
	tmp2 = inv_mod(e, phi);
	d = tmp2;

}

void rsa_encrypt(uint65_t n, uint65_t e, AXI_VAL plain, AXI_VAL& cipher){
	uint65_t plainVal = plain.data.to_int();
	cipher.data = mod_exp(plainVal, e, n);
}

void rsa_decrypt(uint65_t n, uint65_t d, uint65_t cipher, uint65_t& plain){
	plain = mod_exp(cipher, d, n);
}
void encrypt (hls::stream<AXI_VAL>& dataIn, hls::stream<AXI_VAL>& dataOut, int prime1, int prime2, bool ende) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=dataIn
#pragma HLS INTERFACE axis port=dataOut
#pragma HLS INTERFACE s_axilite port=prime1
#pragma HLS INTERFACE s_axilite port=prime2
#pragma HLS INTERFACE s_axilite port=ende

	// Update 4/10: encrypt/decrypt bool - 0 = encrypt, 1 = decrypt
	uint65_t n, e, d; // Variables used for the keys (now not static and will get from generate key function).
	//AXI_VAL p1, q1;
	AXI_VAL plain;
	AXI_VAL cipher;
	//uint128_t decrypted_plain;

	uint65_t res;
	uint65_t a;

	//p = p1.data.to_int();
	//q = q1.data.to_int();
	generate_key(prime1, prime2, n, e, d); // Gets the values for the keys using prime numbers in the function.
	uint65_t nStat = n;
	uint65_t eStat = e;
	uint65_t dStat = d;
	while(1){
		dataIn.read(plain); // Sets the plain to the value read

		n = nStat;
		e = eStat;
		d = dStat;

		//cipher.data = mod_exp(plain.data.to_int(), e, n);

		res = 1;

		a = plain.data.to_int();

		a = a%n;
		// Using ende boolean, determine if encrypt or decrypt is passed
		if(ende == 1){
			e = d;
		}
		else {
			e =e;
		}
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


