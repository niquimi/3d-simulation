#include "Renderer.hpp"

void clear(HDC hdc, RECT rc) {
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdc, &rc, brush);
    DeleteObject(brush);
}

static void drawLine(HDC hdc, POINT a, POINT b) {
    MoveToEx(hdc, a.x, a.y, NULL);
    LineTo(hdc, b.x, b.y);
}

void drawCube(HDC hdc, float h, const Camera& cam, int w, int screenH) {

    HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HGDIOBJ oldPen = SelectObject(hdc, pen);

    Vec3 v[8] = {
        {-h,-h,-h}, { h,-h,-h}, { h, h,-h}, {-h, h,-h},
        {-h,-h, h}, { h,-h, h}, { h, h, h}, {-h, h, h}
    };

    POINT p[8];
    for (int i = 0; i < 8; i++) {
        p[i] = cam.projectPoint(v[i], w, screenH);
    }

    int edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    for (int i = 0; i < 12; i++) {
        POINT a = p[edges[i][0]];
        POINT b = p[edges[i][1]];

        if (a.x == -1 || b.x == -1) continue;

        drawLine(hdc, a, b);
    }

    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void drawSphere(HDC hdc, const RigidBody& sphere, const Camera& cam, int w, int screenH) {
    POINT center = cam.projectPoint(sphere.position, w, screenH);

    if (center.x == -1) return;

    Vec3 rel = sphere.position - cam.position;
    float z = rel.dot(cam.forward);
    int screenRadius = (int)(cam.focalPoint * sphere.radius / z);

    Ellipse(
        hdc,
        center.x - screenRadius,
        center.y - screenRadius,
        center.x + screenRadius,
        center.y + screenRadius
    );
}