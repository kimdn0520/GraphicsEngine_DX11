#include "pch.h"
#include "Serializer/BinarySerializer.h"
#include "ParserData/ParserData.h"

void BinarySerializer::SaveBinaryFile(std::shared_ptr<FBXModel> fbxModel, std::string name, std::string path)
{
	std::ofstream output(name + ".noob", std::ios_base::binary);

	/*typedef std::vector<char> buffer_type;
	buffer_type buffer;
	boost::iostreams::stream<boost::iostreams::back_insert_device<buffer_type>> output(buffer);*/

	boost::archive::binary_oarchive oa(output);				// 연 스트림을 넘겨주어서 직렬화 객체 초기화

	std::vector<FBXBinaryData::MeshData> meshInfoList;

	for (auto& mesh : fbxModel->fbxMeshInfoList)
	{
		std::vector<FBXBinaryData::VertexData> vertexDataList;

		for (auto& vertex : mesh->meshVertexList)
		{
			FBXBinaryData::Float3 position(vertex.position.x, vertex.position.y, vertex.position.z);
			FBXBinaryData::Float4 color(vertex.color.x, vertex.color.y, vertex.color.z, vertex.color.w);
			FBXBinaryData::Float2 uv(vertex.uv.x, vertex.uv.y);
			FBXBinaryData::Float3 normal(vertex.normal.x, vertex.normal.y, vertex.normal.z);
			FBXBinaryData::Float3 tangent(vertex.tangent.x, vertex.tangent.y, vertex.tangent.z);

			FBXBinaryData::VertexData vertexdata(position, color, uv, normal, tangent);

			vertexDataList.emplace_back(vertexdata);
		}

		FBXBinaryData::MeshData meshData(mesh->meshName, mesh->parentName, mesh->materialName, vertexDataList, mesh->indices, ConvertFloat4x4(mesh->nodeTM));

		meshInfoList.emplace_back(meshData);
	}

	std::vector<FBXBinaryData::MaterialData> materialList;

	for (auto& mat : fbxModel->materialList)
	{
		FBXBinaryData::Float4 material_Ambient(mat->material_Ambient.x, mat->material_Ambient.y, mat->material_Ambient.z, mat->material_Ambient.w);
		FBXBinaryData::Float4 material_Diffuse(mat->material_Diffuse.x, mat->material_Diffuse.y, mat->material_Diffuse.z, mat->material_Diffuse.w);
		FBXBinaryData::Float4 material_Specular(mat->material_Specular.x, mat->material_Specular.y, mat->material_Specular.z, mat->material_Specular.w);
		FBXBinaryData::Float4 material_Emissive(mat->material_Emissive.x, mat->material_Emissive.y, mat->material_Emissive.z, mat->material_Emissive.w);

		FBXBinaryData::MaterialData materialData(
			mat->materialName, mat->albedoMap, mat->normalMap, mat->metallicMap, mat->roughnessMap, mat->AOMap, mat->emissiveMap,
			material_Ambient, material_Diffuse, material_Specular, material_Emissive, mat->material_Transparency, mat->material_Reflectivity,
			mat->metallic, mat->roughness);

		materialList.emplace_back(materialData);
	}

	std::vector<FBXBinaryData::BoneData> boneInfoList;

	for (auto& bone : fbxModel->fbxBoneInfoList)
	{
		FBXBinaryData::BoneData boneData(bone->boneName, bone->parentIndex, ConvertFloat4x4(bone->offsetMatrix), ConvertFloat4x4(bone->nodeMatrix));

		boneInfoList.emplace_back(boneData);
	}

	std::vector<FBXBinaryData::AnimationClipData> animationClipList;

	for (auto& anim : fbxModel->animationClipList)
	{
		std::vector<std::vector<FBXBinaryData::KeyFrameInfoData>> keyFrameInfoList;

		for (auto& keyList : anim->keyFrameList)
		{
			std::vector<FBXBinaryData::KeyFrameInfoData> keyFrames;

			for (auto& key : keyList)
			{
				FBXBinaryData::Float3 localTransform(key->localTransform.x, key->localTransform.y, key->localTransform.z);
				FBXBinaryData::Float4 localRotation(key->localRotation.x, key->localRotation.y, key->localRotation.z, key->localRotation.w);
				FBXBinaryData::Float3 localScale(key->localScale.x, key->localScale.y, key->localScale.z);

				FBXBinaryData::KeyFrameInfoData keyFrameData(key->time, localTransform, localRotation, localScale);

				keyFrames.emplace_back(keyFrameData);
			}

			keyFrameInfoList.emplace_back(keyFrames);
		}

		FBXBinaryData::AnimationClipData animData(
			anim->animationName, anim->frameRate, anim->tickPerFrame, anim->totalKeyFrame, anim->startKeyFrame, anim->endKeyFrame,
			keyFrameInfoList);

		animationClipList.emplace_back(animData);
	}

	FBXBinaryData::ModelData modelData(meshInfoList, materialList, boneInfoList, animationClipList, fbxModel->isSkinnedAnimation);

	oa << modelData;
}
