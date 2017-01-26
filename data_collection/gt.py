from itertools import product
import scipy.io
# from svmutil import *
import numpy as np
import math
import matplotlib.pyplot as plt
from os import listdir
from os.path import join
import sklearn.preprocessing
from skimage.segmentation import slic
from skimage.segmentation import mark_boundaries
from skimage.util import img_as_float
from skimage import io


coords = []
numSegments_upper = 400
numSegments_lower = 150
clicks = 100

def get_patch_coords(segments,coords):
    patch_pixels=[]
    all_patch_coords=[]
    lengths=[]
    # all_patch_pixels = np.zeros((len(coords),), dtype=np.object)
    for i in range(0,len(coords)):
        x=int(math.floor(coords[i][0]))
        y=int(math.floor(coords[i][1]))
        label_selected = segments[y,x]
        patch_coords=np.where(segments==label_selected)
        all_patch_coords.append(patch_coords)
        
    return all_patch_coords

def onclick(event):
    global ix, iy
    ix, iy = event.xdata, event.ydata
    global coords
    coords.append((ix, iy))

    if len(coords) == clicks:
        fig.canvas.mpl_disconnect(cid)
        plt.close()
    return

datapath = '/home/ajwahir/IGVC/dataset/lanes/'

for f in listdir(datapath):
        
    image = img_as_float(io.imread(join(datapath,f)))

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

    gt_coordinates=get_patch_coords(segments,coords)
    gt_image=np.zeros((image.shape[0],image.shape[1]))
  

    if len(gt_coordinates) is not 0:
        for i in range(0,len(gt_coordinates)):
            for j in range(0,len(gt_coordinates[i][0])):
                    gt_image[gt_coordinates[i][0][j],gt_coordinates[i][1][j]]=255

        coords=[]
        plt.imshow(gt_image,cmap=plt.cm.gray)
    #I dont know why it is not continuous, so you will have to run this part of the code after selecting the points
    #seperately 


    # patch_pixels=[]
    # all_patch_pixels = np.zeros((len(coords),), dtype=np.object)
    # for i in range(0,len(coords)):
    #     x=int(math.floor(coords[i][0]))
    #     y=int(math.floor(coords[i][1]))
    #     label_selected = segments[x,y]
    #     patch_coords=np.where(segments==label_selected)
    #     for j in range(0,len(patch_coords[0])):
    #         xp=patch_coords[0][j]
    #         yp=patch_coords[1][j]
    #         patch_pixels.append(image[xp,yp,1])
    #     all_patch_pixels[i]=patch_pixels
    #     patch_pixels=[]