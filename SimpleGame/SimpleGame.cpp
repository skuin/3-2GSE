/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "ScnMgr.h"
#include "Global.h"

ScnMgr *g_ScnMgr = NULL;
DWORD g_PrevTime = 0;

BOOL g_KeyW = FALSE;
BOOL g_KeyS = FALSE;
BOOL g_KeyA = FALSE;
BOOL g_KeyD = FALSE;

int g_Shoot = SHOOT_NONE;

void RenderScene(void)
{
	//Calc Elapsed Time
	if (g_PrevTime == 0)
	{
		g_PrevTime = GetTickCount();
		return;
	}

	DWORD currTime = GetTickCount();
	DWORD elapsedTime = currTime - g_PrevTime;
	g_PrevTime = currTime;
	float eTime = (float)(elapsedTime / 1000.0f);

	//std::cout << "elapsedTime : " << eTime << std::endl;
	//std::cout << "w: " << g_KeyW << ", A: " << g_KeyA << ", S: "
	// << g_KeyS << ", D: " << g_KeyD << std::endl;


	float forceX = 0.f;
	float forceY = 0.f;

	if (g_KeyW)
	{
		forceY += 1.f;
	}
	if (g_KeyS)
	{
		forceY -= 1.f;
	}
	if (g_KeyA)
	{
		forceX -= 1.f;
	}
	if (g_KeyD)
	{
		forceX += 1.f;
	}

	g_ScnMgr->ApplyForce(forceX, forceY,eTime);
	
	//update
	g_ScnMgr->Update(eTime);

	//renderer
	g_ScnMgr->RenderScene();
	g_ScnMgr->Shoot(g_Shoot);
	g_ScnMgr->GarbageCollector();
	g_ScnMgr->DoCollisionTest();
	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	RenderScene();
}

void KeyUpInput(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		g_KeyW = FALSE;
	}

	else if (key == 's')
	{
		g_KeyS = FALSE;
	}

	else if (key == 'a')
	{
		g_KeyA = FALSE;
	}

	else if (key == 'd')
	{
		g_KeyD = FALSE;
	}
}

void KeyDownInput(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		g_KeyW = TRUE;
	}

	else if (key == 's')
	{
		g_KeyS = TRUE;
	}

	else if (key == 'a')
	{
		g_KeyA = TRUE;
	}
	
	else if (key == 'd')
	{
		g_KeyD = TRUE;
	}
}

void SpecialKeyUpInput(int key, int x, int y)
{
	g_Shoot = SHOOT_NONE;
}

void SpecialKeyDownInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_Shoot = SHOOT_UP;
		break;
	case GLUT_KEY_DOWN:
		g_Shoot = SHOOT_DOWN;
		break;
	case GLUT_KEY_LEFT:
		g_Shoot = SHOOT_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		g_Shoot = SHOOT_RIGHT;
		break;
	default:
		break;
	}
}


int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Init g_PrevTime
	g_PrevTime = timeGetTime();

	// Initialize Renderer
	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutMouseFunc(MouseInput);
	//glutSpecialFunc(SpecialKeyInput);
	glutSpecialFunc(SpecialKeyDownInput);
	glutSpecialUpFunc(SpecialKeyUpInput);

	//glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	
	
	// Init Scrmgr ( DO ONCE! )
	g_ScnMgr = new ScnMgr();

	glutMainLoop();

	delete g_ScnMgr;

    return 0;
}

