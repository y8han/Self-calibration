#include<iostream>
#include<string>
#include <opencv2/opencv.hpp>
#include <random>
using namespace std;
using namespace cv;
class calibration{
public:
    calibration() = default;
    void Configuration(int, vector<vector<Point3f> >, Size, double, double, vector<Mat>, float);
    double calibration_kernel(void);
    void add_noise(void);
    void image_points_show(void);
    void record_data(void);
private:
    void set_image_numbers(int);
    void set_object_points(vector<vector<Point3f> >);
    void set_image_size(Size);
    void set_intrinsic(double, double);
    void set_extrinsic(vector<Mat>);
    void set_image_points();
    void set_iteration(int);
    void results_compare();
    void set_noise(float);
    void Calibration_camera(vector<vector<Point3f> >&, vector<vector<Point2f> >&,
                            vector<int>&, Size&, Mat&);
    vector<vector<Point2f> > image_points;
    vector<vector<Point3f> > object_points;
    vector<int> npoints;
    Mat intrinsic_matrix = Mat(3, 3, CV_64FC1);
    Mat intrinsic_new = Mat(3, 3, CV_64FC1);
    Mat intrinsic_noise = Mat(3, 3, CV_64FC1);
    vector<Mat> extrinsic_matirx;
    double reprojection_error;
    double reprojection_error_noise;
    int iteration = 1;
    Size image_size;
    int number = 0;
    float magnitude_of_noise = 0;
    bool noise_add = false;
};