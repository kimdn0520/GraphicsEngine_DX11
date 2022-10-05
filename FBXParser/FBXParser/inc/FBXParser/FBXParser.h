#pragma once
#include "ParserBase/ParserBase.h"

/// <summary>
/// ���߿��� �Ľ̸��� �������� �ϴ� ���ø����̼����� �ϳ� �� ���� �� ��
/// �ϴ� �̰��� FBXLoad �׽�Ʈ�� dll, binarySerializer�� �׽�Ʈ�� ���� ����
/// �ؾ��� ��
/// 1. FBXLoad Static Mesh �� ����� �Ľ��� �Ǵ��� (���ؽ� �ɰ���, �ߺ� ���� �� ��)
/// 2. FBXLoad Skinned Mesh�� ����� �Ľ��� �Ǵ��� (Animation�� ����� �Ǵ��� Ȯ�� �� ��)
/// 3. BinarySerialization ���� ���� �� �ҷ��ͼ� ����� �Ľ��� �Ǵ��� Ȯ��
/// 4. FBXLoad -> BinarySerialzation ��Ű�� ���ø����̼� ������ ��
///	   [������ ��ο��� FBX�� �����´� -> Binaryȭ �ؼ� bin ������ �����Ѵ�]
/// </summary>
class FBXParser : public ParserBase
{
public:
	FBXParser();
	~FBXParser();

public:
	std::shared_ptr<FbxModel> LoadFbx(const std::wstring& path) override;

	void Release() override;
};

