from itertools import product
import scipy.io
from svmutil import *
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


def zerolistmaker(n):
    listofzeros = [1] * n
    return listofzeros

def remove_unconnected(segments):
	return 0


# load the model
model = svm_load_model("lane_det.model")
p_threshold=1.0
datapath = '/home/ajwahir/IGVC/dataset/lanes/'

for f in listdir(datapath):
	    
	frame = img_as_float(io.imread(join(datapath,f)))

	# frame=img_as_float(io.imread("/home/ajwahir/IGVC/dataset/lanes/image-034.jpeg"))

	lower_half_percentage=0.7
	numSegments_lower = 150


	image=frame[frame.shape[0]*(1-lower_half_percentage):frame.shape[1],:,:]
	segments = slic(image, n_segments = numSegments_lower, sigma = 5)

	all_patch_pixels=[]

	for i in range(0,np.amax(segments)):
		patch_pixels=[]
		patch_coords=np.where(segments==i)
		for j in range(0,len(patch_coords[0])):
			xp=patch_coords[0][j]
			yp=patch_coords[1][j]
			patch_pixels.append(image[xp,yp,1])
		all_patch_pixels.append(patch_pixels)

	bins=30

	features=[]

	for i in range(0,len(all_patch_pixels)):
		h_inter=np.histogram(all_patch_pixels[i],bins)
		features.append(h_inter[1].tolist())

	fake_class=(np.array(zerolistmaker(len(features)))*[2]).tolist()
	p_label, accuracy, p_val=svm_predict(fake_class,features, model)
	

	# p_lane_labels=np.where(np.asarray(p_label)==1)
	p_lane_labels=np.where(np.asarray(p_val)>p_threshold)
	p_lane_labels=p_lane_labels[0].tolist()

	lane_segments=segments*0

	for i in range(0,len(p_lane_labels)):
		x,y=(np.where(segments==p_lane_labels[i]))
		lane_segments[x,y]=i+1




	fig = plt.figure("Superpixels")
	ax = fig.add_subplot(1, 1, 1)
	ax.imshow(mark_boundaries(image, lane_segments))
	plt.axis("off")
	plt.show()

