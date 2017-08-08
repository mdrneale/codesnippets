
# coding: utf-8

# In[45]:

get_ipython().magic(u'matplotlib inline')

import matplotlib.pyplot as plt
import PIL.Image as pili
import numpy as np
from math import pi,sin,cos,tan
from PIL import Image
from math import pi,sin,cos,tan,atan2,hypot,floor
from numpy import clip


# In[443]:

im = pili.open('/Users/matt/Downloads/Zappar Office 360 photos/Main Office 2/mainoffice2.JPG')


# In[444]:

cube = pili.new('RGB', (im.size[0],int(im.size[1]*1.5)), (128, 0, 0))


# In[445]:

im


# In[446]:

cube.putpixel((256,128), (0, 255, 0))


# In[447]:

def getpixel(im, x, y):
    px = np.round(x * im.width)
    py = np.round(y * im.height)
    return im.getpixel((px,py))


# In[448]:

# get x,y,z coords from out image pixels coords
# i,j are pixel coords
# face is face number
# edge is edge length
def outImgToXYZ(i,j,face,edge):
    a = 2.0*float(i)/edge
    b = 2.0*float(j)/edge
    if face==0: # back
        (x,y,z) = (-1.0, 1.0-a, 3.0 - b)
    elif face==1: # left
        (x,y,z) = (a-3.0, -1.0, 3.0 - b)
    elif face==2: # front
        (x,y,z) = (1.0, a - 5.0, 3.0 - b)
    elif face==3: # right
        (x,y,z) = (7.0-a, 1.0, 3.0 - b)
    elif face==4: # top
        (x,y,z) = (b-1.0, a - 5.0, 1.0)
    elif face==5: # bottom
        (x,y,z) = (5.0-b, a-5.0, -1.0)
    else:
        (x,y,z) = 0,0,0;
    return (x,y,z)


# In[449]:

def bilinear(u, v, pixels, sz):
    # Use bilinear interpolation between the four surrounding pixels
    ui = floor(u)  # coord of pixel to bottom left
    vi = floor(v)
    u2 = ui+1       # coords of pixel to top right
    v2 = vi+1
    mu = u-ui      # fraction of way across pixel
    nu = v-vi
    # Pixel values of four corners
    A = pixels[ui % sz[0], vi % sz[1]]
    B = pixels[u2 % sz[0], vi % sz[1]]
    C = pixels[ui % sz[0], v2 % sz[1]]
    D = pixels[u2 % sz[0], v2 % sz[1]]
    # interpolate
    return (A[0] * (1-mu) * (1-nu) + B[0] * (mu) * (1-nu) + C[0] * (1-mu) * nu + D[0] * mu * nu,
            A[1] * (1-mu) * (1-nu) + B[1] * (mu) * (1-nu) + C[1]*(1-mu) * nu + D[1] * mu * nu,
            A[2] * (1-mu) * (1-nu) + B[2] * (mu) * (1-nu) + C[2]*(1-mu) * nu + D[2] * mu * nu)


# In[450]:

def fill(outIm, inIm, x, y):
    outIm.paste(inIm.crop((x, y, x + outIm.size[0], y + outIm.size[1])))


# In[451]:

def fill2(outIm, inIm, x, y):
    outIm.paste(inIm, (x,y))


# In[452]:

def convertBack(imgIn, imgOut):
    inSize = imgIn.size
    outSize = imgOut.size
    inPix = imgIn.load()
    outPix = imgOut.load()
    edge = inSize[0]/4   # the length of each edge in pixels
    for i in xrange(0, outSize[0]):
        face = int(i/edge) # 0 - back, 1 - left 2 - front, 3 - right
        if face==2:
            rng = xrange(0,edge*3)
        else:
            rng = xrange(edge, edge*2)

        for j in rng:
            if j<edge:
                face2 = 4 # top
            elif j>=2*edge:
                face2 = 5 # bottom
            else:
                face2 = face
            (x,y,z) = outImgToXYZ(i+0.5,j+0.5,face2,edge)
            theta = atan2(y,x) # range -pi to pi
            r = hypot(x,y)
            phi = atan2(z,r) # range -pi/2 to pi/2
            # source img coords
            uf = ( 2.0*edge*(theta + pi)/pi )
            vf = ( 2.0*edge * (pi/2 - phi)/pi)
            (r, g, b) = bilinear(uf, vf, inPix, inSize)

            outPix[i,j] = (int(round(r)),int(round(g)),int(round(b)))


# In[453]:

convertBack(im, cube)


# In[454]:

final = pili.new('RGB', (im1.size[0] * 4, im1.size[1] * 2), (128, 128, 128))
fill(im1, cube, 0, 1*im1.size[1])
fill2(final, im1, 0, im1.size[1]/2)
fill(im1, cube, 1*im1.size[0], 1*im1.size[1])
fill2(final, im1, im1.size[0], im1.size[1]/2)
fill(im1, cube, 2*im1.size[0], 1*im1.size[1])
fill2(final, im1, 2*im1.size[0], im1.size[1]/2)
fill(im1, cube, 3*im1.size[0], 1*im1.size[1])
fill2(final, im1, 3*im1.size[0], im1.size[1]/2)

fill(im1, cube, 2*im1.size[0], 0)
fill2(final, im1, 2*im1.size[0], -im1.size[1]/2)
im1 = im1.rotate(90);
fill2(final, im1, 1*im1.size[0], -im1.size[1]/2)
im1 = im1.rotate(90);
fill2(final, im1, 0*im1.size[0], -im1.size[1]/2)
im1 = im1.rotate(90);
fill2(final, im1, 3*im1.size[0], -im1.size[1]/2)

fill(im1, cube, 2*im1.size[0], 2*im1.size[1])
fill2(final, im1, 2*im1.size[0], 3*im1.size[1]/2)
im1 = im1.rotate(-90);
fill2(final, im1, 1*im1.size[0], 3*im1.size[1]/2)
im1 = im1.rotate(-90);
fill2(final, im1, 0*im1.size[0], 3*im1.size[1]/2)
im1 = im1.rotate(-90);
fill2(final, im1, 3*im1.size[0], 3*im1.size[1]/2)



# In[455]:

final


# In[456]:

final.save('/Users/matt/Downloads/Zappar Office 360 photos/sqrmainoffice2.JPG')


# In[428]:

z=0


# In[211]:

for i in xrange(4,8):
    print i


# im1

# In[254]:

fill(im1, cube, im1.size[0] * 2, 2*im1.size[1])


# In[255]:

im1


# In[ ]:



