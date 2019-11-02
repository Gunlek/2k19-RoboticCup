import cv2 as cv
from picamera import PiCamera
from picamera.array import PiRGBArray
import time
import sys

filename = "opencv_capture.jpg"
#if len(sys.argv) > 0:
#	filename = sys.argv[0]

camera = PiCamera()
rawCapture = PiRGBArray(camera) #RGB Array allows image usage without jpeg compression => Faster

#Allow camera's warm up
time.sleep(2)

camera.capture(rawCapture, format="bgr")
image = rawCapture.array

cv.imwrite(filename, image)
# cv.imshow("Image", image)
# cv.waitKey(0)
