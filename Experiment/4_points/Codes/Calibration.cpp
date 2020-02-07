#include "Calibration.hpp"
using namespace std;
using namespace cv;
void calibration::set_image_numbers(int n) {
    if(number == 0)
        number = n;
    else
        cout << "The number of images has been set!" << endl;
}
void calibration::set_object_points(vector<vector<Point3f> > input) {
    int image_numbers = input.size();
    if(image_numbers == number) {
        for(int i = 0; i < number; i ++)
            object_points.push_back(input[i]);}
    else {
        if(image_numbers > number)
            for(int i = 0; i < number; i ++)
                object_points.push_back(input[i]);
        else
            cout << "Wrong input images! Required more images!" << endl;
    }
}
void calibration::set_image_size(Size size) {
    if(size.height > 0 && size.width > 0)
        image_size = size;
}
void calibration::set_intrinsic(double fx, double fy){
    double &f1 = intrinsic_matrix.at<double>(0,0);
    double &f2 = intrinsic_matrix.at<double>(1,1);
    double &cx = intrinsic_matrix.at<double>(0,2);
    double &cy = intrinsic_matrix.at<double>(1,2);
    f1 = fx;
    f2 = fy;
    cx = image_size.width / 2;
    cy = image_size.height / 2;
    intrinsic_matrix.at<double>(2,2) = 1;
}
void calibration::set_extrinsic(vector<Mat> n) {
    if(n.size() == number) {
        for(int i = 0; i < number; i++)
            extrinsic_matirx.push_back(n[i]); }
    else{
        if(n.size() > number){
            for(int i = 0; i < number; i++)
                extrinsic_matirx.push_back(n[i]);
        }else
            cout << "Wrong input images! Required more images!" << endl; }
}
void calibration::set_image_points(){
    double fx=intrinsic_matrix.at<double>(0,0);
    double cx=intrinsic_matrix.at<double>(0,2);
    double fy=intrinsic_matrix.at<double>(1,1);
    double cy=intrinsic_matrix.at<double>(1,2);
    for(int i =0; i < number; i++)
    {
        vector<Point3f> object = object_points[i];
        Mat extrinsic = extrinsic_matirx[i];
        double r0_0 = extrinsic.at<double>(0, 0);
        double r0_1 = extrinsic.at<double>(0, 1);
        double r1_0 = extrinsic.at<double>(1, 0);
        double r1_1 = extrinsic.at<double>(1, 1);
        double r2_0 = extrinsic.at<double>(2, 0);
        double r2_1 = extrinsic.at<double>(2, 1);
        double t_0=extrinsic.at<double>(0,2);
        double t_1=extrinsic.at<double>(1,2);
        double t_2=extrinsic.at<double>(2,2);
        vector<Point2f> image_point;
        for(int i = 0; i < object.size(); i++)
        {
            Point2f new_point;
            double tmp=r2_0*object[i].x+r2_1*object[i].y+t_2;
            new_point.x=fx*(r0_0*object[i].x+r0_1*object[i].y+t_0)/tmp+cx;
            new_point.y=fy*(r1_0*object[i].x+r1_1*object[i].y+t_1)/tmp+cy;
            image_point.push_back(new_point);
        }
        image_points.push_back(image_point);
    }
}
void calibration::set_iteration(int n) {
    iteration = n;
}
void calibration::set_noise(float n){
    magnitude_of_noise = n;
}
void calibration::Configuration(int number, vector<vector<Point3f> > objp, Size size, double fx, double fy, vector<Mat> ex, float noise){
    set_image_numbers(number);
    set_object_points(objp);
    set_image_size(size);
    set_intrinsic(fx,fy);
    set_extrinsic(ex);
    set_image_points();
    set_iteration(1);
    set_noise(noise);
}
void calibration::results_compare(){
    if(!noise_add) {
        cout << "The original intrinsic matrix is:\n" << intrinsic_matrix;
        cout << "The computed intrinsic matrix is:\n" << intrinsic_new;
    }
    else{
        cout << "The original intrinsic matrix is:\n" << intrinsic_matrix;
        cout << "The computed intrinsic matrix is:\n" << intrinsic_noise;
    }
}
double calibration::calibration_kernel(){
    Mat distortion_coeffs;
    if(!noise_add) {
        reprojection_error = cv::calibrateCamera(
                object_points, image_points, image_size, intrinsic_new,
                distortion_coeffs, cv::noArray(), cv::noArray(),
                cv::CALIB_ZERO_TANGENT_DIST,
                TermCriteria(
                        TermCriteria::COUNT + TermCriteria::EPS, iteration, DBL_EPSILON));
        cout << "The reprojection error is:" << reprojection_error << endl;
        results_compare();
        noise_add = true;
    } else{
        reprojection_error_noise = cv::calibrateCamera(
                object_points, image_points, image_size, intrinsic_noise,
                distortion_coeffs, cv::noArray(), cv::noArray(),
                cv::CALIB_ZERO_TANGENT_DIST,
                TermCriteria(
                        TermCriteria::COUNT + TermCriteria::EPS, iteration, DBL_EPSILON));
        cout << "The reprojection error is:" << reprojection_error_noise << endl;
        results_compare();
    }
}
void calibration::add_noise(void) {
    default_random_engine e;
    normal_distribution<float> n(0, magnitude_of_noise);
    for(int i = 0; i < number; i++) {
        vector<Point2f>& image_p = image_points[i];
        for(int j = 0; j < image_p.size(); j++){
            Point2f& image = image_p[j];
            image.x += n(e);
            image.y += n(e);
        }
    }
}
void calibration::imgge_points_show() {
    for(int i = 0; i < image_points.size(); i++)
    {
        vector<Point2f> i_point = image_points[i];
        for(int j = 0; j < i_point.size(); j++)
            cout << i_point[j] << endl;
    }
}
void calibration::record_data() {
    // save results
    int numberPoints = 0;
    if(image_points.size() > 0)
        numberPoints = image_points[0].size();
    string file_name("../ResultIdealPoints/");
    file_name += to_string(static_cast<int>(sqrt(numberPoints)));
    file_name += "*";
    file_name += to_string(static_cast<int>(sqrt(numberPoints)));
    file_name += string(".xml");
    cv::FileStorage fs(file_name, cv::FileStorage::WRITE);
    fs << "image_width" << image_size.width << "image_height" << image_size.height
       << "Original_camera_matrix" << intrinsic_matrix << "Calibrated_camera_matrix" <<
       intrinsic_new;
    fs << "object_points" << object_points << "image_points" << image_points ;
    fs << "Generated_extrinsic_matrix" << extrinsic_matirx << "reprojection_error" << reprojection_error;
    fs << "variance_of_random_noise" << magnitude_of_noise;
    fs << "Original_camera_matrix" << intrinsic_matrix << "Calibrated_camera_matrix_noise" <<
       intrinsic_noise << "reprojection_error_noise" << reprojection_error_noise;
    fs.release();
}