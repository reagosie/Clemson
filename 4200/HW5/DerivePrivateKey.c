/* bn_sample.c */
#include <stdio.h>
#include <openssl/bn.h>
#define NBITS 256
void printBN(char *msg, BIGNUM * a)
{
	/* Use BN_bn2hex(a) for hex string
	* Use BN_bn2dec(a) for decimal string */
	char * number_str = BN_bn2hex(a);
	printf("%s %s\n", msg, number_str);
	OPENSSL_free(number_str);
}

int main ()
{
	BN_CTX *ctx = BN_CTX_new();
	BIGNUM *p = BN_new();
	BIGNUM *q = BN_new();
	BIGNUM *e = BN_new();
	BIGNUM *d = BN_new();
	BIGNUM *res1 = BN_new();
	BIGNUM *res2 = BN_new();
	BIGNUM *res3 = BN_new();
	BIGNUM *one = BN_new();


	// Initialize p, q, e , here you need to provide the values
	BN_hex2bn(&p, "");
	BN_hex2bn(&q, "");
	BN_hex2bn(&e, "");
	BN_hex2bn(&one, "1");

	//res1 = p-1
	BN_sub(res1, p, one);
	
	//res2 = q -1
	BN_sub(res2, q, one);	

	// res = res1*res2
	BN_mul(res3, res1, res2, ctx);

	//Mod inverse: e*d mod res3 = 1
	BN_mod_inverse(d, e, res3, ctx);
	printBN("d = ", d);
 
	//verify
	BN_mod_mul(res1, e, d, res3, ctx);
	printBN("Verify: e*d mod (p-1)*(q-1) = ", res1);
	return 0;
}

