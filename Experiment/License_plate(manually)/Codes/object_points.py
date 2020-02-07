import numpy as np

LP_WIDTH = 304.8
LP_HEIGHT = 152.4

# LP_WIDTH = 434
# LP_HEIGHT = 217

# ratio of "feature points' coordinates (x_f, y_f) with respect to the number bounding box's center (x_c, y_c)" and "number bounding box's width and height"
# (x_f-x_c/LP_WIDTH, y_f-y_c/LP_HEIGHT)
relative_features = {
    '5': (np.array([(32,83),(72,83),(72,96),(44,96),(44,117),(44,137),(32,137)], dtype=np.float64)-np.array((54,133)))/np.array((434, 216)), # lp3
    'L': (np.array([(284,281),(320,281),(320,551),(425,551),(425,586),(284,586)], dtype=np.float64)-np.array((354,434)))/np.array((1400, 705)) #lp
}

relative_features_coarse = {
    # letters
    'B': (np.array([(205,90),(205,132),(205,174)], dtype=np.float64)-np.array((218,133)))/np.array((434,215)), #lp6-idx=3
    'L': (np.array([(303,300),(303,571),(410,571)], dtype=np.float64)-np.array((354,434)))/np.array((1400, 705)), #lp-idx=1
    'Y': (np.array([(202,89),(234,89),(218,122),(218,173)], dtype=np.float64)-np.array((218,133)))/np.array((435,216)), #lp5-idx=3
    # numbers
    '2': (np.array([(826,319),(817,569),(926,569)], dtype=np.float64)-np.array((873,434)))/np.array((1400, 705)), #lp-idx=4
    '3': (np.array([(40,98),(58,133),(40,167)], dtype=np.float64)-np.array((56,132)))/np.array((434,217)), #lp4-idx=0
    '5': (np.array([(38,91),(66,91),(38,131),(38,169)], dtype=np.float64)-np.array((54,133)))/np.array((434, 216)), # lp3-idx=0
    '6': (np.array([(230,290),(149,435),(179,573)], dtype=np.float64)-np.array((177,434)))/np.array((1400, 705)), #lp-idx=0
    '7': (np.array([(995,293),(1099,293),(1052,563)], dtype=np.float64)-np.array((1048,434)))/np.array((1400, 705)), #lp-idx=6
    '8': (np.array([(380,90),(380,134),(380,175)], dtype=np.float64)-np.array((380,132)))/np.array((435,216)), #lp5-idx=6
}

# ratio of "number bounding box's center (x_c, y_c)" and "license plate's width and height"
# (x_c/LP_WIDTH, y_c/LP_HEIGHT)
relative_position_centers = np.array([(177, 434), (354, 434), (525, 434), (697, 434), (873, 434), (1048, 434), (1223, 434)], dtype=np.float64)/np.array((1400, 705))

# object corner points
corner_points = np.array([
    [0,0],
    [304.8,0],
    [304.8,152.4],
    [0,152.4]
], dtype=np.float64)


def get_object_points(char, idx, lp_width=LP_WIDTH, lp_height=LP_HEIGHT):
    features = np.array(relative_features_coarse[char])
    positions = relative_position_centers[idx, :]

    return (features+positions)*np.array([lp_width, lp_height])

def write_object_points(char_list, idx_list, addr = './', file_name = 'features.txt'):
    assert len(char_list) == len(idx_list), "size not match: character list vs index list"

    # get feature points for each character in its index position
    feature_points_list = [corner_points]
    for i in range(len(char_list)):
        char = char_list[i]
        idx = idx_list[i]

        feature_points_list.append(get_object_points(char, idx))
    
    # concatenate all characters' feature points into a single d*2 array (d: number of feature points in total)
    feature_points_array = np.concatenate(feature_points_list)

    h, _ = feature_points_array.shape

    # append 0's to the last column as z-coordinate
    feature_points_array = np.concatenate([feature_points_array, np.zeros((h,1))], axis=1)
    assert feature_points_array.shape == (h, 3)

    # write feature points into a txt file
    np.savetxt(addr+file_name, feature_points_array, fmt='%.3f')

    return feature_points_array



if __name__ == "__main__":
    idx = [0,2,3,4,5,6]
    char = ['7','Y','B','6','3','5']
    print('>> (lp width, lp height) = (%d, %d)'%(LP_WIDTH, LP_HEIGHT))
    print('>> char:', char)
    print('>> idx:', idx)
    print('>> feature points:')
    print(write_object_points(char, idx, addr='./objects/' ,file_name='Oleftcamera14_1.txt'))
    # print(relative_position_centers[6,:])
