#pragma once

#include "Sofia/Scene/Scene.h"
#include "Sofia/Scene/Entity.h"
#include "Sofia/Timestep.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace Sofia {

	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity
	};
	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;
	};
	struct ScriptFieldInstance
	{
		friend class ScriptEngine;
		friend class ScriptInstance;

		ScriptField Field;

		ScriptFieldInstance() { memset(m_Buffer, 0, sizeof(m_Buffer)); }
		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type too large");
			return *(T*)m_Buffer;
		}
		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[16];
	};
	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class ScriptClass : public RefCounted
	{
		friend class ScriptEngine;
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int argCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }
	private:
		std::string m_ClassNamespace, m_ClassName;
		MonoClass* m_Class = nullptr;
		std::map<std::string, ScriptField> m_Fields;
	};
	class ScriptInstance : public RefCounted
	{
		friend class ScriptEngine;
		friend struct ScriptFieldInstance;
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(Timestep ts);

		Ref<ScriptClass> GetScriptClass() { return m_Class; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 16, "Type too large");
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();
			return *(T*)s_FieldValueBuffer;
		}
		template<typename T>
		void SetFieldValue(const std::string& name, T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large");
			SetFieldValueInternal(name, &value);
		}
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	private:
		Ref<ScriptClass> m_Class;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Contructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;

		inline static uint8_t s_FieldValueBuffer[16];
	};

	class ScriptEngine
	{
		friend class Application;
		friend class ScriptClass;
	public:
		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath, bool isCore = false);

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExist(const std::string& className);
		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, Timestep ts);

		static Scene* GetScene();
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID entityID);
		static Ref<ScriptClass> GetEntityClass(const std::string& name);
		static const std::unordered_map<std::string, Ref<ScriptClass>>& GetEntityClasses();
		static ScriptFieldMap& GetScriptFieldMap(Entity entity);

		static MonoImage* GetCoreAssemblyImage();
	private:
		static void Init();
		static void Shutdown();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();
	};

	namespace Utils {

		inline const char* ScriptFieldTypeToString(ScriptFieldType fieldType)
		{
			switch (fieldType)
			{
			case ScriptFieldType::None:		return "None";
			case ScriptFieldType::Float:	return "Float";
			case ScriptFieldType::Double:	return "Double";
			case ScriptFieldType::Bool:		return "Bool";
			case ScriptFieldType::Char:		return "Char";
			case ScriptFieldType::Byte:		return "Byte";
			case ScriptFieldType::Short:	return "Short";
			case ScriptFieldType::Int:		return "Int";
			case ScriptFieldType::Long:		return "Long";
			case ScriptFieldType::UByte:	return "UByte";
			case ScriptFieldType::UShort:	return "UShort";
			case ScriptFieldType::UInt:		return "UInt";
			case ScriptFieldType::ULong:	return "ULong";
			case ScriptFieldType::Vector2:	return "Vector2";
			case ScriptFieldType::Vector3:	return "Vector3";
			case ScriptFieldType::Vector4:	return "Vector4";
			case ScriptFieldType::Entity:	return "Entity";
			}
			SOF_CORE_THROW_INFO("Unknown ScriptFieldType");
			return "None";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType)
		{
			if (fieldType == "None")	return ScriptFieldType::None;
			if (fieldType == "Float")	return ScriptFieldType::Float;
			if (fieldType == "Double")	return ScriptFieldType::Double;
			if (fieldType == "Bool")	return ScriptFieldType::Bool;
			if (fieldType == "Char")	return ScriptFieldType::Char;
			if (fieldType == "Byte")	return ScriptFieldType::Byte;
			if (fieldType == "Short")	return ScriptFieldType::Short;
			if (fieldType == "Int")		return ScriptFieldType::Int;
			if (fieldType == "Long")	return ScriptFieldType::Long;
			if (fieldType == "UByte")	return ScriptFieldType::UByte;
			if (fieldType == "UShort")	return ScriptFieldType::UShort;
			if (fieldType == "UInt")	return ScriptFieldType::UInt;
			if (fieldType == "ULong")	return ScriptFieldType::ULong;
			if (fieldType == "Vector2")	return ScriptFieldType::Vector2;
			if (fieldType == "Vector3")	return ScriptFieldType::Vector3;
			if (fieldType == "Vector4")	return ScriptFieldType::Vector4;
			if (fieldType == "Entity")	return ScriptFieldType::Entity;

			SOF_CORE_THROW_INFO("Unknown ScriptFieldType");
			return ScriptFieldType::None;
		}
	}
}
