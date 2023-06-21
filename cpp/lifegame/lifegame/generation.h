#pragma once
#ifndef GENERATION_H
#define GENERATION_H

#include <vector>

std::pair<int, std::vector<std::vector<std::string>>> generation(const std::vector<std::vector<std::vector<int>>>& first, const std::vector<int>& rule, const std::vector<std::vector<std::vector<int>>>& filter, int count);

#endif // GENERATION_H