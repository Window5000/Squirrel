#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Squirrel.h"

#include "Walnut/Image.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnDetach() override {
		Squirrel::writeFile();
	}

	virtual void OnUIRender() override
	{
		Squirrel::Start();
		
		if(ImGui::BeginTabItem("AutoClickers")) {
			Squirrel::BuyButton("Example1", 5, "An example", 10.0f, SquirrelConf::ItemList::Example1, Squirrel::Type::AutoClicker);

			ImGui::EndTabItem();
		}
		
		if(ImGui::BeginTabItem("Multipliers")) {
			Squirrel::BuyButton("Example2", 10, "Another example", 15.0f, SquirrelConf::ItemList::Example2, Squirrel::Type::Multiplier);

			ImGui::EndTabItem();
		}
		
		Squirrel::End();
		//ImGui::ShowDemoWindow();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Squirrel::addItems();
	Squirrel::CheckFiles();
	Squirrel::loadFile();
	
	Walnut::ApplicationSpecification spec;
	spec.Name = "Squirrel Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	
	Squirrel::MakeMenu(app);
	
	return app;
}
