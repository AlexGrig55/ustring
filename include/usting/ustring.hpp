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
	constexpr UString(std::wstring_view str):src_str(str){}
	constexpr UString(const char* str) : src_str() {
		if (str)
		{
			auto auxilPtr = str;
			while (auxilPtr != 0)
				++auxilPtr;
			resize(auxilPtr - str);
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
	}
	constexpr UString(std::string_view str)
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
	constexpr UString(const UString& other):src_str(other) {}
	constexpr UString(UString&& other)noexcept: src_str(other) {}


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
	using src_str::substr;

	using src_str::find;
	using src_str::rfind;
	using src_str::find_first_not_of;
	using src_str::find_first_of;
	using src_str::find_last_not_of;
	using src_str::find_last_of;

	using src_str::starts_with;
	using src_str::ends_with;

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

	constexpr bool operator==(std::string_view val) const noexcept { 
		if (size() != val.size())
			return false;
		auto wptr = data();
		auto ptr = val.data();
		for (auto end = ptr + size(); ptr < end; ++wptr, ++ptr)
			if (*wptr != *ptr)
				return false;
		return true;
	}
	constexpr bool operator!=(std::string_view val)const noexcept { return !operator==(val); }

	void convertToUpper(const std::locale& locale= std::locale());
	void convertToLower(const std::locale& locale = std::locale());

	UString toUpper(const std::locale& locale = std::locale())const;
	UString toLower(const std::locale& locale = std::locale())const;

	template <class T>
	T toNumber(bool* done=nullptr) const;
	//return char >=0 only
	std::string toLatin()const;
	std::string toString(const std::locale& locale = std::locale())const;
	std::string toStringUtf8()const;
	std::u8string toUtf8()const;
	std::u16string toUtf16()const;
	std::u32string toUtf32()const;
	std::wstring toWString()const;
#ifdef QT_CORE_LIB
	QString toQString()const { return QString::fromStdWString(*this); }
#endif

	enum class CaseSens { Sensitive, Insensitive };

	static bool compare(const UString& str0, const UString& str1, CaseSens caseSence= CaseSens::Sensitive) noexcept;

	template <class T>
	static UString fromIntegral(T num,int base=10);
	template <class T>
	static UString fromFloatingPoint(T num, std::chars_format format = std::chars_format::general);
	static UString fromString(std::string_view str, const std::locale& locale = std::locale());
	//copy only symbols char >= 0
	static UString fromLatin(std::string_view str);
	static UString fromUtf8(std::string_view str);
	static UString fromUtf8(std::u8string_view str);
	static UString fromUtf16(std::u16string_view str);
	static UString fromUtf32(std::u32string_view str);
	static UString fromWString(std::wstring_view str);
#ifdef QT_CORE_LIB
	static UString fromQString(const QString& str)const { return UString(str.toStdWString()); }
#endif
};



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
	char buf[21]{};
	auto res=std::to_chars(buf, buf + 21, num, base);
	int size = res.ptr - buf;
	String resStr(size);

	for (int i = 0; i < size; i++)
	{
		resStr[i] = buf[i];
	}

	return resStr;
}

template<class T>
inline UString UString::fromFloatingPoint(T num, std::chars_format format)
{
	char buf[21]{};
	auto res = std::to_chars(buf, buf + 21, num, format);
	int size = res.ptr - buf;
	UString resStr(size);

	for (int i = 0; i < size; i++)
	{
		resStr[i] = buf[i];
	}

	return resStr;
}


inline void UString::convertToUpper(const std::locale& locale)
{
	auto& ctype = std::use_facet<std::ctype<wchar_t>>(locale);
	std::transform(begin(), end(), begin(), [&](auto c) {return ctype.toupper(c); });
}

inline void UString::convertToLower(const std::locale& locale)
{
	auto& ctype = std::use_facet<std::ctype<wchar_t>>(locale);
	std::transform(begin(), end(), begin(), [&](auto c) {return ctype.tolower(c); });
}

inline UString UString::toUpper(const std::locale& locale) const
{
	UString res(*this);
	res.convertToUpper(locale);
	return res;
}

inline UString UString::toLower(const std::locale& locale) const
{
	UString res(*this);
	res.convertToLower(locale);
	return res;
}

inline std::string UString::toLatin() const
{
	std::string res(size(), 0);
	auto resPtr = res.data();
	for (auto i = data(), end = i + size(); i < end; i++)
	{
		if (*i >= 0)
		{
			*resPtr = *i;
			++resPtr;
		}
	}
	return res;
}

inline UString UString::toString(const std::locale& locale) const
{
	//https://www.codeproject.com/Tips/196097/Converting-ANSI-to-Unicode-and-back
	//auto wv = std::wstring_view(this->c_str());
	//
	//return std::filesystem::_Convert_wide_to<std::char_traits<char>, std::allocator<char>, char>(wv, std::allocator<char>());

	auto _size = this->size() * 2;
	auto ptr = data();

	std::string res(_size, 0);

	std::use_facet<std::ctype<wchar_t> >(locale).narrow(ptr, ptr + _size, '-', res.data());

	auto pos = res.find(char(0), 0);

	if (pos != res.npos)
		res.resize(pos);

	return res;
}

inline UString UString::toStringUtf8() const
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
		auto _size = size();
		std::u16string res(_size, 0);
		memccpy(res.data(), data(), _size, sizeof(char16_t));
		return res;
	}
	else
	{
		//std::wstring_convert<std::codecvt_utf16<wchar_t>> converter;
		//auto buf = converter.to_bytes(*this);
		//
		//std::u16string res(buf.size()/sizeof(char16_t),0);
		//
		//memccpy(res.data(), buf.data(), 1, buf.size());
		//
		//return res;

		auto wv = std::wstring_view(this->c_str());

		return std::filesystem::_Convert_wide_to<std::char_traits<char16_t>,
			std::allocator<char16_t>, char16_t>(wv, std::allocator<char16_t>());
	}
}

inline std::u32string UString::toUtf32() const
{
	if constexpr (sizeof(wchar_t) == sizeof(char32_t))
	{
		auto _size = size();
		std::u32string res(_size, 0);
		memccpy(res.data(), data(), _size, sizeof(char32_t));
		return res;
	}
	else
	{

		//std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> convert;
		//auto buf = convert.from_bytes(,);
		//
		//std::u32string res(buf.size() / sizeof(char32_t), 0);
		//
		//memccpy(res.data(), buf.data(), 1, buf.size());
		//
		//return res;

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
			auto& ctype = std::use_facet<std::ctype<wchar_t>>(std::locale());
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

	auto _size = str.size() * 2;
	auto ptr = str.data();
	UString res(_size, 0);

	std::use_facet<std::ctype<wchar_t> >(locale).widen(ptr, ptr + _size, res.data());

	auto pos = res.find(wchar_t(0), 0);
	if (pos != res.npos)
		res.resize(pos);

	return res;
}

inline UString UString::fromLatin(std::string_view str)
{
	return UString(str);
}

inline UString UString::fromUtf8(std::string_view str)
{
	return UString(std::filesystem::_Convert_narrow_to_wide(__std_code_page::_Utf8, str).c_str());
}

inline UString UString::fromUtf8(std::u8string_view str)
{
	return fromUtf8(std::string_view{ reinterpret_cast<const char*>(str.data()), str.size() });
}

inline UStringUString::fromUtf16(std::u16string_view str)
{
	if constexpr (sizeof(wchar_t) == sizeof(char16_t))
	{
		auto _size = str.size();
		UString res(_size);
		memccpy(res.data(), str.data(), _size, sizeof(char16_t));
		return res;
	}
	else
		return UString(std::filesystem::_Convert_stringoid_to_wide(str, std::filesystem::_Normal_conversion{}).c_str());
}

inline UString UString::fromUtf32(std::u32string_view str)
{
	if constexpr (sizeof(wchar_t) == sizeof(char32_t))
	{
		auto _size = str.size();
		UString res(_size);
		memccpy(res.data(), str.data(), _size, sizeof(char32_t));
		return res;
	}
	else
		return UString(std::filesystem::_Convert_stringoid_to_wide(str,
			std::filesystem::_Normal_conversion{}).c_str());
}

inline UString UString::fromWString(std::wstring_view str)
{
	return UString(str.data());
}