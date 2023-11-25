/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <cstdint>
#include "file.hpp"

int main(int argc, char **argv)
{
	startApp(static_cast<int32_t>(argc), static_cast<int8_t**>(static_cast<void*>(argv)));
	return 0;
}
