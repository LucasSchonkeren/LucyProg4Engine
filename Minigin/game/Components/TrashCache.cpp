#include "TrashCache.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui_plot.h"

#include <chrono>
#include <algorithm>
#include <numeric>

struct transform {
	float matrix[16]{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
};

class gameObject3d {
public:
	transform local;
	int id;
};

class gameObject3dAlt {
public:
	transform* local;
	int id;
};

void cpt::TrashCacheImgui1::RenderImgui() {
	// Main body of the window starts here.
	if (!ImGui::Begin("Exercise 1")) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::InputInt("# samples", &m_SampleCount);

	if (!m_Calculating and ImGui::Button("Trash the cache")) m_Calculating = true;
	else if (m_Calculating) {
		ImGui::Text("Wait for it...");

		std::vector<int> arr(67'000'000);
		for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2) {
			std::vector<int> subMeasurements{};
			for (int sampleId{}; sampleId < m_SampleCount; ++sampleId) {
				auto start = std::chrono::high_resolution_clock::now();
				for (int i{ 0 }; i < static_cast<int>(arr.size()); i += stepSize) {
					arr[i] *= 2;
				}
				subMeasurements.emplace_back(static_cast<int>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count()));
			};
			std::ranges::sort(subMeasurements);
			std::rotate(subMeasurements.begin(), subMeasurements.begin() + 1, subMeasurements.end());
			subMeasurements.pop_back();
			subMeasurements.pop_back();

			m_PlotValues.emplace_back(static_cast<float>(std::accumulate(subMeasurements.begin(), subMeasurements.end(), 0) / subMeasurements.size()) / 1'000'000.f);

			m_Calculating = false;
		};
	}

	if (!m_PlotValues.empty()) {
		ImGui::PlotConfig config{
			ImGui::PlotConfig::Values{
				nullptr, m_PlotValues.data(), static_cast<int>(m_PlotValues.size())
			},
			ImGui::PlotConfig::Scale{
			0, 60
			},
			ImGui::PlotConfig::Tooltip{},
			ImGui::PlotConfig::Grid{}, ImGui::PlotConfig::Grid{},
			ImGui::PlotConfig::Selection{},
			ImGui::PlotConfig::VerticalLines{},
			ImVec2(200.f, 150.f)
		};

		ImGui::Plot("Trash the cache plot", config);
	}

	ImGui::End();
}

void cpt::TrashCacheImgui2::RenderImgui() {
		// Main body of the window starts here.
	if (!ImGui::Begin("Exercise 2")) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::InputInt("# samples", &m_SampleCount);

	if (!m_Calculating and ImGui::Button("Trash the cache with GameObject3D")) m_Calculating = true;
	else if (m_Calculating) {
		ImGui::Text("Wait for it...");

		std::vector<gameObject3d> arr(3'000'000);
		for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2) {
			std::vector<int> subMeasurements{};
			for (int sampleId{}; sampleId < m_SampleCount; ++sampleId) {
				auto start = std::chrono::high_resolution_clock::now();
				for (int i{ 0 }; i < static_cast<int>(arr.size()); i += stepSize) {
					arr[i].id *= 2;
				}
				subMeasurements.emplace_back(static_cast<int>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count()));
			};
			std::ranges::sort(subMeasurements);
			std::rotate(subMeasurements.begin(), subMeasurements.begin() + 1, subMeasurements.end());
			subMeasurements.pop_back();
			subMeasurements.pop_back();

			m_PlotValues.emplace_back(static_cast<float>(std::accumulate(subMeasurements.begin(), subMeasurements.end(), 0) / subMeasurements.size()) / 1'000'000.f);

			m_Calculating = false;
		};
	}

	if (!m_PlotValues.empty()) {
		ImGui::PlotConfig config{
			ImGui::PlotConfig::Values{
				nullptr, m_PlotValues.data(), static_cast<int>(m_PlotValues.size())
			},
			ImGui::PlotConfig::Scale{
			0, 30
			},
			ImGui::PlotConfig::Tooltip{},
			ImGui::PlotConfig::Grid{}, ImGui::PlotConfig::Grid{},
			ImGui::PlotConfig::Selection{},
			ImGui::PlotConfig::VerticalLines{},
			ImVec2(200.f, 150.f)
		};

		ImGui::Plot("Trash the cache plot", config);
	}

	if (!m_CalculatingAlt and ImGui::Button("Trash the cache with GameObject3DAlt")) m_CalculatingAlt = true;
	else if (m_CalculatingAlt) {
		ImGui::Text("Wait for it...");

		std::vector<gameObject3dAlt> arr(3'000'000);
		for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2) {
			std::vector<int> subMeasurements{};
			for (int sampleId{}; sampleId < m_SampleCount; ++sampleId) {
				auto start = std::chrono::high_resolution_clock::now();
				for (int i{ 0 }; i < static_cast<int>(arr.size()); i += stepSize) {
					arr[i].id *= 2;
				}
				subMeasurements.emplace_back(static_cast<int>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count()));
			};
			std::ranges::sort(subMeasurements);
			std::rotate(subMeasurements.begin(), subMeasurements.begin() + 1, subMeasurements.end());
			subMeasurements.pop_back();
			subMeasurements.pop_back();

			m_PlotValuesAlt.emplace_back(static_cast<float>(std::accumulate(subMeasurements.begin(), subMeasurements.end(), 0) / subMeasurements.size()) / 1'000'000.f);

			m_CalculatingAlt = false;
		};
	}

	if (!m_PlotValuesAlt.empty()) {
		ImGui::PlotConfig config{
			ImGui::PlotConfig::Values{
				nullptr, m_PlotValuesAlt.data(), static_cast<int>(m_PlotValues.size())
			},
			ImGui::PlotConfig::Scale{
			0, 30
			},
			ImGui::PlotConfig::Tooltip{},
			ImGui::PlotConfig::Grid{}, ImGui::PlotConfig::Grid{},
			ImGui::PlotConfig::Selection{},
			ImGui::PlotConfig::VerticalLines{},
			ImVec2(200.f, 150.f)
		};

		ImGui::Plot("Trash the cache alt plot", config);
	}

//	if (!m_PlotValues.empty() and !m_PlotValuesAlt.empty()) {
//		ImGui::Text("Combined:");
//
//		std::vector<float*> ys{ m_PlotValues.data(), m_PlotValuesAlt.data() };
//
//		ImGui::PlotConfig config{
//			ImGui::PlotConfig::Values{
//				nullptr, nullptr, static_cast<int>(m_PlotValues.size()), 0, 0,
//				ys.data(), 2
//			},
//			ImGui::PlotConfig::Scale{
//			0, 30
//			},
//			ImGui::PlotConfig::Tooltip{},
//			ImGui::PlotConfig::Grid{}, ImGui::PlotConfig::Grid{},
//			ImGui::PlotConfig::Selection{},
//			ImGui::PlotConfig::VerticalLines{},
//			ImVec2(200.f, 150.f)
//		};
//
//		ImGui::Plot("Trash the cache combined plot", config);
//	}
//
	ImGui::End();
}
