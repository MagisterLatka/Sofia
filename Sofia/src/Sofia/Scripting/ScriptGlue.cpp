#include <pch.h>
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include "Sofia/Input.h"

#define SOF_ADD_INTERNAL_CALL(name) mono_add_internal_call("Sofia.InternalCalls::"#name, name);

namespace Sofia {

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

	static void NativeLog(MonoString* string, int value)
	{
		char* str = mono_string_to_utf8(string);
		std::string text(str);
		mono_free(str);
		std::cout << text << ", " << value << std::endl;
	}
	static void NativeLog_Vector(glm::vec3* value, glm::vec3* result)
	{
		SOF_CORE_WARN("Value: {0}, {1}, {2}", value->x, value->y, value->z);
		*result = glm::normalize(*value);
	}
	static float NativeLog_VectorDot(glm::vec3* value)
	{
		SOF_CORE_WARN("Value: {0}, {1}, {2}", value->x, value->y, value->z);
		return glm::dot(*value, *value);
	}

	static bool Entity_HasComponent(UUID id, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetScene();
		SOF_CORE_ASSERT(scene);
		Entity entity = scene->GetEntity(id);
		SOF_CORE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		SOF_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}
	static void TransformComponent_GetPosition(UUID id, glm::vec3* position)
	{
		Scene* scene = ScriptEngine::GetScene();
		SOF_CORE_ASSERT(scene);
		Entity entity = scene->GetEntity(id);
		SOF_CORE_ASSERT(entity);

		*position = entity.GetTransformComponent().Position;
	}
	static void TransformComponent_SetPosition(UUID id, glm::vec3* position)
	{
		Scene* scene = ScriptEngine::GetScene();
		SOF_CORE_ASSERT(scene);
		Entity entity = scene->GetEntity(id);
		SOF_CORE_ASSERT(entity);

		entity.GetTransformComponent().Position = *position;
	}

	static bool Input_IsKeyDown(KeyCode code)
	{
		return Input::IsKeyPressed(code);
	}

	template<typename ...Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			uint64_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = fmt::format("Sofia.{0}", structName);

			MonoType* type = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!type)
			{
				SOF_CORE_ERROR("Could not find component type {0}", managedTypename);
				return;
			}
			s_EntityHasComponentFuncs[type] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}
	template<typename ...Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}
	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		SOF_ADD_INTERNAL_CALL(NativeLog);
		SOF_ADD_INTERNAL_CALL(NativeLog_Vector);
		SOF_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		SOF_ADD_INTERNAL_CALL(Entity_HasComponent);
		SOF_ADD_INTERNAL_CALL(TransformComponent_GetPosition);
		SOF_ADD_INTERNAL_CALL(TransformComponent_SetPosition);

		SOF_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}
