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
    float vAngle[nAngle] = {
        0.,
        40.,
        80.,
        120.,
        160.,
        -160.,
        -120.,
        -80.,
        -40.,
    };
    TH1F *vHisto[nAngle];
    TF1 *func[nAngle];
    TSpectrum *spettro = new TSpectrum(2 * 2); //2*n_picchi
    for (int i = 0; i < nAngle; i++)
    {
        vHisto[i] = new TH1F(Form("hHisto%d", i), ";x (cm); counts", 31, -0.25, 15.25);
    }
    float interval = 0.5; //cm
    float y0 = -6.8;      //cm

    int prev_index = 0;

    TFile fFile("histos.root", "recreate");
    std::ofstream fit_results("fit_results.txt");

    float angle_tmp;
    while (input_file1 >> coordinate_theta >> coordinate_y >> counts)
    {
        angle_tmp = (coordinate_theta >= 0) ? coordinate_theta : 360 + coordinate_theta;
        int index = (int)angle_tmp / 40;
        if (coordinate_theta == 666)
        {
            index = nAngle;
        }
        if (index != prev_index)
        {
            int nfound = spettro->Search(vHisto[prev_index], 1, "nobackground new", 0.0005);
            std::cout << coordinate_theta << " Peaks found: " << nfound << std::endl;
            double *xpeaks = spettro->GetPositionX();
            func[prev_index] = new TF1(Form("func%d", index), "gaus(0)+gaus(3)", 2, 12);
            func[prev_index]->SetParLimits(2, .2, 2.0); //sigma
            func[prev_index]->SetParameter(2, 0.5);
            func[prev_index]->SetParLimits(5, .2, 2.0); //sigma
            func[prev_index]->SetParameter(5, .5);
            for (int p = 0; p < nfound; ++p)
            {
                double xp = xpeaks[p];
                std::cout << "Peaks found x: " << xp << std::endl;
                int bin = vHisto[prev_index]->GetXaxis()->FindBin(xp);
                double yp = vHisto[prev_index]->GetBinContent(bin);
                func[prev_index]->SetParameter(3 * p, yp);                 // Gaussian Nomalization
                func[prev_index]->SetParameter(3 * p + 1, xp);             // Gaussian mean
                func[prev_index]->SetParLimits(3 * p + 1, xp - 1, xp + 1); //sigma
            }
            vHisto[prev_index]->Fit(Form("func%d", index));
            fit_results << "angle : " << coordinate_theta << std::endl;
            fit_results << "Norm1 : " << func[prev_index]->GetParameter(0) << " Mu1 : " << func[prev_index]->GetParameter(1) << " Sigma1 : " << func[prev_index]->GetParameter(2) << std::endl;

            fit_results << "Norm2 : " << func[prev_index]->GetParameter(3) << " Mu2 : " << func[prev_index]->GetParameter(4) << " Sigma2 : " << func[prev_index]->GetParameter(5) << std::endl;
            fit_results << "****************************************************" << std::endl;
            vHisto[prev_index]->Write();
            prev_index = index;
        }
        if (index == nAngle)
        {
            break;
        }
        vHisto[index]->Fill(coordinate_y, counts);
        if (vHisto[index]->GetBinContent(vHisto[index]->FindBin(coordinate_y)) > 0)
        {
            vHisto[index]->SetBinError(vHisto[index]->FindBin(coordinate_y), TMath::Sqrt(counts));
        }
    }

    // vHisto[0]->Fit("func");

    // TCanvas* c = new TCanvas("c","c");
    // vHisto[0]->Draw("histo");
    // func->Draw("same");
}