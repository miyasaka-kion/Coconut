#pragma once

struct Settings
{
	Settings()
	{
		Reset();
	}

	void Reset()
	{
		m_testIndex = 0;
		m_windowWidth = 1600;
		m_windowHeight = 900;
		m_hertz = 60.0f;
		m_velocityIterations = 8;
		m_positionIterations = 3;
        // m_screenHeightWorld = 50.0f;
        
		m_drawShapes = true;
		m_drawJoints = true;
		m_drawAABBs = false;
		m_drawContactPoints = false;
		m_drawContactNormals = false;
		m_drawContactImpulse = false;
		m_drawFrictionImpulse = false;
		m_drawCOMs = false;
		m_drawStats = false;
		m_drawProfile = false;
		m_enableWarmStarting = true;
		m_enableContinuous = true;
		m_enableSubStepping = false;
		m_enableSleep = true;
		m_pause = false;
		m_singleStep = false;
	}

	void Save();
	void Load();

	int m_testIndex;
	int m_windowWidth;
	int m_windowHeight;
	float m_hertz;
	int m_velocityIterations;
	int m_positionIterations;
    const float m_defaultScreenHeightWorld = 50.0f;

	bool m_drawShapes;
	bool m_drawJoints;
	bool m_drawAABBs;
	bool m_drawContactPoints;
	bool m_drawContactNormals;
	bool m_drawContactImpulse;
	bool m_drawFrictionImpulse;
	bool m_drawCOMs;
	bool m_drawStats;
	bool m_drawProfile;
	bool m_enableWarmStarting;
	bool m_enableContinuous;
	bool m_enableSubStepping;
	bool m_enableSleep;
	bool m_pause;
	bool m_singleStep;
};
