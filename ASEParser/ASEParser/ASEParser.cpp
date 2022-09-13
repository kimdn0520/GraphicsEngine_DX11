#include "pch.h"
#include "Lexer.h"
#include "ASEToken.h"
#include "ASEParser.h"

ASEParser::ASEParser()
	: _lexer(new Lexer())
{
	_parsingMeshInfo->meshName = "";
	_parsingMeshInfo->parentName = "";
}

ASEParser::~ASEParser()
{}

ASEData::ASEModel* ASEParser::Load(string filePath)
{
	FILE* file;
	fopen_s(&file, filePath.c_str(), "r");

	int nowToken = 0;

	_ASEModel = new ASEData::ASEModel();

	// feof(FILE* stream) // 파일의 끝에 도달하게되면 0이 아닌 값을 반환
	while (!feof(file))
	{
		fgets(_lineArray, 256, file);

		_lexer->ResetChIndex();

		nowToken = _lexer->GetToken(_lineArray);

		switch (nowToken)
		{
		case TOKEND_BLOCK_START:
			break;
		case TOKEND_BLOCK_END:
			break;

		case TOKENR_HELPER_CLASS:
			break;

		case TOKENR_3DSMAX_ASCIIEXPORT:
		{
			_asciiexport = Parsing_Int();
		}
		break;

		case TOKENR_COMMENT:
			break;

			//--------------------------------------------------
			// SCENE(FrameData)
			//--------------------------------------------------
		case TOKENR_SCENE:
			break;
		case TOKENR_SCENE_FILENAME:
		{
			ASEData::ASEFrameData* frameData = new ASEData::ASEFrameData();
			_ASEModel->frameData = frameData;
			_ASEModel->frameData->fileName = Parsing_String();
		}
		break;
		case TOKENR_SCENE_FIRSTFRAME:
		{
			_ASEModel->frameData->firstFrame = Parsing_Int();
		}
		break;
		case TOKENR_SCENE_LASTFRAME:
		{
			_ASEModel->frameData->lastFrame = Parsing_Int();
		}
		break;
		case TOKENR_SCENE_FRAMESPEED:
		{
			_ASEModel->frameData->frameSpeed = Parsing_Int();
		}
		break;
		case TOKENR_SCENE_TICKSPERFRAME:
		{
			_ASEModel->frameData->ticksPerFrame = Parsing_Int();
		}
		break;
		case TOKENR_SCENE_MESHFRAMESTEP:
			break;
		case TOKENR_SCENE_KEYFRAMESTEP:
			break;
		case TOKENR_SCENE_BACKGROUND_STATIC:
			break;
		case TOKENR_SCENE_AMBIENT_STATIC:
			break;
		case TOKENR_SCENE_ENVMAP:
			break;

		//---------------------------------------------------------
		// MATERIAL_LIST
		// 일단! SubMaterial 생각은 하지 않고 만들어보자..
		//---------------------------------------------------------
		case TOKENR_MATERIAL_COUNT:
		{
			int materialCnt = Parsing_Int();

			if (materialCnt == 0)
			{
				_ASEModel->materials.resize(1);
				ASEData::ASEMaterialInfo* newMaterial = new ASEData::ASEMaterialInfo();
				_ASEModel->materials[_materialIndex] = newMaterial;
			}
			else
			{
				_ASEModel->materials.resize(materialCnt);
			}
		}
		break;
		case TOKENR_MATERIAL:
		{
			_materialIndex = Parsing_Int();

			ASEData::ASEMaterialInfo* newMaterial = new ASEData::ASEMaterialInfo();
			_ASEModel->materials[_materialIndex] = newMaterial;
		}
		break;
		case TOKENR_MATERIAL_AMBIENT:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();

			_ASEModel->materials[_materialIndex]->ambient = Vector4(x, y, z, 1.0f);
		}
		break;
		case TOKENR_MATERIAL_DIFFUSE:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();

			_ASEModel->materials[_materialIndex]->diffuse = Vector4(x, y, z, 1.0f);
		}
		break;
		case TOKENR_MATERIAL_SPECULAR:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();

			_ASEModel->materials[_materialIndex]->specular = Vector4(x, y, z, 1.0f);
		}
		break;
		case TOKENR_MAP_DIFFUSE:
		{
			curTexture = 0;
			_ASEModel->materials[_materialIndex]->isDiffuse = true;
		}
		break;
		case TOKENR_MAP_BUMP:
		{
			curTexture = 1;
			_ASEModel->materials[_materialIndex]->isNormal = true;
		}
		break;
		case TOKENR_MAP_SPECULAR:
		{
			curTexture = 2;
			_ASEModel->materials[_materialIndex]->isSpecular = true;
		}
		break;
		case TOKENR_BITMAP:
		{
			// 이름만 가져오쟈! 메모장에서 바꺼
			string str = Parsing_String();
			wstring wstr(str.begin(), str.end());

			if (curTexture == 0)
			{
				_ASEModel->materials[_materialIndex]->diffuseTexName = wstr;
			}
			else if (curTexture == 1)
			{
				_ASEModel->materials[_materialIndex]->normalTexName = wstr;
			}
			else if (curTexture == 2)
			{
				_ASEModel->materials[_materialIndex]->specularTexName = wstr;
			}
		}
		break;

		//-----------------------------------------
		// GEOMOBJECT, HELPEROBJECT, SHAPEOBJECT
		//-----------------------------------------
		case TOKENR_GROUP:
			break;

			// vertices는 없고 Control(animation)만 있는 녀석
		case TOKENR_HELPEROBJECT:
		{
			PushMesh();

			ASEData::ASEMeshInfo* aseMeshInfo = new ASEData::ASEMeshInfo();
			_ASEModel->meshInfos.push_back(aseMeshInfo);
			_parsingMeshInfo = aseMeshInfo;
		}
		break;
		case TOKENR_GEOMOBJECT:
		{
			PushMesh();

			ASEData::ASEMeshInfo* aseMeshInfo = new ASEData::ASEMeshInfo();
			_ASEModel->meshInfos.push_back(aseMeshInfo);
			_parsingMeshInfo = aseMeshInfo;
		}
		break;
		case TOKENR_SHAPEOBJECT:		// 쓰지말쟈~
			break;

		case TOKENR_NODE_NAME:
		{
			_parsingMeshInfo->meshName = Parsing_String();
		}
		break;

		case TOKENR_NODE_PARENT:
			// 현 노드의 부모 노드의 정보.
			// 새로운 게임오브젝트를 만들어서 해당 부모노드의 이름을 가진 게임 오브젝트를 찾아서
			// 그 부모의 자식으로 넣어줄 것..?
			_parsingMeshInfo->parentName = Parsing_String();
			break;

			//-----------------------------------------
			// NODE_TM
			//-----------------------------------------
		case TOKENR_NODE_TM:
			break;

		case TOKENR_INHERIT_POS:
			break;
		case TOKENR_INHERIT_ROT:
			break;
		case TOKENR_INHERIT_SCL:
			break;
		case TOKENR_TM_ROW0:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();
			_row0 = { x, z, y };
		}
		break;
		case TOKENR_TM_ROW1:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();
			_row1 = { x, z, y };
		}
		break;
		case TOKENR_TM_ROW2:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();
			_row2 = { x, z, y };
		}
		break;
		case TOKENR_TM_ROW3:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();
			_row3 = { x, z, y };

			// ROW3 에서 월드행렬을 만들어준당
			_parsingMeshInfo->nodeTM =
			{ _row0.x, _row0.y, _row0.z, 0,
			  _row2.x, _row2.y, _row2.z, 0,
			  _row1.x, _row1.y, _row1.z, 0,
			  _row3.x, _row3.y, _row3.z, 1.0f
			};

			// 여기서 네거티브 스케일 작업해야함. 
			// 반환 값으로 (detA, detA, detA, detA) 이렇게 나온다고 한다. 다똑같음
			XMVECTOR det = XMMatrixDeterminant(_parsingMeshInfo->nodeTM);

			// 음수면 네거리브~
			if (det.m128_f32[0] < 0)
			{
				// Decompose 했다가 scale -주고 다시 합쳐야함..
				XMVECTOR scale;
				XMVECTOR rotQuat;
				XMVECTOR trans;
				XMMatrixDecompose(&scale, &rotQuat, &trans, _parsingMeshInfo->nodeTM);
				scale = -1 * scale;

				// 다시 SRT 조립
				_parsingMeshInfo->nodeTM = XMMatrixScaling(scale.m128_f32[0], scale.m128_f32[1], scale.m128_f32[2]) *
					XMMatrixRotationQuaternion(rotQuat) *
					XMMatrixTranslation(trans.m128_f32[0], trans.m128_f32[1], trans.m128_f32[2]);

				_isNegativeScale = true;
			}
		}
		break;
		case TOKENR_TM_POS:
			break;
		case TOKENR_TM_ROTAXIS:
			break;
		case TOKENR_TM_ROTANGLE:
			break;
		case TOKENR_TM_SCALE:
			break;
		case TOKENR_TM_SCALEAXIS:
			break;
		case TOKENR_TM_SCALEAXISANG:
			// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
			break;

			//-----------------------------------------
			// Animation Data
			//-----------------------------------------
		case TOKENR_TM_ANIMATION:
		{
			_ASEModel->isAnimation = true;		// 애니메이션을 갖고있는 오브젝트다..

			ASEData::ASEAnimation* animationClip = new ASEData::ASEAnimation();
			_parsingMeshInfo->animationClips.push_back(animationClip);
			_parsingAnimation = animationClip;

			_parsingAnimation->nodename = _parsingMeshInfo->meshName;
		}
		break;
		case TOKENR_ANIMATION_NAME:
		{
			_parsingAnimation->animName = Parsing_String();
		}
		break;
		case TOKENR_CONTROL_POS_SAMPLE:
		{
			// 로컬 좌표 랍니다.
			ASEData::Animation_pos* aniPos = new ASEData::Animation_pos();
			aniPos->tick = Parsing_Int();
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();
			aniPos->pos = { x, z, y, 1.0f };
			_parsingAnimation->position.push_back(aniPos);
		}
		break;
		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			// 로컬 이랍니다.
			ASEData::Animation_rot* aniRot = new ASEData::Animation_rot();
			aniRot->tick = Parsing_Int();
			float x = Parsing_Float();
			float z = Parsing_Float();
			float y = Parsing_Float();
			// 라디안 단위의 회전 각도를 받는다.
			// 각도는 회전 축을 따라 원점을 향하여 볼 때 시계 방향으로 측정된다.
			float angle = Parsing_Float();

			XMVECTOR axis = { x, y, z, 0.f };						// axis를 받는다.
			XMVECTOR quat = XMQuaternionRotationAxis(axis, angle);	// axis에 대한 회전 쿼터니언을 계산한다.

			// 쿼터니언 각도가 누적이기때문에.. 성현이의 exporter는 누적해서 나온대
			/*if (!_parsingAnimation->rotation.empty())
			{
				quat = XMQuaternionMultiply(_parsingAnimation->rotation.back()->rotQT, quat);
			}*/

			aniRot->rotQT = quat;
			_parsingAnimation->rotation.push_back(aniRot);
		}
		break;
		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			// ? 무슨 값을..???
		}
		break;

		//---------------------------------------------------------------------------------------------------------
		//			M        E       S         H
		//---------------------------------------------------------------------------------------------------------
		case TOKENR_MESH:
			break;
		case TOKENR_TIMEVALUE:
			break;
		case TOKENR_MESH_NUMBONE:
			break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			break;
		case TOKENR_MESH_NUMVERTEX:
		{
			_numVertex = Parsing_Int();
			_tmpVertices.resize(Parsing_Int(), Vector3(0, 0, 0));
		}
		break;
		case TOKENR_MESH_NUMFACES:
		{
			// 임시 Index 사이즈랑, 임시 Normal 사이즈, Vertices 사이즈를 정해준다.
			int size = Parsing_Int();
			_tmpIndices.resize(size * 3, 0);
			_tmpNormal.resize(size * 3, Vector3{ 1, 1, 1 });
		}
		break;

		///// MESH_VERTEX_LIST

		case TOKENR_MESH_VERTEX_LIST:
			break;
		case TOKENR_MESH_VERTEX:
		{
			// Max -> DirectX  는 y랑 z값만 바꿔주면 된다.
			int index = Parsing_Int();
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();

			// 하... 제대로 안넣어줘서 박스 잘못그리고있었다.. 2022-05-12
			/*_ASEModel->minPos.x = min(_ASEModel->minPos.x, x);
			_ASEModel->minPos.z = min(_ASEModel->minPos.z, y);
			_ASEModel->minPos.y = min(_ASEModel->minPos.y, z);
			_ASEModel->maxPos.x = max(_ASEModel->maxPos.x, x);
			_ASEModel->maxPos.z = max(_ASEModel->maxPos.z, y);
			_ASEModel->maxPos.y = max(_ASEModel->maxPos.y, z);*/

			// 각 Vertex들에 NODE_TM에서 얻어온 월드 행렬, 즉 자신의 월드행렬의
			// 역행렬을 곱해주어 Vertex들이 로컬 좌표를 가지고 있게 한다.
			XMMATRIX worldTMInverse = XMMatrixInverse(nullptr, _parsingMeshInfo->nodeTM);
			XMVECTOR vertexVec = { x, z, y };
			XMVECTOR vertexXworldTM = XMVector3Transform(vertexVec, worldTMInverse);
			XMFLOAT3 vertex;
			XMStoreFloat3(&vertex, vertexXworldTM);

			_tmpVertices[index].pos.x = vertex.x;
			_tmpVertices[index].pos.y = vertex.y;
			_tmpVertices[index].pos.z = vertex.z;
		}
		break;

		//---------------------------------------------------------------------------------------------------------
		//			B        O       N         E
		//---------------------------------------------------------------------------------------------------------
		case TOKENR_SKIN_INITTM:
			break;
		case TOKENR_BONE_LIST:
			break;
		case TOKENR_BONE:
		{
			// _ASEModel->boneInfos.resize(Parsing_Int());
		}
		break;
		case TOKENR_BONE_NAME:
		{
			ASEData::ASEBoneInfo* aseBoneInfo = new ASEData::ASEBoneInfo();
			_parsingMeshInfo->boneInfos.push_back(aseBoneInfo);
			_parsingMeshInfo->boneInfos[_parsingMeshInfo->boneInfos.size() - 1]->boneName = Parsing_String();
		}
		break;
		case TOKENR_BONE_PROPERTY:
			break;
		case TOKENR_MESH_WVERTEXS:
			break;
		case TOKENR_MESH_WEIGHT:
		{
			_weightNum = Parsing_Int();
			_weightCount = 0;
		}
		break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			_tmpVertices[_weightNum].boneIndices[_weightCount] = Parsing_Int();
			_tmpVertices[_weightNum].weights[_weightCount] = Parsing_Float();
			_weightCount++;
		}
		break;


		/// MESH_FACE_LIST
		case TOKENR_MESH_FACE_LIST:
			break;
		case TOKENR_MESH_FACE:
		{
			// 아마 정보가 A: ( 면의 첫 번째 점 ) B: ( 면의 세 번째 점 ) C: ( 면의 두 번째 점 ) 이런식이다.
			// blender는 그냥 넣엇는뎅.. max는 ACB ?
			int faceIndex = Parsing_Int() * 3;
			Parsing_Int();						// A: B: C: 이런녀석들때문에..
			int first = Parsing_Int();
			Parsing_Int();
			int second = Parsing_Int();
			Parsing_Int();
			int third = Parsing_Int();

			_tmpIndices[faceIndex] = first;
			_tmpIndices[faceIndex + 1] = third;
			_tmpIndices[faceIndex + 2] = second;
		}
		break;

		case TOKENR_MESH_NUMTVERTEX:
		{
			// 매핑채널 조심..?
			_tmpTVert.resize(Parsing_Int(), { Vector2(0, 0) });
		}
		break;
		case TOKENR_MESH_TVERTLIST:
			break;
		case TOKENR_MESH_TVERT:
		{
			// 새로운 TVertex를 만들어서 벡터에 넣는다, 값이 3개 있는데 끝에거는 버린다.
			int index = Parsing_Int();
			float x = Parsing_Float();
			float y = Parsing_Float();

			_tmpTVert[index].x = x;
			_tmpTVert[index].y = 1.0f - y;
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
		{
			_tmpTFaceIndices.resize(Parsing_Int() * 3);
		}
		break;
		case TOKENR_MESH_TFACE:
		{
			int tFaceIndex = Parsing_Int() * 3;
			int first = Parsing_Int();
			int second = Parsing_Int();
			int third = Parsing_Int();

			_tmpTFaceIndices[tFaceIndex] = first;
			_tmpTFaceIndices[tFaceIndex + 1] = third;
			_tmpTFaceIndices[tFaceIndex + 2] = second;
		}
		break;

		case TOKENR_MESH_FACENORMAL:
			break;
		case TOKENR_MESH_VERTEXNORMAL:
		{
			// 근데 VertexNoraml 값이 맨 끝에 있어가지고... 근데 FACE에서 나누게되면
			// 그 새로운 버텍스의 노말값을 주기가 어려워서.. 근데..
			// 그냥 그래서 tmp에 모아두고 optimize에서 다시 설정할거임
			Parsing_Int();							// 3367 0 7834 무시

			int index = _normalIdx++;
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();

			_tmpNormal[index] = { x, z, y };

			if (_normalIdx % 3 == 0)
				swap(_tmpNormal[_normalIdx - 1], _tmpNormal[_normalIdx - 2]);
		}
		break;

		//---------------------------------------------------------------------------------------------------------
		// OBJECT 의 끝이다. 
		// 게임오브젝트를 만들어 넣어주고 기존 변수들을 초기화 해준다.
		// 여러개의 재질이 사용되는 경우도 판단해서 넣어준다
		//---------------------------------------------------------------------------------------------------------
		case TOKENR_MATERIAL_REF:
		{
			_materialNum = Parsing_Int();		// 어떤 매터리얼을 가져올지..
		}
		break;

		// 딱히 안씀
		case TOKEND_END:
			break;

		default:
			break;
		}
	}

	PushMesh();

	return _ASEModel;
}


void ASEParser::Optimize()
{
	_indices.resize(_tmpIndices.size());

	// FACE 값들을 순회하면서 tmp버텍스들을 본체에 넣어주잖아? 근데 이제
	// tmpIndices에서 똑같은 값이 나왔을때! if분기하고 근데 그게 uv, normal이 전에 값과
	// 다르면 넣어주고(쪼갠다) 같으면 안 넣어준다.
	// [결론 : vertex 갯수를 tvert 갯수랑 맞춰주면 된다. 아니다 더 늘어날 수있다. 꼭 같은건 아니였다. 더 많아지면 늘리면됨 ㅇㅇ]
	// 해당 vertex에다가 VertexInfo(uv, normal, index)을 갖고있게하자. 
	vector<vector<VertexInfo>> tmpList;
	tmpList.resize(_numVertex);				// numvertex 만큼 가지고 있으면 됨 ㅇㅇ 

	_vertices.resize(_tmpTVert.size());

	int cnt = 0;

	for (int i = 0; i < _tmpIndices.size(); i++)
	{
		// 같은 버텍스가 있으니 중복 판단해서 넣어줄지 말지 해준다.
		if (!tmpList[_tmpIndices[i]].empty())
		{
			bool isUV = false;
			bool isNormal = false;

			// 그 같은 인덱스 찾아서 넣을때 어느녀석인지 찾을려구..
			int sameIndex = 0;

			for (int j = 0; j < tmpList[_tmpIndices[i]].size(); j++)
			{
				if (_tmpTVert.size() != 0)
				{
					if (tmpList[_tmpIndices[i]][j].uv == _tmpTVert[_tmpTFaceIndices[i]])
						isUV = true;
				}
				else
				{
					isUV = true;
				}

				if (tmpList[_tmpIndices[i]][j].normal == _tmpNormal[i])
					isNormal = true;

				if (isUV && isNormal)
				{
					sameIndex = j;	// 요녀석이구나..
					break;
				}

				isUV = false; isNormal = false;
			}

			// 같으면 continue로 넣지 않는다.
			if (isUV && isNormal)
			{
				// 같은 인덱스 넣어주기!! 중요!
				_indices[i] = tmpList[_tmpIndices[i]][sameIndex].index;		// 요녀석의 index 박아주기~
				continue;
			}
		}

		_indices[i] = cnt;

		// 쪼개기를 위한 정보 넣어주자.
		VertexInfo vertexInfo;

		if (_tmpTVert.size() != 0 && cnt < _tmpTVert.size())
			_vertices[cnt].pos = _tmpVertices[_tmpIndices[i]].pos;
		else
			_vertices.push_back(_tmpVertices[_tmpIndices[i]]);

		// TFace에 해당하는 인덱스의 TVert를 가져와서 넣어준다. 어떤 TVert를 가져올건지..
		if (_tmpTVert.size() != 0)
		{
			_vertices[cnt].uv = _tmpTVert[_tmpTFaceIndices[i]];
			vertexInfo.uv = _tmpTVert[_tmpTFaceIndices[i]];
		}

		// 네거티브 스케일인 오브젝트 였다면..! 노말도 -1 곱해줘야한다..!
		if (_isNegativeScale)
			_tmpNormal[i] = _tmpNormal[i] * -1.f;

		_vertices[cnt].normal = _tmpNormal[i];
		vertexInfo.normal = _tmpNormal[i];
		vertexInfo.index = cnt;
		tmpList[_tmpIndices[i]].push_back(vertexInfo);


		for (int j = 0; j < 8; j++)
		{
			_vertices[cnt].boneIndices[j] = _tmpVertices[_tmpIndices[i]].boneIndices[j];
			_vertices[cnt].weights[j] = _tmpVertices[_tmpIndices[i]].weights[j];
		}

		cnt++;
	}

	// NormalMap을 위한 tangent 계산
	// 노말맵텍스쳐를 reverse 하기에는 값이 이상해져버렷? 그럼 1-v를 해줘야하나? 근데 안되는것같은디 ㅠ
	// 일단 리버스노말맵을 쓰니까 나오긴하는디 셰이더로 빛 잘 받는지 확인해야함?
	for (int i = 0; i < _indices.size(); i += 3) // 삼각형의 개수
	{
		int i0 = _indices[i];
		int i1 = _indices[i + 1];
		int i2 = _indices[i + 2];

		// e1 = p1 - p0, e2 = p2 - p0
		Vector3 e1 = _vertices[i1].pos - _vertices[i0].pos;
		Vector3 e2 = _vertices[i2].pos - _vertices[i0].pos;

		float x1 = _vertices[i1].uv.x - _vertices[i0].uv.x; // u1 - u0
		float y1 = _vertices[i1].uv.y - _vertices[i0].uv.y; // v1 - v0
		float x2 = _vertices[i2].uv.y - _vertices[i0].uv.x; // u2 - u0
		float y2 = _vertices[i2].uv.y - _vertices[i0].uv.y; // v2 - v0

		float det = (x1 * y2) - (x2 * y1); // ad - bc
		if (det == 0.f) { det = 0.00001f; }
		float r = 1.0f / det;

		Vector3 tangent = (e1 * y2 - e2 * y1) * r;

		_vertices[i0].tangent += tangent;
		_vertices[i1].tangent += tangent;
		_vertices[i2].tangent += tangent;
	}
}

void ASEParser::ResetData()
{
	_vertices.clear();
	_indices.clear();
	_tmpVertices.clear();
	_tmpIndices.clear();
	_tmpTVert.clear();
	_tmpTFaceIndices.clear();
	_tmpNormal.clear();
	vector<ASEData::Vertex>().swap(_vertices);
	vector<unsigned int>().swap(_indices);
	vector<ASEData::Vertex>().swap(_tmpVertices);
	vector<unsigned int>().swap(_tmpIndices);
	vector<Vector2>().swap(_tmpTVert);
	vector<unsigned int>().swap(_tmpTFaceIndices);
	vector<Vector3>().swap(_tmpNormal);

	_materialIndex = 0;
	_materialNum = 0;
	curTexture = 0;
	_numVertex = 0;
	_normalIdx = 0;
	_weightNum = 0;
	_weightCount = 0;
	_normalIdx = 0;

	_isNegativeScale = false;

	_parsingMeshInfo->meshName = "";
	_parsingMeshInfo->parentName = "";
}

void ASEParser::PushMesh()
{
	if (_parsingMeshInfo != nullptr)
	{
		Optimize();
		_parsingMeshInfo->vertices = _vertices;
		_parsingMeshInfo->indices = _indices;
		ResetData();
	}
}


string ASEParser::Parsing_String()
{
	_lexer->GetToken(_lineArray);

	return _lexer->GetWord();
}

int ASEParser::Parsing_Int()
{
	_lexer->GetToken(_lineArray);
	int tmpInt = stoi(_lexer->GetWord());

	return tmpInt;
}

float ASEParser::Parsing_Float()
{
	_lexer->GetToken(_lineArray);
	float tmpFloat = stof(_lexer->GetWord());

	return tmpFloat;
}

void ASEParser::DecomposeMatrix(XMMATRIX worldTM, XMFLOAT3& scale, XMFLOAT3& rotation, XMFLOAT3& trans)
{
	XMVECTOR localScaleVec;
	XMVECTOR localRotQuatVec;
	XMVECTOR localTransVec;
	XMVECTOR localRotationVec;
	XMMatrixDecompose(&localScaleVec, &localRotQuatVec, &localTransVec, worldTM);

	// 쿼터니언을 오일러각으로 바꾸는 과정..
	double x = localRotQuatVec.m128_f32[0];
	double y = localRotQuatVec.m128_f32[1];
	double z = localRotQuatVec.m128_f32[2];
	double w = localRotQuatVec.m128_f32[3];
	double sqx = x * x;
	double sqy = y * y;
	double sqz = z * z;
	double sqw = w * w;
	float eulerX = asin(2.0f * (w * x - y * z));
	float eulerY = atan2(2.0f * (x * z + w * y), (-sqx - sqy + sqz + sqw));
	float eulerZ = atan2(2.0f * (x * y + z * w), (-sqx + sqy - sqz + sqw));
	rotation = { eulerX, eulerY, eulerZ };

	XMStoreFloat3(&scale, localScaleVec);
	XMStoreFloat3(&trans, localTransVec);
	rotation = { XMConvertToDegrees(rotation.x),
					  XMConvertToDegrees(rotation.y),
					  XMConvertToDegrees(rotation.z) };
}


