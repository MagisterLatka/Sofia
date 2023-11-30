#include <pch.h>
#include "SceneSerializer.h"

#include "Sofia/Scene/Scene.h"
#include "Sofia/Scene/Entity.h"
#include "Sofia/Scene/Components.h"
#include "Sofia/Scripting/ScriptEngine.h"
#include "Sofia/UUID.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			return node;
		}
		static bool decode(const Node& node, glm::vec2& value)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::uvec2>
	{
		static Node encode(const glm::uvec2& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			return node;
		}
		static bool decode(const Node& node, glm::uvec2& value)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			value.x = node[0].as<uint32_t>();
			value.y = node[1].as<uint32_t>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			node.push_back(value.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& value)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			value.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			node.push_back(value.z);
			node.push_back(value.w);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& value)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			value.z = node[2].as<float>();
			value.w = node[3].as<float>();
			return true;
		}
	};
	template<>
	struct convert<Sofia::UUID>
	{
		static Node encode(const Sofia::UUID& value)
		{
			Node node;
			node.push_back((uint64_t)value);
			return node;
		}
		static bool decode(const Node& node, Sofia::UUID& value)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			value = node.as<uint64_t>();
			return true;
		}
	};
}
namespace Sofia {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::uvec2& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
		return out;
	}

#define WriteScriptField(FieldType, Type)\
	case ScriptFieldType::FieldType:\
		out << scriptField.GetValue<Type>();\
		break;
#define ReadScriptField(FieldType, Type)\
	case ScriptFieldType::FieldType:\
	{\
		Type data = scriptField["Data"].as<Type>();\
		fieldInstance.SetValue(data);\
		break;\
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		SOF_CORE_ASSERT(entity.HasComponent<IDComponent>());

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "Tag component";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "Transform component";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << tc.Position;
			out << YAML::Key << "Orientation" << YAML::Value << tc.Orientation;
			out << YAML::Key << "Size" << YAML::Value << tc.Size;

			out << YAML::EndMap; // TransformComponent
		}
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "Camera component";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();

			if (cameraComponent.Camera.CanConvert<OrthographicCamera>())
			{
				auto camera = cameraComponent.Camera.As<OrthographicCamera>();
				out << YAML::Key << "Type" << YAML::Value << "Orthographic";
				out << YAML::Key << "Size" << YAML::Value << camera->GetSize();
				out << YAML::Key << "Aspect ratio" << YAML::Value << camera->GetAspectRatio();
				out << YAML::Key << "Near clip" << YAML::Value << camera->GetNearClip();
				out << YAML::Key << "Far clip" << YAML::Value << camera->GetFarClip();
			}

			out << YAML::EndMap; // CameraComponent
		}
		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::Key << "Sprite component";
			out << YAML::BeginMap; // SpriteComponent

			auto& sc = entity.GetComponent<SpriteComponent>();
			out << YAML::Key << "Color" << YAML::Value << sc.Color;
			if (sc.Texture)
			{
				out << YAML::Key << "Texture";
				out << YAML::BeginMap; //Texture

				auto& props = sc.Texture->GetProps();
				std::string path = props.Filepath.empty() ? std::string() : props.Filepath.string();
				out << YAML::Key << "Path" << YAML::Value << path;
				out << YAML::Key << "Size" << YAML::Value << glm::uvec2(props.Width, props.Height); //TODO: do it in asset manager
				out << YAML::Key << "Format" << YAML::Value << (int)props.Format;
				out << YAML::Key << "GenerateMips" << YAML::Value << props.GenerateMipMaps;
				out << YAML::Key << "Sampling" << YAML::Value << (int)props.Sampling;
				out << YAML::Key << "MaxAnisotropy" << YAML::Value << props.MaxAnisotropy;
				out << YAML::Key << "Wrap" << YAML::Value << (int)props.Wrap;
				out << YAML::Key << "BorderColor" << YAML::Value << props.BorderColor;

				out << YAML::EndMap; //Texture
			}
			out << YAML::Key << "Tilling factor" << YAML::Value << sc.TillingFactor;

			out << YAML::EndMap; // SpriteComponent
		}
		if (entity.HasComponent<CircleComponent>())
		{
			out << YAML::Key << "Circle component";
			out << YAML::BeginMap; // CircleComponent

			auto& cc = entity.GetComponent<CircleComponent>();
			out << YAML::Key << "Color" << YAML::Value << cc.Color;
			out << YAML::Key << "Thickness" << YAML::Value << cc.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << cc.Fade;

			out << YAML::EndMap; //CircleComponent
		}
		if (entity.HasComponent<ScriptComponent>())
		{
			out << YAML::Key << "Script component";
			out << YAML::BeginMap; //ScriptComponent

			auto& sc = entity.GetComponent<ScriptComponent>();
			out << YAML::Key << "Class name" << YAML::Value << sc.ClassName;

			Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(sc.ClassName);
			const auto& fields = entityClass->GetFields();
			if (fields.size() > 0)
			{
				out << YAML::Key << "Script Fields" << YAML::Value;
				auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
				out << YAML::BeginSeq;
				for (const auto& [name, field] : fields)
				{
					if (entityFields.find(name) == entityFields.end())
						continue;

					out << YAML::BeginMap; // ScriptField
					out << YAML::Key << "Name" << YAML::Value << name;
					out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);

					out << YAML::Key << "Data" << YAML::Value;
					ScriptFieldInstance& scriptField = entityFields.at(name);
					switch (field.Type)
					{
						WriteScriptField(Float,		float);
						WriteScriptField(Double,	double);
						WriteScriptField(Bool,		bool);
						WriteScriptField(Char,		char);
						WriteScriptField(Byte,		int8_t);
						WriteScriptField(Short,		int16_t);
						WriteScriptField(Int,		int32_t);
						WriteScriptField(Long,		int64_t);
						WriteScriptField(UByte,		uint8_t);
						WriteScriptField(UShort,	uint16_t);
						WriteScriptField(UInt,		uint32_t);
						WriteScriptField(ULong,		uint64_t);
						WriteScriptField(Vector2,	glm::vec2);
						WriteScriptField(Vector3,	glm::vec3);
						WriteScriptField(Vector4,	glm::vec4);
						WriteScriptField(Entity,	UUID);
					}
					out << YAML::EndMap; // ScriptField
				}
				out << YAML::EndSeq;
			}

			out << YAML::EndMap; //ScriptComponent
		}

		out << YAML::EndMap; // Entity
	}
	void SceneSerializer::Serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_Name;
		out << YAML::Key << "Scene ID" << YAML::Value << m_Scene->m_ID;
		out << YAML::Key << "Scene camera" << YAML::Value << Entity(m_Scene->m_Camera, m_Scene.Raw()).GetUUID();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.Raw() };
			if (!entity)
				return;
			if (entity.HasComponent<Scene::SceneComponent>())
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		YAML::Node data = YAML::LoadFile(filepath.string());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		SOF_CORE_TRACE("Deserializing scene '{0}'", sceneName);
		m_Scene->m_Name = sceneName;
		m_Scene->m_ID = data["Scene ID"].as<uint32_t>();
		uint64_t sceneCamera = data["Scene camera"].as<uint64_t>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["Tag component"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				SOF_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);
				
				if (auto transformComponent = entity["Transform component"])
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Position = transformComponent["Position"].as<glm::vec3>();
					tc.Orientation = transformComponent["Orientation"].as<glm::vec3>();
					tc.Size = transformComponent["Size"].as<glm::vec3>();
				}
				if (auto cameraComponent = entity["Camera component"])
				{
					std::string cameraType = cameraComponent["Type"].as<std::string>();
					if (cameraType == "Orthographic")
					{
						float size = cameraComponent["Size"].as<float>();
						float aspectRatio = cameraComponent["Aspect ratio"].as<float>();
						float nearClip = cameraComponent["Near clip"].as<float>();
						float farClip = cameraComponent["Far clip"].as<float>();
						Ref<OrthographicCamera> camera = Ref<OrthographicCamera>::Create(aspectRatio, nearClip, farClip);
						camera->SetSize(size);
						deserializedEntity.AddComponent<CameraComponent>(camera);
					}
				}
				if (auto spriteComponent = entity["Sprite component"])
				{
					auto& sc = deserializedEntity.AddComponent<SpriteComponent>();
					sc.Color = spriteComponent["Color"].as<glm::vec4>();
					if (auto camera = spriteComponent["Texture"])
					{
						Texture2DProps props;
						props.Filepath = camera["Path"].as<std::string>();
						glm::uvec2 size = camera["Size"].as<glm::uvec2>();
						props.Width = size.x;
						props.Height = size.y;
						props.Format = (TextureFormat)camera["Format"].as<int>();
						props.GenerateMipMaps = camera["GenerateMips"].as<bool>();
						props.Sampling = (TextureSampling)camera["Sampling"].as<int>();
						props.MaxAnisotropy = camera["MaxAnisotropy"].as<int>();
						props.Wrap = (TextureWrap)camera["Wrap"].as<int>();
						props.BorderColor = camera["BorderColor"].as<glm::vec4>();
						sc.Texture = Texture2D::Create(props);
					}
					sc.TillingFactor = spriteComponent["Tilling factor"].as<float>();
				}
				if (auto circleComponent = entity["Circle component"])
				{
					auto& cc = deserializedEntity.AddComponent<CircleComponent>();
					cc.Color = circleComponent["Color"].as<glm::vec4>();
					cc.Thickness = circleComponent["Thickness"].as<float>();
					cc.Fade = circleComponent["Fade"].as<float>();
				}
				if (auto scriptComponent = entity["Script component"])
				{
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
					sc.ClassName = scriptComponent["Class name"].as<std::string>();

					if (auto scriptFields = scriptComponent["Script Fields"])
					{
						Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(sc.ClassName);
						SOF_CORE_ASSERT(entityClass);
						const auto& fields = entityClass->GetFields();
						auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);

						for (auto scriptField : scriptFields)
						{
							std::string name = scriptField["Name"].as<std::string>();
							std::string typeString = scriptField["Type"].as<std::string>();
							ScriptFieldType type = Utils::ScriptFieldTypeFromString(typeString);
							ScriptFieldInstance& fieldInstance = entityFields[name];
							if (fields.find(name) == fields.end())
							{
								SOF_CORE_WARN("Class {0} has no field called \"{1}\" (type: {2})", sc.ClassName, name, typeString);
								continue;
							}

							fieldInstance.Field = fields.at(name);
							switch (type)
							{
								ReadScriptField(Float,		float);
								ReadScriptField(Double,		double);
								ReadScriptField(Bool,		bool);
								ReadScriptField(Char,		char);
								ReadScriptField(Byte,		int8_t);
								ReadScriptField(Short,		int16_t);
								ReadScriptField(Int,		int32_t);
								ReadScriptField(Long,		int64_t);
								ReadScriptField(UByte,		uint8_t);
								ReadScriptField(UShort,		uint16_t);
								ReadScriptField(UInt,		uint32_t);
								ReadScriptField(ULong,		uint64_t);
								ReadScriptField(Vector2,	glm::vec2);
								ReadScriptField(Vector3,	glm::vec3);
								ReadScriptField(Vector4,	glm::vec4);
								ReadScriptField(Entity,		UUID);
							}
						}
					}
				}
				
				if (uuid == sceneCamera)
					m_Scene->SetCameraEntity(deserializedEntity);
			}
		}
		return true;
	}
}
