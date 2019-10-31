import cv2 as cv
from cv2 import aruco

capture = cv.VideoCapture(0)

if not capture.isOpened:
    print("Unable to open camera feed")
    exit(0)

while True:
    # Read a frame from 'capture' device
    # return false if no frames has been grabbed
    ret, frame = capture.read()

    # ret = true if a frame has been grabbed, else false
    # frame contains a Mat image (ready to be used with openCV)

    # Ensure a frame has been grabbed
    if frame is None:
        break

    # Detect aruco markers and register them in corners / ids (rejectedImgPoints is for debug)
    corners, ids, rejectedImgPoints = aruco.detectMarkers(frame, aruco.getPredefinedDictionary(aruco.DICT_4X4_1000))
    # aruco.estimatePoseSingleMarkers(corners, )

    if ids is not None and len(ids) > 0:
        aruco.drawDetectedMarkers(frame, corners, ids, (0, 0, 255))
    print(rejectedImgPoints)

    cv.imshow("Frame", frame)
    keyboard = cv.waitKey(30)

    if keyboard == 113:
        break
