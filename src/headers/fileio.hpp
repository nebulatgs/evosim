#include <iostream>

static const char *kReadFailure = "Read failure";
static const char *kWriteFailure = "Write error";
static const char *kArrayTooBig = "Array too big";

template<class T>
void write(std::ofstream& f, const T& v)
{
	static_assert(std::is_trivially_copyable_v<T>);
	if (!f.write(reinterpret_cast<const char*>(&v), sizeof(T)))
		throw kWriteFailure;
}

template<class T>
void writeN(std::ofstream& f, const T* v, size_t n)
{
	static_assert(std::is_trivially_copyable_v<T>);
	if (!f.write(reinterpret_cast<const char*>(v), sizeof(T) * n))
		throw kWriteFailure;
}

template<class T>
void write(std::ofstream& f, const std::vector<T>& v)
{
	if (v.size() > UINT32_MAX) // Not using size_t as that's different for x86 and x64.
		throw kArrayTooBig;
	write(f, u	int32_t(v.size()));
	writeN(f, v.data(), v.size());
}

template<class T>
void read(std::ifstream& f, T& v)
{
	static_assert(std::is_trivially_copyable_v<T>);
	if (!f.read(reinterpret_cast<char*>(&v), sizeof(T)))
		throw kReadFailure;
}

template<class T>
void readN(std::ifstream& f, T* v, size_t n)
{
	static_assert(std::is_trivially_copyable_v<T>);
	if (!f.read(reinterpret_cast<char*>(v), sizeof(T) * n))
		throw kReadFailure;
}

template<class T>
T read(std::ifstream& f)
{
	T value;
	return read(f, value), value;
}

template<class T>
void read(std::ifstream& f, std::vector<T>& v)
{
	const uint32_t n = read<uint32_t>(f);
	v.resize(n);
	readN(f, v.data(), n);
}