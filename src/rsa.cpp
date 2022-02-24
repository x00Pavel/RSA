#include <iostream>
#include <math.h>
#include "rsa.hpp"

using namespace std;


long int p, q, n;


long int gcd(long int a, long int b){
    int tmp = a % b;
    while (tmp != 0)
    {
        tmp = a % b;
        a = b;
        b = tmp;
    }
    return b;
}

long int generate_prime(){
    return n;
}

void generate_options(rsa_options *opts){
    // opts->q = generate_prime();
    // opts->p = generate_prime();
    opts->q = 13;
    opts->p = 11;
    long int n = opts->q * opts->q;
    long int phi_n = (opts->q - 1) * (opts->q - 1);
    long int e = 1 + (random() % phi_n);
    do{
        e = 1 + (random() % phi_n);
    }
    while (gcd(e,phi_n) != 1);
    
}