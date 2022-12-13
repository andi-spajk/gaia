/** prime.c

Find and check for prime numbers. Resize symbol tables with these functions.

*/

#include <math.h>

/** is_prime()
	@x              possible prime to check

	@return         1 if prime, 0 if not

	Check if x is a prime number.
*/
static int is_prime(const int x)
{
	if (x < 2)
		return 0;
	if (x < 4)
		return 1;
	if (x % 2 == 0)
		return 0;
	for (int i = 3; i <= floor(sqrt((double)x)); i += 2) {
		if (x % i == 0)
			return 0;
	}
	return 1;
}

/** next_prime()
	@x              starting number to check for primality

	@return         the next prime number after x

	Calculate the next prime number after x.
*/
int next_prime(int x)
{
	// always increment x before checking is_prime() so we don't return
	// x unchanged
	do {
		x++;
	} while (!is_prime(x));
	return x;
}