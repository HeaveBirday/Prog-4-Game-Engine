#pragma once
#include <Components/Component.h>

class ScoreComponent final : public dae::Component
{
public:
	ScoreComponent(dae::GameObject* owner, int scoreValue) : Component(owner), m_ScoreValue(scoreValue) {}

	int GetScoreValue() const { return m_ScoreValue; }
private:
	int m_ScoreValue{};
};