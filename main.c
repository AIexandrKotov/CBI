#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

struct biginteger_t
{
	unsigned char negative;
	unsigned char count;
	unsigned char* symbols;
};

typedef struct biginteger_t BigInteger;

unsigned char digits_from_int(int a)
{
	if (a < 0) a = -a;
	unsigned char ret = 0;
	while (a > 0) a /= 10, ret++;
	return ret;
}

void bi_init(BigInteger* p)
{
	if (p->count < 1) exit(EXIT_FAILURE);
	p->symbols = malloc(p->count);
}

void bi_free(BigInteger* p)
{
	free(p->symbols);
	free(p);
}

BigInteger* bi_from_int(int src)
{
	BigInteger* ret = calloc(1, sizeof(BigInteger));
	if (ret == NULL) exit(EXIT_FAILURE);
	ret->negative = src < 0;
	ret->count = digits_from_int(src);
	bi_init(ret);
	
	if (src < 0) src = -src;
	for (unsigned char i = 0; i < ret->count; i++)
	{
		*(ret -> symbols + i) = src % 10;
		src /= 10;
	}
	return ret;
}

BigInteger* bi_from_str(char* str)
{
	BigInteger* ret = calloc(1, sizeof(BigInteger));
	if (ret == NULL) exit(EXIT_FAILURE);
	if (*str == '-') ret->negative = 1, str++;
	int shift = 0;
	while (*(str + shift)) shift++;
	ret->count = shift;
	bi_init(ret);
	shift = 0;
	while (shift < ret->count)
		*(ret->symbols + ret->count - shift++ - 1) = *(str++) - '0';
	return ret;
}

void bi_print(BigInteger* p)
{
	if (p->negative) printf("-");
	for (int i = p->count - 1; i >= 0; i--)
		printf("%d", *(p->symbols + i));
}

void internal_bi_sum(BigInteger* left, BigInteger* right, BigInteger* ret)
{
	unsigned char context = 0;
	unsigned char sum;
	for (unsigned char i = 0; i < left->count; i++)
	{
		sum = context;
		sum += *(left->symbols + i) + *(right -> symbols + i);
		context = sum / 10;
		*(ret -> symbols + i) = sum % 10;
	}
	*(ret->symbols + ret->count - 1) = context;
}

BigInteger* bi_sum(BigInteger* left, BigInteger* right)
{
	BigInteger* ret = calloc(1, sizeof(BigInteger));
	ret->negative = left->negative ^ right->negative;
	if (left->count == right->count) ret->count = left->count + 1;
	else ret->count = left->count > right->count ? left->count + 1 : right->count + 1;
	ret->symbols = malloc(ret->count);
	internal_bi_sum(left, right, ret);
	if (*(ret->symbols + ret->count - 1) == 0) ret->count -= 1;
	return ret;
}

int main(int argc, char *argv[])
{
	char* str1 = "9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
	BigInteger* bi1 = bi_from_str(str1);
	BigInteger* bi2 = bi_from_str(str1);
	
	bi_print(bi1);
	printf("\n");
	bi_print(bi2);
	printf("\n");
	
	BigInteger* bi3 = bi_sum(bi1, bi2);
	bi_print(bi3);
	
	bi_free(bi1);
	bi_free(bi2);
	bi_free(bi3);
	return 0;
}
