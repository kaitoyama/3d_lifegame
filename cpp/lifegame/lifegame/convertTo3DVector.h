#pragma once
#ifndef CONVERTTO3DVECTOR_H
#define CONVERTTO3DVECTOR_H

#include <vector>

std::vector<std::vector<std::vector<int>>> convertTo3DVector(const std::vector<int>& flatVector, int n);

#endif // CONVERTTO3DVECTOR_H