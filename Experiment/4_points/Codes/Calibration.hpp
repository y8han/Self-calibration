#include<iostream>
#include<string>
#include <opencv2/opencv.hpp>
#include <random>
#include <chrono>
using namespace std;
using namespace cv;
class calibration{
public:
    calibration() = default;
    calibration(const int &s, const int& p) {
                for(int j = 0; j != p; j++) {
                    intrinsic_new.push_back(vector<Mat>(s, Mat(3, 3, CV_64FC1)));
                    fx_error.push_back(vector<double>(s, 0));
                    fy_error.push_back(vector<double>(s, 0));
                    Frobenius_norm.push_back(vector<double>(s, 0));
                }
            }
    void Configuration(int, Size, double, double);
    void set_extrinsic(vector<Mat>);
    void Correspondence(vector<vector<Point3f> > objp, float noise);
    vector<Mat> extrin_init(int);
    vector<vector<Point3f> > objp_init(int);
    double calibration_kernel(void);
    void add_noise(int);
    void image_points_show(void);
    void record_data(string, int, int, float, float);
    void loop_ter(string, int, int);
    void set_iteration(int);
    void clear(void);
private:
    void set_image_numbers(int);
    void set_object_points(vector<vector<Point3f> >);
    void set_image_size(Size);
    void set_intrinsic(double, double);
    void set_image_points();
    void results_compare();
    void set_noise(float);
    void Calibration_camera(vector<vector<Point3f> >&, vector<vector<Point2f> >&,
                            vector<int>&, Size&, Mat&);
    int index = -1; //for number of points
    int iindex = 0; //for index of magnitude
    int image_numbers;
    vector<vector<Point2f> > image_points;
    vector<vector<Point3f> > object_points;
    vector<int> npoints;
    int iteration = 1;
    Mat intrinsic_matrix = Mat(3, 3, CV_64FC1);
    vector<vector<Mat> > intrinsic_new;
    vector<Mat> extrinsic_matirx;
    vector<vector<Mat> > extrinsic_matrix_store;
    vector<vector<float> > noise_x;
    vector<vector<float> > noise_y;
    vector<vector<double> > fx_error;
    vector<vector<double> > fy_error;
    vector<vector<double> > Frobenius_norm;
    Size image_size;
    int number = 0;
    float magnitude_of_noise = 0;
};