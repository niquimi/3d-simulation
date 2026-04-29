#include "Renderer.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

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

void drawSpheres(HDC hdc, const std::vector<RigidBody>& spheres, const Camera& cam, int w, int screenH) {
    
    for (const RigidBody& sphere : spheres) {

        POINT center = cam.projectPoint(sphere.position, w, screenH);

        if (center.x == -1) continue;

        Vec3 rel = sphere.position - cam.position;
        float z = rel.dot(cam.forward);
        float radiusF = cam.focalPoint * sphere.radius / z;
        int screenRadius = (int)(std::ceil(radiusF));

        Ellipse(
            hdc,
            center.x - screenRadius,
            center.y - screenRadius,
            center.x + screenRadius,
            center.y + screenRadius
        );
    }
}

void drawSpheresRaycast(HDC hdc, const std::vector<RigidBody>& spheres, const Camera& cam, int w, int h, Vec3 lightDir, uint8_t* pixelBuffer) {
    lightDir = lightDir.normalized();

    // Z-buffer: un float por píxel, inicializado a infinito.
    // Representa la t mínima (más cercana) que ha tocado cada píxel.
    std::vector<float> zBuffer(w * h, std::numeric_limits<float>::infinity());

    // Primera pasada: para cada píxel encontrar la esfera más cercana
    // y guardar su índice y t en buffers auxiliares.
    std::vector<int>   hitSphere(w * h, -1);
    std::vector<float> hitT     (w * h, std::numeric_limits<float>::infinity());

    for (int i = 0; i < (int)spheres.size(); i++) {
        const RigidBody& sphere = spheres[i];

        POINT center = cam.projectPoint(sphere.position, w, h);
        if (center.x == -1) continue;

        Vec3 rel = sphere.position - cam.position;
        float z = rel.dot(cam.forward);
        if (z <= 0.0f) continue;
        float radiusF = cam.focalPoint * sphere.radius / z;
        int screenRadius = (int)(std::ceil(radiusF));
        if (screenRadius <= 0) continue;

        int cx = (int)center.x;
        int cy = (int)center.y;

        int x0 = (std::max)(0, cx - screenRadius);
        int x1 = (std::min)(w - 1, cx + screenRadius);
        int y0 = (std::max)(0, cy - screenRadius);
        int y1 = (std::min)(h - 1, cy + screenRadius);

        for (int py = y0; py <= y1; py++) {
            for (int px = x0; px <= x1; px++) {

                float ndcX = (px - w * 0.5f) / cam.focalPoint;
                float ndcY = (h * 0.5f - py) / cam.focalPoint;

                Vec3 rayDir    = (cam.forward + cam.right * ndcX + cam.up * ndcY).normalized();
                Vec3 rayOrigin = cam.position;

                Vec3  oc = rayOrigin - sphere.position;
                float b = 2.0f * oc.dot(rayDir);
                float c = oc.dot(oc) - sphere.radius * sphere.radius;
                float discriminant = b * b - 4.0f * c;
                if (discriminant < 0.0f) continue;

                float t = (-b - std::sqrt(discriminant)) * 0.5f;
                if (t <= 0.0f) continue;

                // Solo actualizamos si esta esfera está más cerca
                // que cualquier otra que haya tocado este píxel antes.
                int idx = py * w + px;
                if (t < hitT[idx]) {
                    hitT[idx]      = t;
                    hitSphere[idx] = i;
                }
            }
        }
    }

    int stride = w * 4;

    // Segunda pasada: shading solo del píxel ganador (la esfera más cercana).
    for (int py = 0; py < h; py++) {
        for (int px = 0; px < w; px++) {

            int idx = py * w + px;
            if (hitSphere[idx] == -1) continue;  // ninguna esfera tocó este píxel

            const RigidBody& sphere = spheres[hitSphere[idx]];

            float ndcX = ((px + 0.5f) - w * 0.5f) / cam.focalPoint;
            float ndcY = (h * 0.5f - (py + 0.5f)) / cam.focalPoint;

            Vec3 rayDir    = (cam.forward + cam.right * ndcX + cam.up * ndcY).normalized();
            Vec3 rayOrigin = cam.position;

            float t        = hitT[idx];
            Vec3  hitPoint = rayOrigin + rayDir * t;
            Vec3  normal   = (hitPoint - sphere.position).normalized();

            float ambient   = 0.15f;
            float diffuse   = (std::max)(0.0f, normal.dot(lightDir));
            Vec3  viewDir   = (rayOrigin - hitPoint).normalized();
            Vec3  halfVec   = (lightDir + viewDir).normalized();
            float specAngle = (std::max)(0.0f, normal.dot(halfVec));
            float specular  = std::pow(specAngle, 48.0f);

            Vec3  baseColor = sphere.color;
            Vec3  specColor = Vec3(1.0f, 1.0f, 1.0f);

            float r  = (std::min)(baseColor.x * (ambient + diffuse * 0.7f) + specular * 0.5f, 1.0f);
            float g  = (std::min)(baseColor.y * (ambient + diffuse * 0.7f) + specular * 0.5f, 1.0f);
            float b2 = (std::min)(baseColor.z * (ambient + diffuse * 0.7f) + specular * 0.5f, 1.0f);

            int bufferIdx = (h - 1 - py) * stride + px * 4;
            pixelBuffer[bufferIdx] = (uint8_t)(b2 * 255.0f);
            pixelBuffer[bufferIdx + 1] = (uint8_t)(g * 255.0f);
            pixelBuffer[bufferIdx + 2] = (uint8_t)(r * 255.0f);
            pixelBuffer[bufferIdx + 3] = 0;
        }
    }
}