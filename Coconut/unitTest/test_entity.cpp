#include <iostream>

#include <gtest/gtest.h>

#include "Core/GameContext.h"
#include "ECS/Entity.h"
#include "ECS/Components.h"

TEST(EntityTest, TrivialTest) {
    EXPECT_TRUE(true);
}

TEST(EntityTest, Constructor) {
    GameContext gc;
    auto entity = gc.CreateEntity();
    EXPECT_TRUE(static_cast<bool>(entity));
}

TEST(EntityTest, AddComponent) {
    GameContext gc;
    auto entity = gc.CreateEntity();
    entity.AddComponent<PlayerComponent>(/* sprite component arguments */);
    // entity.AddComponent<NativeScriptComponent>();
    EXPECT_TRUE(entity.HasComponent<PlayerComponent>());
    EXPECT_TRUE(true);
}

// Add more tests as needed