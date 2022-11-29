#pragma once
#include "State.h"
class SelectState :
	public State
{
public:
	void StateUpdate() override;
	void Start() override;

public:
	SelectState();
	~SelectState();
};

