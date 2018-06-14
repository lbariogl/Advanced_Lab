#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TMath.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
//#include <pair>

void Time(const char *input_name)
{
    std::ifstream input_file1(input_name);
    int channel;
    long clock_counts;
    int energy;
    int dummy1, dummy2;

    TH1F *hTime = new TH1F("hTime", ";t (clock counts); counts", 10001, -0.5, 10000.5);

    std::vector<std::pair<long, int>> channel0, channel1;
    while (input_file1 >> channel >> clock_counts >> energy >> dummy1 >> dummy1)
    {
        if (energy > 3000 and energy < 5000)
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
    for (auto &&item : channel0)
    {

        long minimum = 999999999999;
        for (auto &&item1 : channel1)
        {
            if (TMath::Abs(item.first - item1.first) < minimum)
            {
                minimum = TMath::Abs(item.first - item1.first);
                continue;
            }

            hTime->Fill(minimum);
            std::cout << minimum << std::endl;
            break;
            index_channel1++;
        }
    }

    TCanvas *cTime = new TCanvas("cTime", "cTime");
    hTime->Draw();
}