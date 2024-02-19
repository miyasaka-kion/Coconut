#include "Camera.h"

#include "Scene.h"

Camera g_camera;

Camera::Camera()
{
	// m_width = 1280;
	// m_height = 800;
	ResetView();
}

//
void Camera::ResetView()
{
	m_center.Set(0.0f, 0.0f);
	m_zoom = 1.0f;
}

//
b2Vec2 Camera::ConvertScreenToWorld(const b2Vec2& ps)
{
	float w = float(m_width);
	float h = float(m_height);
	float u = ps.x / w;
	float v = (h - ps.y) / h;

	float ratio = w / h;
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	b2Vec2 pw;
	pw.x = (1.0f - u) * lower.x + u * upper.x;
	pw.y = (1.0f - v) * lower.y + v * upper.y;
	return pw;
}

float Camera::ConvertScreenToWorld(int length) {
    float PixelsPerMeter = static_cast< float >(g_settings.m_windowHeight) / (g_settings.m_defaultScreenHeightWorld * m_zoom);
    return length / PixelsPerMeter;
}

int Camera::ConvertWorldToScreen(float length) {
    float PixelsPerMeter = static_cast< float >(g_settings.m_windowHeight) / (g_settings.m_defaultScreenHeightWorld * m_zoom);
    return length * PixelsPerMeter;
}

//
b2Vec2 Camera::ConvertWorldToScreen(const b2Vec2& pw)
{
	float w = float(m_width);
	float h = float(m_height);
	float ratio = w / h;
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	float u = (pw.x - lower.x) / (upper.x - lower.x);
	float v = (pw.y - lower.y) / (upper.y - lower.y);

	b2Vec2 ps;
	ps.x = u * w;
	ps.y = (1.0f - v) * h;
	return ps;
}

// Convert from world coordinates to normalized device coordinates.
// http://www.songho.ca/opengl/gl_projectionmatrix.html
void Camera::BuildProjectionMatrix(float* m, float zBias)
{
	float w = float(m_width);
	float h = float(m_height);
	float ratio = w / h;
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	m[0] = 2.0f / (upper.x - lower.x);
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = 2.0f / (upper.y - lower.y);
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;

	m[12] = -(upper.x + lower.x) / (upper.x - lower.x);
	m[13] = -(upper.y + lower.y) / (upper.y - lower.y);
	m[14] = zBias;
	m[15] = 1.0f;
}
