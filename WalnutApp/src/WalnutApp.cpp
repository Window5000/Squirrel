#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

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
		
		if(ImGui::BeginTabItem("Test1")) {
			Squirrel::BuyButton("Example1", 5, "An example", 10.0f, SquirrelConf::ItemList::Example1, SquirrelConf::Type::Test1);
		}
		
		if(ImGui::BeginTabItem("Test2")) {
			Squirrel::BuyButton("Example2", 10, "Another example", 15.0f, SquirrelConf::ItemList::Example2, SquirrelConf::Type::Test2);
		}
		
		Squirrel::End();
		//ImGui::ShowDemoWindow();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Squirrel::addItems();
	Squirrel::loadFile();
	
	Walnut::ApplicationSpecification spec;
	spec.Name = "Squirrel Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	
	Squirrel::MakeMenu();
	
	return app;
}
