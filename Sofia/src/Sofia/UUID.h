#pragma once

namespace Sofia {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid) : m_ID(uuid) {}
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_ID; }
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
