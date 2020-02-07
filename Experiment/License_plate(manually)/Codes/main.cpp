#include<iostream>
#include<string>
#include <opencv2/opencv.hpp>
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using namespace std;
using namespace cv;
vector<vector<cv::Point3f> > object_points;
vector<vector<cv::Point2f> > image_points;
cv::Mat intrinsic_matrix, distortion_coeffs;
int main() {
    int mode;
    cin>>mode;
    if(mode==0) //input images or not
        cout<<"using images"<<endl;
    else
        cout<<"no images input"<<endl;
    if(mode==0) {
        int start_point=16;
        int index = start_point;
        Size img_size;
        int board_h = 6;
        int board_w = 8;
        int n_boards = 12;
        int board_n = board_w * board_h;
        Size board_sz = Size(board_w, board_h);
        while (index < start_point + n_boards) {
            string img_path = string("./calib-1-2/left-00");
            if(index <= 9)
                img_path += string("0");
            string img = to_string(index);
            img += string(".png");
            string img_name = img_path + img;
            Mat image = imread(img_name);
            vector<Point2f> image_point;
            bool found = cv::findChessboardCorners(image, board_sz, image_point);
            drawChessboardCorners(image, board_sz, image_point, found);
            imshow("image", image);
            waitKey(300);
            if(found) {
                img_size=image.size();
                image_points.push_back(image_point);
                object_points.push_back(vector<cv::Point3f>());
                vector<cv::Point3f> &opts = object_points.back();
                opts.resize(board_n);
                for (int j = 0; j < board_n; j++) {
                    opts[j] = cv::Point3f(static_cast<float>((j) / board_w),
                                          static_cast<float>((j) % board_w), 0.0f);
                }
                index++;
            }
        }
        cout << img_size << endl;
        double err = cv::calibrateCamera(object_points, image_points, img_size, intrinsic_matrix,
                                         distortion_coeffs, cv::noArray(), cv::noArray(),
                                         cv::CALIB_ZERO_TANGENT_DIST,
                                         TermCriteria(
                                                 TermCriteria::COUNT + TermCriteria::EPS, 400, DBL_EPSILON));
        cout << "error:" << err << endl;
        cout << "\nintrinsic matrix:" << intrinsic_matrix;
        cout << "\ndistortion coefficients: " << distortion_coeffs << endl;
        string file_name("intrinsics.xml");
        cv::FileStorage fs(file_name, cv::FileStorage::WRITE);
        fs << "image_width" << img_size.width << "image_height" << img_size.height
           << "camera_matrix" << intrinsic_matrix << "distortion_coefficients"
           << distortion_coeffs;
        fs.release();
    } else{
        int index = 1;
        string file_path = string("./p/manuallabels/images/");
        string file_path2 = string("./p/manuallabels/objects/");
        Mat img=imread("./leftcamera1.jpg");
        Size img_size=img.size();
        while (true) {
            string name = to_string(index);
            name += string(".txt");
            string file_name;
            string file_name2;
            file_name = file_path + name;
            file_name2 = file_path2 + name;
            ifstream ifs;
            ifs.open(file_name, ios::in);
            if (!ifs.is_open()) {
                cout << file_name << " dosen't exist!" << endl;
                break;
            } else {
                float x;
                float y;
                vector<Point2f> image_point;
                while(!ifs.eof()) {
                    ifs >> x;
                    ifs >> y;
                    image_point.push_back(Point2f(x, y));
                }
                image_point.pop_back();
                ifs.close();
                vector<cv::Point3f> known_object;
                ifstream ofs;
                ofs.open(file_name2, ios::in);
                float x2,y2,z2;
                while(!ofs.eof()) {
                    ofs >> x2;
                    ofs >> y2;
                    ofs >> z2;
                    known_object.push_back(Point3f(x2, y2, z2));
                }
                known_object.pop_back();
                ofs.close();
                object_points.push_back(known_object);
                image_points.push_back(image_point);
                image_point.clear();
                known_object.clear();
                index++;
            }
        }
        cout<<img_size<<endl;
        double err = cv::calibrateCamera(object_points, image_points, img_size, intrinsic_matrix,
                                         distortion_coeffs, cv::noArray(), cv::noArray(),
                                         cv::CALIB_ZERO_TANGENT_DIST,
                                         TermCriteria(
                                                 TermCriteria::COUNT + TermCriteria::EPS, 500, DBL_EPSILON));
        cout << "error:" << err << endl;
        cout << "\nintrinsic matrix:" << intrinsic_matrix;
        cout << "\ndistortion coefficients: " << distortion_coeffs << endl;
        string file_name("intrinsics_manually.xml");
        cv::FileStorage fs(file_name, cv::FileStorage::WRITE);
        fs << "image_width" << img_size.width << "image_height" << img_size.height
           << "camera_matrix" << intrinsic_matrix << "distortion_coefficients"
           << distortion_coeffs;
        fs.release();
    }
    return 0;
}