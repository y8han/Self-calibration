import cv2
import numpy as np
from tabulate import tabulate

class VideoReader:
    def __init__(self, video_dir):
        self.__reader = cv2.VideoCapture(video_dir)

        # video name
        file_name = video_dir.split('/')[-1]
        self.__video_name = file_name.split('.')[0]

        # FPS
        self.__fps = self.__reader.get(cv2.CAP_PROP_FPS)
        # number of frames
        self.__frame_cnt = int(self.__reader.get(cv2.CAP_PROP_FRAME_COUNT))
        # frame size
        self.__frame_size = self.__reader.get(cv2.CAP_PROP_FRAME_WIDTH), self.__reader.get(cv2.CAP_PROP_FRAME_HEIGHT)

        # summary
        print(tabulate([["fps", "frame cnt", "frame size"], [self.__fps, self.__frame_cnt, self.__frame_size]], headers="firstrow", tablefmt="psql"))

    def gen_frames_by_num(self, num_frame, save_dir):
        if not isinstance(num_frame, int):
            print(f"Error: invalid parameter {num_frame}, need int")
            return
        n = num_frame
        if num_frame > self.__frame_cnt:
            print(f"required frame number {num_frame} exceeds maximum {int(self.__frame_cnt)}, return all")
            n = self.__frame_cnt
        if num_frame <= 0:
            print(f"not requiring any frame, return all {int(self.__frame_cnt)}")
            n = self.__frame_cnt

        period = int(self.__frame_cnt / n)
        # tick = 0

        # ret = True
        # while ret:
        #     tick += 1
        #     ret, frame = self.__reader.read()
        #     if tick == period:
        #         cv2.imwrite(f'{save_dir}/{self.__video_name}.jpg', frame)
        #         tick = 0

        for i in range(1, self.__frame_cnt+1, 1):
            ret, frame = self.__reader.read()
            if i % period == 0:
                cv2.imwrite(f'{save_dir}/{self.__video_name}{int(i//period)}.jpg', frame)

        print(f"{n} frames generated in {save_dir}")

    def gen_frames_by_period(self, period, save_dir):
        if not isinstance(period, int):
            print(f"Error: invalid parameter {period}, need int")
            return
        p = period
        if period > self.__frame_cnt:
            print(f"required period {period} exceeds maximum number of frames{int(self.__frame_cnt)}, return the last frame")
            p = self.__frame_cnt
        if period <= 0:
            print(f"required period {period} invalid, return all {int(self.__frame_cnt)} frames")
            p = 1

        # tick = 0

        # ret = True
        # while ret:
        #     tick += 1
        #     ret, frame = self.__reader.read()
        #     if tick == p:
        #         cv2.imwrite(f'{save_dir}/{self.__video_name}.jpg', frame)
        #         tick = 0

        for i in range(1, self.__frame_cnt+1, 1):
            ret, frame = self.__reader.read()
            if i % p == 0:
                cv2.imwrite(f'{save_dir}/{self.__video_name}{int(i//p)}.jpg', frame)

        print(f"{int(self.__frame_cnt / p)} frames generated in {save_dir}")

if __name__ == "__main__":
    vr = VideoReader('./large_lp.mp4')
    vr.gen_frames_by_num(20, './image/')
