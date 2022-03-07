#include "RSAParams.hpp"

void generate_options(RSAParams *);
long int gcd(long int, long int);
mpz_class encrypt(mpz_t, mpz_t, mpz_t);
mpz_class decrypt(mpz_t, mpz_t, mpz_t);
mpz_class basic_factorisation(mpz_class);