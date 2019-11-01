import cv2 as cv
from picamera import PiCamera
from picamera.array import PiRGBArray
import time

camera = PiCamera()
rawCapture = PiRGBArray(camera) #RGB Array allows image usage without jpeg compression => Faster

#Allow camera's warm up
time.sleep(2)

camera.capture(rawCapture, format="bgr")
image = rawCapture.array

cv.imwrite("opencv_capture.jpg", image)
# cv.imshow("Image", image)
# cv.waitKey(0)
