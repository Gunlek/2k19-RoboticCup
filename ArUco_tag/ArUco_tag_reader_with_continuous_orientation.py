import cv2 as cv
from cv2 import aruco
import numpy as np

capture = cv.VideoCapture(0)

marker_length = 100

camera_matrix = np.array([
    [1112.24452, 0.00000000, 533.513273],
    [0.00000000, 1285.19949, 697.210272],
    [0.00000000, 0.00000000, 1.00000000]
])

distance_coeffs = np.array([[0.06348886, -0.03007106, -0.01719968, -0.03735041, -0.37339238]])

aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_4X4_1000)

while True:
    ret, frame = capture.read()

    corners, ids, rejectedPoints = aruco.detectMarkers(frame, aruco_dict)
    rvecs, tvecs, _ = aruco.estimatePoseSingleMarkers(corners, marker_length, camera_matrix, distance_coeffs)

    aruco.drawDetectedMarkers(frame, corners, ids)
    if rvecs is not None:
        for index in range(len(rvecs)):
            aruco.drawAxis(frame, camera_matrix, distance_coeffs, rvecs[index], tvecs[index], 50)

    print("rvecs ", rvecs)

    cv.imshow("Frame", frame)
    cv.waitKey(30)