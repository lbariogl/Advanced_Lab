#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>

#include <fstream>
#include <iostream>

void Calibration(const char *input_name)
{
    std::ifstream input_file(input_name);
    int channel;
    long clock_counts;
    int energy;
    int dummy1, dummy2;

    TH1F *hEnergy0 = new TH1F("hEnergy0", "channel 0;E (a.u.);counts", 15001, -0.5, 15000.5);
    TH1F *hEnergy1 = new TH1F("hEnergy1", "channel 1;E (a.u.);counts", 15001, -0.5, 15000.5);

    while (input_file >> channel >> clock_counts >> energy >> dummy1 >> dummy1)
    {
        if (channel == 0)
        {
            hEnergy0->Fill(energy);
        }
        else if (channel == 1)
        {
            hEnergy1->Fill(energy);
        }
        else
        {
            std::cout << "schifo" << std::endl;
        }
    }
    TCanvas *cEnergy = new TCanvas("cEnergy", "cEnergy");
    cEnergy->Divide(1, 2);
    cEnergy->cd(1);
    hEnergy0->Draw();
    cEnergy->cd(2);
    hEnergy1->Draw();
}