del MyMesh.exe
cl /EHsc /I. MyMesh.cpp MyMeshTest.cpp Affine.cpp
del *.obj
MyMesh.exe