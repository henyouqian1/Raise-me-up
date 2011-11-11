#ifndef __GEOM_DRAW_H__
#define __GEOM_DRAW_H__

void geomDrawScreenBegin();
void geomDrawScreenEnd();
void geomDrawBegin();
void geomDrawEnd();

void geomDrawCube(float x, float y, float w, float h, const lw::Color& fillColor, const lw::Color& outlineColor);
void geomDrawCircle(float x, float y, float radius, const lw::Color& fillColor, const lw::Color& outlineColor, int segments = 12);
void geomDrawLine(float x1, float y1, float x2, float y2, const lw::Color& color);


#endif //__GEOM_DRAW_H__