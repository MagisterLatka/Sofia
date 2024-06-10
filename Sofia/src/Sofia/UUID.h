#pragma once

namespace Sofia {

	class UUID
	{
	public:
		SOF_CORE UUID();
		SOF_CORE UUID(uint64_t uuid) : m_ID(uuid) {}
		SOF_CORE UUID(const UUID&) = default;

		
		SOF_CORE operator uint64_t() const { return m_ID; }
	private:
		uint64_t m_ID;
	};
}

namespace std {

	template<>
	struct hash<Sofia::UUID>
	{
		std::size_t operator()(const Sofia::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}
