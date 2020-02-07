# Experiment on Ideal Points Calibration
## Goal
Check if calibration algorithm works with only 4 ("ideal") points.<br/>
## Methods
1. randomly pick n (n>3) points **x** from an image;<br/>
2. move the origin of image coordinates from upper left corner to the image center **x**-(w/2, h/2) ;<br/>
3. take the identity matrix I as the initial intrinsic matrix K;<br/>
4. randomly set the values of the extrinsic matrix [R|**t**];<br/>
5. generate corresponding 3D object points (w.r.t. world coordinates) **X** (these are "ideal" object points);<br/>
6. use **x** and **X** to calibrate the intrinsic matrix K';<br/>
7. compare K and K';<br/>
8. add noises to **x** or **X**, and repeat 6-7.<br/> 

## [Results](https://github.com/JaySparrow/Camera-Self-Calibration/tree/master/Experiment/4_points/Codes/ResultIdealPoints)

## Conclusion
1. Four (ideal) points are enough to calibrate the intrinsic matrix;
2. By adding feature points, the output intrinsic matrix K' has a trend to be more robust to noises.

## Guidance for Future Research
1. Build a model of relation between intrinsic parameters, noises, and the number of feature points.


