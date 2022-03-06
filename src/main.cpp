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

            if (size == 0){
                ERROR("Can't convert size string to and integer");
                return 1;
            }

            params = new RSAParams(size);
            generate_options(params);

            LOG("Values are");
            cout<<"P: "<<params->p.get_str()<<endl;
            cout<<"Q: "<<params->q.get_str()<<endl;
            cout<<"N: "<<params->n.get_str()<<endl;
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
