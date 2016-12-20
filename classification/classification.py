from itertools import product
import scipy.io
from svmutil import *
import numpy as np
import math
import matplotlib.pyplot as plt
from os import listdir
from os.path import join
import sklearn.preprocessing

lanes_pixels=np.load('/home/ajwahir/IGVC/codes/lane_detection/data_collection/lanes_pixels.npy')
background_pixels=np.load('/home/ajwahir/IGVC/codes/lane_detection/data_collection/background_pixels.npy')

