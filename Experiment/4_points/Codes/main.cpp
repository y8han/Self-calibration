#include<iostream>
#include<string>
#include <opencv2/opencv.hpp>
#include <random>
#include "Calibration.hpp"
using namespace std;
using namespace cv;i
int main() {
    double fx = 1500; //the ideal iintrinsic parameters fx
    double fy = 1500; //the ideal intrinsic parameters fy
    Size image_size = Size(1920, 1440);
    vector<int> board_w = {2,3,4,5,6,7,8,9,10}; //the number of image points(each row & each column)
    vector<float> magnitude_noise = {0., 0.05, 0.1, 0.15, 0.2}; //the magnitude of random noise
    calibration calibration_model(board_w.size(), magnitude_noise.size());
    int image_numbers = 2; //the number of plane objects
    vector<Mat> extrinsics = calibration_model.extrin_init(image_numbers);
    calibration_model.Configuration(image_numbers, image_size, fx, fy, extrinsics);
    for(int j = 0; j != magnitude_noise.size(); j++) {
        for (int i = 0; i != board_w.size(); i++) {
            int point_numbers = pow(board_w[i], 2);
            vector<vector<Point3f> > objp = calibration_model.objp_init(point_numbers);
            calibration_model.Correspondence(objp, magnitude_noise[j]);
            //calibration_model.image_points_show();
            calibration_model.add_noise();
            calibration_model.calibration_kernel();
        }
        calibration_model.loop_ter();
    }
    calibration_model.record_data();
    return 0;
}