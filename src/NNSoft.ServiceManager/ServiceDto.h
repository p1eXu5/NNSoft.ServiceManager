#pragma once

#include<string>

enum Statuses { Running };

struct ServiceDto
{
private:
	std::string _name;
	__int32 _pid;
	std::string _description;
	Statuses _status;
	std::string _group;
	std::string _image;
};

