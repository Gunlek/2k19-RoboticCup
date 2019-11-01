import cv2 as cv
from cv2 import aruco

dic = aruco.getPredefinedDictionary(aruco.DICT_4X4_250)
markerImg = aruco.drawMarker(dic, 58, 400)

cv.imwrite("marker.png", markerImg)

board = aruco.GridBoard_create(4, 6, 100, 50, dic)
boardImg = aruco.drawPlanarBoard(board, (720, 1080))
cv.imwrite("board.png", boardImg)