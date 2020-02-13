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
        for(int i = 0; i < number; i ++) {
            object_points.push_back(input[i]);
            npoints.push_back(input[i].size());
        }
    }
    else {
        if(image_numbers > number)
            for(int i = 0; i < number; i ++) {
                object_points.push_back(input[i]);
                npoints.push_back(input[i].size());
            }
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
        cout << "The original intrinsic matrix is:\n" << intrinsic_matrix << endl;
        cout << "The computed intrinsic matrix is:\n" << intrinsic_new << endl;
    }
    else{
        cout << "The original intrinsic matrix is:\n" << intrinsic_matrix << endl;
        cout << "The computed intrinsic matrix is:\n" << intrinsic_noise << endl;
    }
}
double calibration::calibration_kernel(){
    Mat distortion_coeffs;
    if(!noise_add) {
        Calibration_camera(object_points, image_points, npoints, image_size, intrinsic_new);
        results_compare();
        noise_add = true;
    } else{
        Calibration_camera(object_points, image_points, npoints, image_size, intrinsic_noise);
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
void calibration::image_points_show() {
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
    //chech whether the file already exists.
    string file_name("./ResultIdealPoints/");
    file_name += to_string(static_cast<int>(sqrt(numberPoints)));
    file_name += "*";
    file_name += to_string(static_cast<int>(sqrt(numberPoints)));
    file_name += string(".xml");
    fstream _file;
    _file.open(file_name, ios::in);
    if(!_file) {
        cv::FileStorage fs(file_name, cv::FileStorage::WRITE);
        fs << "image_width" << image_size.width << "image_height" << image_size.height
           << "Original_camera_matrix" << intrinsic_matrix << "Calibrated_camera_matrix" <<
           intrinsic_new;
        fs << "object_points" << object_points << "image_points" << image_points;
        fs << "Generated_extrinsic_matrix" << extrinsic_matirx << "reprojection_error" << reprojection_error;
        fs << "variance_of_random_noise" << magnitude_of_noise;
        fs << "Original_camera_matrix" << intrinsic_matrix << "Calibrated_camera_matrix_noise" <<
           intrinsic_noise << "reprojection_error_noise" << reprojection_error_noise;
        fs.release();
    } else
        cout << file_name + " already exists!\n";
}
void calibration::Calibration_camera(vector<vector<Point3f> >& objectPoints, vector<vector<Point2f> >& imagePoints,
                                     vector<int>& npoints, Size& imageSize, Mat& cameraMatrix) {
    Ptr<CvMat> matA, _b, _allH;

    int i, j, pos, nimages, ni = 0;
    nimages = npoints.size();
    double a[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };
    double H[9] = {0}, f[2] = {0};
    Mat matH = Mat( 3, 3, CV_64F, H );
    CvMat _f = cvMat( 2, 1, CV_64F, f );
    matA.reset(cvCreateMat( 2*nimages, 2, CV_64F ));
    _b.reset(cvCreateMat( 2*nimages, 1, CV_64F ));
    _allH.reset(cvCreateMat( nimages, 9, CV_64F ));
    a[2] = (!imageSize.width) ? 0.5 : (imageSize.width)*0.5;
    a[5] = (!imageSize.height) ? 0.5 : (imageSize.height)*0.5;
    for(i = 0, pos = 0; i < nimages; i++, pos += ni ) {
        double *Ap = matA->data.db + i * 4;
        double *bp = _b->data.db + i * 2;
        ni = npoints[i];
        double h[3], v[3], d1[3], d2[3];
        double n[4] = {0,0,0,0};
        matH = findHomography( objectPoints[i], imagePoints[i]);
        H[0] = matH.at<double>(0,0);
        H[1] = matH.at<double>(0,1);
        H[2] = matH.at<double>(0,2);
        H[3] = matH.at<double>(1,0);
        H[4] = matH.at<double>(1,1);
        H[5] = matH.at<double>(1,2);
        H[6] = matH.at<double>(2,0);
        H[7] = matH.at<double>(2,1);
        H[8] = matH.at<double>(2,2);
        //cout << matH << endl;
        memcpy( _allH->data.db + i*9, H, sizeof(H) );
        H[0] -= H[6]*a[2]; H[1] -= H[7]*a[2]; H[2] -= H[8]*a[2]; //After this operation:
        //H[0] = fx*R00, H[1] = fx*R01, H[2] = fx*t0
        H[3] -= H[6]*a[5]; H[4] -= H[7]*a[5]; H[5] -= H[8]*a[5];
        for( j = 0; j < 3; j++ )
        {
            double t0 = H[j*3], t1 = H[j*3+1];
            h[j] = t0; v[j] = t1;
            d1[j] = (t0 + t1)*0.5;
            d2[j] = (t0 - t1)*0.5;
            n[0] += t0*t0; n[1] += t1*t1; //the initial value of n is zero
            n[2] += d1[j]*d1[j]; n[3] += d2[j]*d2[j];
        }
        for( j = 0; j < 4; j++ )
            n[j] = 1./std::sqrt(n[j]);
        for( j = 0; j < 3; j++ )
        {
            h[j] *= n[0]; v[j] *= n[1];
            d1[j] *= n[2]; d2[j] *= n[3];
        }
        Ap[0] = h[0]*v[0]; Ap[1] = h[1]*v[1];  //Ap -> matA
        Ap[2] = d1[0]*d2[0]; Ap[3] = d1[1]*d2[1];  //bp -> _b
        //ap
        bp[0] = -h[2]*v[2]; bp[1] = -d1[2]*d2[2];  //remove scale?
    }
    cvSolve( matA, _b, &_f, CV_NORMAL + CV_SVD );
    a[0] = std::sqrt(fabs(1./f[0]));
    a[4] = std::sqrt(fabs(1./f[1]));
    for(int i = 0; i++; i<3)
        for(int j= 0; j++; j<3)
            cameraMatrix.at<double>(i,j) = 0;
    cameraMatrix.at<double>(0,0) = a[0];
    cameraMatrix.at<double>(0,2) = a[2];
    cameraMatrix.at<double>(1,1) = a[4];
    cameraMatrix.at<double>(1,2) = a[5];
}