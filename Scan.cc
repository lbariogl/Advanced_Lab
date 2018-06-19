#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TMath.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
//#include <pair>

int ReadFile(const char* input_name);

void Scan(const char* list_name = "input_files/list.txt"){
    TH2F* hScan = new TH2F("hScan",";y (cm); #theta (#circ)", 21,1.75,12.25,9,-20,340);
    float y_arr[12] = {4.,4.5,5.,5.5,6.,6.5,7.,7.5,8.5,9.,9.5,10.};
    float y_pos, angle;
    std::string file_name;
    int i=0;
    std::ifstream input_list(list_name);
    while(std::getline(input_list, file_name)){
        cout << file_name << endl;
        hScan->Fill(y_arr[i++],0.,ReadFile(Form("input_files/%s",file_name.c_str())));
    }
    TCanvas* cScan = new TCanvas("cScan","cScan");
    hScan->Draw("colz");
}



int ReadFile(const char *input_name)
{
    std::ifstream input_file1(input_name);
    int channel;
    long clock_counts;
    int energy;
    int dummy1, dummy2;

    std::vector<std::pair<long, int>> channel0, channel1;
    // Filling the  two vectors channel0 and channel1 with the time and the energy of the photons that are in the correct energy region
    while (input_file1 >> channel >> clock_counts >> energy >> dummy1 >> dummy2)
    {
        if (energy > 4000 and energy < 4500)
        {
            if (channel == 0)
            {
                channel0.push_back(std::make_pair(clock_counts, energy));
            }
            else if (channel == 1)
            {
                channel1.push_back(std::make_pair(clock_counts, energy));
            }
        }
    }

    uint index_channel1 = 0;
    uint time_window = 5;
    long diff = 0;
    int counter = 0;
    for (auto &item : channel0)
    {
        for (uint index = index_channel1; index < channel1.size(); index++)
        {
            /// updating the minimum value
            diff = item.first - channel1[index].first;
            if (TMath::Abs(diff) < time_window)
            {
                index_channel1 = index;
                counter++;
                break;
            }
        }
    }

    std::cout << "counter: " << counter << std::endl;

    return counter;
}
