#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>

#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>

void Coincidence_vec(const char *input_name)
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

    int minEn_ch0 = 3800;
    int maxEn_ch0 = 4400;
    int minEn_ch1 = 4000;
    int maxEn_ch1 = 4450;

    TH1F *hDeltaTime = new TH1F("hDeltaTime", ";Delta t (a.u.);counts", 40, -20., 20);


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

    // combine times
    long int min_delatT;
    for(long unsigned int i = 0; i < time_ch0.size(); ++i )
    {
      min_delatT = 999999999999;
      for(long unsigned int j = 0; j < time_ch1.size(); ++j )
      {
        int temp_deltaT = time_ch0.at(i) - time_ch1.at(j);
        if (fabs(temp_deltaT) > fabs(min_delatT)) continue;
        min_delatT = temp_deltaT;
      }
      
      if(fabs(min_delatT) > 20) continue;
      std::cout<<min_delatT<<std::endl;
      
      hDeltaTime->Fill(min_delatT);
    }

    
    TCanvas *cTime = new TCanvas("cTime", "cTime");
    hDeltaTime->Draw();
    hDeltaTime->Fit("gaus");
    cTime->Update();

}
