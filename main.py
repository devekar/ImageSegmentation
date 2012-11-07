from scipy.misc import imresize
from scipy.misc import imsave
import graphcut
from numpy import *
from PIL import Image
from pylab import *

im = array(Image.open('empire.jpg'))
im = imresize(im,0.04,interp='bilinear')


size = im.shape[:2]
print size

# add two rectangular training regions
labels = zeros(size)
labels[3:18,3:18] = -1
labels[-18:-3,-18:-3] = 1

# create graph
g = graphcut.build_bayes_graph(im,labels,kappa=1)


out=open("tocpp.txt",'w')
out.write(str(len(g))+'\n')

edlist=g.edges()
for a in edlist:
    out.write(str(a[0]) + ' ' + str(a[1]) + ' ' + str(g.edge_weight((a[0],a[1])))+'\n')
out.write(str(-1)+' '+str(-1)+' '+str(-1)+'\n')
out.close()


algo=r"edmond_karp.exe"
#algo=r"push_relabel.exe"
print algo

# cut the graph
res = graphcut.cut_graph(g,size,algo)


#figure()
#graphcut.show_labeling(im,labels)

figure()
imshow(res)
gray()
axis('off')
show()


