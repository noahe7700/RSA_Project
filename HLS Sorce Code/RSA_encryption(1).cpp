#include "ap_int.h"
#include "ap_fixed.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include "hls_math.h"
#include "stdint.h"
#include "stdlib.h"


using namespace std;


typedef ap_uint<512> uint512_t;
typedef ap_uint<1024> uint1024_t;

typedef ap_fixed<64, 32, AP_RND_CONV, AP_SAT> fixed_t;


// Inverted modulo equation to help compute the private key value
// Code adapted from https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
uint1024_t inv_mod(uint1024_t a, uint1024_t m){
	uint1024_t m0 = m;
	uint1024_t y =0, x = 1;

	if (m==1)
		return 0;

	while (a>1){
		uint1024_t q = a / m;
		uint1024_t t = m;

		m = a % m, a=t;
		t = y;
		y = x - q * y;
		x = t;

	}
	if (x<0)
		x+=m0;

	return x;
}
// Simple GCD code using large data types large prime values or keys
uint1024_t gcd(uint1024_t a, uint1024_t b){
	while(b!=0){
		uint1024_t t =b;
		b = a % b;
		a = t;
	}
	return a;
}

// Exponential modulo function to replicate the "pow(a, b, c)" in python
// Code replicated from: https://www.geeksforgeeks.org/modular-exponentiation-power-in-modular-arithmetic/

uint1024_t mod_exp(uint1024_t a, uint1024_t exp, uint1024_t modu){
	uint1024_t res = 1;
	// 
	a = a % modu
	while (exp > 0) {
		//if exponent is odd
        if (exp % 2 == 1) {
            res = (res * a) % modu;
        }
        base = (base * base) % modu;
        exp = exp / 2;
    }
    return res;
}

// Generates the values used for the private and public keys
// Note: for now, the values p and q are not pass to the function since I am testing with known primes.

void generate_key(uint1024_t& n, uint1024_t& e, uint1024_t& d){
	uint1024_t p, q, phi;
	uint512_t tmp;

	p = 2027;
	q = 3011;
	n = p * q;
	phi = (p-1)*(q-1);
	// Generates a large public key for the value
	for (uint1024_t i = 137; i < phi; i+=2){
		// Check if coprime with totient
		tmp = gcd(i,phi);
		if (tmp == 1){
			// When coprime, set the value of the key to i and break the loop
			e=i;
			break;
		}
	}

	// 
	d = inv_mod(e, phi);
}
// If needed, however I can also used "encrypted_data = mod_exp(inputData, e, n)" in top function
void rsa_encrypt(uint1024_t n, uint1024_t e, uint512_t inputData, uint1024_t& encrData){
	encrData = mod_exp(inputData, e, n);

}
// If needed, however I can also used "decrypted_data = mod_exp(encrData, d, n)" in top function
void rsa_decrypt(uint1024_t n, uint1024_t d, uint1024_t encrData, uint512_t& inputData){
	inputData = mod_exp(encrData, d, n);
}
// Function to test this in cpp encironment
int main(){
	uint1024_t n, e, d;
	uint512_t inputData = 123456789;
	uint1024_t encrData;
	uint512_t decrData;

	generate_key(n, e, d);

	rsa_encrypt(n, e, inputData, encrData);
	rsa_decrypt(n, d, encrData, decrData);



//	Print(decrData);
	return 1;

}
// WORK IN PROGRESS - Still need to implement the stream
// Building this with the tutorials used for HW4 to help with AXI-stream

// Set to top function to encrypt - testing seperately for now.
void streamEncryption(hls::stream< ap_axis< 64, 2, 5, 6> > &A, hls::stream< ap_axis< 64, 2, 5, 6> > &B){

	#pragma HLS INTERFACE axis port=A
	#pragma HLS INTERFACE axis port=B
	// For controlling the IP
	#pragma HLS INTERFACE s_axilite port=return


	ap_axis<64, 2, 5, 6> tmp;
	// For this version, I will be generating the prime numbers and keys within the code to generate
	uint1024_t n, e, d, data;
	generate_key(n, e, d)

	while(1){
		// Read the data
		A.read(tmp)

		// Perform the encryption on the value.
		tmp.data = mod_exp(tmp.data.to_int(), e, n);
		B.write(tmp);

		if(tmp.last){
			break;
		}
	}
}
// Set to top function to decrypt - testing seperately for now.
void streamDecryption(hls::stream< ap_axis< 64, 2, 5, 6> > &A, hls::stream< ap_axis< 64, 2, 5, 6> > &B){

	#pragma HLS INTERFACE axis port=A
	#pragma HLS INTERFACE axis port=B
	// For controlling the IP
	#pragma HLS INTERFACE s_axilite port=return


	ap_axis<64, 2, 5, 6> tmp;
	// For this version, I will be generating the prime numbers and keys within the code to generate
	uint1024_t n, e, d, data;
	generate_key(n, e, d)

	while(1){
		// Read the data
		A.read(tmp)

		// Perform the decryption on the value.
		tmp.data = mod_exp(tmp.data.to_int(), d, n);
		B.write(tmp);

		if(tmp.last){
			break;
		}
	}
}
