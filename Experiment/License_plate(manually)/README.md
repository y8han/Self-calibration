# Experiment on Calibration with Manually Selected License Plate Points
## Goal
Check if better calibration parameters can be obtained with more than four points selected on each license plate.<br/>
## Methods
1. (preparation) manually pick feature points from license plate's characters; 
2. measure their physical distances to the upper left corner of their bounding boxes;<br/>
3. use OCR to detect license plate's characters and their index positions (one of the seven positions for California license plates) from images; 
4. calculate their feature points' physical distances (object points **X**) to the upper left corner of license plates (see code *object_points.py*)<br/>
5. manually pick characters' corresponding feature points from images (image points **x**);<br/>
6. feed **X** and **x** into OpenCV calibration algorithm to obtain intrinsic matrix K';<br/>
7. compare K' with ground truth K;<br/>

## [Results](https://github.com/JaySparrow/Camera-Self-Calibration/tree/master/Experiment/License_plate(manually)/Codes/Results)

## Conclusion
1. Hard to find a solution with only four points (with noises);
2. Better performance for calibration with more points.

## Guidance for Future Research
1. Experiment on larger and clearer license plate images (to reduce noises).

