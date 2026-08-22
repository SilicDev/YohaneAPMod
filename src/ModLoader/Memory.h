#pragma once
#include "windows.h"
#include <stdint.h>

static uintptr_t GetBaseAddress() {
	char* buf = new char[MAX_PATH];
	uint32_t bufsize = GetModuleFileNameA(0, buf, MAX_PATH);
	HMODULE hModule = GetModuleHandleA(buf);
	return (uintptr_t)hModule;
}
static uintptr_t base = GetBaseAddress();

template <typename Tret = size_t, typename T, size_t N>
static constexpr Tret SizeOfArray(const T(&)[N]) noexcept
{
	return (Tret)(N * sizeof(T));
}

static inline bool WriteData(void* addr, const void* data, size_t size)
{
    DWORD old_protect;
    VirtualProtect(addr, size, PAGE_EXECUTE_WRITECOPY, &old_protect);
    memcpy(addr, data, size);
    //VirtualProtect(address, size, old_protect, &old_protect);
    return true;
}

template<typename T>
static inline bool WriteData(T const* writeaddr, const T data)
{
	return WriteData((void*)writeaddr, (void*)&data, sizeof(data));
}

template<typename T>
static inline bool WriteData(T* writeaddr, const T& data)
{
	return WriteData(writeaddr, &data, sizeof(data));
}

template <typename T, size_t N>
static inline bool WriteData(void* writeaddr, const T(&data)[N])
{
	return WriteData(writeaddr, data, SizeOfArray(data));
}

#pragma pack(1)
union JMPFarAbs {
	struct {
		uint16_t mov;
		uint64_t address;
		uint16_t jmp;
	};
	uint8_t u8[12] = { 0 };
	JMPFarAbs() {};

	JMPFarAbs(intptr_t addr)
	{
		mov = 0xb848;
		address = addr;
		jmp = 0xe0ff;
	}

	JMPFarAbs(void* addr)
	{
		mov = 0xb848;
		address = (uint64_t)addr;
		jmp = 0xe0ff;
	}
};
#pragma pack()

static inline BOOL WriteLongJump(void* writeaddr, void* funcaddr)
{
	JMPFarAbs data(funcaddr);
	return WriteData(writeaddr, data.u8);
}

#define DataPointer(type, name, addr) \
	static type &name = *(type *)addr
#define DataArrayPointer(type, name, addr, len) \
	static DataArray_t<type, len> name = DataArray_t<type, len>(addr);

template<typename T, size_t len>
struct DataArray_t final
{
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	const uint64_t addr;

	DataArray_t(uint64_t addr) : addr(addr) {}
	DataArray_t(const DataArray_t&) = delete;
	DataArray_t(const DataArray_t&&) = delete;

	constexpr bool empty() const
	{
		return len == 0;
	}

	constexpr size_type size() const
	{
		return len;
	}

	constexpr reference operator[](size_type index)
	{
		return addr[index];
	}

	constexpr const_reference operator[](size_type index) const
	{
		return addr[index];
	}

	constexpr pointer operator&() const
	{
		return this[0];
	}

	constexpr operator pointer() const
	{
		return this[0];
	}

	constexpr reference front() {
		return this[0];
	}

	constexpr const_reference front() const {
		return this[0];
	}

	constexpr reference back() {
		return this[len - 1];
	}

	constexpr const_reference back() const {
		return this[len - 1];
	}

	constexpr pointer begin()
	{
		return this[0];
	}

	constexpr const_pointer cbegin() const
	{
		return this[0];
	}

	constexpr pointer end()
	{
		return this[0] + len;
	}

	constexpr const_pointer cend() const
	{
		return this[0] + len;
	}

	constexpr pointer rbegin()
	{
		return this[0] + len;
	}

	constexpr const_pointer crbegin() const
	{
		return this[0] + len;
	}

	constexpr pointer rend()
	{
		return this[0];
	}

	constexpr const_pointer crend() const
	{
		return this[0];
	}
};

#define FuncPointer(ret, name, args, addr) \
	static ret (__fastcall *const name)args = (ret (__cdecl *)args)addr
