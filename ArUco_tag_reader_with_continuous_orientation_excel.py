import cv2 as cv
from cv2 import aruco
import numpy as np
import xlwt
import time
import math

capture = cv.VideoCapture(0)

marker_length = 100

camera_matrix = np.array([
    [1112.24452, 0.00000000, 533.513273],
    [0.00000000, 1285.19949, 697.210272],
    [0.00000000, 0.00000000, 1.00000000]
])

distance_coeffs = np.array([[0.06348886, -0.03007106, -0.01719968, -0.03735041, -0.37339238]])

aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_4X4_1000)

excelBook = xlwt.Workbook()
excelSheet = excelBook.add_sheet("rvecs")

wereDetected = False
time_list_1 = []
rvecs_list = []

while True:
    ret, frame = capture.read()

    corners, ids, rejectedPoints = aruco.detectMarkers(frame, aruco_dict)
    rvecs, tvecs, _ = aruco.estimatePoseSingleMarkers(corners, marker_length, camera_matrix, distance_coeffs)

    aruco.drawDetectedMarkers(frame, corners, ids)
    if rvecs is not None:
        wereDetected = True
        for index in range(len(rvecs)):
            time_list_1.append(time.perf_counter())
            rvecs_list.append(rvecs[index])
            aruco.drawAxis(frame, camera_matrix, distance_coeffs, rvecs[index], tvecs[index], 50)
    else:
        if wereDetected:
            break

    cv.imshow("Frame", frame)
    cv.waitKey(30)

z_vector = np.array([[0], [0], [1]])
excelSheet.write(0, 0, "Temps")
excelSheet.write(0, 1, "rvecs_x")
excelSheet.write(0, 2, "rvecs_y")
excelSheet.write(0, 3, "rvecs_z")
excelSheet.write(0, 4, "rot_x")
excelSheet.write(0, 5, "rot_y")
excelSheet.write(0, 6, "rot_z")
excelSheet.write(0, 7, "rot_x_deg")
excelSheet.write(0, 8, "rot_y_deg")
excelSheet.write(0, 9, "rot_z_deg")

for index in range(0, len(rvecs_list)):
    excelSheet.write(index+1, 0, time_list_1[index])
    excelSheet.write(index+1, 1, rvecs_list[index][0][0])
    excelSheet.write(index+1, 2, rvecs_list[index][0][1])
    excelSheet.write(index+1, 3, rvecs_list[index][0][2])

    rodrigues, _ = cv.Rodrigues(rvecs_list[index])
    rotation = np.dot(rodrigues, z_vector)
    excelSheet.write(index + 1, 4, math.atan2(rodrigues[2][1], rodrigues[2][2]))
    excelSheet.write(index + 1, 5, math.atan2(-rodrigues[2][0], math.sqrt((rodrigues[2][1] ** 2 + rodrigues[2][2] ** 2))))
    excelSheet.write(index + 1, 6, math.atan2(rodrigues[1][0], rodrigues[0][0]))
    excelSheet.write(index + 1, 7, math.atan2(rodrigues[2][1], rodrigues[2][2])*180/math.pi)
    excelSheet.write(index + 1, 8, math.atan2(-rodrigues[2][0], math.sqrt((rodrigues[2][1]**2 + rodrigues[2][2]**2))) * 180 / math.pi)
    excelSheet.write(index + 1, 9, math.atan2(rodrigues[1][0], rodrigues[0][0])*180/math.pi)

excelBook.save('output.xls')