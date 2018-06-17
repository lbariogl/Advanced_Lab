#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TFile.h>

#include <fstream>
#include <iostream>
#include <deque>
#include <cmath>

void Spectrum_online(const char *input_name, const char* output_name)
{
    std::ifstream input_file1(input_name);
    int channel;
    int dummy1, dummy2;

    struct event{
        long time;
        int energy;
    };

    event evt_tmp;

    long time_diff;
    int time_window = 5;

    std::deque<event> q_ch0, q_ch1;

    TH1F *hTime = new TH1F("hTime", ";t_{ch0} - t_{ch1}; counts", 41, -20.5, 20.5);
    TH2F *hEnergy = new TH2F("hEnergy", ";E(ch0); E(ch1)", 201, -0.5, 14000.5, 201, -0.5, 14000.5);

    std::vector<std::pair<long, int>> channel0, channel1;
    // Filling the  two vectors channel0 and channel1 with the time and the energy of the photons that are in the correct energy region
    while (input_file1 >> channel >> evt_tmp.time >> evt_tmp.energy >> dummy1 >> dummy2)
    {
        /// Filling the corresponding queue
        if (channel == 0)
        {
            q_ch0.push_back(evt_tmp);
        }
        else if (channel == 1)
        {
            q_ch1.push_back(evt_tmp);
        }
        else{
            std::cout << "schifo" << std::endl;
        }

        for(uint i_ch0 = 0; i_ch0 < q_ch0.size(); i_ch0++){

            for(uint i_ch1 = 0; i_ch1 < q_ch1.size(); i_ch1++){

                time_diff = q_ch0[i_ch0].time - q_ch1[i_ch1].time;

                if (time_diff > time_window){
                    q_ch1.pop_front();
                    i_ch1 = 0;
                    continue;
                } else if (fabs(time_diff) <= time_window){
                    hTime->Fill(time_diff);
                    hEnergy->Fill(q_ch0[i_ch0].energy,q_ch1[i_ch1].energy);
                    q_ch1.pop_front();
                    q_ch0.pop_front();
                    i_ch0 = 0;
                    break;
                } else {
                    q_ch0.pop_front();
                    i_ch0 = 0;
                    break;
                }
            }
        }
    }

    TCanvas *cTime = new TCanvas("cTime", "cTime");
    cTime->SetLogy();
    hTime->Draw();
    hTime->Fit("gaus");
    TCanvas *cEnergy = new TCanvas("cEnergy", "cEnergy");
    cEnergy->SetLogz();
    hEnergy->Draw("colz");
    TFile f(Form("ROOT_files/%s.root",output_name),"recreate");
    hTime->Write();
    hEnergy->Write();
    cEnergy->SaveAs(Form("plots/%s_energy.pdf",output_name));
    cTime->SaveAs(Form("plots/%s_time.pdf",output_name));
}
