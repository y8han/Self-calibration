import os
import argparse
import logging
from tqdm import tqdm

import numpy as np

import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt

from lp_feature import LPFeature

class MultiFeature:
    def __init__(self, image_dir = './', feature_dir = './', point_dir='./', lp_width=304.8, lp_height=152.4):
        self.__feature_dir = feature_dir
        self.__point_dir = point_dir
        
        # read images
        self.image_dict = dict()

        if os.path.isdir(image_dir):
            image_names = sorted(os.listdir(image_dir))
            if '.DS_Store' in image_names:
                image_names.remove('.DS_Store')
            cnt = 0
            logging.log(logging.INFO, "Loading images ...")
            for image_name in tqdm(image_names):
                image_path = image_dir + '/' + image_name
                if os.path.isfile(image_path):
                    cnt += 1
                    self.image_dict[image_name] = plt.imread(image_path)
            logging.log(logging.INFO, "{} images are loaded.".format(cnt))
        else:
            logging.log(logging.WARNING, "{} is not a folder!".format(image_dir))
            exit(1)

        # unprocessed image list
        self.unprocessed_images = sorted(list(self.image_dict.keys()))

        # feature table (image) & point table (physical)
        self.feature_table = []
        self.point_array = None

        # physical points
        self.lpfeature = LPFeature()
        self.lpfeature.lp_width = lp_width
        self.lpfeature.lp_height = lp_height
        self.lpfeature.lp_summary()
        self.__char_list = []
        self.__idx_list = []

        # call back functions
        self.__fig, self.__ax = plt.subplots()

        self.__cid1 = self.__fig.canvas.mpl_connect('button_press_event', self.__on_click)
        self.__cid2 = self.__fig.canvas.mpl_connect('key_press_event', self.__on_key_press)

        # load images to GUI
        try:
            self.current_image = self.unprocessed_images.pop()
            print(f">> load img: {self.current_image}")
            self.lpfeature.char_summary()
            self.__ax.imshow(self.image_dict[self.current_image])
            plt.show()
        except:
            # disconnect callbacks
            self.__fig.canvas.mpl_disconnect(self.__cid1)
            self.__fig.canvas.mpl_disconnect(self.__cid2)

    def __on_click(self, event):
        if not event.dblclick:
            if event.button == 1:
                if not isinstance(event.xdata, float) and not isinstance(event.ydata, float):
                    logging.log(logging.WARNING, "{} is a invalid click!".format((event.xdata, event.ydata)))
                    return
                self.feature_table.append([event.xdata, event.ydata])
                print(f"[{len(self.feature_table)}]: ({event.xdata}, {event.ydata}) saved")  
            if event.button == 3:
                if len(self.feature_table) <= 0:
                    logging.log(logging.WARNING, "deletion failure - no feature points stored!")
                    return
                print(f"[{len(self.feature_table)}]: ({self.feature_table[-1][0]}, {self.feature_table[-1][1]}) deleted")
                self.feature_table = self.feature_table[:-1]

    def __on_key_press(self, event):
        """ terminate program """
        if event.key == 'escape':
            # close plt window
            plt.close()

            # disconnect callbacks
            self.__fig.canvas.mpl_disconnect(self.__cid1)
            self.__fig.canvas.mpl_disconnect(self.__cid2)

        """ save features & points, proceed to next image, or terminate """
        if event.key == 'enter':
            """ no physical points generated """
            if not isinstance(self.point_array, np.ndarray):
                # print(f"Input character/index list: [char0] [idx0] [char1] [idx1] ...")
                # terminal_input = input("=")
                # input_list = terminal_input.split(' ')
                # for i in range(0, len(input_list), 2):
                #     self.__char_list.append(input_list[i])
                #     self.__idx_list.append(int(input_list[i+1]))
                self.__get_inputs(self.__char_list, self.__idx_list)
                self.point_array = self.lpfeature.get_points(self.__char_list, self.__idx_list)

            """ image features & physical points mismatch """
            while True:
                # too few image features: restart
                if self.point_array.shape[0] > len(self.feature_table):
                    logging.log(logging.WARNING, f"image features {len(self.feature_table)} more than physical points {self.point_array.shape[0]} - restart for current image {self.current_image}!")
                    self.point_array = None
                    self.feature_table = []
                    self.__char_list = []
                    self.__idx_list = []
                    return
                # too few physical points: re-enter char/idx list
                elif self.point_array.shape[0] < len(self.feature_table):
                    logging.log(logging.WARNING, f"physical points {self.point_array.shape[0]} more than image features {len(self.feature_table)} - please re-enter!")
                    self.point_array = None
                    self.__char_list = []
                    self.__idx_list = []
                    self.__get_inputs(self.__get_inputs, self.__idx_list)
                    self.point_array = self.lpfeature.get_points(self.__char_list, self.__idx_list)
                    continue
                break

            """ save features & points """
            # save image feature
            np.savetxt(f"{self.__feature_dir+'/'+self.current_image[:self.current_image.find('.')]}.txt", np.array(self.feature_table), fmt="%1.3f")
            print(f"<< image features saved: {self.current_image[:self.current_image.find('.')]}.txt")
            # save physical points
            np.savetxt(f"{self.__point_dir+'/'+self.current_image[:self.current_image.find('.')]}.txt", self.point_array, fmt="%1.3f")
            print(f"<< physical points saved: {self.current_image[:self.current_image.find('.')]}.txt")

            """ load new image to GUI """
            try:
                self.feature_table = []
                self.point_array = None
                self.__char_list = []
                self.__idx_list = []
                self.current_image = self.unprocessed_images.pop()
                print(f">> load img: {self.current_image}")
                self.lpfeature.char_summary()
                self.__ax.imshow(self.image_dict[self.current_image])
                plt.show()
            except:
                # close plt window
                plt.close()

                # disconnect callbacks
                self.__fig.canvas.mpl_disconnect(self.__cid1)
                self.__fig.canvas.mpl_disconnect(self.__cid2)
    
    def __get_inputs(self, char_list, idx_list):
        print(f"[action] input character/index list: [char0] [idx0] [char1] [idx1] ...")
        while True:
            try: 
                terminal_input = input("=")
                input_list = terminal_input.split(' ')
                for i in range(0, len(input_list), 2):
                    char_list.append(input_list[i])
                    idx_list.append(int(input_list[i+1]))
                break
            except:
                char_list.clear()
                idx_list.clear()
                logging.log(logging.WARNING, f"invalid inputs - please re-enter!")



if __name__ == "__main__":
    """ command-line parser """
    parser = argparse.ArgumentParser(description="License plate's features & physical points")
    parser.add_argument('image_dir', action='store', help='the directory of images from which features are picked')
    parser.add_argument('feature_dir', action='store', help='the directory to store image features')
    parser.add_argument('point_dir', action='store', help='the directory to store physical points')
    parser.add_argument('--lp_size', action='store', dest='lp_size', required=False, help="license plate's physical size: [width] [height]", nargs=2)
    args = parser.parse_args()

    if not os.path.exists(args.image_dir):
        logging.log(logging.ERROR, f"image_dir {args.image_dir} is not a valid directory")
        exit(1)
    if not os.path.exists(args.feature_dir):
        logging.log(logging.ERROR, f"feature_dir {args.feature_dir} is not a valid directory")
        exit(1)
    if not os.path.exists(args.point_dir):
        logging.log(logging.ERROR, f"point_dir {args.point_dir} is not a valid directory")
        exit(1)

    if args.lp_size is None:
        mf = MultiFeature(args.image_dir, args.feature_dir, args.point_dir)
    else:
        try:
            mf = MultiFeature(args.image_dir, args.feature_dir, args.point_dir, lp_width=float(args.lp_size[0]), lp_height=float(args.lp_size[1]))
        except:
            logging.log(logging.ERROR, f"lp_size {args.lp_size} is not valid")
            mf = MultiFeature(args.image_dir, args.feature_dir, args.point_dir)

    
    