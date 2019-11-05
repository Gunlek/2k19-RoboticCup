import cv2 as cv
from cv2 import aruco

img_path = "board_capture_3.jpg"
img = cv.imread(img_path)

# Detect aruco markers and register them in corners / ids (rejectedImgPoints is for debug)
corners, ids, rejectedImgPoints = aruco.detectMarkers(img, aruco.getPredefinedDictionary(aruco.DICT_4X4_1000))
# aruco.estimatePoseSingleMarkers(corners, )

if ids is not None and len(ids) > 0:
    aruco.drawDetectedMarkers(img, corners, ids, (0, 0, 255))
print(rejectedImgPoints)

cv.imshow("Frame", img)
cv.waitKey(0)