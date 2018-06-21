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

#include <TSystem.h>
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

  gStyle -> SetOptStat(0);
  gStyle -> SetOptFit(1111);
  string inputPath = "160deg";

  const int dimPos = 21;
  float xPos[dimPos] = {2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.0,8.5,9.0,9.5,10.0,10.5,11.0,11.5,12.0};
  float xPos_err[dimPos];
  string sPos[dimPos] = {"2.0","2.5","3.0","3.5","4.0","4.5","5.0","5.5","6.0","6.5","7.0","7.5","8.0","8.5","9.0","9.5","10.0","10.5","11.0","11.5","12.0"};
  float nCounts[dimPos];
  float nCounts_err[dimPos];

  long *dummy1 = 0, *dummy2 = 0, *dummy3 = 0, *dummy4 = 0;
  if(gSystem -> GetPathInfo(Form((inputPath + "/pet_scan_%s.root").c_str(),inputPath.c_str()),dummy1,dummy2,dummy3,dummy4) == 0){
    TFile *file = new TFile(Form((inputPath + "/pet_scan_%s.root").c_str(),inputPath.c_str()),"READ");
    TH1D *hScan = (TH1D*) file -> Get("hScan");

    bool isGaus1, isGaus2;
    double parGaus1[3], rangeGaus1[2], parGaus2[3], rangeGaus2[2];

    if(inputPath == "0deg" || inputPath == "40deg"){
      isGaus1 = kTRUE;
      parGaus1[0] = 100000.; parGaus1[1] = 10.; parGaus1[2] = 2.;
      rangeGaus1[0] = 8.; rangeGaus1[1] = 12.;
      isGaus2 = kTRUE;
      parGaus2[0] = 10000.; parGaus2[1] = 5.2; parGaus2[2] = 0.5;
      rangeGaus2[0] = 4. ; rangeGaus2[1] = 7.;
    }

    if(inputPath == "80deg"){
      isGaus1 = kTRUE;
      parGaus1[0] = 100000.; parGaus1[1] = 10.; parGaus1[2] = 2.;
      rangeGaus1[0] = 6.; rangeGaus1[1] = 9.;
      isGaus2 = kTRUE;
      parGaus2[0] = 10000.; parGaus2[1] = 5.2; parGaus2[2] = 0.5;
      rangeGaus2[0] = 4. ; rangeGaus2[1] = 6.;
    }

    if(inputPath == "120deg"){
      isGaus1 = kTRUE;
      parGaus1[0] = 10000.; parGaus1[1] = 5.2; parGaus1[2] = 0.5;
      rangeGaus1[0] = 4.; rangeGaus1[1] = 6.;
      isGaus2 = kFALSE;
      parGaus2[0] = 10000.; parGaus2[1] = 5.2; parGaus2[2] = 0.5;
      rangeGaus2[0] = 4. ; rangeGaus2[1] = 6.;
    }

    if(inputPath == "160deg"){
      isGaus1 = kTRUE;
      parGaus1[0] = 10000.; parGaus1[1] = 5.2; parGaus1[2] = 0.5;
      rangeGaus1[0] = 4.; rangeGaus1[1] = 6.;
      isGaus2 = kTRUE;
      parGaus2[0] = 10000.; parGaus2[1] = 5.2; parGaus2[2] = 0.5;
      rangeGaus2[0] = 7.5 ; rangeGaus2[1] = 9.;
    }
    //gScan->SetMarkerStyle(20);
    //gScan->Draw("APE1");
    //cout << (long) gScan << endl;

    TF1 *fitFunc1 = new TF1("fitFunc1","gaus",2,12);
    fitFunc1->SetParameter(0,parGaus1[0]);
    fitFunc1->SetParameter(1,parGaus1[1]);
    fitFunc1->SetParameter(2,parGaus1[2]);
    if(isGaus1) hScan->Fit(fitFunc1,"R0","",rangeGaus1[0],rangeGaus1[1]);

    TF1 *fitFunc2 = new TF1("fitFunc2","gaus",2,12);
    fitFunc2->SetParameter(0,parGaus2[0]);
    fitFunc2->SetParameter(1,parGaus2[1]);
    fitFunc2->SetParameter(2,parGaus2[2]);
    if(isGaus2) hScan->Fit(fitFunc2,"R0","",rangeGaus2[0],rangeGaus2[1]);

    TCanvas* cScan = new TCanvas("cScan","cScan");
    hScan -> Draw();
    if(isGaus1) fitFunc1 -> Draw("same");
    if(isGaus2) fitFunc2 -> Draw("same");
    cScan->SaveAs(Form("scanPET_%s.root",inputPath.c_str()));
    cScan->SaveAs(Form("scanPET_%s.png",inputPath.c_str()));
  }

  else{
    TH1D *hScan = new TH1D("hScan","",dimPos,1.75,12.25);

    for(int i = 0; i < dimPos; i++){
      xPos_err[i] = 0.2;

      nCounts[i] = countCoincidences(Form((inputPath + "/pet_scan_%scm_%s.txt").c_str(),sPos[i].c_str(),inputPath.c_str()));
      nCounts_err[i] = TMath::Sqrt(nCounts[i]);
      cout<<nCounts[i]<<" "<<nCounts_err[i]<<endl;
      hScan -> SetBinContent(i+1,nCounts[i]);
      hScan -> SetBinError(i+1,nCounts_err[i]);
    }

    TFile *file = new TFile(Form((inputPath + "/pet_scan_%s.root").c_str(),inputPath.c_str()),"RECREATE");
    hScan -> Write();
    file -> Close();
  }



    //ofstream myfile;
    //myfile.open ("example.txt","update");
    //myfile << inputPath << " " << fitFunc1 -> GetParameter(1) << " " << fitFunc2 -> GetParameter(1);
    //myfile.close();


  /*TGraphErrors* gScan = new TGraphErrors(dimPos,xPos,nCounts,xPos_err,nCounts_err);

  TCanvas* cScan = new TCanvas("cScan","cScan");
  gScan->SetMarkerStyle(20);
  gScan->Draw("APE1");
  cout << (long) gScan << endl;

  //TF1 *fitFunc = new TF1("fitFunc","gaus(0)+gaus(3)",2,12);
  TF1 *fitFunc = new TF1("fitFunc","gaus(0)",2,12);
  fitFunc->SetParameter(0,100000.);
  fitFunc->SetParameter(1,10.);
  fitFunc->SetParameter(2,2.);
  gScan->Fit(fitFunc,"R","",9,11);

  cScan->Update();

  cScan->SaveAs(Form("scanPET_%s.root",inputPath.c_str()));
  cScan->SaveAs(Form("scanPET_%s.png",inputPath.c_str()));*/


}
