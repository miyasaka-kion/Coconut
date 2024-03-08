#pragma once

struct HealthComponent {
    float health = 100.0f;

    void TakeDamage(float damage) {
        health -= damage;
    }

    bool IsDead() {
        return health <= 0.0f;
    }
};
