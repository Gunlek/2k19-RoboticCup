import cv2 as cv
from cv2 import aruco
import numpy as np
from pathlib import Path

# img_path = Path(__file__).absolute()
# img_path = img_path.joinpath("pi-camera")
# img_path = img_path.joinpath("samples")
# img_path = img_path.joinpath("board")

marker_size = 100
marker_sep = 50

aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_4X4_1000)
aruco_board = aruco.GridBoard_create(4, 6, marker_size, marker_sep, aruco_dict)

aruco_params = aruco.DetectorParameters_create()

img_1 = cv.imread("board_capture_1.jpg")
img_2 = cv.imread("board_capture_2.jpg")
img_3 = cv.imread("board_capture_3.jpg")
img_list = [img_1, img_2, img_3]
width, height = img_1.shape[:2]

counter, corners_list, id_list = [], [], []

first = True

for img_sample in img_list:
    img_gray = cv.cvtColor(img_sample, cv.COLOR_RGB2GRAY)
    corners, ids, rejectedImgPoints = aruco.detectMarkers(img_gray, aruco_dict)
    if first:
        corners_list = corners
        id_list = ids
        first = False
    else:
        corners_list = np.vstack((corners_list, corners))
        id_list = np.vstack((id_list, ids))
    counter.append(len(ids))

print('Found {} unique markers'.format(np.unique(id_list)))

counter = np.array(counter)
# aruco.drawDetectedMarkers(img, corners, ids, (255, 0, 0))
# cv.imshow("Frame", img)
# cv.waitKey(0)

ret, cameraMatrix, distCoeffs, _, _ = aruco.calibrateCameraAruco(corners_list, id_list, counter, aruco_board, img_gray.shape, None, None)

print("Camera matrix is \n", cameraMatrix)
print()
print("Distance coeff are \n", distCoeffs)

aruco.drawDetectedMarkers(img_gray, corners, ids)
cv.imshow("Frame", img_gray)
cv.waitKey(0)
