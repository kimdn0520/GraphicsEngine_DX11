#pragma once

struct FBXModel;

class BinarySerializer
{
private:
	

public:
	BinarySerializer() = default;
	~BinarySerializer() {};

public:
	void SaveBinaryFile(std::shared_ptr<FBXModel> fbxModel, std::string name, std::string path);
};

