#include <pch.h>
#include "DXGIInfoManager.h"

#include "Sofia/Platform/DX11/DX11Context.h"
#include "Sofia/Platform/Windows/WindowsWindow.h"

void DxgiInfoManager::Init()
{
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	const auto hModDxgiDebug = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
		throw SOF_WINDOWS_WINDOW_LAST_EXCEPTION();

	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));
	if (DxgiGetDebugInterface == nullptr)
		throw SOF_WINDOWS_WINDOW_LAST_EXCEPTION();

	HRESULT hr;
	SOF_DX_GRAPHICS_CALL(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &m_DxgiInfoQueue));
}

void DxgiInfoManager::Set() noexcept
{
	m_Next = m_DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::string DxgiInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = m_DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	if (m_Next == end) return {};
	for (auto i = m_Next; i < end; ++i)
	{
		HRESULT hr;
		uint64_t messageLength = 0;
		SOF_DX_GRAPHICS_CALL(m_DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

		auto bytes = std::make_unique<byte[]>(messageLength);
		auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		SOF_DX_GRAPHICS_CALL(m_DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &messageLength));
		messages.emplace_back(message->pDescription);
	}
	std::ostringstream oss;
	for (auto a : messages)
		oss << a << '\n';
	return oss.str();
}
