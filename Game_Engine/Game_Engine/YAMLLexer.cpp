#include "pch.h"
#include "YAMLLexer.h"

// # 추가할 때마다 NUMBER_OF_RESERVEDWORD 갯수 바꾸기
const char* YAMLToken[13] =
{
   "GameObject:",			// 게임 오브젝트
   "m_Name:",				// 게임 오브젝트 이름
   
   "Transform:",			// 그 게임 오브젝트의 트랜스폼
   "m_LocalRotation:",		// rotation
   "m_LocalPosition:",		// position
   "m_LocalScale:",			// scale
   
   "BoxCollider:",			// 박스 콜라이더
   
   "SphereCollider:",		// 구 콜라이더
   
   "Camera:",				// 카메라 오브젝트
   
   "PrefabInstance:",		// 프리팹
   "m_LocalRotation.x",		// rotation
   "m_LocalPosition:.x",	// position
   "m_LocalScale:",			// scale
};

int YAMLLexer::GetToken(char* line)
{
	// 앞쪽 쓸데없는 문자찾기 while로 쭉 ~ 다음 문자로 넘어간다.
	while (line[ch_Index] == ' ' || line[ch_Index] == '\t' || line[ch_Index] == '\n' || line[ch_Index] == '{' || line[ch_Index] == '}')
	{
		ch_Index++;
	}

	// 숫자인지를 확인한다.
	if (line[ch_Index] >= '0' && line[ch_Index] <= '9' || line[ch_Index] == '-')
	{
		word = "";

		while ((line[ch_Index] >= '0' && line[ch_Index] <= '9') || line[ch_Index] == '-' || line[ch_Index] == '.')
		{
			word += line[ch_Index];
			ch_Index++;
		}

		return	TOKEND_NOTDEFINED;
	}
	else
	{
		char tokenStr[80] = { 0 };
		int tokenIndex = 0;

		while (line[ch_Index] != ' ' && line[ch_Index] != '\t' && line[ch_Index] != '"' && line[ch_Index] != '{' && line[ch_Index] != '}')
		{
			tokenStr[tokenIndex++] += line[ch_Index];
			ch_Index++;
		}

		for (int i = 0; i < NUMBER_OF_RESERVEDWORD; i++)
		{
			if (!strcmp(tokenStr, YAMLToken[i]))
			{
				// 맞는 Token을 찾았다면 그 번호를 return시켜준다.
				return	i;
			}
		}

		return	TOKEND_NOTDEFINED;
	}
}