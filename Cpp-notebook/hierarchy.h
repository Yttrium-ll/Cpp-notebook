#pragma once
class king
{
public:
	king() {};

	virtual void action() {};
};

class heir : public king
{
public:
	heir() : king() {};

	virtual void action() override final {};
};