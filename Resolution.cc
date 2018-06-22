#include <TH1F.h>
#include <TF1.h>
#include <TH2F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TSpectrum.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>

void Resolution()
{
    std::string filename("reprocessed/singlesource.txt");
    //std::string filename("reprocessed/uniform.txt");
    std::ifstream input_file1(filename.c_str());
    float coordinate_y = 0;
    float coordinate_theta = 0;
    float counts = 0;
    TH1F *hHisto = new TH1F("hHisto", ";x (cm); counts", 31, -0.25, 15.25);
    TF1 *func = new TF1("func", "gaus", 2, 10);
    float interval = 0.5; //cm
    float y0 = -6.8;      //cm

    TFile fFile("resolution.root", "recreate");

    while (input_file1 >> coordinate_theta >> coordinate_y >> counts)
    {
        hHisto->Fill(coordinate_y, counts);
        hHisto->SetBinError(hHisto->FindBin(coordinate_y), TMath::Sqrt(counts));
    }
    hHisto->Fit("func");
    hHisto->Write();

    std::cout << "sigma : " << func->GetParameter(2) << std::endl;
}