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

def getclasses(lengths):
	classes=[]
	for i in range(0,len(lengths)):
		for j in range(0,lengths[i]):
			classes.append(i+1)
	return classes

def combinelists(l1,l2):
	l = [None]*(len(l1)+len(l2))
	l[0:len(l1)] = l1
	l[len(l1):len(l1)+len(l2)] = l2
	return l

def zerolistmaker(n):
    listofzeros = [1] * n
    return listofzeros

bins=30

flanes =[]
fback = []
for i in range(0,len(lanes_pixels)):
	h_inter=np.histogram(lanes_pixels[i],bins)
	flanes.append(h_inter[1].tolist())

for i in range(0,len(background_pixels)):
	h_inter=np.histogram(background_pixels[i],bins)
	fback.append(h_inter[1].tolist())




lanes_train_length=int(len(flanes)*0.7)
lanes_val_length=len(flanes)-lanes_train_length
lanes_class_train=(np.array(zerolistmaker(lanes_train_length))*[1]).tolist()
lanes_class_val=(np.array(zerolistmaker(lanes_val_length))*[1]).tolist()


back_class_train =[]
back_class_val = []
back_train_length=int(len(fback)*0.7)
back_val_length=len(fback)-back_train_length
back_class_train=(np.array(zerolistmaker(back_train_length))*[-1]).tolist()
back_class_val=(np.array(zerolistmaker(back_val_length))*[-1]).tolist()


train=combinelists(flanes[0:lanes_train_length],fback[0:back_train_length])
val = combinelists(flanes[lanes_train_length:],fback[back_train_length:])

trainclass=combinelists(lanes_class_train,back_class_train)
valclass=combinelists(lanes_class_val,back_class_val)


c=50

params='-t 2 -g 10 -c 200'	
# params='-t 1 -g 0.03 -c 900 -d 3 -r 4'
# params='-t 0 -c 5 ' 
# clss='12345'

m=svm_train(trainclass,train,params)
p_label, accuracy, p_val = svm_predict(valclass,val, m)

# save model
svm_save_model("lane_det.model",m)