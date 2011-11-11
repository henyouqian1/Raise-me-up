#include "stdafx.h"
#include "box2dDraw.h"


void box2dDrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	//glColor3f(color.r, color.g, color.b);
	//glBegin(GL_LINE_LOOP);
	//for (int32 i = 0; i < vertexCount; ++i)
	//{
	//	glVertex2f(vertices[i].x, vertices[i].y);
	//}
	//glEnd();
}

void box2dDrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

	glVertexPointer(2, GL_FLOAT, sizeof(b2Vec2), vertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

	glDisable(GL_BLEND);
	glColor4f(color.r, color.g, color.b, 1.0f);
	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
}

void box2dDrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	std::vector<float> vtx;
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		vtx.push_back(v.x);
		vtx.push_back(v.y);
		theta += k_increment;
	}
	glVertexPointer(2, GL_FLOAT, sizeof(b2Vec2), &(vtx[0]));
	glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)(vtx.size()>>1));
}

void box2dDrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const float32 k_segments = 32.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	std::vector<float> vtx;
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		vtx.push_back(v.x);
		vtx.push_back(v.y);
		theta += k_increment;
	}
	glVertexPointer(2, GL_FLOAT, sizeof(b2Vec2), &(vtx[0]));
	glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)(vtx.size()>>1));
	glDisable(GL_BLEND);

	theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	vtx.clear();
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		vtx.push_back(v.x);
		vtx.push_back(v.y);
		theta += k_increment;
	}
	glVertexPointer(2, GL_FLOAT, sizeof(b2Vec2), &(vtx[0]));
	glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)(vtx.size()>>1));

	vtx.clear();
	b2Vec2 p = center + radius * axis;
	vtx.push_back(center.x);
	vtx.push_back(center.y);
	vtx.push_back(p.x);
	vtx.push_back(p.y);
	glVertexPointer(2, GL_FLOAT, sizeof(b2Vec2), &(vtx[0]));
	glDrawArrays(GL_LINES, 0, (GLsizei)(vtx.size()>>1));
}

void box2dDrawLine(float x1, float y1, float x2, float y2){
	float vtx[4];
	vtx[0] = x1;
	vtx[1] = y1;
	vtx[2] = x2;
	vtx[3] = y2;
	glVertexPointer(2, GL_FLOAT, sizeof(b2Vec2), vtx);
	glDrawArrays(GL_LINES, 0, 2);
}

void box2dDrawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color)
{
	switch (fixture->GetType())
	{
	case b2Shape::e_circle:
		{
			b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();

			b2Vec2 center = b2Mul(xf, circle->m_p);
			float32 radius = circle->m_radius;
			b2Vec2 axis = xf.R.col1;

			box2dDrawSolidCircle(center, radius, axis, color);
		}
		break;

	case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
			int32 vertexCount = poly->m_vertexCount;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			b2Vec2 vertices[b2_maxPolygonVertices];

			for (int32 i = 0; i < vertexCount; ++i)
			{
				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
			}

			box2dDrawSolidPolygon(vertices, vertexCount, color);
		}
		break;
	}
}

void box2dDrawBegin(float viewScale, float x, float y){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	cml::Matrix44 m;
#ifdef __APPLE__
	float rotDegree = 0;
	const lw::App::Orientation orient = lw::App::s().getConfig().orientation;
	switch (orient) {
		case lw::App::ORIENTATION_LEFT:
			rotDegree = 90;
			break;
		case lw::App::ORIENTATION_RIGHT:
			rotDegree = -90;
			break;
		default:
			break;
	}
	glRotatef(rotDegree, 0, 0, 1);
#endif
#ifdef WIN32
	int dOrient = (lw::App::s().getConfig().orientation-lw::App::s().getConfig().orientation0);
	float rotDegree = -dOrient * 90.f;
	glRotatef(rotDegree, 0, 0, 1);
#endif
	const lw::App::Config conf = lw::App::s().getConfig();
	//x *= viewScale;
	//y *= viewScale;
	cml::matrix_orthographic_RH(m, -(float)conf.width*viewScale*0.5f+x, (float)conf.width*viewScale*0.5f+x, -(float)conf.height*viewScale*0.5f+y, (float)conf.height*viewScale*0.5f+y, -1000.f, 1000.f, cml::z_clip_neg_one);
	glMultMatrixf(m.data());
}

void box2dDrawEnd(){
	glDisableClientState(GL_VERTEX_ARRAY);
}

void box2dDrawWorld(b2World* pWorld, float viewScale, float x, float y){
	box2dDrawBegin(viewScale, x, y);

	for (b2Body* b = pWorld->GetBodyList(); b; b = b->GetNext())
	{
		const b2Transform& xf = b->GetTransform();
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			if (b->IsActive() == false)
			{
				box2dDrawShape(f, xf, b2Color(0.5f, 0.5f, 0.3f));
			}
			else if (b->GetType() == b2_staticBody)
			{
				box2dDrawShape(f, xf, b2Color(0.5f, 0.9f, 0.5f));
			}
			else if (b->GetType() == b2_kinematicBody)
			{
				box2dDrawShape(f, xf, b2Color(0.5f, 0.5f, 0.9f));
			}
			else if (b->IsAwake() == false)
			{
				box2dDrawShape(f, xf, b2Color(0.6f, 0.6f, 0.6f));
			}
			else
			{
				box2dDrawShape(f, xf, b2Color(0.9f, 0.7f, 0.7f));
			}
		}
	}

	box2dDrawEnd();
}

namespace{
	float g_lookatX = 0;
	float g_lookatY = 0;
	float g_scale = 1.f;
}

void box2dCamera(float lookatX, float lookatY, float scale){
	g_lookatX = lookatX;
	g_lookatY = lookatY;
	g_scale = scale;

	cml::Matrix44 m;

	//glMatrixMode(GL_MODELVIEW);
	//cml::matrix_look_at_RH(m, lookatX, lookatY, 1.f, lookatX, lookatY, 0.f, 0.f, 1.f, 0.f);
	//glLoadIdentity();
	//glMultMatrixf(m.data());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

#ifdef __APPLE__
	float rotDegree = 0;
	const lw::App::Orientation orient = lw::App::s().getConfig().orientation;
	switch (orient) {
		case lw::App::ORIENTATION_LEFT:
			rotDegree = 90;
			break;
		case lw::App::ORIENTATION_RIGHT:
			rotDegree = -90;
			break;
		default:
			break;
	}
	glRotatef(rotDegree, 0, 0, 1);
#endif
#ifdef WIN32
	int dOrient = (lw::App::s().getConfig().orientation-lw::App::s().getConfig().orientation0);
	float rotDegree = -dOrient * 90.f;
	glRotatef(rotDegree, 0, 0, 1);
#endif
	const lw::App::Config conf = lw::App::s().getConfig();
	cml::matrix_orthographic_RH(m, -(float)conf.width*scale*0.5f+lookatX, (float)conf.width*scale*0.5f+lookatX, -(float)conf.height*scale*0.5f+lookatY, (float)conf.height*scale*0.5f+lookatY, -1000.f, 1000.f, cml::z_clip_neg_one);
	glMultMatrixf(m.data());
}

void box2dGetLookat(float& x, float& y){
	x = g_lookatX;
	y = g_lookatY;
}

float box2dGetScale(){
	return g_scale;
}