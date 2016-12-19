# -*- coding: utf-8 -*-
"""
Created on Fri Sep 16 23:06:52 2016

@author: Ajwahir
"""

from skimage.segmentation import slic
from skimage.segmentation import mark_boundaries
from skimage.util import img_as_float
from skimage import io
import matplotlib.pyplot as plt
import numpy as np
import math


coords = []
numSegments_upper = 400
numSegments_lower = 150
clicks = 2


def onclick(event):
    global ix, iy
    ix, iy = event.xdata, event.ydata
    print 'inside onclick'
    # print 'x = %d, y = %d'%(
    #     ix, iy)

    # assign global variable to access outside of function
    global coords
    coords.append((ix, iy))

    if len(coords) == clicks:
        fig.canvas.mpl_disconnect(cid)
        plt.close()
    return

image = img_as_float(io.imread("H:\ubuntu_backup\IGVC_TESTINGS\Ip_cpp_codes\lane_detection\image-021.jpeg"))
upper_half=image[0:image.shape[0]/2,:,:]
lower_half=image[image.shape[0]/2:image.shape[1],:,:]


# apply SLIC and extract (approximately) the supplied number
# of segments
#segments = slic(image, n_segments = numSegments, sigma = 5)
segments_upper = slic(upper_half, n_segments = numSegments_upper, sigma = 5)
segments_lower = slic(lower_half, n_segments = numSegments_lower, sigma = 5)

segments_lower=segments_upper[:].max()+segments_lower
segments = np.concatenate((segments_upper, segments_lower), axis=0)

fig = plt.figure("Superpixels")
ax = fig.add_subplot(1, 1, 1)
ax.imshow(mark_boundaries(image, segments))
plt.axis("off")
cid = fig.canvas.mpl_connect('button_press_event', onclick)
plt.show()
plt.close('all')
#I dont know why it is not continuous, so you will have to run this part of the code after selecting the points
#seperately 


patch_pixels=[]
all_patch_pixels = np.zeros((len(coords),), dtype=np.object)
for i in range(0,len(coords)):
    x=int(math.floor(coords[i][0]))
    y=int(math.floor(coords[i][1]))
    label_selected = segments[x,y]
    patch_coords=np.where(segments==label_selected)
    for j in range(0,len(patch_coords[0])):
        xp=patch_coords[0][j]
        yp=patch_coords[1][j]
        patch_pixels.append(image[xp,yp,1])
    all_patch_pixels[i]=patch_pixels
    patch_pixels=[]
    
np.save('image_1_background.npy',all_patch_pixels)
    

        
#
#
## show the output of SLIC
#fig = plt.figure("Superpixels -- upper")
#ax = fig.add_subplot(1, 1, 1)
##fig = plt.gcf()
##ax = plt.gca()
#ax.imshow(mark_boundaries(upper_half, segments_upper))
#plt.axis("off")
# 
#cid = fig.canvas.mpl_connect('button_press_event', onclick)
## show the plots
#plt.show()
#
#
## show the output of SLIC
#fig = plt.figure("Superpixels --lower")
#ax = fig.add_subplot(1, 1, 1)
##fig = plt.gcf()
##ax = plt.gca()
#ax.imshow(mark_boundaries(lower_half, segments_lower))
#plt.axis("off")
# 
#cid = fig.canvas.mpl_connect('button_press_event', onclick)
## show the plots
#plt.show()