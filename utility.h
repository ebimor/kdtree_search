#ifndef UTILITY_H
#define UTILITY_H

#include <random>
#include "plot_lib/pbPlots.hpp"
#include "plot_lib/supportLib.hpp"
#include <vector>
#include <algorithm>
#include <chrono>

/*
   utility functions are included in this file
 */

 /**
 @brief class timer for measuring execution time
 */
class timer {
public:
timer(){
        std::cout<<"timer initiated"<<std::endl;
}
void tic(){
        start_time = std::chrono::system_clock::now();
}

double toc(){
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start_time;
        return elapsed_seconds.count();
}
private:
std::chrono::time_point<std::chrono::system_clock> start_time;
};


/**
 * @brief random function to sample a point uniformly in the range high and low
 */
double generate_random(double lowerbound, double upperbound){
        random_device rd; // obtain a random number from hardware
        mt19937 gen(rd()); // seed the generator
        uniform_real_distribution<double> distr(lowerbound, upperbound); // define the range
        return distr(gen);
}

/**
 * @brief plots saves the points in the tree as a scatter image
 */
void export_to_image(const vector<vector<double> >& tree, const vector<double>& point, const vector<vector<double> >& res){

        RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();

        vector<double> xs, ys;
        for(vector<double> node : tree) {
                xs.push_back(node[0]);
                ys.push_back(node[1]);
        }

        vector<double> xs_p, ys_p;
        xs_p.push_back(point[0]);
        ys_p.push_back(point[1]);

        vector<double> xs_res, ys_res;
        for(vector<double> node : res) {
                xs_res.push_back(node[0]);
                ys_res.push_back(node[1]);
        }

        ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
        series->xs = &xs;
        series->ys = &ys;
        series->linearInterpolation = false;
        series->pointType = toVector(L"circles");
        series->lineThickness = 15;
        series->color = CreateRGBColor(0,1,0);

        ScatterPlotSeries *series2 = GetDefaultScatterPlotSeriesSettings();
        series2->xs = &xs_p;
        series2->ys = &ys_p;
        series2->linearInterpolation = false;
        series2->pointType = toVector(L"circles");
        series2->lineThickness = 20;
        series2->color = CreateRGBColor(1,0,0);

        ScatterPlotSeries *series3 = GetDefaultScatterPlotSeriesSettings();
        series3->xs = &xs_res;
        series3->ys = &ys_res;
        series3->linearInterpolation = false;
        series3->pointType = toVector(L"triangles");
        series3->lineThickness = 20;
        series3->color = CreateRGBColor(0,0,1);

        ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
        settings->width = 1000;
        settings->height = 1000;
        settings->autoBoundaries = true;
        settings->autoPadding = true;
        settings->title = toVector(L"Samples");
        settings->xLabel = toVector(L"Y axis");
        settings->yLabel = toVector(L"X axis");

        settings->scatterPlotSeries->push_back(series);
        settings->scatterPlotSeries->push_back(series2);
        settings->scatterPlotSeries->push_back(series3);


        DrawScatterPlotFromSettings(imageReference, settings);


        vector<double> *pngdata = ConvertToPNG(imageReference->image);
        WriteToFile(pngdata, "sample_points.png");
        DeleteImage(imageReference->image);

}

#endif // UTILITY_H
