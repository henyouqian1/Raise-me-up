#include "stdafx.h"
#include "geomDraw.h"

void geomDrawScreenBegin(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnableClientState(GL_VERTEX_ARRAY);

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
	cml::matrix_orthographic_RH(m, -(float)conf.width*0.5f, (float)conf.width*0.5f, -(float)conf.height*0.5f, (float)conf.height*0.5f, -1000.f, 1000.f, cml::z_clip_neg_one);
	glMultMatrixf(m.data());
}

void geomDrawScreenEnd(){
	glDisableClientState(GL_VERTEX_ARRAY);
}

void geomDrawBegin(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnableClientState(GL_VERTEX_ARRAY);

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
	//cml::matrix_orthographic_RH(m, 0.f, (float)conf.width, -(float)conf.height, 0.f, -1000.f, 1000.f, cml::z_clip_neg_one);
	cml::matrix_orthographic_RH(m, 0.f, (float)conf.width, (float)conf.height, 0.f, -1000.f, 1000.f, cml::z_clip_neg_one);
	glMultMatrixf(m.data());
}

void geomDrawEnd(){
	glDisableClientState(GL_VERTEX_ARRAY);
}

void geomDrawCube(float x, float y, float w, float h, const lw::Color& fillColor, const lw::Color& outlineColor){
	glEnable(GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	float vtx[8];
	vtx[0] = x;
	vtx[1] = y;
	vtx[2] = x+w;
	vtx[3] = y;
	vtx[4] = x+w;
	vtx[5] = y+h;
	vtx[6] = x;
	vtx[7] = y+h;
	if ( fillColor.a > 0 ){
		glColor4f(fillColor.r/255.f, fillColor.g/255.f, fillColor.b/255.f, fillColor.a/255.f);
		glVertexPointer(2, GL_FLOAT, sizeof(float)*2, vtx);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	if ( outlineColor.a > 0 ){
		glColor4f(outlineColor.r/255.f, outlineColor.g/255.f, outlineColor.b/255.f, outlineColor.a/255.f);
		glVertexPointer(2, GL_FLOAT, sizeof(float)*2, vtx);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}
	glDisable(GL_BLEND);
}

void geomDrawCircle(float x, float y, float radius, const lw::Color& fillColor, const lw::Color& outlineColor, int segments){
	const float k_segments = (float)segments;
	const float k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(fillColor.r/255.f, fillColor.g/255.f, fillColor.b/255.f, fillColor.a/255.f);
	std::vector<float> vtx;
	for (int32 i = 0; i < k_segments; ++i)
	{
		float xx = x + radius * cosf(theta);
		float yy = y + radius * sinf(theta);
		vtx.push_back(xx);
		vtx.push_back(yy);
		theta += k_increment;
	}
	glVertexPointer(2, GL_FLOAT, sizeof(float)*2, &(vtx[0]));
	glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)(vtx.size()>>1));

	theta = 0.0f;
	glColor4f(outlineColor.r/255.f, outlineColor.g/255.f, outlineColor.b/255.f, outlineColor.a/255.f);
	vtx.clear();
	for (int32 i = 0; i < k_segments; ++i)
	{
		float xx = x + radius * cosf(theta);
		float yy = y + radius * sinf(theta);
		vtx.push_back(xx);
		vtx.push_back(yy);
		theta += k_increment;
	}
	glVertexPointer(2, GL_FLOAT, sizeof(float)*2, &(vtx[0]));
	glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)(vtx.size()>>1));
	glDisable(GL_BLEND);
}

void geomDrawLine(float x1, float y1, float x2, float y2, const lw::Color& color){
	glColor4f(color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f);
	float vtx[4];
	vtx[0] = x1;
	vtx[1] = y1;
	vtx[2] = x2;
	vtx[3] = y2;
	glVertexPointer(2, GL_FLOAT, sizeof(float)*2, vtx);
	glDrawArrays(GL_LINES, 0, 2);
}