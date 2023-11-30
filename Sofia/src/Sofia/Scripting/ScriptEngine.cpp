#include <pch.h>
#include "ScriptEngine.h"

#include "Sofia/Scripting/ScriptGlue.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/tabledefs.h>

namespace Sofia {

	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap = {
		{ "System.Single", ScriptFieldType::Float },
		{ "System.Double", ScriptFieldType::Double },
		{ "System.Boolean", ScriptFieldType::Bool },
		{ "System.Char", ScriptFieldType::Char },
		{ "System.Int16", ScriptFieldType::Short },
		{ "System.Int32", ScriptFieldType::Int },
		{ "System.Int64", ScriptFieldType::Long },
		{ "System.Byte", ScriptFieldType::Byte },
		{ "System.UInt16", ScriptFieldType::UShort },
		{ "System.UInt32", ScriptFieldType::UInt },
		{ "System.UInt64", ScriptFieldType::ULong },

		{ "Sofia.Vector2", ScriptFieldType::Vector2 },
		{ "Sofia.Vector3", ScriptFieldType::Vector3 },
		{ "Sofia.Vector4", ScriptFieldType::Vector4 },

		{ "Sofia.Entity", ScriptFieldType::Entity },
	};

	namespace Utils {

		static char* ReadBytes(const std::filesystem::path& filepath, _Out_ uint32_t& size)
		{
			std::ifstream file(filepath, std::ios::binary | std::ios::ate);
			if (!file)
				return nullptr;

			std::streampos end = file.tellg();
			file.seekg(0, std::ios::beg);
			uint32_t fileSize = (uint32_t)(end - file.tellg());

			if (fileSize == 0)
				return nullptr;

			char* buffer = new char[fileSize];
			file.read(buffer, fileSize);
			file.close();
			size = fileSize;
			return buffer;
		}
		static MonoAssembly* LoadAssembly(const std::filesystem::path& assemblyPath)
		{
			std::string path = assemblyPath.string();
			uint32_t fileSize = 0;
			char* data = ReadBytes(assemblyPath, fileSize);

			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(data, fileSize, 1, &status, 0);
			if (status != MONO_IMAGE_OK)
			{
				SOF_CORE_ERROR("Could not load assembly from {0}:\n{1}", path, mono_image_strerror(status));
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, path.c_str(), &status, 0);
			mono_image_close(image);

			delete[] data;
			return assembly;
		}
		static void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int i = 0; i < numTypes; ++i)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				SOF_CORE_TRACE("{0}.{1}", nameSpace, name);
			}
		}
		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			std::string typeName = mono_type_get_name(monoType);

			auto it = s_ScriptFieldTypeMap.find(typeName);
			if (it == s_ScriptFieldTypeMap.end())
			{
				SOF_CORE_ERROR("Unknown type: {0}", typeName);
				return ScriptFieldType::None;
			}

			return it->second;
		}
	}

	struct ScriptEngineData
	{
		MonoDomain* rootDomain = nullptr;
		MonoDomain* appDomain = nullptr;

		MonoAssembly* coreAssembly = nullptr;
		MonoImage* coreAssemblyImage = nullptr;

		MonoAssembly* appAssembly = nullptr;
		MonoImage* appAssemblyImage = nullptr;

		ScriptClass entityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> entityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> entityInstances;
		std::unordered_map<UUID, ScriptFieldMap> entityScriptFields;

		Scene* scene = nullptr;
	};
	static ScriptEngineData* s_ScriptEngineData = nullptr;

	void ScriptEngine::Init()
	{
		s_ScriptEngineData = new ScriptEngineData;

		mono_set_assemblies_path("mono/lib");
		s_ScriptEngineData->rootDomain = mono_jit_init("SofiaJIT");
		SOF_CORE_ASSERT(s_ScriptEngineData->rootDomain);

		LoadAssembly("assets/scripts/Sofia-ScriptCore.dll");
		LoadAppAssembly("assets/scripts/Binaries/Game.dll");
		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();

		s_ScriptEngineData->entityClass = ScriptClass("Sofia", "Entity", true);
		/*MonoObject* instance = s_ScriptEngineData->entityClass.Instantiate();

		MonoMethod* printMessageFunc = s_ScriptEngineData->entityClass.GetMethod("PrintMessage", 0);
		s_ScriptEngineData->entityClass.InvokeMethod(instance, printMessageFunc);

		MonoMethod* printIntFunc = s_ScriptEngineData->entityClass.GetMethod("PrintInt", 1);
		int value = 5;
		void* param = &value;
		s_ScriptEngineData->entityClass.InvokeMethod(instance, printIntFunc, &param);

		MonoMethod* printIntsFunc = s_ScriptEngineData->entityClass.GetMethod("PrintInts", 2);
		int value2 = 508;
		void* params[2] =
		{
			&value,
			&value2
		};
		s_ScriptEngineData->entityClass.InvokeMethod(instance, printIntsFunc, params);

		MonoString* monoString = mono_string_new(s_ScriptEngineData->appDomain, "Hello World from C++!");
		MonoMethod* printCustomMessageFunc = s_ScriptEngineData->entityClass.GetMethod("PrintMessage", 1);
		void* stringParam = monoString;
		s_ScriptEngineData->entityClass.InvokeMethod(instance, printCustomMessageFunc, &stringParam);*/
	}
	void ScriptEngine::Shutdown()
	{
		delete s_ScriptEngineData;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_ScriptEngineData->appDomain = mono_domain_create_appdomain((char*)"SofiaScriptRuntime", nullptr);
		mono_domain_set(s_ScriptEngineData->appDomain, true);

		s_ScriptEngineData->coreAssembly = Utils::LoadAssembly(filepath);
		s_ScriptEngineData->coreAssemblyImage = mono_assembly_get_image(s_ScriptEngineData->coreAssembly);
	}
	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath, bool isCore)
	{
		s_ScriptEngineData->appAssembly = Utils::LoadAssembly(filepath);
		s_ScriptEngineData->appAssemblyImage = mono_assembly_get_image(s_ScriptEngineData->appAssembly);
	}
	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_ScriptEngineData->scene = scene;
	}
	void ScriptEngine::OnRuntimeStop()
	{
		s_ScriptEngineData->scene = nullptr;
		s_ScriptEngineData->entityInstances.clear();
	}

	bool ScriptEngine::EntityClassExist(const std::string& className)
	{
		return s_ScriptEngineData->entityClasses.find(className) != s_ScriptEngineData->entityClasses.end();
	}
	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		auto& sc = entity.GetComponent<ScriptComponent>();
		if (EntityClassExist(sc.ClassName))
		{
			UUID id = entity.GetUUID();
			Ref<ScriptInstance> instance = Ref<ScriptInstance>::Create(s_ScriptEngineData->entityClasses[sc.ClassName], entity);
			s_ScriptEngineData->entityInstances[id] = instance;

			if (s_ScriptEngineData->entityScriptFields.find(id) != s_ScriptEngineData->entityScriptFields.end())
			{
				const ScriptFieldMap& fieldMap = s_ScriptEngineData->entityScriptFields.at(id);
				for (const auto& [name, fieldInstance] : fieldMap)
					instance->SetFieldValueInternal(name, fieldInstance.m_Buffer);
			}

			instance->InvokeOnCreate();
		}
	}
	void ScriptEngine::OnUpdateEntity(Entity entity, Timestep ts)
	{
		UUID id = entity.GetUUID();
		SOF_CORE_ASSERT(s_ScriptEngineData->entityInstances.find(id) != s_ScriptEngineData->entityInstances.end());
		auto instance = s_ScriptEngineData->entityInstances[id];
		instance->InvokeOnUpdate(ts);
	}

	Scene* ScriptEngine::GetScene()
	{
		return s_ScriptEngineData->scene;
	}
	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID entityID)
	{
		auto it = s_ScriptEngineData->entityInstances.find(entityID);
		if (it == s_ScriptEngineData->entityInstances.end())
			return nullptr;
		return it->second;
	}
	Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& name)
	{
		if (s_ScriptEngineData->entityClasses.find(name) == s_ScriptEngineData->entityClasses.end())
			return nullptr;
		return s_ScriptEngineData->entityClasses.at(name);
	}
	const std::unordered_map<std::string, Ref<ScriptClass>>& ScriptEngine::GetEntityClasses()
	{
		return s_ScriptEngineData->entityClasses;
	}
	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity)
	{
		SOF_CORE_ASSERT(entity);
		return s_ScriptEngineData->entityScriptFields[entity.GetUUID()];
	}
	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_ScriptEngineData->coreAssemblyImage;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_ScriptEngineData->appDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}
	void ScriptEngine::LoadAssemblyClasses()
	{
		s_ScriptEngineData->entityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_ScriptEngineData->appAssemblyImage, MONO_TABLE_TYPEDEF);
		int numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(s_ScriptEngineData->coreAssemblyImage, "Sofia", "Entity");

		for (int i = 0; i < numTypes; ++i)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_ScriptEngineData->appAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(s_ScriptEngineData->appAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{0}.{1}", nameSpace, name);
			else
				fullName = name;

			MonoClass* monoClass = mono_class_from_name(s_ScriptEngineData->appAssemblyImage, nameSpace, name);
			if (monoClass == entityClass)
				continue;

			if (!mono_class_is_subclass_of(monoClass, entityClass, false)) //isEntity
				continue;

			Ref<ScriptClass> scriptClass = Ref<ScriptClass>::Create(nameSpace, name);
			s_ScriptEngineData->entityClasses[fullName] = scriptClass;

			int fieldCount = mono_class_num_fields(monoClass);
			SOF_CORE_WARN("{0} has {1} fields:", name, fieldCount);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);
					SOF_CORE_WARN("  {0} ({1})", fieldName, Utils::ScriptFieldTypeToString(fieldType));

					scriptClass->m_Fields[fieldName] = { fieldType, fieldName, field };
				}
			}
		}
	}



	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_Class = mono_class_from_name(isCore ? s_ScriptEngineData->coreAssemblyImage : s_ScriptEngineData->appAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_Class);
	}
	MonoMethod* ScriptClass::GetMethod(const std::string& name, int argCount)
	{
		return mono_class_get_method_from_name(m_Class, name.c_str(), argCount);
	}
	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}



	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		: m_Class(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();
		m_Contructor = s_ScriptEngineData->entityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = s_ScriptEngineData->entityClass.GetMethod("OnCreate", 0);
		m_OnUpdateMethod = s_ScriptEngineData->entityClass.GetMethod("OnUpdate", 1);

		UUID id = entity.GetUUID();
		void* param = &id;
		m_Class->InvokeMethod(m_Instance, m_Contructor, &param);
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if (m_OnCreateMethod)
			m_Class->InvokeMethod(m_Instance, m_OnCreateMethod);
	}
	void ScriptInstance::InvokeOnUpdate(Timestep ts)
	{
		if (m_OnUpdateMethod)
		{
			void* param = &ts;
			m_Class->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
		}
	}
	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_Class->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_Instance, field.ClassField, buffer);
		return true;
	}
	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_Class->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_set_value(m_Instance, field.ClassField, (void*)value);
		return true;
	}
}
