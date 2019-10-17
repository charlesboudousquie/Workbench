/*!***************************************************************************************
\file       main.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include <Engine.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "EngineController.hpp"
#include "EditorWindow.hpp"
#include "DataSelect.hpp"
#include <filesystem>
#include <windows.h>
#include "imguifilesystem.h"
#include <fstream>

#include "EditorLogger.hpp"
#include <InputSourceInterface.hpp>
#include <ExternalWindowInterface.hpp>
#include <RenderContextInterface.hpp>
#include <ComponentRepository.hpp>

#pragma comment(lib, "shlwapi.lib")

const std::string g_windowTitle("ToyBox Editor");

bool graphics_OnInitialize(GLFWwindow*& p_window, int& p_result)
{
	// Initialize glfw library
	if (!glfwInit())
	{
		p_result = -1;
		return true;
	}

	// create a windowed mode window and the opengl context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	p_window = glfwCreateWindow(1920, 1080, g_windowTitle.c_str(), NULL, NULL);
	if (!p_window)
	{
		glfwTerminate();
		p_result = -1;
		return true;
	}

	// make the context current
	glfwMakeContextCurrent(p_window);

	// initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		p_result = 1;
		return true;
	}
	return false;
}

void imgui_OnInitialize(GLFWwindow* window)
{
	// setup imgui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
	ImGui::StyleColorsDark();
}

void editor_OnStartFrame()
{
	// start imgui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void editor_OnEndFrame()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_OnShutdown()
{
	// shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void graphics_OnShutdown(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}


class externalWindowInterfaceAdapter : public externalWindowInterface
{
public:
	externalWindowInterfaceAdapter(GLFWwindow* p_window);

	void handleProcessEvents() override;
	void handleResizeRequest(int p_width, int p_height) override;
	void handleQuitRequest() override;
};

class renderContextInterfaceAdapter : public renderContextInterface
{
public:
	renderContextInterfaceAdapter(GLFWwindow* p_window);

	void resize(int width, int height);

	void handleSwapBuffersRequest() override;
	bool queryContextChanged() override;
	renderContextParameters queryContextParameters() override;
	void updateAssetPath(const std::string& newPath) override;
	void draw(const renderable& r) override;
	void updateCameraMatrices(const cameraData& c) override;
private:
	bool m_context_changed = true;
	renderContextParameters m_parameters;
};

class inputSourceInterfaceAdapter : public inputSourceInterface
{
public:
	inputSourceInterfaceAdapter(GLFWwindow* p_window);

	void setKeyboardCallback(keyboardCallbackFunction p_function_ptr) override;
	void setMousePosCallback(mousePosCallbackFunction p_function_ptr) override;
	void setMouseButtonCallback(mouseButtonCallbackFunction p_function_ptr) override;
	scancodeWrapper mapLetterToScancode(unsigned p_letter) override;
	scancodeWrapper mapMouseButtonToScancode(unsigned p_button_id) override;
	void setGamepadButtonCallback(gamepadButtonCallbackFunction p_function_ptr) override;
	void setGamepadAxisCallbackFunction(gamepadAxisCallbackFunction p_function_ptr) override;
};

void inputSourceInterfaceAdapter::setGamepadButtonCallback(gamepadButtonCallbackFunction p_function_ptr)
{
	(void)p_function_ptr;
}

void inputSourceInterfaceAdapter::setGamepadAxisCallbackFunction(gamepadAxisCallbackFunction p_function_ptr)
{
	(void)p_function_ptr;
}

//Forward declare
bool openProjectFile(const std::string & p_project_path, std::string & p_project_file_name, std::string & p_project_file_path, bool & p_dark_mode);
bool createProjectFile(const std::string & p_project_path, std::string & p_project_file_name, std::string & p_project_file_path, bool p_dark_mode);

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#else
int main(int, char **)
#endif
{
	GLFWwindow* l_window;
	int resultValue;

	// initialize everthing
	if (graphics_OnInitialize(l_window, resultValue))
		return resultValue;
	imgui_OnInitialize(l_window);

	// make the context current
	glfwMakeContextCurrent(l_window);

	// get the current directory path
	//std::string l_base_path;
	//HMODULE l_module_handle = GetModuleHandle(NULL);
	//if(l_module_handle != NULL)
	//{
	//	char l_exe_path[MAX_PATH] = { 0 };
	//	GetCurrentDirectory((sizeof(l_exe_path)), l_exe_path);
	//	//PathRemoveFileSpec(l_exe_path);
	//	l_base_path = l_exe_path;
	//}

	externalWindowInterfaceAdapter l_external_window(l_window);
	renderContextInterfaceAdapter l_render_context(l_window);
	inputSourceInterfaceAdapter l_input_source(l_window);

	bool m_show_welcome_window = true;
	bool m_show_project_options = true;
	bool m_showWelcomeOnce = true;
	bool m_show_file_system = false;
	bool m_show_folder_system = false;
	bool m_quit_selected = false;
	bool m_dark_mode = false;

	std::string l_project_file_name;
	std::string l_project_file_path;

	//Must go into iumgui.ini and change the position of [Window][Welcome]
	// Pos = 1200, 60
	Editor::EditorLogger l_logger;

	while (m_show_project_options)
	{
		// process events
		glfwPollEvents();

		// render
		glClear(GL_COLOR_BUFFER_BIT);

		editor_OnStartFrame();

		l_logger.Draw("Welcome");

		if (m_show_welcome_window)
		{
			if (m_showWelcomeOnce)
			{
				ImGui::OpenPopup("Please choose a project option.");
				m_showWelcomeOnce = false;
			}

			if (ImGui::BeginPopupModal("Please choose a project option.", NULL))
			{
				if (ImGui::Button("Open Existing Project.", ImVec2(250.0f, 20.0f)))
				{
					m_show_file_system = true;
					m_show_welcome_window = false;
				}

				if (ImGui::Button("Create New Project.", ImVec2(250.0f, 20.0f)))
				{
					m_show_folder_system = true;
					m_show_welcome_window = false;
				}

				if (ImGui::Button("Quit", ImVec2(250.0f, 20.0f)))
				{
					m_show_welcome_window = false;
					m_show_project_options = false;
					m_quit_selected = true;
				}

				ImGui::EndPopup();
			}
		}

		if (m_show_file_system)
		{
			static ImGuiFs::Dialog dlg;    // one per dialog (and must be static)
			static std::string chosenPath = "../../";
			std::string l_newPath;

			l_newPath = dlg.chooseFileDialog(m_show_file_system, chosenPath.c_str());

			l_newPath = dlg.getLastDirectory();

			if (l_newPath != chosenPath)
			{
				chosenPath = l_newPath;
			}

			// If you want to copy the (valid) returned path somewhere, you can use something like:
			static char myPath[4096];

			if (dlg.hasUserJustCancelledDialog())
			{
				m_show_file_system = false;
				m_showWelcomeOnce = true;
				m_show_welcome_window = true;
			}

			if (strlen(dlg.getChosenPath()) > 0)
			{
				strcpy(myPath, dlg.getChosenPath());
				//Make sure we are only opening up .prj files
				char l_ext[40];
				ImGuiFs::PathGetExtension(myPath, l_ext);

				if (strcmp(l_ext, ".prj") == 0)
				{
					std::string l_project(myPath);

					if (openProjectFile(l_project, l_project_file_name, l_project_file_path, m_dark_mode))
					{
						//m_project_folder_set = true;
						m_show_file_system = false;
						m_show_project_options = false;
					}
					else
					{
						l_logger.AddLog("[ERROR] Unable to open Project file!\n");
					}
				}
				else
				{
					l_logger.AddLog("[WARNING] %s is not a valid extension.\n Please select a .prj file.\n", l_ext);
				}
			}
		}

		static ImGuiFs::Dialog l_folderDlg;    // one per dialog (and must be static)
		static std::string l_chosenFolder("./");
		std::string l_newFolder;

		if (m_show_folder_system)
		{
			l_newFolder = l_folderDlg.chooseFolderDialog(m_show_folder_system, l_chosenFolder.c_str());             // see other dialog types and the full list of arguments for advanced usage

			l_newFolder = l_folderDlg.getLastDirectory();

			if (l_newFolder != l_chosenFolder)
			{
				l_chosenFolder = l_newFolder;
			}

			// If you want to copy the (valid) returned path somewhere, you can use something like:
			static char myPath[4096];

			if (l_folderDlg.hasUserJustCancelledDialog())
			{
				m_show_folder_system = false;
				m_show_welcome_window = true;
				m_showWelcomeOnce = true;
			}

			if (strlen(l_folderDlg.getChosenPath()) > 0)
			{
				strcpy(myPath, l_folderDlg.getChosenPath());

				//Create .prj file
				std::string l_path(myPath);

				if (createProjectFile(l_path, l_project_file_name, l_project_file_path, m_dark_mode))
				{
					//m_project_folder_set = true;
					m_show_folder_system = false;
					m_show_project_options = false;
				}
				else
				{
					l_logger.AddLog("[ERROR] Unable to create project file in that directory!\n");
				}

			}
		}

		editor_OnEndFrame();

		glfwSwapBuffers(l_window);
	}

	if (!m_quit_selected)
	{
		Editor::editorWindow l_editor_window(&l_external_window, &l_render_context, &l_input_source, l_project_file_path, l_project_file_name, m_dark_mode);

		ImVec2 p_sceneview_size(640, 480);

		// setup a FBO for background render-to-texture
		GLuint l_frameBufferID = 0;
		glGenFramebuffers(1, &l_frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, l_frameBufferID);
		// create the texture
		GLuint l_background_texture;
		glGenTextures(1, &l_background_texture);
		glBindTexture(GL_TEXTURE_2D, l_background_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		GLuint l_depth_render_buffer;
		glGenRenderbuffers(1, &l_depth_render_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, l_depth_render_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, l_depth_render_buffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, l_background_texture, 0);
		GLenum l_draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, l_draw_buffers);

		// loop until window closes
		while (l_editor_window.isRunning())
		{
			if (glfwWindowShouldClose(l_window))
			{
				glfwSetWindowShouldClose(l_window, 0);
				l_editor_window.requestShutdown();
			}
			// render engine to offscreeen
			glBindFramebuffer(GL_FRAMEBUFFER, l_frameBufferID);
			glBindTexture(GL_TEXTURE_2D, l_background_texture);
			l_render_context.resize(static_cast<int>(p_sceneview_size.x), static_cast<int>(p_sceneview_size.y));

			l_editor_window.getEngineController().processEvents();

			// switch back to the window context
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, 1024, 768);

			// process events
			glfwPollEvents();

			// render
			glClear(GL_COLOR_BUFFER_BIT); // is this complete?


			editor_OnStartFrame();
			l_editor_window.getSceneWindow().setImageSize(p_sceneview_size.x, p_sceneview_size.y);
			l_editor_window.getSceneWindow().setTextureID(reinterpret_cast<void*>(l_background_texture));
			l_editor_window.render();

			editor_OnEndFrame();
			glfwSwapBuffers(l_window);
		}

		l_editor_window.shutdown();
	}

	imgui_OnShutdown();
	graphics_OnShutdown(l_window);

	return 0;
}

externalWindowInterfaceAdapter::externalWindowInterfaceAdapter(GLFWwindow * p_window)
{
}

void externalWindowInterfaceAdapter::handleProcessEvents()
{
}

void externalWindowInterfaceAdapter::handleResizeRequest(int p_width, int p_height)
{
}

void externalWindowInterfaceAdapter::handleQuitRequest()
{
}

static renderContextParameters getWindowRect(GLFWwindow * p_window)
{
	renderContextParameters l_winDim;
	glfwGetWindowSize(p_window, &l_winDim.width, &l_winDim.height);
	return l_winDim;
}

renderContextInterfaceAdapter::renderContextInterfaceAdapter(GLFWwindow* p_window)
	: m_parameters{getWindowRect(p_window)}
{

}

void renderContextInterfaceAdapter::resize(int p_width, int p_height)
{
	if (m_parameters.width != p_width || m_parameters.height != p_height)
	{
		m_parameters.width = p_width;
		m_parameters.height = p_height;
		m_context_changed = true;
	}
}

void renderContextInterfaceAdapter::handleSwapBuffersRequest()
{
}

bool renderContextInterfaceAdapter::queryContextChanged()
{
	return m_context_changed;
}

renderContextParameters renderContextInterfaceAdapter::queryContextParameters()
{
	m_context_changed = false;
	return m_parameters;
}


void renderContextInterfaceAdapter::updateAssetPath(const std::string& newPath)
{
	(void)newPath;
}

void renderContextInterfaceAdapter::draw(const renderable& r)
{
	(void)r;
}

void renderContextInterfaceAdapter::updateCameraMatrices(const cameraData& c)
{
	(void)c;
}


inputSourceInterfaceAdapter::inputSourceInterfaceAdapter(GLFWwindow* p_window)
{
}

void inputSourceInterfaceAdapter::setKeyboardCallback(keyboardCallbackFunction p_function_ptr)
{
}

void inputSourceInterfaceAdapter::setMousePosCallback(mousePosCallbackFunction p_function_ptr)
{
}

void inputSourceInterfaceAdapter::setMouseButtonCallback(mouseButtonCallbackFunction p_function_ptr)
{
}

scancodeWrapper inputSourceInterfaceAdapter::mapLetterToScancode(unsigned p_letter)
{
	scancodeWrapper l_result = { 0 };
	l_result.scancode = p_letter;
	return l_result;
}

scancodeWrapper inputSourceInterfaceAdapter::mapMouseButtonToScancode(unsigned p_button_id)
{
	scancodeWrapper l_result = { 0 };
	l_result.scancode = p_button_id;
	return l_result;
}

bool openProjectFile(const std::string & p_project_path, std::string & p_project_file_name, std::string & p_project_file_path, bool & p_dark_mode)
{
	std::ifstream l_project(p_project_path);

	if (l_project.good())
	{
		//m_logger.AddLog("[EDITOR] Opening project file:\n %s\n", p_project_path.c_str());

		char l_result[4096];
		ImGuiFs::PathGetFileName(p_project_path.c_str(), l_result);
		char l_resultPath[4096];
		ImGuiFs::PathGetDirectoryName(p_project_path.c_str(), l_resultPath);

		p_project_file_name = l_result;
		p_project_file_path = l_resultPath;

		bool l_found = false;

		std::string l_line = "Style:";
		char search[5] = "dark";
		//auto l_buffer = l_project.rdbuf();
		for (unsigned int l_curLine = 0; std::getline(l_project, l_line); ++l_curLine)
		{
			if (l_line.find(search) != std::string::npos)
			{
				l_found = true;
				break;
			}
		}

		p_dark_mode = l_found;

		l_project.close();
	}
	else
	{
		//m_logger.AddLog("[ERROR] Failed to open project file:\n %s\n", p_project_path.c_str());
		return false;
	}

	return true;
}

bool createProjectFile(const std::string & p_project_path, std::string & p_project_file_name, std::string & p_project_file_path, bool p_dark_mode)
{
	//Get the end folder
	char l_return[4096];
	ImGuiFs::PathGetFileName(p_project_path.c_str(), l_return);
	std::string l_fileName(l_return);
	//l_fileName.insert(0, 1, '/');
	l_fileName.append(".prj");

	std::string l_fullPath = p_project_path + '/' + l_fileName;

	std::ifstream l_exisitingFile(l_fullPath);
	if (l_exisitingFile.good())
	{
		//m_logger.AddLog("[ERROR] Project File already exists here! Please choose an empty project folder.");
		return false;
	}

	std::ofstream l_file(l_fullPath);

	if (l_file.is_open())
	{
		l_file << "Project Path: " << l_fullPath << std::endl;
		//Save style
		if (p_dark_mode)
		{
			l_file << "Style: " << "dark" << std::endl;
		}
		else
		{
			l_file << "Style: " << "light" << std::endl;
		}

		l_file.close();

		p_project_file_path = p_project_path;
		p_project_file_name = l_fileName;

		//m_logger.AddLog("[EDITOR] Creating project file:\n %s", l_fullPath.c_str());
	}
	else
	{
		//m_logger.AddLog("[ERROR] Can not create project file in directory: %s.\n Please choose a writable directory.\n", p_project_path.c_str());
		return false;
	}

	//Project file creation success.
	return true;
}
