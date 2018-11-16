#include "stdafx.h"
#include "ScnMgr.h"

int g_Seq = 0;

ScnMgr::ScnMgr()
{
	m_Renderer = NULL;
	
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		m_Objects[i] = NULL;
	}


	//Init Renderer
	m_Renderer = new Renderer(500, 500);

	// load texture
	//m_TexSeq = m_Renderer->CreatePngTexture("./textures/sprites-cat-running.png");
	m_TestTexture = m_Renderer->CreatePngTexture("./textures/Texture.png");

	//Create Hero Object
	m_Objects[HERO_ID] = new Object();
	m_Objects[HERO_ID]->SetPosition(0.f, 0.f,0.5f);
	m_Objects[HERO_ID]->SetVel(0.f, 0.f);
	m_Objects[HERO_ID]->SetAcc(0.f, 0.f);
	m_Objects[HERO_ID]->SetSize(0.8f, 0.8f);
	m_Objects[HERO_ID]->SetMass(0.15f);
	m_Objects[HERO_ID]->SetCoefFricf(0.5f);
	m_Objects[HERO_ID]->SetColor(1.f, 1.f, 1.f, 1.f);
	m_Objects[HERO_ID]->SetKind(KIND_HERO);

	//AddObject(1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f);
	AddObject(-1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f);
}

ScnMgr::~ScnMgr()
{
	// nullptr이아닐경우 다 진입한다.
	// 이렇게 딜리트를 해주는 
	if (m_Renderer) {
		delete m_Renderer;
		m_Renderer = NULL;
	}

	if (m_Objects[HERO_ID]) {
		delete[] m_Objects;
		for(int i = 0 ; i< HERO_ID ; i++)
			m_Objects[i] = NULL;
	}

}

void ScnMgr::Shoot(int shootID)
{
	if (shootID == SHOOT_NONE)
	{
		return;
	}
	float amount = 10.f;
	float bvX, bvY;

	bvX = 0.f;
	bvY = 0.f;

	switch (shootID)
	{
	case SHOOT_UP:
		bvX = 0.f;
		bvY = amount;
		break;
	case SHOOT_DOWN:
		bvX = 0.f;
		bvY = -amount;
		break;
	case SHOOT_LEFT:
		bvX = -amount;
		bvY = 0.f;
		break;
	case SHOOT_RIGHT:
		bvX = amount;
		bvY = 0;
		break;
	default:
		break;
	}

	
	float pX, pY, pZ;
	m_Objects[HERO_ID]->GetPosition(&pX, &pY, &pZ);
	float vX, vY;
	m_Objects[HERO_ID]->GetVelocity(&vX, &vY);

	bvX = bvX + vX;
	bvY = bvY + vY;

	AddObject(pX, pY, pZ, 0.2, 0.2, bvX, bvY);

}

void ScnMgr::AddObject(float x, float y, float z, float sx, float sy, float vx, float vy)
{
	int id = FindEmptyObjectSlot();

	if (id < 0)
	{
		return;
	}

	m_Objects[id] = new Object();	// id 를 찾고 해당되는 곳에 포인터를 생성 후 넘겨준다
	m_Objects[id]->SetPosition(x,y,z);
	m_Objects[id]->SetVel(vx, vy);
	m_Objects[id]->SetAcc(0.f, 0.f);
	m_Objects[id]->SetSize(sx, sy);
	m_Objects[id]->SetMass(0.15f);
	m_Objects[id]->SetCoefFricf(0.5f);
	m_Objects[id]->SetColor(1.f, 1.f, 1.f, 1.f);
	m_Objects[id]->SetKind(KIND_HERO);

	std::cout << "id : "<< id << std::endl;

}

int ScnMgr::FindEmptyObjectSlot()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (m_Objects[i] == NULL)
			return i;
	}

	std::cout << "No more empty slot! \n" << std::endl;
	return -1; // error check
}

void ScnMgr::DeleteObject(int id)
{
	if (m_Objects[id] != NULL)
	{
		delete m_Objects[id];
		m_Objects[id] = NULL;
	}
}

void ScnMgr::RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


	for (int i = 0; i < MAX_OBJECTS; i++) {
		if (m_Objects[i] != NULL)
		{
			float x, y, z, sx, sy, r, g, b, a;

			m_Objects[i]->GetPosition(&x, &y, &z);
			m_Objects[i]->GetSize(&sx, &sy);
			m_Objects[i]->GetColor(&r, &g, &b, &a);

			float newX, newY, newZ, newW, newH;

			newX = x * 100;
			newY = y * 100;
			newZ = z * 100;

			newW = sx * 100;
			newH = sy * 100;

			// Renderer Test
			//m_Renderer->DrawSolidRect (x*100, y*100, 0, sx*100,sy*100, r, g, b, a);
			//m_Renderer->DrawTextureRect(x * 100, y * 100, 0, sx * 100, sy * 100, r, g, b, a, m_TestTexture);

			m_Renderer->DrawTextureRectHeight(
			newX, newY, 0.0f,
			newW, newH,
			r,g,b,a,
			m_TestTexture,newZ
			);
		}
	}
	

	/*int seqX = 0;
	int seqY = 0;
	seqX = g_Seq % 4;
	seqY = (int)(g_Seq / 4);

	g_Seq++;
	if (g_Seq > 7)
		g_Seq = 0;

	m_Renderer->DrawTextureRectSeqXY(
		newX, newY, 0.0f,
		newW, newH,
		r, g, b, a,
		m_TexSeq,
		seqX, seqY,4,2);*/
}

float g_temp = 5.f;

void ScnMgr::Update(float eTime)
{
	for (int i = 0; i < MAX_OBJECTS; i++) {
		if(m_Objects[i] != NULL)
			m_Objects[i]->Update(eTime);
	}
	
}

void ScnMgr::GarbageCollector()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (m_Objects[i] != NULL)
		{
			float x, y, z;
			m_Objects[i]->GetPosition(&x, &y, &z);
			if (x > 2.5f || x< -2.5f || y>2.5f || y < -2.5f)
			{
				DeleteObject(i);
			}
		}
	}
}

void ScnMgr::DoCollisionTest()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (m_Objects[i] == NULL)
		{
			continue;
		}
		for (int j = 0; j < MAX_OBJECTS; j++)
		{
			if (m_Objects[j] == NULL)
			{
				continue;
			}

			if (i == j)
			{
				continue;
			}


			// i Object
			float pX, pY, pZ;
			float sX, sY;
			float minX, minY, maxX, maxY;
			m_Objects[i]->GetPosition(&pX, &pY, &pZ);
			m_Objects[i]->GetSize(&sX, &sY);
			minX = pX - sX / 2.f;
			maxX = pX + sX / 2.f;
			minY = pY - sY / 2.f;
			maxY = pY + sY / 2.f;

			// j Object
			float pX1, pY1, pZ1;
			float sX1, sY1;
			float minX1, minY1, maxX1, maxY1;
			m_Objects[j]->GetPosition(&pX1, &pY1, &pZ1);
			m_Objects[j]->GetSize(&sX1, &sY1);
			minX1 = pX1 - sX1 / 2.f;
			maxX1 = pX1 + sX1 / 2.f;
			minY1 = pY1 - sY1 / 2.f;
			maxY1 = pY1 + sY1 / 2.f;

			if (RRCollision(minX, minY, maxX, maxY, minX1, minY1, maxX1, maxY1))
			{
				std::cout << "collision\n";
				DeleteObject(j);
			}
		}
	}
}

bool ScnMgr::RRCollision(float minX, float minY, float maxX, float maxY, float minX1, float minY1, float maxX1, float maxY1)
{
	if (maxX < minX1) return false;
	if (maxX1 < minX) return false;
	if (maxY < minY1) return false;
	if (maxY1 < minY) return false;
	return true;
}

void ScnMgr::ApplyForce(float x, float y, float eTime)
{
	m_Objects[HERO_ID]->ApplyForce(x, y,eTime);
}
