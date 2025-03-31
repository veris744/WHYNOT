#include "SerializeButton.h"

#include <fstream>
#include <Managers/World.h>
#include <UI/Containers/Panel.h>
#include <UI/Text/Text.h>
#include <Utils/Debugger.h>
#include <Utils/Parser.h>


SerializeButton::SerializeButton(const vec2& _pos, const vec2& _size, const string& _name)
    : Button(_pos, _size, _name)
{

}

void SerializeButton::Initialize()
{
    Button::Initialize();

    std::shared_ptr<Panel> background = std::make_shared<Panel>();
    background->autoSizing = AutoSizing::ALL;
    std::shared_ptr<Text> text = std::make_shared<Text>();
    text->SetText("Serialize");
    text->align = TextAlign::CENTER;

    AddWidget(background);
    AddWidget(text);
}

YAML::Node SerializeButton::GenerateYamlContent()
{
    YAML::Node YamlContent = YAML::Node();
    YamlContent["entities"] = YAML::Node(YAML::NodeType::Sequence);
    for (const auto& [name, entity] : World::GetInstance()->GetEntities())
    {
        YAML::Node entityNode;
        entityNode["name"] = entity->GetName();
        const auto* typeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(typeid(*entity.get()).name()));
        if (typeInfo != nullptr)
        {
            entityNode["type"] = typeInfo->type_name;
        }

        entityNode["components"] = YAML::Node(YAML::NodeType::Sequence);
        for (const auto& component : entity->GetComponents()) {
            YAML::Node componentNode = GenerateObjectYAML(component.get());
            entityNode["components"].push_back(componentNode);
        }

        YamlContent["entities"].push_back(entityNode);
    }
    return YamlContent;
}

YAML::Node SerializeButton::GenerateObjectYAML(ReflectedObject* object)
{
    const auto* typeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(typeid(*object).name()));
    YAML::Node objectNode;
    if (typeInfo)
    {
        for (const auto& member : typeInfo->members)
        {
            const auto* memberTypeInfo = TypeRegistry::instance().getTypeInfo(Reader::demangleTypeName(member.type_name));

            if (memberTypeInfo && HasProperty(member.properties, MemberProperty::Serializable))
            {
                std::any a = member.getter(object);
                ReflectedObject* object = std::any_cast<ReflectedObject*>(a);
                objectNode[member.name] = GenerateObjectYAML(object);
            }
            else if (HasProperty(member.properties, MemberProperty::Serializable))
            {
                objectNode[member.name] = Parser::ParseValue(member.getter(object), member.type_name);
            }
        }
    }
    return objectNode;
}

string SerializeButton::GenerateFile(const YAML::Node& node)
{
    YAML::Emitter out;
    out << node;

    string filename = "serialize.yaml";

    std::ofstream file(filename);
    if (file.is_open()) {
        file << out.c_str();
        file.close();
        Logger::Log<SerializeButton>(LogLevel::Info, "File serialized successfully in " + filename);
        Debugger::DrawTextDebug("File serialized successfully in " + filename, vec3{0,1,0}, 3);
        return filename;
    }
    Logger::Log<SerializeButton>(LogLevel::Info, "Failed write YAML in file");
    return "";
}

void SerializeButton::OnClick(vec2 _mousePos)
{
    Button::OnClick(_mousePos);

    YAML::Node node = GenerateYamlContent();
    string nameFile = GenerateFile(node);
}
