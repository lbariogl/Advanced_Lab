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

int ReadFile(const char *input_name);

void Scan(const char *output_name = "output_lista.txt")
{

    std::ofstream output_file(output_name);
    const int y_arr_length = 21;
    float y_arr[y_arr_length] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0};
    const int angle_arr_length = 5;//9;
    float angle[angle_arr_length] = {0, 40, 80, 120, 160};//, 200, 240, 280, 320};
    for(int i_angle = 0; i_angle < angle_arr_length; i_angle++){
        for(int i_pos = 0; i_pos < y_arr_length; i_pos++){
            int counts = ReadFile(Form("input_files/pet_scan/%.0fdeg/pet_scan_%.1fcm_%.0fdeg.txt",angle[i_angle],y_arr[i_pos],angle[i_angle]));
            float angle_bis = (angle[i_angle] > 170) ? angle[i_angle] - 360 : angle[i_angle];
            float pos_bis = y_arr[i_pos];
            if(counts<0) continue;
            output_file << angle_bis << " " << pos_bis << " " << counts << std::endl; 
        }
    }
}


void ScanSingleSource(const char *output_name = "reprocessed/singlesource.txt")
{
    std::ofstream output_file(output_name);
    
    const int y_arr_length = 11;
    float y_arr[y_arr_length] = {1.0, 4.0, 4.5, 5.0, 5.5, 6.5, 7.0, 7.5, 8.5, 9.0, 9.5};
    
    for(int i_pos = 0; i_pos < y_arr_length; ++i_pos) {
        std::cout << "Processing position: " << y_arr[i_pos] << std::endl;
        int counts = ReadFile(Form("/workspace/First_scan_with_source/find_position_intense_source_%.1fcm.txt", y_arr[i_pos]));
        float pos_bis = y_arr[i_pos];
        if (counts < 0) {
            continue;
        }
        output_file << 0 << " " << pos_bis << " " << counts << std::endl; 
    }
    
}



int ReadFile(const char *input_name)
{
    std::ifstream input_file1(input_name);
    if(!input_file1.is_open()){
        printf("File %s does not exist\n", input_name);
        return -1;
    }
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

    return counter;
}
