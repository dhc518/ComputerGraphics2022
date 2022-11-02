#include "objReader.h"

void obj::ReadObj()
{
	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
	char count[100];

	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count)); // 배열 초기화
	}
	//--- 2. 메모리 할당

	int vertIndex = 0;
	int faceIndex = 0;
	if (vertex != NULL) free(vertex);
	if (face_v != NULL) free(face_v);

	vertex = (glm::vec3*)malloc(sizeof(glm::vec3) * vertexNum);
	face_v = (int*)malloc(sizeof(int) * faceNum * 3);
	face_vt = (int*)malloc(sizeof(int) * faceNum * 3);
	face_vn = (int*)malloc(sizeof(int) * faceNum * 3);

	rewind(objFile);

	//--- 3. 할당된 메모리에 각 버텍스, 페이스 정보 입력
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&vertex[vertIndex].x, &vertex[vertIndex].y,
				&vertex[vertIndex].z);
			vertIndex++;
		}
		else if (count[0] == 'f' && count[1] == '\0') {
			fscanf(objFile, "%d//%d %d//%d %d//%d",
				&face_v[faceIndex * 3], &face_vn[faceIndex * 3],
				&face_v[faceIndex * 3 + 1], &face_vn[faceIndex * 3 + 1],
				&face_v[faceIndex * 3 + 2], &face_vn[faceIndex * 3 + 2]);

			face_v[faceIndex * 3]--; face_v[faceIndex * 3 + 1]--; face_v[faceIndex * 3 + 2]--;
			face_vn[faceIndex * 3]--; face_vn[faceIndex * 3 + 1]--; face_vn[faceIndex * 3 + 2]--;
			faceIndex++;
		}
	}
}