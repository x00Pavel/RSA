typedef struct
{
    int size; // Size of publick modulus
    long int p, q; // First and second prime numbers
    long int n; // Publick modulus
    long int pub_exp, priv_exp; // Publick and private exponents
} rsa_options;

void generate_options(rsa_options *);
long int gcd(long int, long int);