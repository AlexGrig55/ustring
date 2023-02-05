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

class UString:protected std::wstring
{
	typedef std::wstring src_str;
public:
	UString() = default;
	constexpr UString(uint32_t size, wchar_t ch=L' '):src_str(size, ch) {}
	constexpr UString(const wchar_t* str):src_str(str) {}
	constexpr UString(const wchar_t* str,size_t count) : src_str(str,count) {}
	constexpr UString(const wchar_t* startPtr, const wchar_t* endPtr) : src_str(startPtr, endPtr) {}
	constexpr UString(std::wstring_view str):src_str(str){}

	constexpr UString(const char* str) : UString(std::string_view(str)) {};
	constexpr UString(const char* str, size_t count) : UString(std::string_view(str,count)) {}
	constexpr UString(const char* startPtr, const char* endPtr) : UString(std::string_view(startPtr, endPtr)) {}
	constexpr UString(std::string_view str);

	constexpr UString(const UString& other):src_str(other) {}
	constexpr UString(UString&& other)noexcept: src_str(other) {}

	enum class CaseSens { Sensitive, Insensitive };

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
	using src_str::shrink_to_fit;

	using src_str::clear;
	using src_str::c_str;

	using src_str::data;
	using src_str::empty;

	using src_str::erase;

	using src_str::npos;
	using src_str::find;
	using src_str::rfind;

	constexpr UString& operator=(UString&& val)noexcept { src_str::operator=(val); return *this; }
	constexpr UString& operator=(const UString& val) { src_str::operator=(val); return *this; }
	constexpr UString& operator=(const wchar_t* val) { src_str::operator=(val); return *this; }
	constexpr UString& operator=(std::wstring_view val) { src_str::operator=(val); return *this; }
	constexpr UString& operator=(const char* val) { src_str::operator=(UString(val)); return *this; }
	constexpr UString& operator=(std::string_view val) { src_str::operator=(UString(val)); return *this; }

	constexpr UString& operator+=(UString&& val) { src_str::operator+=(val); return *this; }
	constexpr UString& operator+=(const UString& val) { src_str::operator+=(val); return *this; }
	constexpr UString& operator+=(const wchar_t* val) { src_str::operator+=(val); return *this; }
	constexpr UString& operator+=(std::wstring_view val) { src_str::operator+=(val); return *this; }
	constexpr UString& operator+=(const char* val) { src_str::operator=(UString(val)); return *this; }
	constexpr UString& operator+=(std::string_view val) { src_str::operator=(UString(val)); return *this; }

	constexpr UString operator+(UString&& val) const {
		UString str; str.reserve(size() + val.size()); str = *this; str += val; return str;
	}
	constexpr UString operator+(const UString& val) const{
		UString str; str.reserve(size() + val.size()); str = *this; str += val; return str;
	}
	constexpr UString operator+(const wchar_t* val) const { UString str(*this); str += val; return str;	}
	constexpr UString operator+(std::wstring_view val) const { UString str(*this); str += val; return str;	}
	constexpr UString operator+(const char* val) const { UString str(*this); str += val; return str;}
	constexpr UString operator+(std::string_view val) const { UString str(*this); str += val; return str;	}

	constexpr bool operator==(const UString& val) const noexcept { return *((src_str*)this) == *((src_str*)&val); }
	constexpr bool operator!=(const UString& val)const noexcept { return !operator==(val); }

	constexpr bool operator==(const wchar_t* val) const noexcept { return *((src_str*)this) == val; }
	constexpr bool operator!=(const wchar_t* val)const noexcept { return !operator==(val); }

	constexpr bool operator==(std::wstring_view val) const noexcept { return *((src_str*)this) == val; }
	constexpr bool operator!=(std::wstring_view val)const noexcept { return !operator==(val); }

	constexpr bool operator==(const char* val) const noexcept { return *this == std::string_view(val); }
	constexpr bool operator!=(const char* val)const noexcept { return !operator==(val); }

	constexpr bool operator==(std::string_view val) const noexcept;
	constexpr bool operator!=(std::string_view val)const noexcept { return !operator==(val); }

	constexpr size_t find(const UString& str, size_t off=0)const { return src_str::find(str, off); }
	constexpr size_t rfind(const UString& str, size_t off=0)const { return src_str::rfind(str, off); }
	constexpr UString substr(size_t off, size_t count)const;
	constexpr std::vector<UString> split(const UString& separator, CaseSens mode = CaseSens::Sensitive, bool saveEmpty=true)const;

	void convertToUpper();
	void convertToLower();

	UString toUpper()const;
	UString toLower()const;

	template <class T>
	T toNumber(bool* done = nullptr) const;
	//copy if wchar_t >= 0 && wchar_t <= max_char only
	std::string toLatin()const;
	std::string toString(const std::locale& locale = std::locale(), char _default='-')const;
	std::string toStringUtf8()const;
	std::u8string toUtf8()const;
	std::u16string toUtf16()const;
	std::u32string toUtf32()const;
	std::wstring toWString()const;
#ifdef QT_CORE_LIB
	QString toQString()const { return QString::fromStdWString(*this); }
#endif

	template <class T>
	static UString fromIntegral(T num, int base = 10);
	template <class T>
	static UString fromFloatingPoint(T num, std::chars_format format = std::chars_format::general);
	static UString fromString(std::string_view str, const std::locale& locale = std::locale());
	//copy if char >= 0
	constexpr static UString fromLatin(std::string_view str) { return UString(str); }
	static UString fromUtf8(std::string_view str);
	static UString fromUtf8(std::u8string_view str);
	static UString fromUtf16(std::u16string_view str);
	static UString fromUtf32(std::u32string_view str);
	static UString fromWString(std::wstring_view str);
#ifdef QT_CORE_LIB
	static UString fromQString(const QString& str) { return UString(str.toStdWString()); }
#endif


	static bool compare(const UString& str0, const UString& str1, CaseSens caseSence = CaseSens::Sensitive) noexcept;
};

constexpr UString::UString(std::string_view str)
{
	resize(str.size());
	auto auxilPtr = str.data();
	auto auxilWPtr = data();
	for (size_t i = 0, end = size(); i < end; i++, auxilPtr++)
		if ((*auxilPtr) >= 0)
		{
			*auxilWPtr = *auxilPtr;
			++auxilWPtr;
		}
		else
			pop_back();
}

template<class T>
inline T UString::toNumber(bool* done) const
{
	T resNum=0;
	std::string tmpStr(this->size(),' ');

	uint32_t countSymb = 0;
	for (auto& i : *this)
	{
		if (i < 102 && i > 43)
		{
			tmpStr[countSymb] = i;
			++countSymb;
		}
	}
	auto ptr = tmpStr.data();
	auto res = std::from_chars(ptr, ptr+ countSymb, resNum);

	if (done)
	{
		*done = res.ec==std::errc(0);
	}

	return resNum;
}

template<class T>
inline UString UString::fromIntegral(T num, int base)
{
	char buf[25]{};
	std::to_chars(buf, buf + 25, num, base);

	return UString(buf);
}

template<class T>
inline UString UString::fromFloatingPoint(T num, std::chars_format format)
{
	char buf[25]{};
	std::to_chars(buf, buf + 25, num, format);
	return UString(buf);
}

constexpr UString UString::substr(size_t off, size_t count)const
{
	return UString(src_str::substr(off, count));
}


constexpr std::vector<UString> UString::split(const UString& separator, CaseSens mode,bool saveEmpty)const
{
	std::vector<UString> res;
	size_t pos = 0;
	size_t oldPos = 0;
	const auto sizeSep = separator.size();
	const auto currentSize = size();

	if (mode==CaseSens::Sensitive)
	{
		while (pos<= currentSize)
		{
			oldPos = pos;
			pos = std::min(find(separator, pos), currentSize);

			auto s = pos - oldPos;
			if (s|| saveEmpty)
				res.emplace_back(std::wstring_view(data() + oldPos, s));

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


		auto& ctype = std::use_facet<std::ctype<wchar_t>>(std::locale::classic());

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
				res.emplace_back(std::wstring_view(data() + oldPos, s));

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


inline void UString::convertToUpper()
{
	auto& ctype = std::use_facet<std::ctype<wchar_t>>(std::locale::classic());
	std::transform(begin(), end(), begin(), [&](auto c) {return ctype.toupper(c); });
}

inline void UString::convertToLower()
{
	auto& ctype = std::use_facet<std::ctype<wchar_t>>(std::locale::classic());
	std::transform(begin(), end(), begin(), [&](auto c) {return ctype.tolower(c); });
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

inline std::string UString::toLatin() const
{
	std::string res(size(), 0);
	auto resPtr = res.data();
	for (auto i = data(), end = i + size(); i < end; i++)
	{
		if (*i >= 0 && *i <= std::numeric_limits<char>::max())
		{
			*resPtr = *i;
			++resPtr;
		}
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

inline std::wstring UString::toWString() const
{
	return *this;
}

inline bool UString::compare(const UString& str0, const UString& str1, CaseSens mode) noexcept
{
	switch (mode)
	{
	case UString::CaseSens::Sensitive:
		return str0 == str1;

	case UString::CaseSens::Insensitive:
		if (str0.size() != str1.size())
			return false;
		else
		{
			auto& ctype = std::use_facet<std::ctype<wchar_t>>(std::locale::classic());
			for (auto ptr0 = str0.data(), ptr1 = str1.data(), end = ptr0 + str0.size();
				ptr0 < end; ++ptr0, ++ptr1)
				if (ctype.tolower(*ptr0) != ctype.tolower(*ptr1))
					return false;

			return true;
		}

	default:
		return false;
	}

}

inline UString UString::fromString(std::string_view str, const std::locale& locale)
{
	//https://www.codeproject.com/Tips/196097/Converting-ANSI-to-Unicode-and-back
		//return String(std::filesystem::_Convert_narrow_to_wide(__std_fs_code_page(), str).c_str());

	auto _size = str.size();
	auto ptr = str.data();
	UString res(_size, 0);

	std::use_facet<std::ctype<wchar_t> >(locale).widen(ptr, ptr + _size, res.data());

	auto pos = res.find(wchar_t(0), 0);
	if (pos != res.npos)
		res.resize(pos);

	return res;
}


inline UString UString::fromUtf8(std::string_view str)
{
	return UString(std::filesystem::_Convert_narrow_to_wide(__std_code_page::_Utf8, str).c_str());
}

inline UString UString::fromUtf8(std::u8string_view str)
{
	return fromUtf8(std::string_view{ reinterpret_cast<const char*>(str.data()), str.size() });
}

inline UString UString::fromUtf16(std::u16string_view str)
{
	if constexpr (sizeof(wchar_t) == sizeof(char16_t))
	{
		auto startPtr = (const wchar_t*)str.data();
		return UString(startPtr, startPtr+str.size());
	}
	else
		return UString(std::filesystem::_Convert_stringoid_to_wide(str, std::filesystem::_Normal_conversion{}).c_str());
}

inline UString UString::fromUtf32(std::u32string_view str)
{
	if constexpr (sizeof(wchar_t) == sizeof(char32_t))
	{
		auto startPtr = (const wchar_t*)str.data();
		return UString(startPtr, str.size());
	}
	else
		return UString(std::filesystem::_Convert_stringoid_to_wide(str,
			std::filesystem::_Normal_conversion{}).c_str());
}

inline UString UString::fromWString(std::wstring_view str)
{
	return UString(str.data());
}

constexpr bool UString::operator==(std::string_view val) const noexcept
{
	if (size() != val.size())
		return false;
	auto wptr = data();
	auto ptr = val.data();
	for (auto end = ptr + size(); ptr < end; ++wptr, ++ptr)
		if (*wptr != *ptr)
			return false;
	return true;
}