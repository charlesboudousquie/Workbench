/*!***************************************************************************************
\file       TypeData.hpp
\author     Cannell, Cody
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      All Enumerated types for graphics engine.
*****************************************************************************************/
#pragma once
//========Self Include==================================================================//
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

////////==========================================================================////////
////////  Struct                                                                  ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: renderType
\brief   The type of rendering that will used (ex. forward, deferred).
*****************************************************************************************/
enum class renderType
{
    enm_uber_forward = 0,  //!<  The Basic type of rendeing all in one large shader
    enm_count,             //!<  The count for enum class
};

/*!***************************************************************************************
\par class: shaderType
\brief      The type of shader
*****************************************************************************************/
enum class shaderType
{
	enm_vertex = 0,         //!< vertex shader
	enm_fragment = 1,       //!< fragment shader
	enm_geometry = 2,       //!< geometry shader
	enm_tessellation = 3,   //!< tessellation shader
	enm_count,              //!<  The count for enum class
};

enum class programType
{
    enm_forward = 0,        //!   The basic vert/frag shader
    enm_passthrough = 1,
	enm_screenSpace = 2,
	enm_screenSpaceHighlight = 3,
	enm_writeNormals,
	enm_SSAO,
	enm_mapShadows,
	enm_accumulateLights,
	enm_forwardPlus,
	enm_screenShadows,
	enm_debugSolid,
	enm_debugLine,
	enm_skybox,
	enm_fsq,


    enm_count,              //!<  The count for enum class
};

/*!***************************************************************************************
\par class: primitiveType
\brief   The mesh shap.
*****************************************************************************************/
enum class primitiveType
{
    enm_line = 0,         //!<  A line mesh
    enm_cube = 1,         //!<  A cube mesh
    enm_chicken = 2,
    enm_duck = 3,
    enm_quad = 4,
    enm_count,            //!<  The count for enum class
};

constexpr const char * CUBE_FILE = "sampleCube.obj";
constexpr const char * CHICKEN_FILE = "sampleCube.obj";
constexpr const char * DUCK_FILE = "sampleCube.obj";
constexpr const char * FullScreenQuad = "sampleCube.obj";
/*!***************************************************************************************
\par class: format
\brief   The texture format.
*****************************************************************************************/
enum class format
{
	enm_r,
    enm_rgb,
    enm_rgba,

	enm_rgba_float,

    enm_count,
	enm_INVALID=-1
};

//used for binding texture to specific slots of OpenGL
enum class TextureType : int
{
    DiffuseTexture_0,
    SpecularTexture_0,
    NormalMapTexture_0,

    Count
};

enum class DefaultUvType
{
    None,
    Linear,
    Box,
    Cylindrical,
    Spherical
};

enum class TextureMap
{
    None,
    Single,
    Box,
};

enum class lighttype
{
    enm_directional = 0,
    enm_point = 1,
    enm_spot = 2,
    enm_count
};

enum class shape
{
    enm_line = 0,
    enm_square = 1,
    enm_sphere = 2,
};

