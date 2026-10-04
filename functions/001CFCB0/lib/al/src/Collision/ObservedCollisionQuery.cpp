#include <Collision/ObservedCollisionQuery.h>

extern "C" unsigned fn_001CFCB0(al::KCollisionServer* server, const sead::Vector3f* point,
    unsigned limit, observed_collision_query::Ring* contacts, float radius) {
    using namespace observed_collision_query;
    Contact contact;
    contact.triangle = 0;
    contact.distance = 0.0f;
    contact.kind = 0;
    sead::Vector3f upper;
    upper.x = point->x + radius;
    upper.y = point->y + radius;
    upper.z = point->z + radius;
    sead::Vector3f lower;
    lower.x = point->x - radius;
    lower.y = point->y - radius;
    lower.z = point->z - radius;
    Coordinates first, last, cursor;
    int shift;
    u16* previous = 0;
    u16* longestCell = 0;
    unsigned found = 0;
    if (!fn_00333A74(server, &first, &last, &lower, &upper)) return 0;
    cursor.z = first.z;
    do {
        int stepZ = 1000000;
        cursor.y = first.y;
        do {
            int stepY = 1000000;
            int longestY = 0;
            cursor.x = first.x;
            do {
                u16* cell = fn_0024A0E0(server, &shift, &cursor);
                int size = 1 << shift;
                int mask = size - 1;
                int stepX = size - (cursor.x & mask);
                int cellY = size - (cursor.y & mask);
                int cellZ = size - (cursor.z & mask);
                if (stepZ > cellZ) stepZ = cellZ;
                if (stepY > cellY) stepY = cellY;
                if (cellY > longestY && cell[1]) {
                    longestCell = cell;
                    longestY = cellY;
                }
                if (!previous || previous != cell) {
                    for (u16* id = cell + 1; *id; ++id) {
                        Triangle* triangles = static_cast<Triangle*>(server->getObservedHeader()->trianglesSection);
                        Triangle* triangle = triangles + *id;
                        contact.triangle = triangle;
                        if (!(triangle->height <= 0.0f)) {
                            Ring::Iterator at = contacts->begin();
                            Ring::Iterator end = contacts->end();
                            while (at != end && (*at).triangle != triangle) ++at;
                            if (at == end) {
                                bool hit = fn_001CF7F0(server, contact.triangle, point, &contact.distance, &contact.kind, radius);
                                if (hit && found < limit) {
                                    contacts->push(contact);
                                    ++found;
                                }
                            }
                        }
                    }
                }
                cursor.x += stepX;
            } while (static_cast<unsigned>(cursor.x) <= static_cast<unsigned>(last.x));
            previous = longestCell;
            cursor.y += stepY;
        } while (static_cast<unsigned>(cursor.y) <= static_cast<unsigned>(last.y));
        cursor.z += stepZ;
    } while (static_cast<unsigned>(cursor.z) <= static_cast<unsigned>(last.z));
    return found;
}

extern "C" unsigned fn_001D0464(al::KCollisionServer* server, const sead::Vector3f* point,
    unsigned limit, observed_collision_query::Ring* contacts, float radius) {
    using namespace observed_collision_query;
    Contact contact;
    contact.triangle = 0;
    contact.distance = 0.0f;
    contact.kind = 0;
    sead::Vector3f upper;
    upper.x = point->x + radius;
    upper.y = point->y + radius;
    upper.z = point->z + radius;
    sead::Vector3f lower;
    lower.x = point->x - radius;
    lower.y = point->y - radius;
    lower.z = point->z - radius;
    Coordinates first, last, cursor;
    int shift;
    u16* previous = 0;
    u16* longestCell = 0;
    unsigned found = 0;
    if (!fn_00333A74(server, &first, &last, &lower, &upper)) return 0;
    cursor.z = first.z;
    do {
        int stepZ = 1000000;
        cursor.y = first.y;
        do {
            int stepY = 1000000;
            int longestY = 0;
            cursor.x = first.x;
            do {
                u16* cell = fn_0024A0E0(server, &shift, &cursor);
                int size = 1 << shift;
                int mask = size - 1;
                int stepX = size - (cursor.x & mask);
                int cellY = size - (cursor.y & mask);
                int cellZ = size - (cursor.z & mask);
                if (stepZ > cellZ) stepZ = cellZ;
                if (stepY > cellY) stepY = cellY;
                if (cellY > longestY && cell[1]) {
                    longestCell = cell;
                    longestY = cellY;
                }
                if (!previous || previous != cell) {
                    for (u16* id = cell + 1; *id; ++id) {
                        Triangle* triangles = static_cast<Triangle*>(server->getObservedHeader()->trianglesSection);
                        Triangle* triangle = triangles + *id;
                        contact.triangle = triangle;
                        if (!(triangle->height <= 0.0f)) {
                            Ring::Iterator at = contacts->begin();
                            Ring::Iterator end = contacts->end();
                            while (at != end && (*at).triangle != triangle) ++at;
                            if (at == end) {
                                bool hit = fn_001CFFA4(server, contact.triangle, point, &contact.distance, &contact.kind, radius);
                                if (hit && found < limit) {
                                    contacts->push(contact);
                                    ++found;
                                }
                            }
                        }
                    }
                }
                cursor.x += stepX;
            } while (static_cast<unsigned>(cursor.x) <= static_cast<unsigned>(last.x));
            previous = longestCell;
            cursor.y += stepY;
        } while (static_cast<unsigned>(cursor.y) <= static_cast<unsigned>(last.y));
        cursor.z += stepZ;
    } while (static_cast<unsigned>(cursor.z) <= static_cast<unsigned>(last.z));
    return found;
}

