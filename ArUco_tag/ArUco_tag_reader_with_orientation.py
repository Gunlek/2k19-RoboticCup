import cv2 as cv
from cv2 import aruco
import numpy as np

img_path = "pi-camera/samples/single/single_capture_3.jpg"
img = cv.imread(img_path)

marker_length = 100

camera_matrix = np.array([
    [1112.24452, 0.00000000, 533.513273],
    [0.00000000, 1285.19949, 697.210272],
    [0.00000000, 0.00000000, 1.00000000]
])

distance_coeffs = np.array([[0.06348886, -0.03007106, -0.01719968, -0.03735041, -0.37339238]])

aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_4X4_1000)

corners, ids, rejectedPoints = aruco.detectMarkers(img, aruco_dict)
rvecs, tvecs, _ = aruco.estimatePoseSingleMarkers(corners, marker_length, camera_matrix, distance_coeffs)

aruco.drawDetectedMarkers(img, corners, ids)
aruco.drawAxis(img, camera_matrix, distance_coeffs, rvecs, tvecs, 10)

cv.imshow("Frame", img)
cv.waitKey(0)