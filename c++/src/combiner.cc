
// C++ program to demonstrate the
// cin object
#include <iostream>
#include <list>
#include <vector>
#include <fstream>
using namespace std;
  
// Driver Code
int main(int argc, char * argv[])
{

    vector<vector<int>> samples;

    std::cerr << "num args: " << argc << "\n";

    for (int i = 1; i < argc; i++) {
        std::cerr << "doing " << i << ", name: " << argv[i] << "\n";
        ifstream file(argv[i]);
        int read;
        samples.push_back(vector<int>());
        while (file >> read)
        {
            samples[i - 1].push_back(read);
        }
        std::cerr << "finished ";
        std::cerr << "light " << i << " size: " << samples[i - 1].size() << "\n";
        file.close();
    }

    for (int i = 0; i < samples[0].size(); i += 3) {
        int r = 0;
        int g = 0;
        int b = 0;
        int num_samples = samples.size();
        for (int j = 0; j < num_samples; j++) {
            r += samples[j][i];
            g += samples[j][i + 1];
            b += samples[j][i + 2];
        }
        r /= num_samples;
        g /= num_samples;
        b /= num_samples;
        cout << r << " " << g << " " << b << "\n";
    }
    
    
    return 0;
}