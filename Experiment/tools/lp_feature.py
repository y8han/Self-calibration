import numpy as np
from tabulate import tabulate

class LPFeature:
    def __init__(self):
        # license plate's physical size
        self.__lp_width = 304.8
        self.__lp_height = 152.4

        # ratio of "feature points' coordinates (x_f, y_f) with respect to the number bounding box's center (x_c, y_c)" and "number bounding box's width and height"
        # (x_f-x_c/LP_WIDTH, y_f-y_c/LP_HEIGHT)
        self.__relative_features = {
            '5': np.array([[-0.05069124, -0.23148148], [0.04147465, -0.23148148], [0.04147465, -0.1712963], [-0.02304147, -0.1712963], [-0.02304147, -0.07407407], [-0.02304147, 0.01851852], [-0.05069124, 0.01851852]]), # lp3
            'L': np.array([[-0.05, -0.21702128], [-0.02428571, -0.21702128], [-0.02428571, 0.16595745], [0.05071429, 0.16595745], [0.05071429, 0.21560284], [-0.05, 0.21560284]]) #lp
        }

        self.__relative_features_coarse = {
            # letters
            'B': np.array([[-0.02995392, -0.2], [-0.02995392, -0.00465116], [-0.02995392, 0.19069767]]), #lp6-idx=3
            'L': np.array([[-0.03642857, -0.19007092], [-0.03642857, 0.19432624], [0.04, 0.19432624]]), #lp-idx=1
            'Y': np.array([[-0.03678161, -0.2037037], [0.0367816,1 -0.2037037], [0., -0.05092593], [0., 0.18518519]]), #lp5-idx=3
            # numbers
            '2': np.array([[-0.03357143, -0.16312057], [-0.04, 0.19148936], [0.03785714, 0.19148936]]), #lp-idx=4
            '3': np.array([[-0.03686636, -0.15668203], [0.00460829, 0.00460829], [-0.03686636, 0.16129032]]), #lp4-idx=0
            '5': np.array([[-0.03686636, -0.19444444], [0.02764977, -0.19444444], [-0.03686636, -0.00925926], [-0.03686636, 0.16666667]]), # lp3-idx=0
            '6': np.array([[0.03785714, -0.20425532], [-0.02, 0.00141844], [0.00142857, 0.19716312]]), #lp-idx=0
            '7': np.array([[-0.03785714, -0.2], [0.03642857, -0.2], [0.00285714, 0.18297872]]), #lp-idx=6
            '8': np.array([[0., -0.19444444], [0., 0.00925926], [0., 0.19907407]]) #lp5-idx=6
        }

        # ratio of "number bounding box's center (x_c, y_c)" and "license plate's width and height"
        # (x_c/LP_WIDTH, y_c/LP_HEIGHT)
        self.__relative_centers = np.array([[0.12642857, 0.61560284], [0.25285714, 0.61560284], [0.375, 0.61560284], [0.49785714, 0.61560284], [0.62357143, 0.61560284], [0.74857143, 0.61560284], [0.87357143, 0.61560284]])

    """ getter and setter of license plate's physical size """
    @property
    def lp_width(self):
        return self.__lp_width

    @property
    def lp_height(self):
        return self.__lp_height

    @lp_width.setter
    def lp_width(self, w):
        if not isinstance(w, float):
            print(f"Error: {w} is not a float number")
            return
        if w <= 0:
            print(f"Error: {w} is not a positive number")
            return
        print("lp_width setter")
        self.__lp_width = w

    @lp_height.setter
    def lp_height(self, h):
        if not isinstance(h, float):
            print(f"Error: {h} is not a float number")
            return
        if h <= 0:
            print(f"Error: {h} is not a positive number")
            return
        print("lp_height setter")
        self.__lp_height = h

    def lp_summary(self):
        print(tabulate([["lp width", "lp height"], [self.__lp_width, self.__lp_height]], headers="firstrow", tablefmt="psql"))

    def char_summary(self):
        print(tabulate([(k, v.shape[0]) for k, v in self.__relative_features_coarse.items()], ("char", "# features"),tablefmt="psql"))

    def __calculate_points(self, char, idx):
        if not isinstance(char, str):
            print(f"Error: {char} is not a string")
            return
        if not idx in range(self.__relative_centers.shape[0]):
            print(f"Error: {idx} is not valid")
            return

        features = np.array(self.__relative_features_coarse[char])
        positions = self.__relative_centers[idx, :]

        return (features+positions) * np.array([self.__lp_width, self.__lp_height])

    def __input_summary(self, char_list, idx_list, add_corners):
        num_points = 0
        plate = []
        cnt = []

        num_pos = self.__relative_centers.shape[0]
        plate += ['-'] * num_pos
        cnt += [0] * num_pos
        for i in range(len(idx_list)):
            idx = idx_list[i]
            char = char_list[i]

            plate[idx] = char
            cnt[idx] = self.__relative_features_coarse[char].shape[0]
            num_points += cnt[idx]

        if add_corners:
            num_points += 4
            plate += ['total (with corners)']
        else:
            plate += ['total (no corners)']

        cnt += [num_points]

        print(tabulate([plate, cnt], headers="firstrow", tablefmt="psql"))


    def get_points(self, char_list, idx_list, add_corners=True, write=False, write_addr='./', file_name='lp_points.txt'):
        points_list = []

        if add_corners:
            # physical corner points
            corner_points = np.array([
                [0., 0.],
                [self.__lp_width, 0.],
                [self.__lp_width, self.__lp_height],
                [0., self.__lp_height]
            ])
            points_list.append(corner_points)

        # calculate physical positions of each character's feature points
        if isinstance(char_list, str) and isinstance(idx_list, int): # inputs are single elements
            points_list.append(self.__calculate_points(char_list, idx_list))
        elif isinstance(char_list, list) and isinstance(idx_list, list) and len(char_list) == len(idx_list): # inputs are lists
            for i in range(len(char_list)):
                points_list.append(self.__calculate_points(char_list[i], idx_list[i]))
        else:
            print(f"Error: char_list and/or idx_list are invalid")
            return

        self.__input_summary(char_list, idx_list, add_corners)

        # concatenate all characters' feature points into a single d*2 array (d: number of feature points in total)
        points_array = np.concatenate(points_list)

        # append 0's to the last column as z-coordinate
        h, _ = points_array.shape
        points_array = np.concatenate([points_array, np.zeros((h,1))], axis=1)
        assert points_array.shape == (h, 3), "Error: points_array dimension not valid"

        # write feature points into a txt file
        if write:
            np.savetxt(write_addr+'/'+file_name, points_array, fmt='%.3f')

        return points_array



if __name__ == "__main__":
    # idx = [0,2,3,4,5,6]
    # char = ['7','Y','B','6','3','5']
    
    idx = []
    char = []

    lp = LPFeature()
    lp.lp_summary()
    lp.char_summary()
    pts = lp.get_points(char, idx, add_corners=True)
    print(">> points")
    print(pts)
