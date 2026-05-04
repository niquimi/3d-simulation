#pragma once
#include <vector>
#include <cmath>
#include "../math/Vec3.hpp"

struct SpatialHash {
    struct Entry {
        int ix;
        int iy;
        int iz;
        int sphereIdx;
    };

    float cellSize;
    int tableSize;

    std::vector<std::vector<Entry>> table;

    SpatialHash(float cellSize, int tableSize);

    void clear();
    void insert(int sphereIdx, Vec3 position);
    void query(Vec3 position, float radius, std::vector<int>& results);

private:
    int hashCell(int ix, int iy, int iz) const;
};