#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#pragma once
struct obj
{
	FILE* objFile;
	glm::vec3* vertex = NULL;
	int* face_v = NULL;
	int* face_vt = NULL;
	int* face_vn = NULL;

	int vertexNum = 0;
	int faceNum = 0;

	void ReadObj();
	obj(const char* s)
	{
		objFile = fopen(s, "rb");
	}
	obj()
	{
		objFile = NULL;
	}
	void OpenFile(const char* s)
	{
		objFile = fopen(s, "rb");
	}
};