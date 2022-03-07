#include <gmpxx.h>

class RSAParams
{
private:
    
public:
    size_t size;
    mpz_class p, q, n, e, d;

    RSAParams(int);
    ~RSAParams();
};
