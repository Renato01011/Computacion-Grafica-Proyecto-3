import open3d as o3d
import numpy as np
import math
import random

# Construction Of Random Points

startX = 0
startY = 0
startZ = 0
numLength = 20
r = 3
L = []
N = []

# Starting Vertex
L.append([0, 0, startZ-0.4])
N.append([0, 0, startZ-0.4])
for angle in range(-180, 180, 15):
    randR = round(random.uniform(0.0, 0.5),4) + 2.5
    tmpX = round(startX + math.cos(angle * math.pi / 180.0)*randR,4)
    tmpY = round(startY + math.sin(angle * math.pi / 180.0)*randR,4)

    L.append([tmpX, tmpY, -0.2])
    mag = math.sqrt(tmpX*tmpX + tmpY*tmpY)
    N.append([tmpX/mag, tmpY/mag, startZ-0.2])


for angle in range(-180, 180, 15):
    randR = round(random.uniform(0.0, 0.5),4) + 1.5
    tmpX = round(startX + math.cos(angle * math.pi / 180.0)*randR,4)
    tmpY = round(startY + math.sin(angle * math.pi / 180.0)*randR,4)

    L.append([tmpX, tmpY, startZ-0.3])
    mag = math.sqrt(tmpX*tmpX + tmpY*tmpY)
    N.append([tmpX/mag, tmpY/mag, startZ-0.3])

# Ending Vertex
L.append([0, 0, (startZ+numLength)+0.3])
N.append([0, 0, (startZ+numLength)+0.3])
for angle in range(-180, 180, 15):
    randR = round(random.uniform(0.0, 0.5),4) + 2.5
    tmpX = round(startX + math.cos(angle * math.pi / 180.0)*randR,4)
    tmpY = round(startY + math.sin(angle * math.pi / 180.0)*randR,2)

    L.append([tmpX, tmpY, (startZ+numLength)+0.1])
    mag = math.sqrt(tmpX*tmpX + tmpY*tmpY)
    N.append([tmpX/mag, tmpY/mag, (startZ+numLength)+0.1])

for angle in range(-180, 180, 15):
    randR = round(random.uniform(0.0, 0.5),4) + 1.5
    tmpX = round(startX + math.cos(angle * math.pi / 180.0)*randR,4)
    tmpY = round(startY + math.sin(angle * math.pi / 180.0)*randR,2)

    L.append([tmpX, tmpY, (startZ+numLength)+0.2])
    mag = math.sqrt(tmpX*tmpX + tmpY*tmpY)
    N.append([tmpX/mag, tmpY/mag, (startZ+numLength)+0.2])

tmpZ = startZ
startingAngle = 0
endingAngle = 360
angleIncrement = 15
for i in np.arange(0, numLength, 0.1):
    #print(startingAngle, endingAngle)
    for angle in range(startingAngle, endingAngle, angleIncrement):
        randR = round(random.uniform(0.0, 0.5),4) + r
        tmpX = round(startX + math.cos(angle * math.pi / 180.0)*randR,4)
        tmpY = round(startY + math.sin(angle * math.pi / 180.0)*randR,4)

        L.append([tmpX, tmpY, tmpZ])
        mag = math.sqrt(tmpX*tmpX + tmpY*tmpY)
        N.append([tmpX/mag, tmpY/mag, 0])
    startingAngle += 10
    endingAngle += 10
    #angleIncrement -= 1
    tmpZ += 0.1

pc_array = np.array(L, dtype=np.float32)
pc_norm_array = np.array(N, dtype=np.float32)
#print(pc_array)

pc = o3d.geometry.PointCloud()
pc.points = o3d.utility.Vector3dVector(pc_array)
pc.normals = o3d.utility.Vector3dVector(pc_norm_array)

o3d.visualization.draw_geometries([pc], point_show_normal=True)

with o3d.utility.VerbosityContextManager(
        o3d.utility.VerbosityLevel.Debug) as cm:
    mesh, densities = o3d.geometry.TriangleMesh.create_from_point_cloud_poisson(
        pc, depth=10)

mesh.compute_vertex_normals()
o3d.visualization.draw_geometries([mesh])

varray = []
for i in np.asarray(mesh.vertices):
    varray.append([round(i[0],4),round(i[1],4),round(i[2],4)])

t = np.asarray(mesh.triangles)
tarray = []
tn = np.asarray(mesh.triangle_normals)
tnarray = []

for i in np.asarray(mesh.triangles):
    tarray.append([i[0],i[1],i[2]])

for i in np.asarray(mesh.triangle_normals):
    tnarray.append([round(i[0],4),round(i[1],4),round(i[2],4)])

VAndNormArray = []
IndexArray = []
for i in range(0, len(tarray)):
    tempv0 = varray[tarray[i][0]]
    tempv1 = varray[tarray[i][1]]
    tempv2 = varray[tarray[i][2]]
    tempsize = len(VAndNormArray)
    VAndNormArray.append([tempv0, tnarray[i]])
    VAndNormArray.append([tempv1, tnarray[i]])
    VAndNormArray.append([tempv2, tnarray[i]])
    IndexArray.append([tempsize, tempsize+1, tempsize+2])

f = open("Points.dat", "w")

f.write('Vertices&Normals ')
f.write(str(len(VAndNormArray)))
f.write('\n')
for i in range(0, len(VAndNormArray)):
    f.write(str(VAndNormArray[i][0][0]))
    f.write(" ")
    f.write(str(VAndNormArray[i][0][1]))
    f.write(" ")
    f.write(str(VAndNormArray[i][0][2]))
    f.write(" ")
    f.write(str(VAndNormArray[i][1][0]))
    f.write(" ")
    f.write(str(VAndNormArray[i][1][1]))
    f.write(" ")
    f.write(str(VAndNormArray[i][1][2]))
    f.write("\n")

f.write('Indexes ')
f.write(str(len(IndexArray)))
for i in range(0, len(IndexArray)):
    f.write('\n')
    f.write(str(IndexArray[i][0]))
    f.write(" ")
    f.write(str(IndexArray[i][1]))
    f.write(" ")
    f.write(str(IndexArray[i][2]))