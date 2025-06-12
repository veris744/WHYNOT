#include "ThrowerManager.h"

#include <random>
#include <Components/Model.h>
#include <Components/Movement.h>
#include <Components/Transform.h>
#include <Components/Colliders/CircleCollider.h>
#include <Graphics/Material.h>
#include <Graphics/Mesh.h>
#include <Managers/ConfigurationValues.h>
#include <Managers/Helper.h>
#include <Managers/Renderer.h>
#include <Managers/World.h>
#include <UI/Containers/ProgressBar.h>
#include <UI/Text/Text.h>

float getRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

void ThrowerManager::PrepareGame()
{
    Helper::SetCursorVisible(false);
    ConfigurationValues::CanPlayerLook = true;
    ConfigurationValues::ArePhysicsActive = true;
    ConfigurationValues::CanPlayerMove = true;
    ConfigurationValues::IsEditorOpen = false;
    ConfigurationValues::IsUIActive = false;

    playerStart = {0, 11, 0};

    for (int i = 0; i < TOTAL_BALLS_HAND; i++)
    {
    Entity* tempBall = GenerateBall(BallType::HAND);
        tempBall->isActive = false;
        ballsReserve[i] = tempBall;
    }

    PrepareUI();
    PrepareScene();
}

void ThrowerManager::StartGame()
{
    World::Resume();
}

void ThrowerManager::EndGame()
{
    player = nullptr;
}

void ThrowerManager::SetPlayer()
{
    if (!player)
    {
        std::shared_ptr<Player> temp = std::make_shared<Player>("Player");
        player = temp.get();
        player->Initialize();

        playerTransform = player->GetComponent<Transform>();
    }

    player->GetComponent<Movement>()->usesPhysics = true;
    player->GetPhysicsMaterial()->hasGravity = true;

    player->GetComponent<Transform>()->position = GetPlayerStart();
    player->GetComponent<Transform>()->SetRotation(GetPlayerStartRotation());

    World::GetInstance()->SetCurrentCamera("Player");

    player->isActive = true;
}

void ThrowerManager::ProcessInput(int key, bool press)
{
    switch (key)
    {
    case GLFW_MOUSE_BUTTON_1:
        if (press)
            ChargeBall();
        else
            ThrowBall();
        break;
    case GLFW_MOUSE_BUTTON_2:
        if (press)
            GrabBall();
        else
            ReleaseBall();
        break;
    }
}

void ThrowerManager::Update(float deltaTime)
{
    if (GrabbedBall)
    {
        GrabbedBall->GetComponent<Transform>()->position = playerTransform->position + playerTransform->forward * 5.f;

        if (isCharging)
        {
            chargeBar->UpdateValue(chargeBar->GetValue() + 1);
        }
    }
}

void ThrowerManager::AddPoints(unsigned int amount)
{
    score += amount;
    pointsCounter->SetText("Score: " + std::to_string(score));
}


void ThrowerManager::PrepareUI()
{
    std::string ballsTxt = "Balls: " + to_string(TOTAL_BALLS_HAND);
    std::shared_ptr<Text> ballsCounterTemp = std::make_shared<Text>(
        ballsTxt, vec3(1, 1, 1), 0.5f,
        vec2(5, 5), "BallsCounterText");
    ballsCounterTemp->align = TextAlign::LEFT;
    ballsCounterTemp->alignVertical = TextAlignVertical::TOP;
    World::GetInstance()->AddWidget(ballsCounterTemp);
    ballsCounter = ballsCounterTemp.get();


    std::string pointsTxt = "Points: " + to_string(score);
    std::shared_ptr<Text> pointsCounterTemp = std::make_shared<Text>(
        pointsTxt, vec3(1, 1, 1), 0.5f,
        vec2(-5, 5), "PointsCounterText");
    pointsCounterTemp->align = TextAlign::RIGHT;
    pointsCounterTemp->alignVertical = TextAlignVertical::TOP;
    World::GetInstance()->AddWidget(pointsCounterTemp);
    pointsCounter = pointsCounterTemp.get();


    std::shared_ptr<ProgressBar> charger = std::make_shared<ProgressBar>(vec2{35, 0}, vec2{20, 100});
    World::GetInstance()->AddWidget(charger);
    chargeBar = charger.get();
}

void ThrowerManager::PrepareScene()
{
    std::vector<vec3> ballsPositions;
    int i = 0;
    while (i < TOTAL_BALLS_SMALL)
    {
        vec3 pos = vec3(getRandomFloat(-BALLS_PLAYGROUND.x, BALLS_PLAYGROUND.x),
            getRandomFloat(0, BALLS_PLAYGROUND.y),
            getRandomFloat(-BALLS_PLAYGROUND.z, BALLS_PLAYGROUND.z));
        bool isValid = true;
        for (const vec3& otherPos : ballsPositions)
        {
            if (distance(pos, otherPos) < 2.5f)
            {
                isValid = false;
                break;
            }
        }
        if (isValid)
        {
            i++;
            Entity* ball = GenerateBall(BallType::SMALL);
            ball->GetComponent<Transform>()->position = pos;
            ballsPositions.push_back(pos);
            ball->GetComponent<PhysicsMaterial>()->hasGravity = true;
            ball->isActive = true;
        }
    }
    i = 0;
    while (i < TOTAL_BALLS_MEDIUM)
    {
        vec3 pos = vec3(getRandomFloat(-BALLS_PLAYGROUND.x, BALLS_PLAYGROUND.x),
            1.5f,
            getRandomFloat(-BALLS_PLAYGROUND.z, BALLS_PLAYGROUND.z));
        bool isValid = true;
        for (const vec3& otherPos : ballsPositions)
        {
            if (distance(pos, otherPos) < 2.5f)
            {
                isValid = false;
                break;
            }
        }
        if (isValid)
        {
            i++;
            Entity* ball = GenerateBall(BallType::MEDIUM);
            ball->GetComponent<Transform>()->position = pos;
            ballsPositions.push_back(pos);
            ball->GetComponent<PhysicsMaterial>()->hasGravity = true;
            ball->isActive = true;
        }
    }
    i = 0;
    while (i < TOTAL_BALLS_LARGE)
    {
        vec3 pos = vec3(getRandomFloat(-BALLS_PLAYGROUND.x, BALLS_PLAYGROUND.x),
            getRandomFloat(0, BALLS_PLAYGROUND.y),
            getRandomFloat(-BALLS_PLAYGROUND.z, BALLS_PLAYGROUND.z));
        bool isValid = true;
        for (const vec3& otherPos : ballsPositions)
        {
            if (distance(pos, otherPos) < 2.5f)
            {
                isValid = false;
                break;
            }
        }
        if (isValid)
        {
            i++;
            Entity* ball = GenerateBall(BallType::LARGE);
            ball->GetComponent<Transform>()->position = pos;
            ballsPositions.push_back(pos);
            ball->GetComponent<PhysicsMaterial>()->hasGravity = true;
            ball->isActive = true;
        }
    }
}

void ThrowerManager::GrabBall()
{
    if (currentBall >= TOTAL_BALLS_HAND) return;

    GrabbedBall = ballsReserve[currentBall];
    GrabbedBall->GetComponent<Transform>()->position = playerTransform->position + playerTransform->forward * 7.f;
    GrabbedBall->isActive = true;

}

void ThrowerManager::ReleaseBall()
{
    if (GrabbedBall)
    {
        GrabbedBall->isActive = false;
        GrabbedBall = nullptr;
    }
    chargeBar->UpdateValue(0);
}

void ThrowerManager::ChargeBall()
{
    if (!GrabbedBall) return;

    isCharging = true;
}

void ThrowerManager::ThrowBall()
{
    if (!GrabbedBall) return;

    isCharging = false;

    float potency = chargeBar->GetValue() * maxPotency * 0.01f;
    potency = std::clamp(potency, minPotency, maxPotency);

    GrabbedBall->GetComponent<PhysicsMaterial>()->hasGravity = true;
    GrabbedBall->GetComponent<Movement>()->AddImpulse(playerTransform->forward * potency);
    GrabbedBall = nullptr;
    currentBall++;
    ballsCounter->SetText("Balls: " + to_string(TOTAL_BALLS_HAND - currentBall));
    chargeBar->UpdateValue(0);
}

Entity* ThrowerManager::GenerateBall(BallType type)
{
    float mass;
    float radius;
    vec3 color;
    int points;
    switch (type)
    {
    case BallType::SMALL:
        mass = 1;
        radius = 0.7f;
        color = vec3(0.8, 0.5, 0.6);
        points = 5;
        break;
    case BallType::MEDIUM:
        mass = 3;
        radius = 0.9f;
        color = vec3(0.8, 0.3, 0.6);
        points = 8;
        break;
    case BallType::LARGE:
        mass = 5;
        radius = 1.2f;
        color = vec3(0.9, 0.0, 0.7);
        points = 10;
        break;
    case BallType::HAND:
        mass = 3;
        radius = 0.9f;
        color = vec3(0.2, 0.0, 0.6);
        points = 2;
        break;
    }

    std::shared_ptr<Entity> temp = std::make_shared<Entity>();

    std::unique_ptr<Transform> transform = std::make_unique<Transform>(vec3(0));
    temp->AddComponent(std::move(transform));

    std::unique_ptr<PhysicsMaterial> physMat = std::make_unique<PhysicsMaterial>();
    physMat->hasGravity = false;
    physMat->bounciness = 0.6f;
    physMat->friction = 0.4f;
    physMat->mass = mass;
    temp->AddComponent(std::move(physMat));

    std::unique_ptr<Movement> movement = std::make_unique<Movement>();
    movement->usesPhysics = true;
    movement->maxSpeed = 100;
    temp->AddComponent(std::move(movement));

    std::unique_ptr<CircleCollider> collider = std::make_unique<CircleCollider>();
    collider->radius = radius;
    collider->profile = CollisionProfile(ColliderType::Dynamic, ColliderMode::All, false);
    temp->AddComponent(std::move(collider));

    Renderer::SetSphereVertex(radius, 32.f, 16.f);
    vector<float> vertex = Renderer::GetSphereVertex();
    vector<unsigned int> index = Renderer::GetSphereIndex();

    std::shared_ptr<Material> mat = std::make_shared<Material>(vector<string>(), DEFAULT_VERTEX_SHADER_PATH,
        "shaders/fragmentColor.glsl");
    mat->materialData.color = vec4(color, 1.0f);
    mat->materialData.type = MaterialType::COLOR;
    mat->materialData.shininess = 32;

    std::unique_ptr<Mesh> sphereMesh = std::make_unique<Mesh>(vertex, vertex.size(), index, mat);

    std::unique_ptr model = std::make_unique<Model>();
    model->AddMesh(std::move(sphereMesh));
    temp->AddComponent(std::move(model));

    temp->AddProperty("points", points);

    temp->Initialize();

    return temp.get();
}
