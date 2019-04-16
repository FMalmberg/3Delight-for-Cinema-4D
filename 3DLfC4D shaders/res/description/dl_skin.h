#pragma once
enum
{
	Shader_Path = 0,
	SKIN_PAGE=100,
	SKIN,
	SKIN_COLOR,
	SKIN_COLOR_SHADER,
	SKIN_ROUGHNESS,
	SKIN_ROUGHNESS_SHADER,
	SKIN_SPECULAR_LEVEL,
	SKIN_SPECULAR_LEVEL_SHADER,
	
	SUBSURFACE_PAGE,
	SUBSURFACE,
	SUBSURFACE_COLOR,
	SUBSURFACE_COLOR_SHADER,
	SUBSURFACE_SCALE,
	SUBSURFACE_SCALE_SHADER,
	SUBSURFACE_IOR,
	SUBSURFACE_IOR_SHADER,
	
	BUMP_PAGE,
	BUMP,
	BUMP_TYPE,
		BUMP_MAP=0,
		NORMAL_MAP_DIRECTX=1,
		NORMAL_MAP_OPENGL=2,
		DISPLACEMENT_0=3,
		DISPLACEMENT_5=4,
	BUMP_VALUE = 200,
	BUMP_INTENSITY,
	DUMP_VALUE
	
};
