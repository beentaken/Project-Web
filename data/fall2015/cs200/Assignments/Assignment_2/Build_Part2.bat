del cs200_h2.exe
cl /EHsc /I. cs200_h2.cpp Affine.cpp MyMesh.cpp QuadMesh.cpp
del *.obj
cs200_h2.exe
PAUSE