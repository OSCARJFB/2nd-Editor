/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "file.hpp"

static std::string getStringFromArgs(int32_t argc, int8_t **argv)
{
	if (argc < 2)
	{
		return std::string();
	}

	return std::string((char *)argv[1]);
}

static std::string readFileAtPath(std::string &path)
{
	if (path.empty())
	{
		return std::string();
	}

	std::ifstream file;
	file.open(path, std::ios::in);
	if (!file.is_open())
	{
		return std::string();
	}

	std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();
	return buffer;
}

void startApp(int32_t argc, int8_t **argv)
{
	std::string path = getStringFromArgs(argc, argv);
	std::string buffer = readFileAtPath(path);
	edit edit(buffer, buffer.size());
}
