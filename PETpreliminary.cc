#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TStyle.h>

#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>

void PETpreliminary(const char *input_name)
{
    std::ifstream input_file(input_name);
    int channel;
    int clock_counts;
    int energy;
    int dummy1, dummy2;

    vector<int> energy_ch0;
    vector<int> time_ch0;
    vector<int> energy_ch1;
    vector<int> time_ch1;

    int minEn_ch0 = 3900;
    int maxEn_ch0 = 4600;
    int minEn_ch1 = 3900;
    int maxEn_ch1 = 4600;

    TH1F *hDeltaTime = new TH1F("hDeltaTime", ";Delta t (x10ns);counts", 40, -20., 20);


    // read file and fill vectors 
    while (input_file >> channel >> clock_counts >> energy >> dummy1 >> dummy1)
    {
        if (channel == 0)
        {
          if(energy < minEn_ch0 || energy > maxEn_ch0) continue;
          time_ch0.push_back(clock_counts);
          energy_ch0.push_back(energy);   
        }
        else if (channel == 1)
        {
          if(energy < minEn_ch1 || energy > maxEn_ch1) continue;
          time_ch1.push_back(clock_counts);
          energy_ch1.push_back(energy);  
        }
        else
        {
            std::cout << "schifo" << std::endl;
        }
    }

    long counter = 0;

    // combine times
    long int min_deltaT = 5;
    for(long unsigned int i = 0; i < time_ch0.size(); ++i )
    {
      for(long unsigned int j = 0; j < time_ch1.size(); ++j )
      {
        int temp_deltaT = time_ch0.at(i) - time_ch1.at(j);
        if (fabs(temp_deltaT) > fabs(min_deltaT)){
          continue;
        }
        counter++;
      }
    }

    std::cout << "Counter: " << counter << std::endl;

}
