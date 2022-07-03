#pragma once
#include "SquirrelConf.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Walnut/Application.h"

namespace Squirrel {
	bool docking = false;
	bool showfps = false;
	bool ws = false; // If the wipe save popup is active
	float clicks = 0.0f;
	float clickmtlr = 1.0f;
	float cps = 0.0f;

	//PriceBegin
	std::vector<unsigned int> gprice;
	void addPrices() {
		for (int i = 0; i < SquirrelConf::ItemList::Last; i++) gprice.push_back(0);
	}
	//PriceEnd

	//ItemsBegin
	std::vector<unsigned int> items;
	void addItems() {
		for (int i = 0; i < SquirrelConf::ItemList::Last; i++) items.push_back(0);
		addPrices();
	}
	//ItemsEnd

	void wipeSave() {
		clicks = 0.0f;
		clickmtlr = 1.0f;
		cps = 0.0f;
		for (int i = 0; i < items.size(); i++) items[i] = 0;
		if (remove("config.txt") != 0 || remove("items.txt") != 0)
			perror("Error wiping save\n");
		else
			puts("Save successfully wiped\n");
	}

	void loadFile() {
		std::string line;
		int i = 0;
		std::fstream config;
		config.open("config.txt");
		if (!config.is_open()) {
			std::cout << "[!!!] Failed loading file\n";
		}
		while (getline(config, line))
		{
			//std::cout << line << '\n';
			if (i == 0) clicks = std::stof(line);
			else if (i == 1) clickmtlr = std::stof(line);
			else if (i == 2) cps = std::stof(line);
			i++;
		}

		config.close();

		int b = 0;
		std::fstream settings;
		settings.open("settings.txt");
		if (!settings.is_open()) {
			std::cout << "[!!!] Failed loading file\n";
		}
		while (getline(settings, line))
		{
			//std::cout << line << '\n';
			if (b == 0) docking = line == "0" ? false : true;
			else if (b == 1) showfps = line == "0" ? false : true;
			b++;
		}
		settings.close();

		int c = 0;
		std::fstream itemsc;
		itemsc.open("items.txt");
		if (!itemsc.is_open()) {
			std::cout << "[!!!] Failed loading file\n";
		}
		while (getline(itemsc, line))
		{
			//std::cout << line << '\n';
			if (c < items.size()) {
				//std::cout << line << '\n';
				items[c] = std::stof(line);
			}
			c++;
		}
		itemsc.close();
	}
	
	void checkFiles() {
		std::fstream config("config.txt");
		if (!config.is_open()) {
			std::ofstream{ "config.txt" };
		}
		std::fstream itemsfile("items.txt");
		if (!itemsfile.is_open()) {
			std::ofstream{ "items.txt" };
		}
		std::fstream settings("settings.txt");
		if (!settings.is_open()) {
			std::ofstream{ "settings.txt" };
		}
	}

	void writeFile() {
		std::fstream config("config.txt");
		if (!config.is_open()) {
			std::cout << "[!!!] Failed loading file\n";
			std::ofstream{ "config.txt" };
		}
		config << clicks;
		config << "\n";
		config << clickmtlr;
		config << "\n";
		config << cps;
		config.close();

		std::fstream settings("settings.txt");
		if (!settings.is_open()) {
			std::cout << "[!!!] Failed loading file\n";
			std::ofstream{ "settings.txt" };
		}
		settings << docking;
		settings << "\n";
		settings << showfps;
		settings.close();

		std::fstream itemsc("items.txt");
		if (!itemsc.is_open()) {
			std::cout << "[!!!] Failed loading file\n";
			std::ofstream{ "items.txt" };
		}
		for (int i = 0; i < items.size(); i++) {
			if (i != 0) itemsc << "\n";
			itemsc << items[i];
		}
		itemsc.close();
	}

	namespace Buttons {
		void BuyButton(const char* name, int price, const char* description, float amount, SquirrelConf::ItemList item, SquirrelConf::Type type) {
			gprice[item] = price;

			std::ostringstream oss;
			oss << "($" << std::to_string(gprice[item]) << ") " << name;
			std::string cstr = oss.str();
			const char* itmname = cstr.c_str();
			if (ImGui::Button(itmname)) {
				if (clicks >= gprice[item]) {
					clicks -= gprice[item];
					SquirrelConf::doType(type, amount, &cps, &clickmtlr, &clicks);
					items[item]++;
				}
			}
			if (ImGui::IsItemHovered()) {
				std::ostringstream oss2;
				oss2 = SquirrelConf::doTypeDescription(type, amount, description);
				std::string cstr2 = oss2.str();
				const char* desc = cstr2.c_str();
				ImGui::SetTooltip(desc);
			}
			ImGui::SameLine();
			ImGui::Text("%u", items[item]);
		}
	}

	void MakeMenu(Walnut::Application* app) {
		app->SetMenubarCallback([app]()
			{
				if (ImGui::BeginMenu("Game"))
				{
					if (ImGui::MenuItem("Write File")) {
						Squirrel::writeFile();
					}
					if (ImGui::MenuItem("Load File")) {
						Squirrel::loadFile();
					}
					if (ImGui::MenuItem("Exit"))
					{
						app->Close();
					}
					if (ImGui::MenuItem("WIPE SAVE")) {
						Squirrel::ws = true;
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Settings")) {
					if (ImGui::MenuItem("Docking")) {
						Squirrel::docking = !Squirrel::docking;
					}
					if (ImGui::MenuItem("Show FPS")) {
						Squirrel::showfps = !Squirrel::showfps;
					}
					ImGui::EndMenu();
				}
			});
	}

	void Start() {
		if (Squirrel::cps != 0.0f) Squirrel::clicks += ((Squirrel::cps * Squirrel::clickmtlr) * ImGui::GetIO().DeltaTime);
		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->Pos.y + (main_viewport->Size.y - 500)));
		ImGui::SetNextWindowSize(ImVec2(500, 500));
		ImGui::Begin("ClickBtn", false, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground);
		ImGui::Text("Total CPS: %.2f", Squirrel::clickmtlr * Squirrel::cps);
		ImGui::Text("Click Multiplier: %.2fx  |  CPS: %.2f", Squirrel::clickmtlr, Squirrel::cps);
		if (ImGui::Button("CLICK", ImVec2(500, 500))) {
			Squirrel::clicks += Squirrel::clickmtlr;
		}
		ImGui::End();
		if (Squirrel::docking) ImGui::Begin("Clicks", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
		else ImGui::Begin("Clicks", false, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::Text("Clicks: %.2f", Squirrel::clicks);
		ImGui::End();
		if (Squirrel::showfps) {
			if (Squirrel::docking) ImGui::Begin("FPS", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
			else ImGui::Begin("FPS", false, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::Text("FPS: %.3f", ImGui::GetIO().Framerate);
			ImGui::End();
		}


		ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + (main_viewport->Size.x - 350), main_viewport->Pos.y + 25));
		ImGui::SetNextWindowSize(ImVec2(350, main_viewport->WorkSize.y - 25));
		if (Squirrel::docking) ImGui::Begin("Shop", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
		else ImGui::Begin("Shop", false, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		ImGui::BeginTabBar("Shop", tab_bar_flags);
	}

	void End() {
		ImGui::EndTabBar();


		ImGui::End();

		if (Squirrel::ws) {
			ImGui::Begin("Wipe Save", false,ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);

			ImGui::Text("Are you really sure that you want to wipe your save?");
			if (ImGui::Button("No")) Squirrel::ws = false;
			ImGui::SameLine();
			if (ImGui::Button("Yes")) {
				Squirrel::ws = false;
				Squirrel::wipeSave();
			}

			ImGui::End();
		}
	}
}
