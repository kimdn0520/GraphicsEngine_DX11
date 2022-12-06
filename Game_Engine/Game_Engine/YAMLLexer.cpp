#include "pch.h"
#include "YAMLLexer.h"

// # �߰��� ������ NUMBER_OF_RESERVEDWORD ���� �ٲٱ�
const char* YAMLToken[13] =
{
   "GameObject:",			// ���� ������Ʈ
   "m_Name:",				// ���� ������Ʈ �̸�
   
   "Transform:",			// �� ���� ������Ʈ�� Ʈ������
   "m_LocalRotation:",		// rotation
   "m_LocalPosition:",		// position
   "m_LocalScale:",			// scale
   
   "BoxCollider:",			// �ڽ� �ݶ��̴�
   
   "SphereCollider:",		// �� �ݶ��̴�
   
   "Camera:",				// ī�޶� ������Ʈ
   
   "PrefabInstance:",		// ������
   "m_LocalRotation.x",		// rotation
   "m_LocalPosition:.x",	// position
   "m_LocalScale:",			// scale
};

int YAMLLexer::GetToken(char* line)
{
	// ���� �������� ����ã�� while�� �� ~ ���� ���ڷ� �Ѿ��.
	while (line[ch_Index] == ' ' || line[ch_Index] == '\t' || line[ch_Index] == '\n' || line[ch_Index] == '{' || line[ch_Index] == '}')
	{
		ch_Index++;
	}

	// ���������� Ȯ���Ѵ�.
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
				// �´� Token�� ã�Ҵٸ� �� ��ȣ�� return�����ش�.
				return	i;
			}
		}

		return	TOKEND_NOTDEFINED;
	}
}