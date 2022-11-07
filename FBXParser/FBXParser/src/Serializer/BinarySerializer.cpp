#include "pch.h"
#include "Serializer/BinarySerializer.h"
#include "ParserData/ParserData.h"

void BinarySerializer::SaveBinaryFile(std::shared_ptr<FBXModel> fbxModel, std::string name, std::string path)
{
	std::ofstream output(name + ".noob", std::ios_base::binary);

	boost::archive::binary_oarchive oa(output);				// 연 스트림을 넘겨주어서 직렬화 객체 초기화

	std::vector<std::shared_ptr<FBXBinaryData::MeshData>> meshInfoList;

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

		std::shared_ptr<FBXBinaryData::MeshData> meshData = std::make_shared<FBXBinaryData::MeshData>(
			mesh->meshName, mesh->parentName, mesh->materialName, vertexDataList, mesh->indices, ConvertFloat4x4(mesh->nodeTM));

		meshInfoList.emplace_back(meshData);
	}

	std::vector<std::shared_ptr<FBXBinaryData::MaterialData>> materialList;

	for (auto& mat : fbxModel->materialList)
	{
		FBXBinaryData::Float4 material_Ambient(mat->material_Ambient.x, mat->material_Ambient.y, mat->material_Ambient.z, mat->material_Ambient.w);
		FBXBinaryData::Float4 material_Diffuse(mat->material_Diffuse.x, mat->material_Diffuse.y, mat->material_Diffuse.z, mat->material_Diffuse.w);
		FBXBinaryData::Float4 material_Specular(mat->material_Specular.x, mat->material_Specular.y, mat->material_Specular.z, mat->material_Specular.w);
		FBXBinaryData::Float4 material_Emissive(mat->material_Emissive.x, mat->material_Emissive.y, mat->material_Emissive.z, mat->material_Emissive.w);

		std::shared_ptr<FBXBinaryData::MaterialData> materialData = std::make_shared<FBXBinaryData::MaterialData>(
			mat->materialName, mat->albedoMap, mat->normalMap, mat->metallicMap, mat->roughnessMap, mat->AOMap, mat->emissiveMap,
			material_Ambient, material_Diffuse, material_Specular, material_Emissive, mat->material_Transparency, mat->material_Reflectivity,
			mat->metallic, mat->roughness);

		materialList.push_back(materialData);
	}

	std::vector<std::shared_ptr<FBXBinaryData::BoneData>> boneInfoList;

	for (auto& bone : fbxModel->fbxBoneInfoList)
	{
		std::shared_ptr<FBXBinaryData::BoneData> boneData = std::make_shared<FBXBinaryData::BoneData>(
			bone->boneName, bone->parentIndex, ConvertFloat4x4(bone->offsetMatrix), ConvertFloat4x4(bone->nodeMatrix));

		boneInfoList.push_back(boneData);
	}

	std::vector<std::shared_ptr<FBXBinaryData::AnimationClipData>> animationClipList;

	for (auto& anim : fbxModel->animationClipList)
	{
		std::vector<std::vector<std::shared_ptr<FBXBinaryData::KeyFrameInfoData>>> keyFrameInfoList;

		for (auto& keyList : anim->keyFrameList)
		{
			std::vector<std::shared_ptr<FBXBinaryData::KeyFrameInfoData>> keyFrames;

			for (auto& key : keyList)
			{
				FBXBinaryData::Float3 localTransform(key->localTransform.x, key->localTransform.y, key->localTransform.z);
				FBXBinaryData::Float4 localRotation(key->localRotation.x, key->localRotation.y, key->localRotation.z, key->localRotation.w);
				FBXBinaryData::Float3 localScale(key->localScale.x, key->localScale.y, key->localScale.z);

				std::shared_ptr<FBXBinaryData::KeyFrameInfoData> keyFrameData = std::make_shared<FBXBinaryData::KeyFrameInfoData>(
					key->time, localTransform, localRotation, localScale);

				keyFrames.emplace_back(keyFrameData);
			}

			keyFrameInfoList.emplace_back(keyFrames);
		}

		std::shared_ptr<FBXBinaryData::AnimationClipData> animData = std::make_shared<FBXBinaryData::AnimationClipData>(
			anim->animationName, anim->frameRate, anim->tickPerFrame, anim->totalKeyFrame, anim->startKeyFrame, anim->endKeyFrame,
			keyFrameInfoList);

		animationClipList.emplace_back(animData);
	}

	std::shared_ptr<FBXBinaryData::ModelData> modelData = std::make_shared<FBXBinaryData::ModelData>(
		meshInfoList, materialList, boneInfoList, animationClipList, fbxModel->isSkinnedAnimation);

	oa << modelData;
}
