#include "pch.h"
#include "GenjiPrefab.h"
#include "Resources.h"
#include "Transform.h"
#include "MonoBehaviour.h"
#include "GameObject.h"

#include "DataManager.h"

GenjiPrefab::GenjiPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	std::vector<shared_ptr<GameObject>> gameObjects = DataManager::Get()->GetDataResources("Genji");

	gameObject->SetTag(Tag::Default);
	gameObject->SetName("Genji");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale({ 1.f, 1.f, 1.f });
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);

	for (auto& gameObj : gameObjects)
	{
		if (gameObj->GetTransform()->GetParent() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

GenjiPrefab::~GenjiPrefab()
{}
