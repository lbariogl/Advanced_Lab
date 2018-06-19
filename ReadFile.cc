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

int ReadFile(const char *input_name, float y_pos, float angle, const char* output_name = "count_list.txt")
{
    std::ifstream input_file1(input_name);
    std::ofstream output_file(output_name, std::ios_base::app);
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

    output_file << y_pos << " " << angle << " " << counter << std::endl;

    return counter;
}
