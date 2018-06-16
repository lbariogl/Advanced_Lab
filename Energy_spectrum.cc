#include <TH2F.h>
#include <TCanvas.h>

#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

void Energy_spectrum(const char *input_name)
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

    float sigma_DeltaT = 0.6; //This means 6ns (times in the input file are expressed in tens of ns)

    TH2F *h_spectrum = new TH2F("h_spectrum", ";ADC channel_0;ADC channel_1", 4000, 0.5, 16000.5, 4000, 0.5, 16000.5);


    // read file and fill vectors 
    while (input_file >> channel >> clock_counts >> energy >> dummy1 >> dummy1)
    {
        if (channel == 0)
        {
          time_ch0.push_back(clock_counts);
          energy_ch0.push_back(energy);   
        }
        else if (channel == 1)
        {
          time_ch1.push_back(clock_counts);
          energy_ch1.push_back(energy);  
        }
        else
        {
            std::cout << "schifo" << std::endl;
        }
    }

    // combine times
    for(long unsigned int i = 0; i < time_ch0.size(); ++i )
    {
      for(long unsigned int j = 0; j < time_ch1.size(); ++j )
      {
        int temp_deltaT = time_ch0.at(i) - time_ch1.at(j);
        if (fabs(temp_deltaT) > 5) continue;
        h_spectrum->Fill(energy_ch0.at(i),energy_ch1.at(j));
      }
    }

    
    TCanvas *cSpectrum = new TCanvas("cSpectrum", "cSpectrum",600,600);
    h_spectrum->Draw("COLZ");
    cSpectrum->Update();
    cSpectrum->SaveAs(("Energy_spectrum_" + (string)input_name + ".pdf").c_str());
    cSpectrum->SaveAs(("Energy_spectrum_" + (string)input_name + ".root").c_str());

}
    
