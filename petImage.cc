#include <TH1F.h>
#include <TF1.h>
#include <TH2F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TMath.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>


//
// Marco Boretto marco.bore@gmail.com
// 19 06 2018
void petImage(int resolution = 401)
{
    std::string filename("reprocessed/output_lista.txt");
    //std::string filename("reprocessed/uniform.txt");
    std::ifstream input_file1(filename.c_str());
    float coordinate_y = 0;
    float coordinate_theta = 0;
    float counts = 0;

    float interval = 0.5; //cm
    float y0 = -6.8; //cm

    TH2F* pet = new TH2F("Pet", ";x (cm); y (cm)", resolution, -10.05, 10.05, resolution, -10.05, 10.05);

    std::vector<std::pair<long, int>> channel0, channel1;
    while (input_file1 >> coordinate_theta >> coordinate_y >> counts)
    {
        float coordinate_theta_rad = (float) coordinate_theta / 360 * 2 * TMath::Pi();
         
        std::cout << "Reading theta " << coordinate_theta << " " << coordinate_y << " " <<  counts << std::endl;
        //std::cout << "theta " << coordinate_theta << " sin " << sin(coordinate_theta_rad) << " cos " << cos(coordinate_theta_rad) << std::endl;
        float epsilon1 = coordinate_y - interval / 2;
        float epsilon2 = coordinate_y + interval / 2;

        float m = sin(coordinate_theta_rad) / cos(coordinate_theta_rad); 
        float q1 = (epsilon1 + y0) * (cos(coordinate_theta_rad) + sin(coordinate_theta_rad) * sin(coordinate_theta_rad) / cos(coordinate_theta_rad));

        float q2 = (epsilon2 + y0) * (cos(coordinate_theta_rad) + sin(coordinate_theta_rad) * sin(coordinate_theta_rad) / cos(coordinate_theta_rad));
        //if (coordinate_theta > 30) {
        //    break;
        //}


        for (int i_x = 1; i_x < resolution; ++i_x) {
            double x_box = pet->GetXaxis()->GetBinCenter(i_x);
            double y_box1 = m * x_box + q1;
            double y_box2 = m * x_box + q2;

            //pet->Fill(x_box, y_box1, 400);
            //continue;

            //TAxis *xaxis = pet->GetXaxis();
            TAxis *yaxis = pet->GetYaxis();
            //Int_t binx = xaxis->FindBin(x);
            int i_y_min = yaxis->FindBin(y_box1);
            int i_y_max = yaxis->FindBin(y_box2);

            //if (i_y_min == i_y_max) {
            //    std::cout << "i_y_min == i_y_max theta: " << coordinate_theta << std::endl;
            //}
            if (i_y_min > i_y_max) {
                int y_temp = i_y_min;
                i_y_min = i_y_max;
                i_y_max  = y_temp;
            }

            for (int i_y = i_y_min; i_y < i_y_max; ++i_y) {
                double y_box_fill = pet->GetYaxis()->GetBinCenter(i_y);

                //if (pet->GetBinContent(x_box, y_box_fill) > 100) {

                //} else {
                //    pet->Fill(x_box, y_box_fill, counts);
                //}
                pet->Fill(x_box, y_box_fill, counts);
            }
        }
    }

    TCanvas *cpet = new TCanvas("cTime", "Reconstructed PET Image", 900, 900);
    pet->Draw("colz");

    cpet->SetLogz();
    std::array<std::pair<double, double>, 4> points = {
        std::make_pair(-2.5, 4),
        std::make_pair(2.5, 4),
        std::make_pair(-2.5, -4),
        std::make_pair(2.5, -4),
    };

    //Draw the box borders
    TLine* line = nullptr;

    //orizontal
    line = new TLine(points[0].first, points[0].second, points[1].first, points[1].second);
    line->SetLineColor(kBlue);
    line->SetLineWidth(3);
    line->Draw("same");
    line = new TLine(points[2].first, points[2].second, points[3].first, points[3].second);
    line->SetLineColor(kBlue);
    line->SetLineWidth(3);
    line->Draw("same");

    //vertical
    line = new TLine(points[0].first, points[0].second, points[2].first, points[2].second);
    line->SetLineColor(kBlue);
    line->SetLineWidth(3);
    line->Draw("same");
    line = new TLine(points[1].first, points[1].second, points[3].first, points[3].second);
    line->SetLineColor(kBlue);
    line->SetLineWidth(3);
    line->Draw("same");


//    hTime->Fit("gaus");
//    TFile f(Form("ROOT_files/%s.root",output_name),"recreate");
//    hTime->Write();
//    cTime->SaveAs(Form("plots/%s.pdf",output_name));
}
