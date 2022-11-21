import numpy as np
import cv2 as cv

sidelen = 15
sidenum = 100

def Genarate_Star(sigma=1, intensity=1, location=(0, 0)):
    onestar = cv.getGaussianKernel(sidelen, float(sigma)) * cv.getGaussianKernel(sidelen,float(sigma)).T
    onestar *= 255
    onestar *= 255/np.max(onestar)
    onestar *= intensity
    onestar = np.round(onestar)
    m = np.float32([[1, 0, location[0]], [0, 1, location[1]]])
    onestar = cv.warpAffine(onestar, m, (onestar.shape[1], onestar.shape[0]), borderValue=(0, 0, 0))
    return onestar

def myrand():
    return np.random.rand(1)

skyimg = np.zeros([sidelen*sidenum, sidelen*sidenum], np.uint8)
for n1 in range(sidenum):
    for n2 in range(sidenum):
        starmat = Genarate_Star(3*myrand()+1, 0.5*myrand()+0.5, (14*myrand()-7, 14*myrand()-7))
        skyimg[n1*sidelen:n1*sidelen+sidelen, n2*sidelen:n2*sidelen+sidelen] = starmat
cv.imwrite('test.png', skyimg)
