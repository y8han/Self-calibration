#include<iostream>
#include<string>
#include <opencv2/opencv.hpp>
#include <random>
#include "Calibration.hpp"
using namespace std;
using namespace cv;
int main() {
    vector<vector<Point3f> > objp;
    double fx = 1500;
    double fy = 1500;
//    double H[9] = {0,1,2,3,4,5,6,7,8};
//    Mat matH = Mat( 3, 3, CV_64F, H );
//    cout << matH << endl;
    Size image_size = Size(1920, 1440);
    calibration calibration_model;
    int image_numbers = 2;
    int point_numbers = 900;
    int board_w = static_cast<int>(sqrt(point_numbers));
    float magnitude_noise = 5.5;
    vector<Mat> extrinsics;
    for (int j = 0; j < image_numbers; j++) {
        vector<Point3f> opts;
        for(int i = 0; i < point_numbers; i++) {
            opts.push_back(Point3f(static_cast<float>((i) / board_w),
                    static_cast<float>((i) % board_w), 0.0f));
        }
        objp.push_back(opts);
        Mat r1 = Mat(3, 3, CV_64FC1);
        Mat r2 = Mat(1, 3, CV_64FC1);
        RNG rnger(cv::getTickCount());
        rnger.fill(r1, cv::RNG::UNIFORM, cv::Scalar::all(0), cv::Scalar::all(256));
        rnger.fill(r2, cv::RNG::UNIFORM, cv::Scalar::all(0), cv::Scalar::all(256));
        Mat left_matrix, right_matrix, singular_values;
        SVD::compute(r1, singular_values, left_matrix, right_matrix);
        Mat rotation_matrix=left_matrix*right_matrix;
        Mat r3 = rotation_matrix;
        r3.at<double>(0,2) = r2.at<double>(0,0);
        r3.at<double>(1,2) = r2.at<double>(0,1);
        r3.at<double>(2,2) = r2.at<double>(0,2);
        cout << "r3:" << rotation_matrix << endl;
        extrinsics.push_back(r3);
    }
    calibration_model.Configuration(image_numbers, objp, image_size, fx, fy, extrinsics, magnitude_noise);
    calibration_model.imgge_points_show();
    calibration_model.calibration_kernel();
    calibration_model.add_noise();
    calibration_model.calibration_kernel();
    calibration_model.record_data();
    return 0;
}