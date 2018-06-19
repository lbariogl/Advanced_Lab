//*************************
//  usage: 
//   - specify the directory name at the end of file (e.g. inputPath = "0deg")
//   - tune the fit function and fit parameters at the end of file
//   - run with: 
//            root -l scanPET.cc++
//
//*************************

#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>

#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TH2F.h>
#include <TStopwatch.h> 
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <TStyle.h>
#include <TF1.h>


using namespace std;

int countCoincidences(const char *input_name)
{
    std::ifstream input_file(input_name);
    int channel;
    long clock_counts;
    int energy;
    int dummy1, dummy2;

    vector<int> energy_ch0;
    vector<long> time_ch0;
    vector<int> energy_ch1;
    vector<long> time_ch1;

    int minEn = 3800;
    int maxEn = 4700;

       
    // read file and fill vectors 
    while (input_file >> channel >> clock_counts >> energy >> dummy1 >> dummy1)
    {
        if (channel == 0)
        {
          //if(energy < minEn_ch0 || energy > maxEn_ch0) continue;
          time_ch0.push_back(clock_counts);
          energy_ch0.push_back(energy);   
        }
        else if (channel == 1)
        {
          //if(energy < minEn_ch1 || energy > maxEn_ch1) continue;
          time_ch1.push_back(clock_counts);
          energy_ch1.push_back(energy);  
        }
        else
        {
            std::cout << "schifo" << std::endl;
        }
    }

    // combine times
    long unsigned int k = 0;
    int nCoincidences = 0;
    for(long unsigned int i = 0; i < time_ch0.size(); ++i )
    {
      for(long unsigned int j = k; j < time_ch1.size(); ++j )
      {
        long deltaT = time_ch0.at(i) - time_ch1.at(j);
        if( time_ch1.at(j) > time_ch0.at(i) + 5) break;

        if (std::abs(deltaT) <= 5){
          if (energy_ch0.at(i) < minEn || energy_ch0.at(i) > maxEn) continue;
          if (energy_ch1.at(j) < minEn || energy_ch1.at(j) > maxEn) continue;
          k = j;
          nCoincidences++;
        }
      }
    }

    return nCoincidences;

}



void scanPET(){

  string inputPath = "0deg";

  const int dimPos = 21;
  float xPos[dimPos] = {2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.0,8.5,9.0,9.5,10.0,10.5,11.0,11.5,12.0};
  float xPos_err[dimPos];
  string sPos[dimPos] = {"2.0","2.5","3.0","3.5","4.0","4.5","5.0","5.5","6.0","6.5","7.0","7.5","8.0","8.5","9.0","9.5","10.0","10.5","11.0","11.5","12.0"};
  float nCounts[dimPos];
  float nCounts_err[dimPos];

  for(int i = 0; i < dimPos; i++){

    xPos_err[i] = 0.2;
    
    nCounts[i] = countCoincidences(Form((inputPath + "/pet_scan_%scm_%s.txt").c_str(),sPos[i].c_str(),inputPath.c_str()));
    nCounts_err[i] = TMath::Sqrt(nCounts[i]);
    cout<<nCounts[i]<<" "<<nCounts_err[i]<<endl;
  }
  
  TGraphErrors* gScan = new TGraphErrors(dimPos,xPos,nCounts,xPos_err,nCounts_err);

  TCanvas* cScan = new TCanvas("cScan","cScan");
  gScan->SetMarkerStyle(20);
  gScan->Draw("APE1");
  cout << (long) gScan << endl; 

  TF1 *fitFunc = new TF1("fitFunc","gaus(0)+gaus(3)",2,12);
  fitFunc->SetParameter(1,3.);
  fitFunc->SetParameter(4,10.);
  gScan->Fit(fitFunc,"R");  
  
  cScan->Update();

  cScan->SaveAs(Form("scanPET_%s.root",inputPath.c_str()));
  cScan->SaveAs(Form("scanPET_%s.png",inputPath.c_str()));
  

}
