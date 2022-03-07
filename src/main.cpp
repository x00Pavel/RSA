#include <getopt.h>
#include <iostream>
#include "rsa.hpp"
#include "main.hpp"
#include <string>

using namespace std;

enum MODE {E,D,B,G} mode;

int main(int argc, char *const argv[]) {
    RSAParams *params = nullptr;

    switch (getopt(argc, argv, ":e:d:b:g:h"))                            
    {
        case 'g':
        {
            mode = G;
            
            int size = stoi(optarg);
            if (size <= 0){
                ERROR("Can't convert size string to and unsigned integer");
                return 1;
            }

            params = new RSAParams(size);
            generate_options(params);

            LOG("Values are");
            LOG("P: 0x"<<params->p.get_str(16));
            LOG("Q: 0x"<<params->q.get_str(16));
            LOG("N: 0x"<<params->n.get_str(16));
            LOG("E: 0x"<<params->e.get_str(16));
            LOG("D: 0x"<<params->d.get_str(16));
            cout << "0x"<<params->p.get_str(16) 
                 << " 0x"<<params->q.get_str(16) 
                 << " 0x"<<params->n.get_str(16) 
                 << " 0x"<<params->e.get_str(16) 
                 << " 0x"<<params->d.get_str(16)
                 << endl;
            break;
        }
        case 'e':
        {   
            string e_str(optarg); // get E
            string n_str(argv[optind]); // get N
            string msg_str(argv[optind+1]); // get MSG

            mpz_class e_(e_str.substr(2), 16);
            mpz_class n_(n_str.substr(2), 16);
            mpz_class msg(msg_str.substr(2), 16);
            mpz_class cypher = encrypt(e_.get_mpz_t(),
                                       n_.get_mpz_t(),
                                       msg.get_mpz_t());
            
            cout << "0x" <<cypher.get_str(16) << endl;

            break;
        }
        case 'd':
        {
            string d_str(optarg);
            string n_str(argv[optind]); 
            string cypher_str(argv[optind+1]);

            mpz_class d_(d_str.substr(2), 16);
            mpz_class n_(n_str.substr(2), 16);
            mpz_class msg(cypher_str.substr(2), 16);
            mpz_class text = decrypt(d_.get_mpz_t(),
                                       n_.get_mpz_t(),
                                       msg.get_mpz_t());
            
            cout << "0x"<<text.get_str(16) << endl;

            break;
        }
        case 'b':
        {   
            string n_str(optarg);
            mpz_class n_(n_str.substr(2), 16);
            mpz_class factor = basic_factorisation(n_);
            cout << "0x"<<factor.get_str(16) << endl;
            break;
        }
        case '?':
        {
            ERROR("Unknown option");
            ERROR(optopt);
            break;
        }
        case 'h':
        default:
            printf("Help/Usage Example\n");
     
            break;
    }
    if (params != nullptr) delete(params);

    return 0;
}