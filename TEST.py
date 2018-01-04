import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial import Voronoi
import random

def voronoi_finite_polygons_2d(vor, radius=None):
    if vor.points.shape[1] != 2:
        raise ValueError("Requires 2D input")

    regions = []
    vertices = vor.vertices.tolist()
    vertices_route = vor.vertices.tolist()

    center = vor.points.mean(axis=0)
    if radius is None:
        radius = vor.points.ptp().max()

    # Construct a map containing all ridges for a given point
    ridges = {}
    for (point1, point2), (vector1, vector2) in zip(vor.ridge_points, vor.ridge_vertices):
        ridges.setdefault(point1, []).append((point2, vector1, vector2))
        ridges.setdefault(point2, []).append((point1, vector1, vector2))

    # Reconstruct infinite regions
    for point1, region in enumerate(vor.point_region):
        vertices = vor.regions[region]

        if all(v >= 0 for v in vertices):
            # finite region
            regions.append(vertices)
            continue

        # reconstruct a non-finite region
        ridges = ridges[point1]
        region = [v for v in vertices if v >= 0]

        for point2, vector1, vector2 in ridges:
            if vector2 < 0:
                vector1, vector2 = vector2, vector1
            if vector1 >= 0:
                # finite ridge: already in the region
                continue
            '''if vor.vertices[vector2][0]>1 or vor.vertices[vector2][0]<0 or vor.vertices[vector2][1]>1 or vor.vertices[vector2][1]<0:
                continue'''

            # Compute the missing endpoint of an infinite ridge

            t = vor.points[point2] - vor.points[point1] # tangent
            t /= np.linalg.norm(t)
            n = np.array([-t[1], t[0]])  # normal

            midpoint = vor.points[[point1, point2]].mean(axis=0)
            direction = np.sign(np.dot(midpoint - center, n)) * n
            far_point = vor.vertices[vector2] + direction * radius
            side_point = vor.vertices[vector2] + direction * radius
            
            # Judge the point place.
            if vor.vertices[vector2][0]<1 and vor.vertices[vector2][0]>0 and vor.vertices[vector2][1]<1 and vor.vertices[vector2][1]>0 :
	            a = abs(vor.vertices[vector2][0]/direction[0])
	            b = abs(vor.vertices[vector2][1]/direction[1])
	            if direction[1]/direction[0] > -1 and direction[1]/direction[0] < 1 :
	                if far_point[0] < 0:

	                    side_point[0] = 1
	                    side_point[1] = vor.vertices[vector2][1] + direction[1]/direction[0]*(1-vor.vertices[vector2][0])
	                    #far_point = vor.vertices[vector2] + direction * a
	                else :
	                    #far_point = vor.vertices[vector2] + [1,1] - direction * a
	                    side_point[0] = 1
	                    side_point[1] = vor.vertices[vector2][1] + direction[1]/direction[0]*(1-vor.vertices[vector2][0])
	            else:
	                if far_point[1] < 0:
	                    side_point[0] = vor.vertices[vector2][0] - direction[0]/direction[1]*(vor.vertices[vector2][1])
	                    side_point[1] = 0
	                    #far_point = vor.vertices[vector2] + direction * b
	                else :
	                    #far_point = vor.vertices[vector2] + [1,1] - direction * b
	                    side_point[0] = vor.vertices[vector2][0] + direction[0]/direction[1]*(1-vor.vertices[vector2][1])
	                    side_point[1] = 1


            region.append(len(vertices))
            vertices.append(far_point.tolist())
            vertices_route.append(side_point.tolist())

        # sort region counterclockwise
        vs = np.asarray([vertices[v] for v in region])
        c = vs.mean(axis=0)
        angles = np.arctan2(vs[:,1] - c[1], vs[:,0] - c[0])
        region = np.array(region)[np.argsort(angles)]

        # finish
        regions.append(region.tolist())


    return regions, np.asarray(vertices), np.asarray(vertices_route)

# make up data points
np.random.seed(123)
points = np.random.rand(8, 2)
#Change the point through input.

# compute Voronoi tesselation
vor = Voronoi(points)

# plot
regions, vertices, sidepoi = voronoi_finite_polygons_2d(vor)
print ("--")
print (regions)
print ("--")
print (sidepoi)

# colorize
for region in regions:
    polygon = vertices[region]
    plt.fill(*zip(*polygon), alpha=0.4)
from PIL import Image
import random
import math


plt.plot(points[:,0], points[:,1], 'ko')
plt.xlim(0,1)
plt.ylim(0,1)
plt.xlim(vor.min_bound[0] , vor.max_bound[0] )
plt.ylim(vor.min_bound[1] , vor.max_bound[1] )

plt.show()

# find the nearest point to (1,1)
print ("--")
minDis = vertices
minDista = list()
for i in range(0,len(minDis)):
    minDista.append(np.linalg.norm(1-minDis[i]))
    mina = min(minDista)
print(len(minDista))
print (mina)
pa=minDista.index(mina)
# find the nearest point to (0,0)
print ("--")
#minDis = vertices
minDistb = list()
for i in range(0,len(minDis)):
    minDistb.append(np.linalg.norm(minDis[i]))
    minb = min(minDistb)
print(len(minDistb))
print (minb) 
pb =minDistb.index(minb)

print(pa)
print(pb)



