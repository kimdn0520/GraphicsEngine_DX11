#include "pch.h"
#include "Serializer/BinarySerializer.h"
#include "ParserData/ParserData.h"
#include "../FBXBinaryLayout.h"

void BinarySerializer::SaveBinaryFile(std::shared_ptr<FBXModel> fbxModel, std::string name, std::string path)
{
	std::ofstream output(name + ".noob", std::ios::binary);

	boost::archive::text_oarchive oa(output);

	// TODO : ModelData로 바꾸는 과정.. 경로는 어케 설정하는거지?

	std::shared_ptr<FBXBinaryData::ModelData> modelData;

	oa << modelData;
}
