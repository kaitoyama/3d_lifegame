#pragma once
#ifndef ROTATEANDFLIPSEARCH_H
#define ROTATEANDFLIPSEARCH_H

#include <vector>
#include <string>

typedef std::vector<std::vector<std::vector<int>>> ThreeDimensionalVector;


std::vector<std::vector<std::vector<int>>> removeOuterFaces(const std::vector<std::vector<std::vector<int>>>& arr);
std::vector<std::vector<std::vector<std::vector<int>>>> enumerateRotations(const std::vector<std::vector<std::vector<int>>>& arr);
std::vector<std::vector<std::vector<std::vector<int>>>> enumerateFlip(const std::vector<std::vector<std::vector<int>>>& arr);
bool isSame(const std::vector<std::vector<std::vector<int>>>& arr1, const std::vector<std::vector<std::vector<int>>>& arr2);
std::string normalize(const std::vector<std::vector<std::vector<int>>>& arr,int n);
std::vector<std::vector<std::vector<std::vector<int>>>> sliceByZeroFace(const std::vector<std::vector<std::vector<int>>>& arr);
std::vector<ThreeDimensionalVector> collectThreeDimensionalVectors(ThreeDimensionalVector& input);

#endif // ROTATEANDFLIPSEARCH_H

