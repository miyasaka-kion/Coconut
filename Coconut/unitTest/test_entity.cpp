#include <iostream>

#include <gtest/gtest.h>

#include "Core/GameContext.h"
#include "ECS/Entity.h"
#include "ECS/Components.h"
#include "Render/SpriteLoader.h"

TEST(EntityTest, TrivialTest) {
    EXPECT_TRUE(true);
}

TEST(EntityTest, AddAndRemoveComponent) {
    GameContext gc;
    auto entity = gc.CreateEntity();
    entity.AddComponent<PlayerComponent>(/* sprite component arguments */);
    EXPECT_TRUE(entity.HasComponent<PlayerComponent>());
    entity.RemoveComponent<PlayerComponent>();
    EXPECT_FALSE(entity.HasComponent<PlayerComponent>());
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




