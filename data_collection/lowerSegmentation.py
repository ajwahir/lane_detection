from skimage.segmentation import slic
from skimage.segmentation import mark_boundaries
from skimage.util import img_as_float
from skimage import io
import matplotlib.pyplot as plt
import numpy as np
import math
from os import listdir
from os.path import join

coords = []
numSegments_upper = 400
lower_half_percentage=0.7
numSegments_lower = 150
clicks = 2


def combinealllists(l):
    k=[]

    for i in range(0,len(l)):
        k=combinelists(k,l[i])
    return k

def combinelists(l1,l2):
    l = [None]*(len(l1)+len(l2))
    l[0:len(l1)] = l1
    l[len(l1):len(l1)+len(l2)] = l2
    return l


def onclick(event):
    global clicks
    global ix, iy
    ix, iy = event.xdata, event.ydata
    print 'inside onclick'
    # print 'x = %d, y = %d'%(
    #     ix, iy)

    # assign global variable to access outside of function
    global coords
    coords.append((ix, iy))
    print clicks
    print len(coords)
    if len(coords) == clicks:
        fig.canvas.mpl_disconnect(cid)
        plt.close()
    return

def get_patch_coords(segments,coords):
    patch_pixels=[]
    all_patch_pixels=[]
    lengths=[]
    # all_patch_pixels = np.zeros((len(coords),), dtype=np.object)
    for i in range(0,len(coords)):
        x=int(math.floor(coords[i][0]))
        y=int(math.floor(coords[i][1]))
        label_selected = segments[y,x]
        patch_coords=np.where(segments==label_selected)
        for j in range(0,len(patch_coords[0])):
            xp=patch_coords[0][j]
            yp=patch_coords[1][j]
            patch_pixels.append(image[xp,yp,1])
        # all_patch_pixels[i]=patch_pixels
        all_patch_pixels.append(patch_pixels)
        lengths.append(len(patch_pixels))
        patch_pixels=[]

    return all_patch_pixels,lengths

merged_pixels = dict()
all_image_patches=[]
all_patches_lengths=[]
datapath = '/home/ajwahir/IGVC/dataset/lanes/'

for f in listdir(datapath):
    
    image = img_as_float(io.imread(join(datapath,f)))
    # upper_half=image[0:image.shape[0]/2,:,:]
    lower_half=image[image.shape[0]*(1-lower_half_percentage):image.shape[1],:,:]
    image=lower_half

    segments_lower = slic(lower_half, n_segments = numSegments_lower, sigma = 5)
    segments=segments_lower

    fig = plt.figure("Superpixels")
    ax = fig.add_subplot(1, 1, 1)
    ax.imshow(mark_boundaries(image, segments))
    plt.axis("off")
    # plt.show()
    # ch = raw_input('do you want to go to next image ? :')
    plt.close('all')

    if str(ch) is 'b':
        break
    if str(ch) is not 'y':
        print str(ch)
        continue
    # clicks=input("enter the number of patches you want to select in this image : ")
    clicks=13

    fig = plt.figure("Superpixels")
    ax = fig.add_subplot(1, 1, 1)
    ax.imshow(mark_boundaries(image, segments))
    plt.axis("off")
    cid = fig.canvas.mpl_connect('button_press_event', onclick)
    plt.show()
    plt.close('all')
    image_patches,corres_lengths=get_patch_coords(segments,coords)
    coords=[]   
    all_image_patches.append(image_patches)
    all_patches_lengths.append(corres_lengths)
    print len(combinealllists(all_image_patches))
    
all_combined=[]
all_combined_lengths=[]
all_combined=combinealllists(all_image_patches)
all_combined_lengths=combinealllists(all_patches_lengths)
 
np.save('background_pixels.npy',np.array(all_combined))
np.save('background_lengths.npy',np.array(all_combined_lengths))

# np.save('lanes_lengths.npy',np.array(all_combined_lengths))
# np.save('lanes_pixels.npy',np.array(all_combined))

