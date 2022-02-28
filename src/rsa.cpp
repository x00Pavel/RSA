#include <iostream>
#include <math.h>
#include <gmpxx.h>
#include "rsa.hpp"
#include <algorithm>

using namespace std;

unsigned short int first_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 
                                     37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
                                     79, 83, 89, 97};

unsigned long int p, q, n;


// long int gcd(long int a, long int b){
//     int tmp;
//     do{
//         tmp = a % b;
//         a = b;
//         b = tmp;
//     } while (tmp != 0);
//     return b;
// }


bool is_prime(unsigned long int n)
{
    for (int p : first_primes){ 
        if ((n % p) == 0) return false;
    }
    

}


void generate_random(unsigned long *p, unsigned long *q){
    mpz_t num, l;
    gmp_randstate_t st;
    
    mpz_init(num);
    // mpz_init(l);
    unsigned long seed = time(nullptr);
    cout << "Seed is " << seed << endl;

    // mpz_ui_pow_ui(l, 2, 199);

    gmp_randinit_mt(st);
    gmp_randseed_ui(st, seed);

    do{
        mpz_urandomb(num, st, 32);
        *p = mpz_get_ui(num);
    } while (!is_prime(*p));
    

    // mpz_urandomb(num, st, 32);
    // *q = mpz_get_ui(num);
    
    gmp_randclear(st);
    mpz_clear(num);
}

void generate_options(rsa_options *opts){
    generate_random(&(opts->p), &(opts->q));
    // generate_prime(&(opts->q), opts->size);
    // generate_prime(&(opts->p), opts->size);
    printf("p: %lu\nq: %lu\n", opts->p, opts->q);
    opts->n = opts->q * opts->q;
    long int phi_n = (opts->q - 1) * (opts->q - 1);
    long int e = 1 + (random() % phi_n);
    // do{
    //     e = 1 + (random() % phi_n);
    // }
    // while (gcd(e,phi_n) != 1);
}