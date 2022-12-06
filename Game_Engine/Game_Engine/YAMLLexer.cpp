#include "pch.h"
#include "YAMLLexer.h"

const char* YAMLToken[10] =
{
   "GameObject:",	// ���� ������Ʈ
   "m_Name:",		// ���� ������Ʈ �̸�
   "Transform:",	// �� ���� ������Ʈ�� Ʈ������
};

int YAMLLexer::GetToken(char* line)
{
	// ���� �������� ����ã��
	while (line[ch_Index] == ' ' || line[ch_Index] == '\t' || line[ch_Index] == '\n')
	{
		ch_Index++;
	}

	// 2) '{'�ΰ��� Ȯ���Ѵ�.
	if (line[ch_Index] == '{')
	{
		ch_Index++;
	}
	// 3) '}'�ΰ��� Ȯ���Ѵ�.
	else if (line[ch_Index] == '}')
	{
		ch_Index++;
	}
	// 4) 'm'�ΰ��� Ȯ���Ѵ�.
	//    ���� '*'�� �����ϴ� ���� Reserved word�̹Ƿ� �˻��Ѵ�!
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
				// �´� Token�� ã�Ҵ�! �� ��ȣ�� return�����ش�.
				return	i;
			}
		}

		return	TOKEND_NOTDEFINED;
	}
	// ���������� Ȯ���Ѵ�.
	// ���࿡ '0'���� '9'�����̰ų� '-'���ϰ� �پ� ������ ���ڶ�� �Ǵ��� �Ѵ�.
	else if (line[ch_Index] >= '0' && line[ch_Index] <= '9' || line[ch_Index] == '-')
	{
		// �ӵ� �ѹ� ����..
		word = "";

		while ((line[ch_Index] >= '0' && line[ch_Index] <= '9') || line[ch_Index] == '-' || line[ch_Index] == '.')
		{
			word += line[ch_Index];
			ch_Index++;
		}

		// MESH_FACE 0: 1: �̷����..?
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