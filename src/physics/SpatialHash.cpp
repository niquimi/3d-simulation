#include "SpatialHash.hpp"

SpatialHash::SpatialHash(float cellSize, int tableSize)
    : cellSize(cellSize), tableSize(tableSize) 
{
    table.resize(tableSize);
}

int SpatialHash::hashCell(int ix, int iy, int iz) const {
    int p1 = 92837111;
    int p2 = 689287499;
    int p3 = 283923481;

    int hash = (ix * p1) ^ (iy * p2) ^ (iz * p3);

    return ((hash % tableSize) + tableSize) % tableSize;
}

void SpatialHash::clear() {
    for (int i = 0; i < this->table.size(); i++) {
        table[i].clear();
    }
}

void SpatialHash::insert(int sphereIdx, Vec3 position) {
    int ix = (int)std::floor(position.x / cellSize);
    int iy = (int)std::floor(position.y / cellSize);
    int iz = (int)std::floor(position.z / cellSize);

    int idx = hashCell(ix, iy, iz);
    table[idx].push_back(Entry{ix, iy, iz, sphereIdx});
}

void SpatialHash::query(Vec3 position, float radius, std::vector<int>& results) {
    results.clear();

    int minX = (int)std::floor((position.x - radius) / cellSize);
    int maxX = (int)std::floor((position.x + radius) / cellSize);
    int minY = (int)std::floor((position.y - radius) / cellSize);
    int maxY = (int)std::floor((position.y + radius) / cellSize);
    int minZ = (int)std::floor((position.z - radius) / cellSize);
    int maxZ = (int)std::floor((position.z + radius) / cellSize);
    
    for (int ix = minX; ix <= maxX; ix++) {
        for (int iy = minY; iy <= maxY; iy++) {
            for (int iz = minZ; iz <= maxZ; iz++) {
                int idx = hashCell(ix, iy, iz);
                // Filter out hash collisions: a hashed bucket can contain entries from many cells.
                for (const Entry& e : table[idx]) {
                    if (e.ix == ix && e.iy == iy && e.iz == iz) {
                        results.push_back(e.sphereIdx);
                    }
                }
            }
        }
    }
}