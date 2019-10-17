/*!***************************************************************************************
\file       GraphicsSystem.cpp
\author     Cannell, Cody & Cook, Patrick
\date       9/27/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Holds all the managers and handles rendering for all objects
*****************************************************************************************/
#include "Precompiled.h"
//========Self Include==================================================================//
#include "GraphicsSystem.hpp"
//========1st Party Includes============================================================//
#include "SystemManager.hpp"

#include "GameObjectFilter.hpp"
#include "GameObject.hpp"

#include "MaterialManager.hpp"
#include "ShaderManager.hpp"

#include "TextureManager.hpp"
#include "Texture.hpp"

#include "VertexArrayManager.hpp"
#include "VertexArrayObject.hpp"

#include "CameraSystem.hpp"
#include "Camera.hpp"

#include "DebugLines.hpp"
#include "Debug.hpp"
#include "GLDebug.hpp"
#include "MeshDebug.hpp"

#include "RigidBody.hpp"
#include "Transform.hpp"

#include "Button.hpp"
#include "Light.hpp"
#include "MeshBuilder.hpp"
#include "RenderTarget.hpp"
#include "Renderer.hpp"
#include "Skybox.hpp"
#include "TextRenderer.hpp"

#include "ModularFrameBuffer.hpp"
#include "MaterialLoader.hpp"
#include "AssetManager.hpp"

//========3rd Party Includes============================================================//
#include <RenderContextInterface.hpp>
#include <iomanip>

const std::array<const char *,graphicsSystem::s_texCount>
graphicsSystem::textureNames =
{
	"gWorldNormal+Shininess",
	"depthStencil",
	"diffuseSSAO",
	"specular",
	"ambient",
	"shadowAtlas",
	"shadowMap",
	"forwardColorOutput",
	"bloom1","bloom2","bloom3","bloom4",
	"maskTexture"
};

enum textureNames
{
	gWorldNormals=0,
	depthStencil,
	diffuseSSAO,
	specular,
	ambient,
	shadowAtlas,
	shadowMap,
	forwardColor,
	bloom1,bloom2,bloom3,bloom4,
	maskTexture,
};

void graphicsSystem::onInitialize()
{
	initGLState();

	textRenderer::initialize();

	initManagers();
	initShaders();
	initFramebuffers();
	initAssets();
}

void graphicsSystem::reloadShaders()
{
	initShaders();
}

void graphicsSystem::initGLState()
{
	#ifdef _DEBUG
		#define DEBUGGIN_GRAPHICS
	#endif
	#ifdef DEBUGGIN_GRAPHICS
		GLDebug::InitializeDebugCallback();
	#endif

	//  renderContextParameters renderParams = m_render_context_ptr->queryContextParameters();

	const auto dim = m_render_context_ptr->queryContextParameters();
	m_screenWidth  = dim.width;
	m_screenHeight = dim.height;

	// assetManager = std::make_shared<AssetManager>();

	auto l_configuration = this->getSystemManager()->getConfigurationFunction();
}

void graphicsSystem::initManagers()
{
	m_vertexArrayManager = new vertexArrayManager();
	m_shaderManager = new shaderManager();
	m_textureManager = new textureManager();
	m_framebufferManager = new framebufferManager();
}

void graphicsSystem::initFramebuffers()
{
	const auto l_lastFBO = getLastAttachedFBO();
	auto fbMan = getFramebufferManager();
	//std::shared_ptr<framebuffer> l_buffer = fbMan->createFramebuffer(framebufferType::CaptureBuffer, 1024, 768);

	constexpr unsigned l_width = 1920;
	constexpr unsigned l_height = 1080;

	constexpr unsigned l_shadowDim = 1024;
	constexpr unsigned l_shadowRows = 3;
	constexpr unsigned l_shadowCols = 4;

	auto tMan = getTextureManager();

	std::array<std::shared_ptr<texture>, s_texCount> l_textures;
	auto&&[l_gNormals, l_depthBuffer,
		l_lightAcc, l_specularAcc, l_ambientAcc,
		l_shadowAtlas, l_shadowMap, l_colorOutput,
		l_bloom0, l_bloom1, l_bloom2, l_bloom3,
		l_maskTexture] = l_textures;

	l_gNormals = tMan->createTexture<simpleTexture>(textureNames[gWorldNormals],
		l_width, l_height, format::enm_rgba_float);

	l_depthBuffer = tMan->createTexture<depthTexture>(textureNames[depthStencil],
		l_width, l_height);

	l_lightAcc = tMan->createTexture<simpleTexture>(textureNames[diffuseSSAO],
		l_width, l_height, format::enm_rgba);

	l_specularAcc = tMan->createTexture<simpleTexture>(textureNames[specular],
		l_width, l_height, format::enm_rgb);

	l_ambientAcc = tMan->createTexture<simpleTexture>(textureNames[ambient],
		l_width, l_height, format::enm_rgb);

	l_shadowAtlas = tMan->createTexture<depthTexture>(textureNames[shadowAtlas],
		l_shadowDim * l_shadowCols, l_shadowDim * l_shadowRows);

	l_shadowMap = tMan->createTexture<simpleTexture>(textureNames[shadowMap],
		l_width, l_height, format::enm_rgba);

	l_colorOutput = tMan->createTexture<simpleTexture>(textureNames[forwardColor],
		l_width, l_height, format::enm_rgba);

	for (size_t i = 0; i != 4; ++i)
	{
		l_textures[i + bloom1] = tMan->createTexture<simpleTexture>(textureNames[i + bloom1],
			l_width / (1 << i), l_height / (1 << i), format::enm_rgb);
	}

	l_maskTexture = tMan->createTexture<simpleTexture>(textureNames[maskTexture],
		l_width, l_height, format::enm_r);

	for (auto&& l_tex : l_textures)
	{
		l_tex->build();
	}

	auto l_gBuffer = fbMan->createModFramebuffer(framebufferType::DeferredGBuffer, l_width, l_height);
	l_gBuffer->build();
	l_gBuffer->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_gNormals),
		"gNormal"),
		0
	);

	l_gBuffer->setDepthAttachment(depthAttachment(
		std::static_pointer_cast<depthTexture>(l_depthBuffer),
		"depth")
	);
	l_gBuffer->reconfigureDrawBuffers();

	l_gBuffer->setClearColor(color{ 0,0,0,0 });
	l_gBuffer->clear();

	auto l_lightAccBuf = fbMan->createModFramebuffer(framebufferType::LightAccBuf, l_width, l_height);
	l_lightAccBuf->build();
	l_lightAccBuf->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_lightAcc),
		"lightAcc"),
		0
	);

	l_lightAccBuf->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_specularAcc),
		"specAcc"),
		1
	);

	l_lightAccBuf->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_ambientAcc),
		"ambAcc"),
		2
	);

	l_lightAccBuf->setDepthAttachment(depthAttachment(
		std::static_pointer_cast<depthTexture>(l_depthBuffer),
		"depth")
	);
	l_lightAccBuf->reconfigureDrawBuffers();

	l_lightAccBuf->setClearColor(color{ 0, 0, 0, 1 });
	l_lightAccBuf->clear();

	auto l_forwardOutput = fbMan->createModFramebuffer(framebufferType::DeferredOutput, l_width, l_height);
	l_forwardOutput->build();
	l_forwardOutput->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_colorOutput),
		"colorOutput"),
		0
	);

	l_forwardOutput->setDepthAttachment(depthAttachment(
		std::static_pointer_cast<depthTexture>(l_depthBuffer),
		"depth")
	);


	glBindFramebuffer(GL_FRAMEBUFFER, l_lastFBO);
	if(!l_lastFBO) glDrawBuffer(GL_BACK_LEFT);
	else {
		constexpr GLenum att = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &att);
	}
	m_textureManager->UnbindAll();
}

void graphicsSystem::initShaders()
{
	using fsPath = std::filesystem::path;
	auto createShader =
		[man = getShaderManager()](
			programType type, fsPath const& vert, fsPath const& frag)
		->std::shared_ptr<shaderProgram>
	{
		return man->createShaderProgram(
			type, assetManager::getAsset(vert), assetManager::getAsset(frag));
	};
	const auto l_shadersDir = fsPath{ "shaders" };
	const auto l_vertexDir = l_shadersDir / "vertex";
	const auto l_fragmentDir = l_shadersDir / "fragment";

	std::array<std::shared_ptr<shaderProgram>, 11> l_shaders;
	auto&&[l_program, l_deferredProgram,
		l_screenSpace,l_screenSpaceHighlight,
		l_debugSolid, l_debugLine,
		l_skyboxShader, l_fsq,
		l_renderNormals, l_accLights, l_forwardPlus] = l_shaders;

	l_program = createShader(programType::enm_forward,
		l_vertexDir / "shader.vert",
		l_fragmentDir / "shader.frag");

	l_deferredProgram = createShader(programType::enm_passthrough,
		l_vertexDir / "PassThrough.vert",
		l_fragmentDir / "PassThrough.frag");

	l_screenSpace = createShader(programType::enm_screenSpace,
		l_vertexDir / "ScreenSpace.vert",
		l_fragmentDir / "ScreenSpace.frag");

	l_screenSpaceHighlight = createShader(programType::enm_screenSpaceHighlight,
		l_vertexDir / "ScreenSpaceHighlight.vert",
		l_fragmentDir / "ScreenSpaceHighlight.frag");

	l_debugSolid = createShader(programType::enm_debugSolid,
		l_vertexDir / "SolidColor.vert",
		l_fragmentDir / "SolidColor.frag");

	l_debugLine = createShader(programType::enm_debugLine,
		l_vertexDir / "InstancedThickLines.vert",
		l_fragmentDir / "InstancedSolidColor.frag");

	l_skyboxShader = createShader(programType::enm_skybox,
		l_vertexDir / "Skybox.vert",
		l_fragmentDir / "Skybox.frag");

	l_fsq = createShader(programType::enm_fsq,
		l_vertexDir / "FSQ.vert",
		l_fragmentDir / "FSQ.frag");

	l_renderNormals = createShader(programType::enm_writeNormals,
		l_vertexDir / "DeferredGBufferPass.vert",
		l_fragmentDir / "DeferredGBufferPass.frag");

	l_accLights = createShader(programType::enm_accumulateLights,
		l_vertexDir / "DeferredLightingPass.vert",
		l_fragmentDir / "DeferredLightingPass.frag");

	l_forwardPlus = createShader(programType::enm_forwardPlus,
		l_vertexDir / "ForwardPlus.vert",
		l_fragmentDir / "ForwardPlus.frag");

	for (auto&& shader : l_shaders)
		shader->build();
}

void graphicsSystem::initAssets()
{
	std::shared_ptr<vertexArrayObject> l_quadMesh = getVertexArrayManager()->buildFullScreenQuad("fullScreenQuad");

	/******************ORDER OF TEXTURES****************/
	//GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
	//GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
	//GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front
	/***************************************************/

	// jpg dont currently work
	/* std::vector<std::string> textureNames{ "wood_create_512x512.jpg", "wood_create_512x512.jpg",
	 *                                        "wood_create_512x512.jpg", "wood_create_512x512.jpg",
	 *                                        "wood_create_512x512.jpg", "wood_create_512x512.jpg" };
	 *
	 */

	//https://opengameart.org/content/interstellar-skybox-png
	std::vector<assetHandle> textureNames2{ assetManager::getAsset(L"textures/skybox_xpos.png"), assetManager::getAsset(L"textures/skybox_xneg.png"),
																					assetManager::getAsset(L"textures/skybox_ypos.png"), assetManager::getAsset(L"textures/skybox_yneg.png"),
																					assetManager::getAsset(L"textures/skybox_zpos.png"), assetManager::getAsset(L"textures/skybox_zneg.png") };

	std::shared_ptr<texture> l_skybox = getTextureManager()->createCubeTexture("sky_box_texture", textureNames2);
	l_skybox->build();

	m_textureManager->UnbindAll();
}

auto graphicsSystem::getObjectList(componentType const& type) const
	-> std::vector<std::shared_ptr<gameObject>>
{
	GameObjectFiltering::componentTypeSet l_pattern;
	l_pattern.setType(type);
	return getSystemManager()->getGameObjectGatherer()->getList(l_pattern);
}

void graphicsSystem::drawLine(
	vector3 const& begin,
	vector3 const& end,
	vector3 const& color,
	float thickness,
	layer layer
)
{
	m_linesToDraw[static_cast<size_t>(layer)].emplace(
		begin, end, color, thickness);
}



void graphicsSystem::onStartFrame()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void graphicsSystem::onUpdate()
{
	//fpsPrint();

	checkAndSetWindowDimensionVariables();

	setCurrentMainCameraMemberVariable();

	if (!m_mainCamera)
	{
		return;
	}

	//auto l_listObjects = getObjectList(renderer::getType());
	//
	//for (auto && object : l_listObjects)
	//{
	//	auto l_transform = object->getComponent<transform>();
	//
	//	if (!l_transform)
	//		continue;
	//
	//	{
	//		auto l_lineSegments = debugLines::getLinesByTag(object, "graphicsDebug");
	//		std::vector<lineSegment>& l_buffer = l_lineSegments->getLineBuffer();
	//
	//		l_buffer.clear();
	//		vector3 offset{ l_transform->getGlobalPosition() };
	//		l_buffer.emplace_back(lineSegment{ offset + vector3{ -1,-1,0 }, offset + vector3{ 1,-1,0 }, vector3{ 1,0,0 }, 0.02f });
	//		l_buffer.emplace_back(lineSegment{ offset + vector3{ 1,-1,0 }, offset + vector3{ 1,1,0 }, vector3{ 1,1,0 }, 0.02f });
	//		l_buffer.emplace_back(lineSegment{ offset + vector3{ 1,1,0 }, offset + vector3{ -1,1,0 }, vector3{ 0,1,0 }, 0.02f });
	//		l_buffer.emplace_back(lineSegment{ offset + vector3{ -1,1,0 }, offset + vector3{ -1,-1,0 }, vector3{ 0,1,1 }, 0.02f });
	//	}
	//
	//	{
	//		auto l_otherSegments = debugLines::getLinesByTag(object, "graphicsDebugElectricBoogaloo");
	//
	//		auto&& l_buffer2 = l_otherSegments->getLineBuffer();
	//		matrix4x4 const& model = l_transform->getGlobalMatrix();
	//		vector3 begin = vector3{ model * vector4{ -1,-1,-1,1 } },
	//			end = vector3{ model * vector4{ 1,1,1,1 } };
	//
	//		l_buffer2.clear();
	//		l_buffer2.emplace_back(lineSegment{ begin, end, vector3{0,0,0}, 0.5f });
	//	}
	//}

	std::shared_ptr<shaderProgram> l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_forward);
	renderToScreen(l_shaderProgram, m_mainCamera);
	renderButtons();
	textRenderer::update();
}

void graphicsSystem::onEndFrame()
{
	getSystemManager()->getSystem<platformWindowing>()->presentFrame();
}

void graphicsSystem::onShutdown()
{
	textRenderer::shutdown();
	delete m_shaderManager;
	delete m_vertexArrayManager;
	delete m_textureManager;
}

float oldfps = 0, newfps = 0;
void graphicsSystem::fpsPrint()
{
	newfps = 1.0f / getDt();

	float smoothing = .99f; // larger=more smoothing
	auto measurement = (oldfps * smoothing) + (newfps * (1.0f - smoothing));

	oldfps = measurement;


	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << measurement;
	std::string mystring = ss.str();
	debug::displayText("fps is (graphicsSystem::onUpdate()): " + mystring, "arial", -500, 350, .5f, color::red);
}

void graphicsSystem::checkAndSetWindowDimensionVariables()
{
	if (m_render_context_ptr->queryContextChanged())
	{
		m_screenWidth = m_render_context_ptr->queryContextParameters().width;
		m_screenHeight = m_render_context_ptr->queryContextParameters().height;
	}
}

void graphicsSystem::setCurrentMainCameraMemberVariable()
{
	auto l_cameraSystem = this->getSystemManager()->getSystem<cameraSystem>();

	m_mainCamera = l_cameraSystem->getMainCamera();
}

GLint graphicsSystem::getLastAttachedFBO()
{
	GLint editorFboId;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &editorFboId);
	return editorFboId;
}

void graphicsSystem::render(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<cameraBase> p_camera)
{
	auto l_listObjects = getObjectList(renderer::getType());
	auto l_listCameras = getObjectList(camera::getType());
	auto l_listLights = getObjectList(light::getType());
	auto l_listSkybox = getObjectList(skybox::getType());

	glEnable(GL_DEPTH_TEST);

	p_camera->process(); //TODO(cody): happening every frame.  add dirty flag

	glDisable(GL_BLEND);

	renderOpaque(l_listObjects, l_listLights, p_camera);



	if (m_textureIndex <= -1)
	{
		renderOld(l_listObjects, l_listLights, p_camera);
	}

	renderDebug(p_camera);
	renderSkybox(l_listSkybox, p_camera);
	renderTextureToScreen(std::static_pointer_cast<simpleTexture>(getTextureManager()->getSimpleTexture(textureNames[forwardColor])));

}

void graphicsSystem::recordToRenderPool(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<camera> p_camera)
{

	// save each frame to the render pool

	p_shaderProgram->bind();

	// get render pool
	unsigned l_index = 0;
	if (!p_camera->hasTape())
	{
		l_index = m_framebufferManager->attachRenderPool();
		p_camera->setTapIndex(l_index);

		p_camera->setHasTap(true);
	}
	else
	{
		l_index = p_camera->getTapeIndex();
	}


	if (p_camera->getCaptureFlag())
	{
		if (!p_camera->getHasCapture())
		{
			p_camera->setCaptureIndex(m_framebufferManager->createCapturePool(p_camera->getTapeIndex()));
			p_camera->setHasCapture(true);
		}
		else
		{
			m_framebufferManager->saveRenderToCapturePool(p_camera->getCaptureIndex(), p_camera->getTapeIndex());
		}

		p_camera->resetCaptureFlag();

	}

	//if (p_camera->isNewRecording())
	//{
	//    //m_framebufferManager->clearBuffer(l_index);

	//    p_camera->isDirty(false);
	//}


   // m_framebufferManager->addFrameToBufferPool(l_index);

	auto & l_bufferPool = m_framebufferManager->getBufferPool(l_index);

	std::shared_ptr<framebuffer> l_temp_buffer = l_bufferPool.front();


	l_temp_buffer->bind();

	l_temp_buffer->clear();

	render(p_shaderProgram, componentHandle<cameraBase>::castHandle(p_camera));

	l_temp_buffer->unbind();

	l_bufferPool.push_back(l_temp_buffer);

	l_bufferPool.pop_front();
}

void graphicsSystem::renderSkybox(objectList const& p_listSkybox, componentHandle<cameraBase> p_camera)
{
	{//========================SkyBox========================//
		glCullFace(GL_FRONT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(false);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		for (auto & renderObj : p_listSkybox)
		{
			auto l_skybox = renderObj->getComponent<skybox>();
			std::shared_ptr<shaderProgram> l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_skybox);

			l_shaderProgram->bind();
			//GLDebug::getLastError();

			// send camea uniforms

			p_camera->setCameraUniforms(l_shaderProgram);

			//materials

			l_shaderProgram->setUniforms("ModelMatrix", matrix4x4::translation(p_camera->getEyePosition()));
			auto&& mat = assetManager::getAsset(std::filesystem::path{"materials"} / "skybox.mtl")->getData<materialData>().getData();
			l_skybox->onRender(l_shaderProgram, mat, m_textureManager->getCubeTexture(l_skybox->getTextureName()));
			std::shared_ptr<vertexArrayObject> vao = m_vertexArrayManager->getVertexArrayBuffer(CUBE_FILE);

			vao->bind();

			vao->render();

			vao->unbind();

			//shaderProgram->unbind();
			//GLDebug::getLastError();

			m_textureManager->UnbindAll();
			//GLDebug::getLastError();
		}

		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glDepthMask(true);
		glDepthFunc(GL_LESS);
		glCullFace(GL_BACK);

	}//=======================================================//
}

void graphicsSystem::renderOpaque(objectList const& p_listObjects, objectList& p_lights, componentHandle<cameraBase> p_camera)
{
	int l_dim[4];
	glGetIntegerv(GL_VIEWPORT, l_dim);
	auto&&[x, y, width, height] = l_dim;

	glViewport(0, 0, 1920, 1080);

	auto&& l_gbuffer = getFramebufferManager()->getModFramebuffer(framebufferType::DeferredGBuffer);

	const auto lastBound = getLastAttachedFBO();
	l_gbuffer->bind();
	l_gbuffer->clear();

	renderNormals(p_listObjects, p_camera);

	auto&& l_lightBuffer = getFramebufferManager()->getModFramebuffer(framebufferType::LightAccBuf);
	l_lightBuffer->bind();
	l_lightBuffer->clear(bufferClearbits::color);
	
	renderLighting(p_lights, p_camera);

	auto&& l_forwardBuffer = getFramebufferManager()->getModFramebuffer(framebufferType::DeferredOutput);
	l_forwardBuffer->bind();
	l_forwardBuffer->clear(bufferClearbits::color);

	renderForwardPlus(p_listObjects, p_camera);

	renderDebug(p_camera);
	renderSkybox(getObjectList(skybox::getType()), p_camera);


	glBindFramebuffer(GL_FRAMEBUFFER, lastBound);
	if (!lastBound) glDrawBuffer(GL_BACK_LEFT);
	glViewport(x, y, width, height);

}

void graphicsSystem::renderNormals(objectList const& p_obj, componentHandle<cameraBase> p_camera)
{
	if (auto&& l_shader = getShaderManager()->getShaderProgram(programType::enm_writeNormals); l_shader)
	{
		l_shader->bind();
		p_camera->setCameraUniforms(l_shader);
		for (auto&& i_obj : p_obj)
		{
			auto&& l_transform = i_obj->getComponent<transform>();

			if (!l_transform) continue;

			auto&& l_renderer = i_obj->getComponent<renderer>();

			auto&& l_vao = m_vertexArrayManager->getVertexArrayBuffer(l_renderer->GetRenderType());

			if (!l_vao) continue;

			if (l_renderer->hasMaterial())
			{
				auto l_materialPtr = assetManager::getAsset(l_renderer->getMaterialPath());
				l_shader->setUniforms("material.specularExponent", l_materialPtr->getData<materialData>().getData().getSpecularExponent());
			}
			
			l_shader->setUniforms("ModelMatrix", l_transform->getGlobalMatrix());

			l_vao->bind();
			l_vao->render();
			l_vao->unbind();
		}

		getTextureManager()->UnbindAll();
	}

}

void graphicsSystem::renderLighting(objectList const& p_lights, componentHandle<cameraBase> p_camera)
{
	// TODO: Light Volume stencil hack

	auto&& l_shader = getShaderManager()->getShaderProgram(programType::enm_accumulateLights);
	l_shader->bind();
	p_camera->setCameraUniforms(l_shader); 
	l_shader->setUniforms("invView", p_camera->getInvViewMatrix());

	vector2 nearFar;
	glGetFloatv(GL_DEPTH_RANGE, nearFar.toFloatPtr());
	l_shader->setUniforms("depthrange", nearFar);


	auto&& l_gBuffer = getFramebufferManager()->getModFramebuffer(framebufferType::DeferredGBuffer);
	l_gBuffer->bindColorAttachments(l_shader);   // 0
	l_gBuffer->bindDepthAttachment(l_shader, 1); // 1
	
	// necessary.
	// inputs: normal+shininess, depth, lighting information
	// outputs: intermediate lighting information


	auto&& l_vao = getVertexArrayManager()->getVertexArrayBuffer(FullScreenQuad);
	l_vao->bind();

	glDepthMask(GL_FALSE);
	glDepthFunc(GL_GREATER);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE); // additive blending
	for (auto&& i_light : p_lights)
	{
		auto&& l_lightC = i_light->getComponent<light>();
		l_lightC->SetLightUniform(0,l_shader, p_camera);
		l_vao->render();
	}
	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	
	l_vao->unbind();


	getTextureManager()->UnbindAll();
}

void graphicsSystem::renderForwardPlus(objectList const& p_objects, componentHandle<cameraBase> p_camera)
{
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1, -1);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	auto&& l_shader = getShaderManager()->getShaderProgram(programType::enm_forwardPlus);
	l_shader->bind();
	p_camera->setCameraUniforms(l_shader);
	l_shader->setUniforms("invView", p_camera->getInvViewMatrix());

	vector2 nearFar;
	glGetFloatv(GL_DEPTH_RANGE, nearFar.toFloatPtr());
	l_shader->setUniforms("depthrange", nearFar);

	auto&& l_lightBuffer = getFramebufferManager()->getModFramebuffer(framebufferType::LightAccBuf);
	l_lightBuffer->bindDepthAttachment(l_shader, 0);  // 0
	l_lightBuffer->bindColorAttachments(l_shader, 3); // 3,4,5

	light::SetGlobalAmbient(l_shader);

	for (auto&& i_obj : p_objects)
	{
		auto&& l_transform = i_obj->getComponent<transform>();
		if (!l_transform) continue;
		
		auto&& l_renderer = i_obj->getComponent<renderer>();
		if (!l_renderer->hasMaterial()) continue;

		l_shader->setUniforms("ModelMatrix", l_transform->getGlobalMatrix());

		auto l_materialPtr = assetManager::getAsset(l_renderer->getMaterialPath());
		l_renderer->setMaterialUniforms(l_shader, l_materialPtr->getData<materialData>().getData(), m_textureManager);

		auto&& l_vao = m_vertexArrayManager->getVertexArrayBuffer(l_renderer->GetRenderType());
		l_vao->bind();
		l_vao->render();
		l_vao->unbind();

		l_renderer->unbindTextures(l_materialPtr->getData<materialData>().getData(), m_textureManager);
	}

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_POLYGON_OFFSET_FILL);

	getTextureManager()->UnbindAll();
}

void graphicsSystem::renderForward(objectList const&, componentHandle<cameraBase>)
{
	// TODO: when applicable, render transparent surfaces here
}

void graphicsSystem::renderOld(objectList const& p_listObjects, objectList & p_lights, componentHandle<cameraBase> p_camera)
{
	for (auto & renderObj : p_listObjects)
	{
		auto l_renderer = renderObj->getComponent<renderer>();

		// get the program object and bind
		std::shared_ptr<shaderProgram> l_shaderProgram = m_shaderManager->getShaderProgram(l_renderer->GetProgrameType());

		// GLDebug::getLastFrameBufferError();
		l_shaderProgram->bind();

		// set camera uniforms
		p_camera->setCameraUniforms(l_shaderProgram);
		//cam->printMatrices();

		int lightCount = static_cast<int>(p_lights.size());

		l_shaderProgram->setUniforms("lightCount", lightCount);
		/*if(l_renderer->GetProgrameType() == programType::enm_screenSpace)
			l_shaderProgram->setUniforms("OrthoMatrix", matrix4x4::computeOrthographicMatrix(m_screenWidth, m_screenHeight, m_mainCamera->getNearPlaneDistance(), m_mainCamera->getFarPlaneDistance()));*/

		for (int i = 0; i < lightCount; ++i)
		{
			auto l_light = p_lights[i]->getComponent<light>();
			l_light->SetLightUniform(i, l_shaderProgram, p_camera);
		}

		if (l_renderer->hasMaterial())
		{
			auto l_materialPtr = assetManager::getAsset(l_renderer->getMaterialPath());
			l_renderer->setMaterialUniforms(l_shaderProgram, l_materialPtr->getData<materialData>().getData(), m_textureManager);
		}
		else
		{
			l_renderer->setTextureUniforms(l_shaderProgram, m_textureManager);
		}

		l_renderer->setRenderUniforms(l_shaderProgram);
		std::shared_ptr<vertexArrayObject> vao = m_vertexArrayManager->getVertexArrayBuffer(l_renderer->GetRenderType());

		if (vao == nullptr)
			continue;

		vao->bind();
		vao->render();
		vao->unbind();

		// unbinding step
	   // shaderProgram->unbind();
		m_textureManager->UnbindAll();
	}
}

void graphicsSystem::renderDebug(componentHandle<cameraBase> p_camera)
{
	renderDebugWireframes(getObjectList(rigidBody::getType()), p_camera);
	renderDebugLines(getObjectList(debugLines::getType()), p_camera);
}


void graphicsSystem::renderDebugWireframes(const objectList & p_physObj, componentHandle<cameraBase> p_camera)
{
	auto l_shader = m_shaderManager->getShaderProgram(programType::enm_debugSolid);

	l_shader->bind();

	p_camera->setCameraUniforms(l_shader);
	l_shader->setUniforms("color", vector4{ 1,0,0,1 });

	auto l_vao = m_vertexArrayManager->getVertexArrayBuffer(CUBE_FILE);

	if (!l_vao) return;

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1, -1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	l_vao->bind();
	for (auto && obj : p_physObj)
	{
		if (auto l_world = obj->getComponent<transform>(); l_world)
		{
			l_shader->setUniforms("ModelMatrix", l_world->getGlobalMatrix());
			l_vao->render();
		}
	}
	l_vao->unbind();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void graphicsSystem::renderDebugLines(const objectList & p_lines, componentHandle<cameraBase> p_camera)
{
	{// DEBUG LINES
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// batched lines
		if (auto l_debugLineShader = getShaderManager()->getShaderProgram(programType::enm_debugLine);
			l_debugLineShader)
		{
			l_debugLineShader->bind();
			p_camera->setCameraUniforms(l_debugLineShader);

			for (auto&& obj : p_lines)
			{
				for (auto&& component : obj->getComponents<debugLines>())
				{
					auto&& l_buffer = (*component)->m_buffer;
					auto l_vao = m_vertexArrayManager->getVertexArrayBuffer(l_buffer.getAssetHandle());

					l_buffer.update();
					l_buffer.bind();
					l_buffer.render(l_vao);
					l_buffer.unbind();
				}
			}
		}

		// immediate lines
		if(auto l_shader = m_shaderManager->getShaderProgram(programType::enm_debugSolid);
			l_shader)
		{
			auto l_vao = m_vertexArrayManager->getVertexArrayBuffer(CUBE_FILE);
			l_vao->bind();

			l_shader->bind();
			p_camera->setCameraUniforms(l_shader);


			auto renderLine = [&l_shader, &l_vao](lineSegment const& toRender)
			{
				auto buildModel = [](vector3 const& begin, vector3 const& end, float weight) -> matrix4x4
				{
					vector3 l_vec = end - begin;
					vector3 l_trans = (begin + end) * 0.5f;
					matrix4x4 l_rot{};
					l_rot.computeModelMatrixFromLookAtVector2(l_trans, l_vec + l_trans);
					vector3 l_scale{ weight, weight, l_vec.distance() * 0.5f + weight };

					return l_rot * matrix4x4::scale(l_scale);
				};

				l_shader->setUniforms("ModelMatrix", buildModel(toRender.begin, toRender.end, toRender.weight));
				l_shader->setUniforms("color", vector4(toRender.color, 1));

				l_vao->render();
			};

			auto renderLineQueue = [&renderLine](auto && lineQueue)
			{
				while (!lineQueue.empty())
				{
					renderLine(lineQueue.front());
					lineQueue.pop();
				}
			};

			glDepthFunc(GL_LESS);
			renderLineQueue(m_linesToDraw.front());

			glDepthFunc(GL_ALWAYS); // so skybox doesn't overwrite lines

			std::for_each(m_linesToDraw.begin() + 1, m_linesToDraw.end(), renderLineQueue);

			glDepthFunc(GL_LESS);
			l_vao->unbind();
		}
	}

	glDepthMask(true);

	m_textureManager->UnbindAll();
}

void graphicsSystem::renderTextureToScreen(std::shared_ptr<simpleTexture> p_texture)
{
	auto l_shader = getShaderManager()->getShaderProgram(programType::enm_fsq);

	l_shader->bind();

	p_texture->bind(0);


	GLint viewportGeom[4];
	glGetIntegerv(GL_VIEWPORT, viewportGeom);
	auto&&[x, y, width, height] = viewportGeom;

	glViewport(width / 2, height / 2, width / 3, height / 3);


	auto l_vao = getVertexArrayManager()->getVertexArrayBuffer("fullScreenQuad");



	l_vao->bind();
	l_vao->render();
	l_vao->unbind();

	// reset viewport
	glViewport(x,y,width,height);

	p_texture->unbind();
}

componentHandle<camera> graphicsSystem::getCameraByChannel(unsigned p_channel)
{
	auto&& l_cameras = getObjectList(camera::getType());

	auto l_found = std::find_if(l_cameras.begin(), l_cameras.end(),
		[p_channel](std::shared_ptr<gameObject> const& obj) -> bool
		{
			return obj->getComponent<camera>()->getBroadcastCannel() == p_channel;
		}
	);

	return componentHandle<camera>();
}

void graphicsSystem::captureForAllRecordingCameras()
{
	for (auto&& l_cameraObject : getObjectList(camera::getType()))
	{
		auto l_camera = l_cameraObject->getComponent<camera>();

		/*if (l_camera->getIsRecording())
		{*/
		auto l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_forward);
		recordToRenderPool(l_shaderProgram, l_camera);
		//}
	}
}

void graphicsSystem::renderToScreen(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<cameraBase> p_camera)
{
	p_shaderProgram->bind();

	//m_framebufferManager->renderToScreen();

	render(p_shaderProgram, p_camera);
}

std::shared_ptr<framebuffer> graphicsSystem::renderToBuffer(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<camera> p_camera)
{
	std::shared_ptr<framebuffer> l_capture_buffer = getFramebufferManager()->getFramebuffer(framebufferType::CaptureBuffer);

	l_capture_buffer->bind();
	l_capture_buffer->clear();

	p_shaderProgram->bind();

	render(p_shaderProgram, componentHandle<cameraBase>::castHandle(p_camera));

	return l_capture_buffer;
}

void graphicsSystem::renderButtons()
{
	// Render stuff to screen

	auto l_listButton = getObjectList(button::getType());
	std::sort(l_listButton.begin(), l_listButton.end(), [](std::shared_ptr<gameObject> & a, std::shared_ptr<gameObject> & b) -> bool
	{ return a->getComponent<transform>()->getPosition().getZ() < b->getComponent<transform>()->getPosition().getZ(); });

	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
   // glDisable(GL_TEXTURE_2D);
   // glDisable(GL_LIGHTING);
	for (auto & buttonObj : l_listButton)
	{
		auto l_transform = buttonObj->getComponent<transform>();
		auto l_uiObject = buttonObj->getComponent<button>();

		if (l_uiObject && l_transform)
		{
			if (l_uiObject->isActive())
			{
				//l_shaderProgram->setUniforms("ModelMatrix", l_transform->getLocalMatrix());

			//l_shaderProgram->setUniforms("ViewMatrix", m_mainCamera->getViewMatrix());
			//			//1024, 768
			//	auto ortho = matrix4x4::computeOrthographicMatrix(0, 1024/*m_mainCamera->getWidth()*/,0, 768/*m_mainCamera->getHeight()*/, m_mainCamera->getNearPlaneDistance(), m_mainCamera->getFarPlaneDistance());
			//l_shaderProgram->setUniforms("OrthoMatrix", ortho);
				std::shared_ptr<shaderProgram> l_shaderProgram;
				if (l_uiObject->isSelected())
				{
					l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_screenSpaceHighlight);
					l_shaderProgram->bind();
				}
				else
				{
					l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_screenSpace);
					l_shaderProgram->bind();
				}

				if (l_shaderProgram == nullptr)
					continue;

				l_shaderProgram->setUniforms("Scale", l_transform->getGlobalScale());

				l_shaderProgram->setUniforms("Transform", l_transform->getGlobalPosition());

				l_shaderProgram->setUniforms("CameraWidth", m_mainCamera->getWidth());

				l_shaderProgram->setUniforms("CameraHeight", m_mainCamera->getHeight());

				std::shared_ptr<texture> l_texture;
				std::string l_textureName = l_uiObject->getTextureName();

				l_texture = m_textureManager->getSimpleTexture(l_textureName);

				/*if(l_uiObject->isSelected())
					l_texture = m_textureManager->getSimpleTexture("button2.png");
				else
					l_texture = m_textureManager->getSimpleTexture("button1.png");*/


					//Bad texture
				if (l_texture == nullptr)
				{
					continue;
				}

				l_texture->bind(0);

				l_shaderProgram->setUniforms("ScreenSampler", l_texture->getBoundSlot());

				std::shared_ptr<vertexArrayObject> vao = m_vertexArrayManager->getVertexArrayBuffer("fullScreenQuad");

				vao->bind();
				vao->render();
				vao->unbind();

				// unbinding step
			   // shaderProgram->unbind();
				m_textureManager->UnbindAll();
			}

		}
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_LIGHTING);
}

void graphicsSystem::initializeRenderTargets()
{
	std::shared_ptr<framebuffer> l_buffer = getFramebufferManager()->createFramebuffer(framebufferType::CaptureBuffer, 1024, 768);

	l_buffer->addColorAttachment("screenSampler");

#if CAPTURE
	for (int i = 0; i < 2; i++)
	{
		m_framebufferManager->createBufferPool(MAXFRAMES);
	}
#endif

	l_buffer->build();
}

void graphicsSystem::updateRenderTargets()
{
	auto l_renderTargetObject = getObjectList(renderTarget::getType());

	std::shared_ptr<shaderProgram> l_shaderProgram;


#if CAPTURE
	captureForAllRecordingCameras();
#endif

	for (auto & l_renderObj : l_renderTargetObject)
	{
		auto l_target = l_renderObj->getComponent<renderTarget>();
		auto l_transform = l_renderObj->getComponent<transform>();
#if CAPTURE
		if (l_target->getPlayState() == play_state::enum_feed)
		{
#endif
			if (auto l_camera2 = getCameraByChannel(l_target->getFeedChannel()); l_camera2)
			{
				l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_forward);

				auto l_capture_buffer = renderToBuffer(l_shaderProgram, l_camera2);

				l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_passthrough);
				l_shaderProgram->bind();


				l_capture_buffer->bind();

				l_capture_buffer->bindColorAttachments(l_shaderProgram);

				//l_capture_buffer->unbind();
			}

			glBindFramebuffer(GL_FRAMEBUFFER, getLastAttachedFBO());



			m_mainCamera->setCameraUniforms(l_shaderProgram);

			l_shaderProgram->setUniforms("ModelMatrix", l_transform->getGlobalMatrix());

			std::shared_ptr<vertexArrayObject> vao = m_vertexArrayManager->getVertexArrayBuffer(FullScreenQuad);

			vao->bind();
			vao->render();

			vao->unbind();
#if CAPTURE
		}
		else if (l_target->getPlayState() == play_state::enum_playback)
		{

			auto l_camera2 = getCameraByChannel(l_target->getPlayBackChannel());

			if (l_camera2 != nullptr)
			{
				if (l_camera2->getHasCapture())
				{
					l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_passthrough);
					l_shaderProgram->bind();

					unsigned l_index = l_camera2->getTapeIndex();
					auto & l_bufferPool = m_framebufferManager->getCapturePool(l_index);
					std::shared_ptr<framebuffer> l_playback_buffer = l_bufferPool.front();

					l_playback_buffer->bind();

					l_playback_buffer->bindColorAttachments(l_shaderProgram);

					l_playback_buffer->unbind();

					l_bufferPool.push_back(l_playback_buffer);

					l_bufferPool.pop_front();

				}
			}


			m_mainCamera->setCameraUniforms(l_shaderProgram);

			l_shaderProgram->setUniforms("ModelMatrix", l_transform->getGlobalMatrix());

			std::shared_ptr<vertexArrayObject> vao = m_vertexArrayManager->getVertexArrayBuffer(primitiveType::enm_quad);


			vao->bind();
			vao->render();
			vao->unbind();
		}
		else
		{
			auto l_camera2 = getCameraByChannel(l_target->getFeedChannel());

			if (l_camera2 != nullptr)
			{

				l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_forward);

				auto l_capture_buffer = renderToBuffer(l_shaderProgram, l_camera2);

				l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_passthrough);
				l_shaderProgram->bind();


				l_capture_buffer->bind();

				l_capture_buffer->bindColorAttachments(l_shaderProgram);

				l_capture_buffer->unbind();
			}




			m_mainCamera->setCameraUniforms(l_shaderProgram);

			l_shaderProgram->setUniforms("ModelMatrix", l_transform->getGlobalMatrix());

			std::shared_ptr<vertexArrayObject> vao = m_vertexArrayManager->getVertexArrayBuffer(primitiveType::enm_quad);

			vao->bind();
			vao->render();
			vao->unbind();
		}

#endif

	}
}
