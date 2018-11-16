#pragma once

#include "Renderer.h"
#include "object.h"
#include "Global.h"

class ScnMgr {
private:
	Object * m_Objects[MAX_OBJECTS];
	Renderer * m_Renderer;

	GLuint m_TestTexture = 0;
	GLuint m_TexSeq = 0;
public:
	ScnMgr();
	~ScnMgr();

	void RenderScene(void);

	void Update(float eTime);
	void ApplyForce(float x, float y, float eTime);
	
	void Shoot(int shootID);
	void AddObject(float x, float y, float z, float sx, float sy, float vx, float vy);
	int FindEmptyObjectSlot();
	void DeleteObject(int id);
	void GarbageCollector();
	
	// Collision Check
	void DoCollisionTest();
	bool RRCollision(
		float minX, float minY,
		float maxX, float maxY,
		float minX1, float minY1,
		float maxX1, float maxY1
	);
};