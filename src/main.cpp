#include <getopt.h>
#include <iostream>
#include "rsa.hpp"

using namespace std;

enum MODE {E,D,B,G} mode;

int main(int argc, char *const argv[]) {
    switch (getopt(argc, argv, ":e:d:b:g:h"))                            
    {
        case 'g':
        {
            mode = G;
            int size;
            try {
                size = stoi(optarg);
            }
            catch (std::invalid_argument const&){
                printf("Not a number: %s\n", optarg);
                return 1;
            }

            rsa_options opts = {size, 0, 0, 0, 0, 0};
            generate_options(&opts);

            cout << "P is: " << opts.p << endl;
            
            break;
        }
        case '?':
        {
            printf("Unknown option: %c\n", optopt);
            break;

        }
        case 'h':
        default:
            printf("Help/Usage Example\n");
            break;
    }


    
    return 0;
}
