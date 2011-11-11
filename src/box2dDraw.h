#ifndef __BOX_2D_DRAW_H__
#define __BOX_2D_DRAW_H__

void box2dDrawBegin(float viewScale, float x = 0, float y = 0);
void box2dDrawEnd();

void box2dDrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
void box2dDrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
void box2dDrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
void box2dDrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
void box2dDrawLine(float x1, float y1, float x2, float y2);

void box2dDrawWorld(b2World* pWorld, float viewScale, float x = 0, float y = 0);
void box2dCamera(float lookatX, float lookatY, float scale);
void box2dGetLookat(float& x, float& y);
float box2dGetScale();

#endif //__BOX_2D_DRAW_H__