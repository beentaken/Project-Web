//******************************************************************************
// Name       : Damien Baca
// Assignment : Final Project First Draft
// Course     : CS200
// Term       : Fall 2015
//
// Description:
//   My created mesh that is used in the project(header)
//******************************************************************************
#include "MyMesh.h"

const Point MyMesh::vertices[8] = { Point(0,4) , Point(1,1) , Point(4,0), 
                                    Point(1,-1), Point(0,-4), Point(-1,-1), 
                                    Point(-4,0) , Point(-1,1) };

const Mesh::Face MyMesh::faces[6] = { Face(0,1,7), Face(2,3,1), Face(4,5,3),
                                      Face(6,7,5), Face(7,1,5), Face(1,5,3) };


const Mesh::Edge MyMesh::edges[13] = { Edge(0,1), Edge(1,2), Edge(2,3),
                                       Edge(3,4), Edge(4,5), Edge(5,6),
                                       Edge(6,7), Edge(7,0), Edge(7,5),
                                       Edge(5,3), Edge(3,1), Edge(1,7),
                                       Edge(1,5) };