#include "RSAParams.hpp"

void generate_options(RSAParams *);
mpz_class encrypt(mpz_t, mpz_t, mpz_t);
mpz_class decrypt(mpz_t, mpz_t, mpz_t);
mpz_class PollardRho_factorisation(mpz_class, gmp_randclass *);