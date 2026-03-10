#include <stdexcept>
#include <cstring>
#include <iostream>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "Benchmark.h"
#include <cfloat>


std::vector<float> g_ex1Times;

namespace
{
	std::vector<float> g_ex1Times;
	int g_runCount = 7;  

	void DrawBenchmarkWindow()
	{
		ImGui::Begin("Exercise 1");

		ImGui::InputInt("Number of Runs", &g_runCount);

		
		if (g_runCount < 3)
			g_runCount = 3;

		if (ImGui::Button("Run benchmark"))
		{
			g_ex1Times = RunExercise1Benchmark(g_runCount);
		}

		if (!g_ex1Times.empty())
		{
			ImGui::PlotLines(
				"Time (ms)",
				g_ex1Times.data(),
				static_cast<int>(g_ex1Times.size()),
				0,
				nullptr,
				0.0f,
				FLT_MAX,
				ImVec2(0, 150)
			);
		}

		ImGui::End();
	}
}

namespace
{
	int g_runCount2 = 5;

	std::vector<float> g_ex2_go3d;
	std::vector<float> g_ex2_go3dAlt;


	int g_selected = -1;

	void DrawExercise2Window()
	{
		ImGui::Begin("Exercise 2");

		ImGui::InputInt("# samples", &g_runCount2);
		if (g_runCount2 < 3) g_runCount2 = 3;

		if (ImGui::Selectable("Thrash the cache with GameObject3D", g_selected == 0))
		{
			g_selected = 0;

			g_ex2_go3d = RunExercise2Benchmark_GameObject3D(g_runCount2);
		}

		if (ImGui::Selectable("Thrash the cache with GameObject3DAlt", g_selected == 1))
		{


			g_selected = 1;
			g_ex2_go3dAlt = RunExercise2Benchmark_GameObject3DAlt(g_runCount2);

		}
		if (!g_ex2_go3d.empty())
		{
			ImGui::Text("GameObject3D");
			ImGui::PlotLines(
				"##go3d",
				g_ex2_go3d.data(),
				static_cast<int>(g_ex2_go3d.size()),
				0,
				nullptr,
				0.0f,
				FLT_MAX,
				ImVec2(0, 150)
			);
		}

		if (!g_ex2_go3dAlt.empty())
		{
			ImGui::Text("GameObject3DAlt");
			ImGui::PlotLines(
				"##go3dalt",
				g_ex2_go3dAlt.data(),
				static_cast<int>(g_ex2_go3dAlt.size()),
				0,
				nullptr,
				0.0f,
				FLT_MAX,
				ImVec2(0, 150)
			);
			
		}

		ImGui::End();
	}
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

#if defined(__EMSCRIPTEN__)
	m_renderer = SDL_CreateRenderer(window, nullptr);
#else
	m_renderer = SDL_CreateRenderer(window, nullptr);
#endif

	if (m_renderer == nullptr)
	{
		std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#if __EMSCRIPTEN__
	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	io.IniFilename = NULL;
#endif

	ImGui_ImplSDL3_InitForSDLRenderer(window, m_renderer);
	ImGui_ImplSDLRenderer3_Init(m_renderer);
}

void dae::Renderer::Render() const
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	DrawBenchmarkWindow();
	DrawExercise2Window();
	//ImGui::ShowDemoWindow(); // For demonstration purposes, do not keep this in your engine

	ImGui::Render();

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
