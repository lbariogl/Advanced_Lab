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

void Peaking()
{
    std::string filename("reprocessed/output_lista.txt");
    //std::string filename("reprocessed/uniform.txt");
    std::ifstream input_file1(filename.c_str());
    float coordinate_y = 0;
    float coordinate_theta = 0;
    float counts = 0;
    const int nAngle = 9;
    TH1F* vHisto[nAngle];
    TF1* func[nAngle];
    TSpectrum* spettro = new TSpectrum(2*2); //2*n_picchi
    for(int i = 0; i < nAngle; i++){
        vHisto[i] = new TH1F(Form("hHisto%d",i),";x (cm); counts",21,1.75,12.25);
    }
    float interval = 0.5; //cm
    float y0 = -6.8; //cm

    int prev_index = 0;

    TFile fFile("histos.root","recreate");

    std::vector<std::pair<long, int>> channel0, channel1;
    while (input_file1 >> coordinate_theta >> coordinate_y >> counts)
    {
        int index = (int)coordinate_theta/40;
        if (index != prev_index){
            int nfound = spettro->Search(vHisto[prev_index],2,"",0.1);
            double* xpeaks = spettro->GetPositionX();
            func[index] = new TF1(Form("func%d",index),"gaus(0)+gaus(3)",2,12);
            func[index]->SetParLimits(2,.01,.8);
            func[index]->SetParameter(2,0.5);
            func[index]->SetParLimits(5,.01,.8);
            func[index]->SetParameter(5,.5);
            for (int p=0;p<nfound;p++) {
                double xp = xpeaks[p];
                int bin = vHisto[prev_index]->GetXaxis()->FindBin(xp);
                double yp = vHisto[prev_index]->GetBinContent(bin);
                func[index]->SetParameter(3*p,yp);
                func[index]->SetParameter(3*p+1,xp);
            }
            vHisto[prev_index]->Fit(Form("func%d",index));
            vHisto[prev_index]->Write();
            prev_index=index;
        }
        vHisto[index]->Fill(coordinate_y,counts);
    }

    
    // vHisto[0]->Fit("func");

    // TCanvas* c = new TCanvas("c","c");
    // vHisto[0]->Draw("histo");
    // func->Draw("same");

}