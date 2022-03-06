#include "rsa.hpp"

#include <gmpxx.h>
#include <math.h>

#include <iostream>

#include "main.hpp"

#define ui unsigned int

using namespace std;

static mpz_class gcd(mpz_class a, mpz_class b) {
    mpz_class t;
    while (1) {
        mpz_mod(t.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
        if (t == 0) return b;
        a = b;
        b = t;
    }
}

static bool MillerPrimes(const mpz_class &n, const size_t rounds,
                         gmp_randclass *rr) {
    // Treat n==1, 2, 3 as a primes
    if (n == 1 || n == 2 || n == 3) return true;

    // Treat negative numbers in the frontend
    if (n <= 0) return false;

    // Even numbers larger than two cannot be prime
    if ((n & 1) == 0) return false;

    // Write n-1 as d*2^s by factoring powers of 2 from n-1
    size_t s = 0;
    {
        mpz_class m = n - 1;
        while ((m & 1) == 0) {
            ++s;
            m >>= 1;
        }
    }
    const mpz_class d = (n - 1) / (mpz_class(1) << s);

    for (size_t i = 0; i < rounds; ++i) {
        const mpz_class a = rr->get_z_range(n - 1);

        mpz_class x;
        mpz_powm(x.get_mpz_t(), a.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());

        if (x == 1 || x == (n - 1)) continue;

        for (size_t r = 0; r < (s - 1); ++r) {
            mpz_powm_ui(x.get_mpz_t(), x.get_mpz_t(), 2, n.get_mpz_t());
            if (x == 1) {
                // Definitely not a prime
                return false;
            }
            if (x == n - 1) break;
        }

        if (x != (n - 1)) {
            // Definitely not a prime
            return false;
        }
    }

    // Might be prime
    return true;
}

void generate_random(mpz_class *p, mpz_class *q, const size_t size,
                     gmp_randclass *rr) {
    do {
        *p = rr->get_z_bits(size);
    } while (!MillerPrimes(*p, 10, rr));

    do {
        *q = rr->get_z_bits(size);
    } while (!MillerPrimes(*q, 10, rr));
}

void generate_options(RSAParams *params) {
    gmp_randclass *r = new gmp_randclass(gmp_randinit_default);
    mpz_class e, phi_n;
    r->seed(time(nullptr));
    generate_random(&(params->p), &(params->q), params->size / 2, r);

    params->n = params->p * params->q;
    phi_n = (params->p - 1) * (params->q - 1);
    
    do{
        e = r->get_z_range(phi_n) + 1;
    } while(gcd(e, phi_n) != 1);

    delete (r);
}