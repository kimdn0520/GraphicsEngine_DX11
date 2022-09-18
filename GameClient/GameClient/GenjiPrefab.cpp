#include "pch.h"
#include "GenjiPrefab.h"
#include "Resources.h"
#include "Transform.h"
#include "MonoBehaviour.h"

#include "DataManager.h"

GenjiPrefab::GenjiPrefab(Vector3 pos)
{
	std::vector<GameObject*> gameObjects = DataManager::Get()->GetDataResources("Genji");

	this->SetTag(Tag::Default);
	this->SetName("Genji");
	this->AddComponent<Transform>();
	this->GetComponent<Transform>()->SetLocalScale({ 1.f, 1.f, 1.f });
	this->GetComponent<Transform>()->SetLocalPosition(pos);

	for (auto& gameObj : gameObjects)
	{
		if (gameObj->GetTransform()->GetParent() == nullptr)
		{
			gameObj->GetTransform()->SetParent(this->GetTransform());
			this->SetChild(gameObj);
		}
	}
}

GenjiPrefab::~GenjiPrefab()
{}
