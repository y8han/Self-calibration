#include<iostream>
#include<string>
#include <opencv2/opencv.hpp>
#include <random>
#include "Calibration.hpp"
using namespace std;
using namespace cv;
int main() {
    double fx = 1500; //the ideal iintrinsic parameters fx
    double fy = 1500; //the ideal intrinsic parameters fy
    int iteration = 10; //obtain the average value from n iterations in total
    Size image_size = Size(1920, 1440);
    vector<int> board_w; //the number of image points
    for(int i = 30; i <= 50; i++)
        board_w.push_back(i);
    vector<float> magnitude_noise = {0., 0.05, 0.1, 0.15, 0.2}; //the magnitude of random noise
    calibration calibration_model(board_w.size(), magnitude_noise.size());
    int image_numbers = 2; //the number of plane objects
    calibration_model.set_iteration(iteration);
    calibration_model.Configuration(image_numbers, image_size, fx, fy);
    for(int z = 0; z < iteration; z++) {
        vector<Mat> extrinsics = calibration_model.extrin_init(image_numbers);
        calibration_model.set_extrinsic(extrinsics);
        for (int j = 0; j != magnitude_noise.size(); j++) {
            for (int i = 0; i != board_w.size(); i++) {
                vector<vector<Point3f> > objp = calibration_model.objp_init(board_w[i]);
                calibration_model.Correspondence(objp, magnitude_noise[j]);
                //calibration_model.image_points_show();
                calibration_model.add_noise(i);
                calibration_model.calibration_kernel();
            }
            calibration_model.loop_ter();
        }
        calibration_model.clear();
    }
    calibration_model.record_data(board_w[0], board_w[1]-board_w[0],
            magnitude_noise[0], magnitude_noise[1] - magnitude_noise[0]);
    return 0;
}