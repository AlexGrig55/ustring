#pragma once
#include <assert.h>
#include <string>
#include <locale>
#include <charconv>
#include <filesystem>
#include <codecvt>

#ifdef QT_CORE_LIB
#include <QString>
#endif

class UString :protected std::wstring
{
	typedef std::wstring src_str;

	inline static const std::ctype<wchar_t>& ctype= std::use_facet<std::ctype<wchar_t>>(std::locale::classic());

	constexpr static bool isValidSymbolForNum(wchar_t c) noexcept;
	constexpr static size_t srtLen(const char* ptr) noexcept;

public:
	UString() = default;
	explicit constexpr UString(uint32_t size, char ch = ' ') :src_str(size, ch) {}
	constexpr UString(const char* str, size_t count);
	constexpr UString(const char* startPtr, const char* endPtr) :
		UString(startPtr, srtLen(startPtr)) {}
	constexpr UString(const char* str) : UString(str, srtLen(str)) {}

	constexpr UString(const UString& other) : src_str(other) {}
	constexpr UString(UString&& other)noexcept : src_str(std::move(other)) {}

	using src_str::at;
	using src_str::size;

	using src_str::operator[];

	using src_str::back;
	using src_str::front;

	using src_str::begin;
	using src_str::end;

	using src_str::capacity;
	using src_str::reserve;
	using src_str::resize;

	using src_str::clear;
	using src_str::c_str;

	using src_str::data;
	using src_str::empty;

	using src_str::erase;

	using src_str::npos;

	void popBack() { src_str::pop_back(); }
	void shrinkToFit() { src_str::shrink_to_fit(); }

	constexpr UString& operator=(UString&& val)noexcept { src_str::operator=(std::move(val)); return *this; }
	constexpr UString& operator=(const UString& val) { src_str::operator=(val); return *this; }
	constexpr UString& operator=(const char* val) { src_str::operator=(UString(val)); return *this; }
	constexpr UString& operator=(const wchar_t* val) { src_str::operator+=(val); return *this; }

	constexpr UString& operator+=(UString&& val) { src_str::operator+=(std::move(val)); return *this; }
	constexpr UString& operator+=(const UString& val) { src_str::operator+=(val); return *this; }
	constexpr UString& operator+=(const char* val);
	constexpr UString& operator+=(const wchar_t* val) { src_str::operator+=(val); return *this; }
	constexpr UString& operator+=(wchar_t val) { src_str::operator+=(val); return *this; }

	constexpr UString operator+(UString&& val) const {
		UString str; str.reserve(size() + val.size()); str = *this; str += val; return str;
	}
	constexpr UString operator+(const UString& val) const {
		UString str; str.reserve(size() + val.size()); str = *this; str += val; return str;
	}
	constexpr UString operator+(const char* val) const { UString str(*this); str += val; return str; }
	constexpr UString operator+(const wchar_t* val) const { UString str(*this); str += val; return str; }
	constexpr UString operator+(wchar_t val) const { UString str(*this); str += val; return str; }

	constexpr bool operator==(UString&& val) const noexcept;
	constexpr bool operator!=(UString&& val) const noexcept { return !operator==(std::move(val)); }
	constexpr bool operator==(const UString& val) const noexcept { return *((src_str*)this) == *((src_str*)&val); }
	constexpr bool operator!=(const UString& val)const noexcept { return !operator==(val); }

	constexpr bool operator==(const char* val) const noexcept;
	constexpr bool operator!=(const char* val)const noexcept { return !operator==(val); }

	constexpr bool operator==(const wchar_t* val) const noexcept { return *((src_str*)this)==val; }
	constexpr bool operator!=(const wchar_t* val)const noexcept { return !operator==(val); }

	constexpr void popFront() { src_str::erase(0, 1); }

	constexpr size_t find(char c, size_t off = 0, bool ignoreCase = false)const noexcept;
	constexpr size_t find(const UString& str, size_t off = 0, bool ignoreCase = false)const noexcept;
	constexpr size_t rfind(char c, size_t off = 0, bool ignoreCase = false)const noexcept;
	constexpr size_t rfind(const UString& str, size_t off = 0, bool ignoreCase = false)const noexcept;

	constexpr UString substr(size_t off, size_t count)const;
	constexpr std::vector<UString> split(const UString& separator, bool ignoreCase = false, bool saveEmpty = true)const;
	constexpr void insert(size_t offset, const char* str);
	constexpr void insert(size_t offset, const UString& str);
	constexpr void insert(size_t offset, UString&& str);

	constexpr bool startsWith(const UString& val, bool ignoreCase = false)const noexcept;
	constexpr bool startsWith(const char* valPtr, bool ignoreCase = false)const noexcept;
	constexpr bool endsWith(const UString& val, bool ignoreCase = false)const noexcept;
	constexpr bool endsWith(const char* valPtr, bool ignoreCase = false)const noexcept;

	constexpr bool contains(const UString& str, bool ignoreCase = false)const noexcept { return find(str,0, ignoreCase) != npos; }

	constexpr size_t count(const UString& subStr, bool ignoreCase = false)const noexcept;
	constexpr size_t count(const char c, bool ignoreCase = false)const noexcept;

	constexpr size_t replace(const UString& before, const UString& after, bool ignoreCase = false);

	void convertToUpper();
	void convertToLower();

	UString toUpper()const;
	UString toLower()const;

	bool isLower()const;
	bool isUpper()const;

	
	template <class T>
	std::enable_if_t<std::is_integral<T>::value, T> 
		toIntegral(uint8_t base=10,bool* done = nullptr) const { return toIntegral<T>(0, size(),base, done); }
	template <class T>
	std::enable_if_t<std::is_integral<T>::value, T>
		toIntegral(size_t startOff, size_t count, uint8_t base = 10, bool* done = nullptr) const;

	template <class T>
	std::enable_if_t<std::is_floating_point<T>::value, T>
		toFloatingPoint(bool* done = nullptr) const { return toFloatingPoint<T>(0, size(), done); }
	template <class T>
	std::enable_if_t<std::is_floating_point<T>::value, T>
		toFloatingPoint(size_t startOff, size_t count, bool* done = nullptr) const;

	constexpr std::string toLatin()const;
	std::string toString(const std::locale& locale = std::locale(), char _default='-')const;
	std::string toStringUtf8()const;
	std::u8string toUtf8()const;
	std::u16string toUtf16()const;
	std::u32string toUtf32()const;
	constexpr const std::wstring& toWString()const { return *this; }
#ifdef QT_CORE_LIB
	QString toQString()const { return QString::fromStdWString(*this); }
#endif

	template <class T>
	static std::enable_if_t<std::is_integral<T>::value, UString> fromIntegral(T num, int base = 10);
	//format: 'g'- general; 'f' - fixed; 's' - scientific; 'h' - hex
	//if precision < 0: auto precision
	template <class T>
	static std::enable_if_t<std::is_floating_point<T>::value, UString> 
		fromFloatingPoint(T num, uint8_t format = 'g', int8_t precision = -1);
	static UString fromString(std::string_view str, const std::locale& locale = std::locale());
	constexpr static UString fromLatin(std::string_view str);
	static UString fromUtf8(std::string_view str);
	static UString fromUtf8(std::u8string_view str);
	static UString fromUtf16(std::u16string_view str);
	static UString fromUtf32(std::u32string_view str);
	constexpr static UString fromWString(std::wstring_view str);
#ifdef QT_CORE_LIB
	static UString fromQString(const QString& str) { return UString::fromWString(str.toStdWString()); }
#endif

	static bool compare(const UString& str0, const UString& str1, bool ignoreCase) noexcept;
};

constexpr UString::UString(const char* str,size_t count)
{
	resize(count);

	for (auto currentPtr = data(), end = currentPtr + count; currentPtr < end; ++str, ++currentPtr)
	{
		*currentPtr = *str;
	}
}

constexpr bool UString::isValidSymbolForNum(wchar_t c)noexcept
{
	return (c > 41 && c < 58) || (c > 64 && c < 71)|| (c > 96 && c < 103);
}

constexpr size_t UString::srtLen(const char* ptr) noexcept
{
	auto auxilPtr = ptr;
	while (*auxilPtr != 0)
		++auxilPtr;
	return auxilPtr - ptr;
}

template<class T>
inline std::enable_if_t<std::is_integral<T>::value, T>
UString::toIntegral(size_t startOff, size_t count,uint8_t base, bool* done) const
{
	if (startOff > size())
	{
		if (done)
			done = false;
		return T(0);
	}

	if (count > size() - startOff)
		count = size() - startOff;


	constexpr const int sizeBuf = 30;
	T resNum = 0;
	char tmpStr[sizeBuf]{};

	uint8_t countSymb = 0;
	for (auto ptr = data() + startOff, end = ptr + count; ptr < end; ptr++)
	{
		if (isValidSymbolForNum(*ptr))
		{
			tmpStr[countSymb] = *ptr;
			++countSymb;
			if (countSymb >= sizeBuf)
			{
				if (done)
				{
					done = false;
					return resNum;
				}
			}
		}
	}

	auto res = std::from_chars(tmpStr, tmpStr + countSymb, resNum, base);

	if (done)
	{
		*done = res.ec == std::errc(0);
	}

	return resNum;
}

template<class T>
inline std::enable_if_t<std::is_floating_point<T>::value, T>
UString::toFloatingPoint(size_t startOff, size_t count, bool* done) const
{
	if (startOff > size())
	{
		if (done)
			done = false;
		return T(0);
	}

	if (count > size() - startOff)
		count = size() - startOff;


	constexpr const int sizeBuf = 30;
	T resNum = 0;
	char tmpStr[sizeBuf]{};

	uint8_t countSymb = 0;
	for (auto ptr = data() + startOff, end = ptr + count; ptr < end; ptr++)
	{
		if (isValidSymbolForNum(*ptr))
		{
			tmpStr[countSymb] = *ptr;
			++countSymb;
			if (countSymb >= sizeBuf)
			{
				if (done)
				{
					done = false;
					return resNum;
				}
			}
		}
	}

	auto res = std::from_chars(tmpStr, tmpStr + countSymb, resNum);

	if (done)
	{
		*done = res.ec == std::errc(0);
	}

	return resNum;
}

template <class T>
inline std::enable_if_t<std::is_integral<T>::value, UString> UString::fromIntegral(T num, int base)
{
	char buf[25]{};
	std::to_chars(buf, buf + 25, num, base);

	return UString(buf);
}

template <class T>
inline std::enable_if_t<std::is_floating_point<T>::value, UString> 
UString::fromFloatingPoint(T num, uint8_t format,int8_t precision)
{
	char buf[40]{};
	std::chars_format f;
	switch (format)
	{
	case 'f':
		f = std::chars_format::fixed;
		break;
	case 's':
		f = std::chars_format::scientific;
		break;
	case 'h':
		f = std::chars_format::hex;
		break;
	default:
		f = std::chars_format::general;
		break;
	}

	if(precision>=0)
		std::to_chars(buf, buf + sizeof(buf), num, f, precision);
	else
		std::to_chars(buf, buf + sizeof(buf), num, f);

	return UString(buf);
}

constexpr size_t  UString::find(char c, size_t off, bool ignoreCase)const noexcept
{
	assert(size() >= off);

	if (ignoreCase==false)
	{
		return src_str::find(c, off);
	}
	else if (1 <= size() - off)
	{
		auto srcPtr = data() + off;
		const auto srcEnd = srcPtr + size();

		auto forCompare = ctype.tolower(c);

		while (srcPtr < srcEnd)
		{
			if (ctype.tolower(*srcPtr) == forCompare)
				return srcPtr - data();

			srcPtr++;
		}
	}
	return npos;
}

constexpr size_t UString::find(const UString& str, size_t off, bool ignoreCase)const noexcept
{
	if (str.size() == 0)
		return npos;

	assert(size() >= off);

	if (ignoreCase == false)
	{
		return src_str::find(str,off);
	}
	else if (str.size() <=size() - off)
	{
		auto srcPtr = data() + off;
		const auto srcEnd = srcPtr + size() - off - str.size() + 1;
		const auto otherPtr = str.data();
		const auto otherEnd = otherPtr + str.size();

		auto c = ctype.tolower(*otherPtr);

		while (srcPtr< srcEnd)
		{
			if (ctype.tolower(*srcPtr)== c)
			{
				auto tmpPtr0 = otherPtr + 1;
				auto tmpPtr1 = srcPtr + 1;
				while (tmpPtr0 < otherEnd)
				{
					if (ctype.tolower(*tmpPtr0) != ctype.tolower(*tmpPtr1))
						break;
					++tmpPtr0; ++tmpPtr1;
				}

				if(tmpPtr0== otherEnd)
					return srcPtr - data();
			}

			srcPtr++;
		}
	}
	return npos;
}

constexpr size_t UString::rfind(char c, size_t off, bool ignoreCase)const noexcept
{
	assert(size() >= off);
	
	auto srcPtr = data() + size() - 1;
	const auto srcEnd = data() + off - 1;

	if (ignoreCase == false)
	{
		while (srcPtr > srcEnd)
		{
			if (*srcPtr == c)
				return srcPtr - data();

			--srcPtr;
		}
	}
	else
	{
		auto forCompare = ctype.tolower(c);
		while (srcPtr > srcEnd)
		{
			if (ctype.tolower(*srcPtr) == forCompare)
				return srcPtr - data();

			--srcPtr;
		}
	}
	
	return npos;
}

constexpr size_t UString::rfind(const UString& str, size_t off, bool ignoreCase)const noexcept
{
	if (str.size() == 0)
		return npos;

	assert(size() >= off);

	auto srcPtr = data() + size() - str.size();
	const auto srcEnd = data() + off - 1;

	const auto otherPtr = str.data();
	const auto otherEnd = otherPtr + str.size();

	if (ignoreCase == false)
	{
		while (srcPtr > srcEnd)
		{
			auto c = str.front();

			if (*srcPtr == c)
			{
				auto tmpPtr0 = otherPtr + 1;
				auto tmpPtr1 = srcPtr + 1;
				while (tmpPtr0 < otherEnd)
				{
					if (*tmpPtr0 != *tmpPtr1)
						break;
					++tmpPtr0; ++tmpPtr1;
				}

				if (tmpPtr0 == otherEnd)
					return srcPtr - data();
			}

			srcPtr++;
		}
	}
	else
	{
		auto c = ctype.tolower(*otherPtr);

		while (srcPtr > srcEnd)
		{
			if (ctype.tolower(*srcPtr) == c)
			{
				auto tmpPtr0 = otherPtr + 1;
				auto tmpPtr1 = srcPtr + 1;
				while (tmpPtr0 < otherEnd)
				{
					if (ctype.tolower(*tmpPtr0) != ctype.tolower(*tmpPtr1))
						break;
					++tmpPtr0; ++tmpPtr1;
				}

				if (tmpPtr0 == otherEnd)
					return srcPtr - data();
			}

			srcPtr++;
		}
	}

	return npos;
}

constexpr UString UString::substr(size_t off, size_t count)const
{
	return UString::fromWString(src_str::substr(off, count));
}

constexpr std::vector<UString> UString::split(const UString& separator, bool ignoreCase,bool saveEmpty)const
{
	std::vector<UString> res;
	size_t pos = 0;
	size_t oldPos = 0;
	const auto sizeSep = separator.size();
	const auto currentSize = size();

	if (ignoreCase == false)
	{
		while (pos<= currentSize)
		{
			oldPos = pos;
			pos = std::min(find(separator, pos), currentSize);

			auto s = pos - oldPos;
			if (s|| saveEmpty)
				res.emplace_back(fromWString(std::wstring_view(data() + oldPos, s)));

			if (npos - pos >= sizeSep)
			{
				pos += sizeSep;
			}
			else
				break;
		}
	}
	else
	{
		const auto auxilSep = separator.toLower();
		const auto otherPtr = auxilSep.data();
		const auto endOtherPtr = otherPtr+ sizeSep;
		const auto currentEndPtr = data() + currentSize;

		while (pos <= currentSize)
		{
			oldPos = pos;
			for (size_t end = size(); pos < end; pos++)
			{
				auto currentPtr = data() + pos;

				bool isFind = true;
				for (auto j = otherPtr; isFind && j < endOtherPtr && currentPtr < currentEndPtr; ++j, ++currentPtr)
					isFind = ctype.tolower(*currentPtr) == *j;

				if (isFind)
					break;
			}
			
			auto s = pos - oldPos;
			if (s || saveEmpty)
				res.emplace_back(fromWString(std::wstring_view(data() + oldPos, s)));

			if (npos - pos >= sizeSep)
			{
				pos += sizeSep;
			}
			else
				break;
		}
	}
	return res;
}

constexpr void UString::insert(size_t ind, const char* str)
{
	src_str::insert(ind, UString(str));
}

constexpr void UString::insert(size_t ind, const UString& str)
{
	src_str::insert(ind, str);
}

constexpr void UString::insert(size_t ind, UString&& str)
{
	src_str::insert(ind, str);
}

constexpr bool UString::startsWith(const UString& val, bool ignoreCase)const noexcept
{
	if (ignoreCase == false)
	{
		return src_str::starts_with(val);
	}
	else if(size()>=val.size())
	{
		for (auto currentPtr = data(), otherPtr = val.data(), end = otherPtr + val.size();
			otherPtr < end; ++otherPtr, ++currentPtr)
		{
			if (ctype.tolower(*currentPtr) != ctype.tolower(*otherPtr))
				return false;
		}
		return true;
	}

	return false;
}

constexpr bool UString::startsWith(const char* valPtr, bool ignoreCase)const noexcept
{
	std::string_view val(valPtr);
	if (size() < val.size())
		return false;

	auto currentPtr = data();
	auto otherPtr = val.data();
	auto end = otherPtr + val.size();

	if (ignoreCase==false)
	{
		for (; otherPtr < end; ++otherPtr, ++currentPtr)
		{
			if (*currentPtr != *otherPtr)
				return false;
		}
	}
	else
	{
		for (;otherPtr < end; ++otherPtr, ++currentPtr)
		{
			if (ctype.tolower(*currentPtr) != ctype.tolower(*otherPtr))
				return false;
		}
	}
	return true;
}

constexpr bool UString::endsWith(const UString& val, bool ignoreCase)const noexcept
{
	if (ignoreCase==false)
	{
		return src_str::ends_with(val);
	}
	else if (size() >= val.size())
	{
		auto sizeVal = val.size();

		for (auto currentPtr = data() + size() - sizeVal, otherPtr = val.data(), end = otherPtr + sizeVal;
			otherPtr < end; ++otherPtr, ++currentPtr)
		{
			if (ctype.tolower(*currentPtr) != ctype.tolower(*otherPtr))
				return false;
		}
		return true;
	}

	return false;
}

constexpr bool UString::endsWith(const char* valPtr, bool ignoreCase)const noexcept
{
	std::string_view val(valPtr);
	auto otherSize = val.size();
	if (size() < otherSize)
		return false;

	auto currentPtr = data()+size()- otherSize;
	auto otherPtr = val.data();
	auto end = otherPtr + otherSize;

	if (ignoreCase == false)
	{
		for (; otherPtr < end; ++otherPtr, ++currentPtr)
		{
			if (*currentPtr != *otherPtr)
				return false;
		}
	}
	else
	{
		for (; otherPtr < end; ++otherPtr, ++currentPtr)
		{
			if (ctype.tolower(*currentPtr) != ctype.tolower(*otherPtr))
				return false;
		}
	}
	return true;
}

constexpr size_t UString::count(const UString& subStr, bool ignoreCase)const noexcept
{
	size_t res = 0;
	size_t off = 0;
	size_t s1 = subStr.size();
	auto end = size();

	while (off < end)
	{
		off = find(subStr, off, ignoreCase);
		if (off==npos)
		{
			break;
		}
		else
		{
			++res;
			off += s1;
		}
	}

	return res;
}

constexpr size_t UString::count(const char c, bool ignoreCase)const noexcept
{
	size_t res = 0;
	size_t off = 0;

	auto end = size();

	while (off < end)
	{
		off = find(c, off, ignoreCase);
		if (off == npos)
		{
			break;
		}
		else
		{
			++res;
			off += 1;
		}
	}

	return res;
}

constexpr size_t UString::replace(const UString& before, const UString& after, bool ignoreCase)
{
	size_t res = 0;
	size_t off = 0;
	size_t sizeBefore = before.size();
	size_t sizeAfter = after.size();
	int64_t delta = sizeBefore - sizeAfter;
	auto end = size();

	while (off < end)
	{
		off = find(before, off, ignoreCase);
		if (off == npos)
		{
			break;
		}
		else
		{
			if (delta>0)
			{
				erase(off + sizeAfter, delta);
			}
			else if (delta < 0)
			{
				src_str::insert(begin() + off + sizeBefore, -delta, L' ');
			}

			for (size_t i = 0; i < sizeAfter; i++)
				operator[](i + off) = after[i];

			++res;
			off += sizeAfter;
		}
	}

	return res;
}

constexpr UString& UString::operator+=(const char* val)
{
	std::string_view strView(val);
	auto oldSize = size();
	resize(oldSize + strView.size());

	for (auto i = data() + oldSize, end = i + strView.size(); i < end; ++i, ++val)
		*i = *val;

	return *this;
}

inline void UString::convertToUpper()
{
	for (auto& i : *this)
		i = ctype.toupper(i);
}

inline void UString::convertToLower()
{
	for (auto& i : *this)
		i = ctype.tolower(i);
}

inline UString UString::toUpper() const
{
	UString res(*this);
	res.convertToUpper();
	return res;
}

inline UString UString::toLower() const
{
	UString res(*this);
	res.convertToLower();
	return res;
}

inline bool UString::isLower()const
{
	for (auto& i : *this)
		if (i != ctype.tolower(i))
			return false;
	return true;
}

inline bool UString::isUpper()const
{
	for (auto& i : *this)
		if (i != ctype.toupper(i))
			return false;
	return true;
}

constexpr std::string UString::toLatin() const
{
	std::string res(size(), 0);
	auto resPtr = res.data();
	for (auto i = data(), end = i + size(); i < end; i++)
	{
		if (*i >= '\a' && *i <= std::numeric_limits<char>::max())
		{
			*resPtr = *i;
			++resPtr;
		}
		else
			res.pop_back();
	}
	return res;
}

inline std::string UString::toString(const std::locale& locale, char _default) const
{
	//https://www.codeproject.com/Tips/196097/Converting-ANSI-to-Unicode-and-back

	auto _size = this->size() * 2;
	auto ptr = data();

	std::string res(_size, 0);

	std::use_facet<std::ctype<wchar_t> >(locale).narrow(ptr, ptr + _size, _default, res.data());

	auto pos = res.find(char(0), 0);

	if (pos != res.npos)
		res.resize(pos);

	return res;
}

inline std::string UString::toStringUtf8() const
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

	return conv.to_bytes(*this);
}

inline std::u8string UString::toUtf8() const
{
	auto wv = std::wstring_view(this->c_str());

	return std::filesystem::_Convert_wide_to<std::char_traits<char8_t>, std::allocator<char8_t>,
		char8_t>(wv, std::allocator<char>());
}

inline std::u16string UString::toUtf16() const
{
	if constexpr (sizeof(wchar_t) == sizeof(char16_t))
	{
		const char16_t* startPtr = (const char16_t*)data();
		return std::u16string(startPtr, startPtr + size());
	}
	else
	{
		auto wv = std::wstring_view(this->c_str());

		return std::filesystem::_Convert_wide_to<std::char_traits<char16_t>,
			std::allocator<char16_t>, char16_t>(wv, std::allocator<char16_t>());
	}
}

inline std::u32string UString::toUtf32() const
{
	if constexpr (sizeof(wchar_t) == sizeof(char32_t))
	{
		const char32_t* startPtr = (const char32_t*)data();
		return std::u32string(startPtr, startPtr + size());
	}
	else
	{
		auto wv = std::wstring_view(*this);

		return std::filesystem::_Convert_wide_to<std::char_traits<char32_t>,
			std::allocator<char32_t>, char32_t>(wv, std::allocator<char32_t>());
	}
}


inline bool UString::compare(const UString& str0, const UString& str1, bool ignoreCase) noexcept
{
	if (ignoreCase)
	{
		if (str0.size() != str1.size())
			return false;
		else
		{
			for (auto ptr0 = str0.data(), ptr1 = str1.data(), end = ptr0 + str0.size();
				ptr0 < end; ++ptr0, ++ptr1)
				if (ctype.tolower(*ptr0) != ctype.tolower(*ptr1))
					return false;

			return true;
		}
	}
	else
	{
		return str0 == str1;
	}

}

inline UString UString::fromString(std::string_view str, const std::locale& locale)
{
	////https://www.codeproject.com/Tips/196097/Converting-ANSI-to-Unicode-and-back
	//	//return String(std::filesystem::_Convert_narrow_to_wide(__std_fs_code_page(), str).c_str());
	//
	//auto _size = str.size();
	//auto ptr = str.data();
	//UString res(_size, 0);
	//
	//std::use_facet<std::ctype<wchar_t> >(locale).widen(ptr, ptr + _size, res.data());
	//
	//auto pos = res.find(wchar_t(0), 0);
	//if (pos != res.npos)
	//	res.resize(pos);
	//
	//std::filesystem::_Convert_stringoid_with_locale_to_wide(str, locale)

	return fromWString(std::filesystem::_Convert_stringoid_with_locale_to_wide(str, locale));
}

constexpr UString UString::fromLatin(std::string_view str)
{
	UString res(str.size());
	auto currentPtr = res.data();
	
	for (auto otherPtr = str.data(),end= otherPtr+str.size(); otherPtr < end; ++otherPtr)
	{
		if (*currentPtr < '\a' )
		{
			res.pop_back();
		}
		else
		{
			*currentPtr = *otherPtr;
			++currentPtr;
		}
	}

	return res;
}

inline UString UString::fromUtf8(std::string_view str)
{
	return fromWString(std::filesystem::_Convert_narrow_to_wide(__std_code_page::_Utf8, str));
}

inline UString UString::fromUtf8(std::u8string_view str)
{
	return fromUtf8(std::string_view{ reinterpret_cast<const char*>(str.data()), str.size() });
}

inline UString UString::fromUtf16(std::u16string_view str)
{
	if constexpr (sizeof(wchar_t) == sizeof(char16_t))
		return fromWString(std::wstring_view((const wchar_t*)str.data(), str.size()));
	else
		return fromWString(std::filesystem::_Convert_stringoid_to_wide(str,
			std::filesystem::_Normal_conversion{}));
}

inline UString UString::fromUtf32(std::u32string_view str)
{
	if constexpr (sizeof(wchar_t) == sizeof(char32_t))
		return fromWString(std::wstring_view((const wchar_t*)str.data(), str.size()));
	else
		return fromWString(std::filesystem::_Convert_stringoid_to_wide(str,
			std::filesystem::_Normal_conversion{}));
}

constexpr UString UString::fromWString(std::wstring_view str)
{
	UString res;
	res.src_str::operator=(str);
	return res;
}

constexpr bool UString::operator==(const char* val) const noexcept
{
	std::string_view view(val);
	if (size() != view.size())
		return false;
	auto wptr = data();

	for (auto end = val + size(); val < end; ++wptr, ++val)
		if (*wptr != *val)
			return false;
	return true;
}


constexpr UString operator+(const char* str0,const UString& str1) {
	std::string_view strV(str0);

	UString res(strV.size() + str1.size());
	auto currentPtr = res.data();
	auto otherPtr = str1.data();

	for (auto end = currentPtr + strV.size(); currentPtr < end; ++currentPtr, ++str0)
	{
		*currentPtr = *str0;
	}

	for (auto end = currentPtr + str1.size(); currentPtr < end; ++currentPtr,++otherPtr)
	{
		*currentPtr = *otherPtr;
	}

	return res;
}

constexpr bool operator==(const char* str0, const UString& str1)noexcept {return str1==str0;}
constexpr bool operator!=(const char* str0, const UString& str1)noexcept { return str1 != str0; }
constexpr bool operator==(const char* str0, UString&& str1)noexcept { return str1 == str0; }
constexpr bool operator!=(const char* str0, UString&& str1)noexcept { return str1 != str0; }
