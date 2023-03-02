#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_stream.h"
#include "hls_math.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "vector"

using namespace std;


typedef ap_uint<512> uint512_t;
typedef ap_uint<1024> uint1024_t;

typedef ap_fixed<64, 32, AP_RND_CONV, AP_SAT> fixed_t;

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

uint1024_t gcd(uint1024_t a, uint1024_t b){
	while(b!=0){
		uint1024_t t =b;
		b = a % b;
		a = t;
	}
	return a;
}

uint1024_t mod_exp(uint1024_t a, uint1024_t b, uint1024_t m){
	uint1024_t res = 1;

	a = a % m;

	while (b > 0){
		if(b & 1)
			res = (res * a) % m;
		b = b >> 1;
		a = (a * a) % m;

	}
	return res;
}

void generate_key(uint1024_t& n, uint1024_t& e, uint1024_t& d){
	uint1024_t p, q, phi;
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
	d = inv_mod(e, phi);
}

void rsa_encrypt(uint1024_t n, uint1024_t e, uint512_t plain, uint1024_t& cipher){
	cipher = mod_exp(plain, e, n);

}

void rsa_decrypt(uint1024_t n, uint1024_t d, uint1024_t cipher, uint512_t& plain){
	plain = mod_exp(cipher, d, n);
}

int main(){
	uint1024_t n, e, d;
	uint512_t plain = 123456789;
	uint1024_t cipher;
	uint512_t decrypted_plain;

	generate_key(n, e, d);

	rsa_encrypt(n, e, plain, cipher);
	rsa_decrypt(n, d, cipher, decrypted_plain);



//	Print(decrypted_plain);
	return 1;

}
