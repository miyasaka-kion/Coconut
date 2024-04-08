#include <iostream>

#include <gtest/gtest.h>
#include <type_traits>

#include "Core/GameContext.h"
#include "ECS/Entity.h"
#include "ECS/Components.h"
#include "Render/SpriteLoader.h"

TEST(EntityTest, TrivialTest) {
    EXPECT_TRUE(true);
}

TEST(EntityTest, AddAndRemoveComponent) {
    GameContext gc;
    auto e = gc.CreateEntity();
    e.AddComponent<PlayerComponent>(/* sprite component arguments */);
    EXPECT_TRUE(e.HasComponent<PlayerComponent>());
    e.RemoveComponent<PlayerComponent>();
    EXPECT_FALSE(e.HasComponent<PlayerComponent>());
}

TEST(EntityTest, GetComponentReturnRef) {
    GameContext gc;
    auto e = gc.CreateEntity();
    e.AddComponent<PlayerComponent>(/* sprite component arguments */);  
    EXPECT_TRUE(std::is_reference<decltype(e.GetComponent<PlayerComponent>())>::value);
}

TEST(EntityTest, EntityCopyConstructor) {
    GameContext gc;
    auto entity1 = gc.CreateEntity();
    entity1.AddComponent<PlayerComponent>(/* sprite component arguments */);

    auto entity2 = gc.CreateEntity();
    entity2 = entity1;
    EXPECT_TRUE(entity2.HasComponent<PlayerComponent>());
}

TEST(EntityTest, EntityCopyAssignmentOperator) {
    GameContext gc;
    auto entity1 = gc.CreateEntity();
    entity1.AddComponent<PlayerComponent>(/* sprite component arguments */);

    auto entity2 = entity1;
    EXPECT_TRUE(entity2.HasComponent<PlayerComponent>());
}




