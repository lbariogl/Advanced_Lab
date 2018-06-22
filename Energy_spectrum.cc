#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TStopwatch.h>

#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

void Energy_spectrum(const char *input_name)
{
    TStopwatch timer;
    timer.Start();
    std::ifstream input_file(input_name);
    int channel;
    long clock_counts;
    int energy;
    int dummy1, dummy2;

    vector<int> energy_ch0;
    vector<long> time_ch0;
    vector<int> energy_ch1;
    vector<long> time_ch1;

    // scatter plot
    TH2F *h_spectrum = new TH2F("h_spectrum", ";ADC channel_0;ADC channel_1", 1000, 0.5, 16000.5, 1000, 0.5, 16000.5);

    // energy spectra in coincidence
    TH1F *hEnergy0 = new TH1F("hEnergy0", "channel 0;E (a.u.);counts", 15001, -0.5, 15000.5);
    TH1F *hEnergy1 = new TH1F("hEnergy1", "channel 1;E (a.u.);counts", 15001, -0.5, 15000.5);


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

    long unsigned k = 0;

    for(long unsigned i = 0; i < time_ch0.size(); ++i )
    {
      //if (i%1000==0){std::cout << "index: " << i << " time_ch0: " << time_ch0.at(i) << " n entries: " << h_spectrum->GetEntries() << std::endl;}
      for(long unsigned j = k; j < time_ch1.size(); ++j )
      {
        
        long temp_deltaT = time_ch0.at(i) - time_ch1.at(j);
        if (time_ch1.at(j) > time_ch0.at(i) + 5) break; //Avoids looping on all the 1s if the time difference between the 0 and the first inspected 1 is greater than 50 ns
        
        if (fabs(temp_deltaT) > 5) continue;
        
        // fill scatter plot
        h_spectrum->Fill(energy_ch0.at(i),energy_ch1.at(j));

        // fill energy spectra
        hEnergy0->Fill(energy_ch0.at(i));
        hEnergy1->Fill(energy_ch1.at(j));

        //if (i%1000==0){std::cout << "j-k: " << j-k << std::endl;}
        k = j;
        //break; //This condition avoids to associate the same 0 with more than one 1
      }
    }

    
    TCanvas *cSpectrum = new TCanvas("cSpectrum", "cSpectrum",1000,1000);
    gPad->SetLogz();
    h_spectrum->Draw("COLZ");
    cSpectrum->Update();
    cSpectrum->SaveAs(("Energy_spectrum_ScatterPlot_" + (string)input_name + ".pdf").c_str());
    cSpectrum->SaveAs(("Energy_spectrum_ScatterPlot_" + (string)input_name + ".root").c_str());


    
    TCanvas *cEnergy = new TCanvas("cEnergy", "cEnergy");
    cEnergy->Divide(1, 2);
    cEnergy->cd(1);
    hEnergy0->Draw();
    cEnergy->cd(2);
    hEnergy1->Draw();

    cEnergy->SaveAs(("Energy_spectrum_coincidence_" + (string)input_name + ".pdf").c_str());
    cEnergy->SaveAs(("Energy_spectrum_coincidence_" + (string)input_name + ".root").c_str());

}
    
