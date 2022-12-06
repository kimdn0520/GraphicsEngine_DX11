#include "pch.h"
#include "YAMLLexer.h"

const char* YAMLToken[10] =
{
   "GameObject:",	// 게임 오브젝트
   "m_Name:",		// 게임 오브젝트 이름
   "Transform:",	// 그 게임 오브젝트의 트랜스폼
};

int YAMLLexer::GetToken(char* line)
{
	// 앞쪽 쓸데없는 문자찾기
	while (line[ch_Index] == ' ' || line[ch_Index] == '\t' || line[ch_Index] == '\n')
	{
		ch_Index++;
	}

	// 2) '{'인가를 확인한다.
	if (line[ch_Index] == '{')
	{
		ch_Index++;
	}
	// 3) '}'인가를 확인한다.
	else if (line[ch_Index] == '}')
	{
		ch_Index++;
	}
	// 4) 'm'인가를 확인한다.
	//    만약 '*'로 시작하는 것은 Reserved word이므로 검색한다!
	else if (line[ch_Index] == '*')
	{
		char tokenStr[80] = { 0 };
		int tokenIndex = 0;

		while (line[ch_Index] != ' ' && line[ch_Index] != '\t' && line[ch_Index] != '"' && line[ch_Index] != '{' && line[_ch_Index] != '}')
		{
			tokenStr[tokenIndex++] += line[_ch_Index];
			_ch_Index++;
		}

		for (int i = 0; i < NUMBER_OF_RESERVEDWORD; i++)
		{
			if (!strcmp(tokenStr, YAMLToken[i]))
			{
				// 맞는 Token을 찾았다! 그 번호를 return시켜준다.
				return	i;
			}
		}

		return	TOKEND_NOTDEFINED;
	}
	// 숫자인지를 확인한다.
	// 만약에 '0'에서 '9'사이이거나 '-'부하고 붙어 있으면 숫자라고 판단을 한다.
	else if (line[ch_Index] >= '0' && line[ch_Index] <= '9' || line[ch_Index] == '-')
	{
		// 속도 한번 볼것..
		word = "";

		while ((line[ch_Index] >= '0' && line[ch_Index] <= '9') || line[ch_Index] == '-' || line[ch_Index] == '.')
		{
			word += line[ch_Index];
			ch_Index++;
		}

		// MESH_FACE 0: 1: 이런경우..?
		if (line[ch_Index] == ':')
			ch_Index++;

		return	TOKEND_NOTDEFINED;
	}
	else
	{
		while (line[ch_Index] != ' ' && line[ch_Index] != '\t')
		{
			ch_Index++;
		}

		return TOKEND_NOTDEFINED;
	}
}