/*!***************************************************************************************
\file       CameraBase.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "CameraBase.hpp"

#include "ShaderProgram.hpp"
#include <MathFunctions.hpp>

matrix4x4 cameraBase::getViewProjectionMatrix()
{
	if (dirty) process();
	return m_viewProjectionMatrix;
}

matrix4x4 cameraBase::getViewMatrix()
{
	if (dirty) process();
	return m_viewMatrix;
}

matrix4x4 cameraBase::getProjectionMatrix()
{
	if (dirty) process();
	return m_projectionMatrix;
}

void cameraBase::setProjectionMatrix(const matrix4x4& p_projMatrix)
{
	m_projectionMatrix = p_projMatrix;
	dirty = false;
}

void cameraBase::setViewMatrix(const matrix4x4& p_viewMatrix)
{
	m_viewMatrix = p_viewMatrix;
	dirty = false;
}

void cameraBase::setViewProjectionMatrix(const matrix4x4& p_viewProjMatrix)
{
	m_viewProjectionMatrix = p_viewProjMatrix;
	dirty = false;
}

void cameraBase::setFieldOfViewDegree(float m_angleDegree)
{
	m_fieldOfView = DegToRad(m_angleDegree);
	dirty = true;
}

float cameraBase::getFieldOfViewDegree()
{
	  return RadToDeg(m_fieldOfView);
}

float cameraBase::getFieldOfViewRad()
{
	return m_fieldOfView;
}

void cameraBase::setDimension(float p_width, float p_height)
{
	m_width = p_width;
	m_height = p_height;

	dirty = true;
}

void cameraBase::setWidth(float p_width)
{
	m_width = p_width;
	dirty = true;
}

void cameraBase::setHeight(float p_height)
{
	m_height = p_height;

	dirty = true;
}

float cameraBase::getAspectRatio() const
{
	return m_width / m_height;
}

void cameraBase::setNearPlaneDistance(float p_zNear)
{
	m_nearPlaneDist = p_zNear;
	dirty = true;
}

void cameraBase::setFarPlaneDistance(float p_zFar)
{
	m_farPlaneDist = p_zFar;

	dirty = true;
}

void cameraBase::calcProjMatrix()
{
	m_projectionMatrix = matrix4x4::computeProjMatrix(m_fieldOfView, (m_width / m_height), m_nearPlaneDist, m_farPlaneDist, false);
}

void cameraBase::calcViewProjMatrix()
{
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void cameraBase::printData()
{
	logger l_logger("Camera");
	l_logger.debug() << "fov: " << m_fieldOfView;
	l_logger.debug() << "width: " << m_width;
	l_logger.debug() << "height: " << m_height;
	l_logger.debug() << "near: " << m_nearPlaneDist;
	l_logger.debug() << "far: " << m_farPlaneDist << "\n";
	printMatrices();
}

void cameraBase::printMatrices()
{
	if (dirty) process();

	logger l_logger("Camera");
	l_logger.debug() << "viewMat: " << m_viewMatrix.toStringPtr() << "\n";
	l_logger.debug() << "projMat: " << m_projectionMatrix.toStringPtr() << "\n";
	l_logger.debug() << "vprojMat: " << m_viewProjectionMatrix.toStringPtr() << "\n";
}

void cameraBase::setCameraUniforms(std::shared_ptr<shaderProgram> p_program)
{
	if (dirty) process();

	p_program->setUniforms("camera.position", vector4(m_cameraPosition, 1));
	p_program->setUniforms("camera.i_fog", m_fogColor);
	p_program->setUniforms("camera.z_far", m_farPlaneDist);
	p_program->setUniforms("camera.z_near", m_nearPlaneDist);
	p_program->setUniforms("ViewMatrix", m_viewMatrix);
	p_program->setUniforms("ProjectionMatrix", m_projectionMatrix);
}
