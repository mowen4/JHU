
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class InputOutput
{
    //TODO: Checking for errors, for negative values and potentially too large values?

    public:
        void getInputs(int inputs[3])
        {
            //temporary local variables
            int temp = 0;
            bool input_ok;

            std::cout << "Triangle Tester" << std::endl;
            
            //for each side repeat the inner loop until a valid integer input is detected
            for ( int i = 0; i < 3; ++i )
            {
                do
                {
                    cout << "Enter side " << i +1 <<":";
                    cin >> inputs[i];

                    if ( cin.fail() )
                    {
                        cout << "Error: Conversion to integer failed!\n";

                        input_ok = false;
                    }
                    else if ( inputs[i] > 2147483647 || inputs[i] <= 0 )
                    {
                        cout << "Error: Number should in range of (1 to 2147483647) only!\n";
                        input_ok = false;
                    }
                    else
                    {
                        input_ok = true;
                    }

                    cin.clear();

                    cin.ignore(2147483647, '\n' );
                } while ( !input_ok );            
            }
        }
};
