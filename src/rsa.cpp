#include "rsa.hpp"

#include <gmpxx.h>
#include <math.h>

#include <iostream>

#include "main.hpp"


using namespace std;

static mpz_class gcd_(mpz_class a, mpz_class b) {
    mpz_class t;
    while (1) {
        mpz_mod(t.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
        if (t == 0) return b;
        a = b;
        b = t;
    }
}


static mpz_class modInverse(mpz_class a, mpz_class m)
{
    mpz_class m0 = m, y = 0, x = 1;
 
    if (m == 1) return 0;
 
    while (a > 1) {
        // q is quotient
        mpz_class q = a / m, t = m;
 
        // m is remainder now, process same as
        // Euclid's algo
        m = a % m, a = t;
        t = y;
 
        // Update y and x
        y = x - q * y;
        x = t;
    }
 
    // Make x positive
    if (x < 0) x += m0;
 
    return x;
}

static bool MillerPrimes(const mpz_class &n, const size_t rounds,
                         gmp_randclass *rr) {

    if ((n & 1) == 0) return false; // if number is even, return false
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
            if (x == 1) return false;
            if (x == n - 1) break;
        }

        if (x != (n - 1)) return false;
    }
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
    mpz_class phi_n;
    r->seed(time(nullptr));
    generate_random(&(params->p), &(params->q), params->size / 2, r);

    params->n = params->p * params->q;
    phi_n = (params->p - 1) * (params->q - 1);
    
    do{
        params->e = r->get_z_range(phi_n) + 1;
    } while(gcd_(params->e, phi_n) != 1);

    params->d = modInverse(params->e, phi_n);

    delete (r);
}


mpz_class encrypt(mpz_t e, mpz_t n, mpz_t msg)
{   
    mpz_class cypher;
    mpz_powm(cypher.get_mpz_t(), msg, e, n);
    return cypher;
}

mpz_class decrypt(mpz_t d, mpz_t n, mpz_t cypher)
{
    mpz_class text;
    mpz_powm(text.get_mpz_t(), cypher, d, n);
    return text;
}


mpz_class PollardRho_factorisation(mpz_class n, gmp_randclass *r)
{        
    // even number
    if ((n & 1) == 0) return 2;
 
    // range [2, N)
    mpz_class x = r->get_z_range(n-2)+2;
    mpz_class y = x;
 
    mpz_class c = r->get_z_range(n-1)+1;
 
    mpz_class d = 1; 
 
    mpz_class tmp1;
    while (d==1)
    {
        // x(i+1) = f(x(i))
        mpz_powm_ui(tmp1.get_mpz_t(), x.get_mpz_t(), 2, n.get_mpz_t());
        tmp1 = tmp1 + c + n; 
        mpz_mod(x.get_mpz_t(), tmp1.get_mpz_t(), n.get_mpz_t());

        // y(i+1) = f(f(y(i)))
        mpz_powm_ui(tmp1.get_mpz_t(), y.get_mpz_t(), 2, n.get_mpz_t());
        tmp1 = tmp1 + c + n; 
        mpz_mod(y.get_mpz_t(), tmp1.get_mpz_t(), n.get_mpz_t());
        mpz_powm_ui(tmp1.get_mpz_t(), y.get_mpz_t(), 2, n.get_mpz_t());
        tmp1 = tmp1 + c + n; 
        mpz_mod(y.get_mpz_t(), tmp1.get_mpz_t(), n.get_mpz_t());

        // check gcd_ of abs(x-y) and n
        d = gcd_(abs(x-y), n);
 
        // retry if the algorithm fails to find prime factor with chosen x and c
        if (d == n) return PollardRho_factorisation(n, r);
    }
    return d;
}