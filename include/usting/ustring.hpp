#ifndef USTRING_HPP
#define USTRING_HPP

#include <assert.h>
#include <string>
#include <locale>
#include <charconv>
#include <filesystem>
#include <codecvt>

#ifdef QT_CORE_LIB
#include <QString>
#endif

class UChar
{
	char16_t c;

	inline static const std::ctype<wchar_t>& ctype = std::use_facet<std::ctype<wchar_t>>(std::locale::classic());

public:
	constexpr UChar() noexcept = default;
	constexpr UChar(char c) noexcept :c(c) {}
	constexpr UChar(char16_t c) noexcept :c(c) {}
	constexpr UChar(char8_t c) noexcept :c(c) {}
	constexpr UChar(wchar_t c) noexcept :c(c) {}

	constexpr UChar(const UChar&) noexcept = default;
	constexpr UChar(UChar&&)noexcept = default;
	constexpr ~UChar()noexcept = default;

	UChar toUpper() const noexcept { return fromCode(ctype.toupper(c)); }
	UChar toLower() const noexcept { return fromCode(ctype.tolower(c)); }

	bool isDigit() const noexcept { return ctype.is(ctype.digit, c); }
	//is this symbol alphabet or number
	bool isAlnum() const noexcept { return ctype.is(ctype.alnum, c); }
	//is hex digit
	bool isXDigit() const noexcept { return ctype.is(ctype.xdigit, c); }
	//is alphabet symbol
	bool isAlpha() const noexcept { return ctype.is(ctype.alpha, c); }
	//is space or hor tab
	bool isSpace() const noexcept { return ctype.is(ctype.space, c); }
	bool isUpper() const noexcept { return ctype.is(ctype.upper, c); }
	bool isLower() const noexcept { return ctype.is(ctype.lower, c); }
	//, . ' and other
	bool isPunct() const noexcept { return ctype.is(ctype.punct, c); }
	//this non control symbols
	bool isPrint() const noexcept { return ctype.is(ctype.graph, c); }
	//is control symbols
	bool isCntrl() const noexcept { return ctype.is(ctype.cntrl, c); }
	constexpr bool isNull() const noexcept { return c == 0; }

	constexpr int toCode() const noexcept { return c; }
	constexpr char toChar() const noexcept { return c; }
	constexpr wchar_t toWChar() const noexcept { return c; }
	constexpr char16_t toChar16() const noexcept { return c; }
	constexpr char8_t toChar8() const noexcept { return c; }
	constexpr char32_t toChar32() const noexcept { return c; }
#ifdef QT_CORE_LIB
	constexpr QChar toQChar() const noexcept { return QChar(c); }
#endif

	static constexpr UChar fromCode(int code)noexcept { UChar res; res.c = code; return res; }

	constexpr operator char16_t() const noexcept { return c; }

	constexpr UChar& operator=(const UChar&)noexcept = default;
	constexpr UChar& operator=(UChar&&)noexcept = default;
};

class UString :
	protected std::basic_string<UChar, std::char_traits<UChar>, std::allocator<UChar>>
{
	typedef std::basic_string<UChar, std::char_traits<UChar>, std::allocator<UChar>> src_str;

	constexpr static bool isValidSymbolForNum(int c) noexcept;
	constexpr static size_t srtLen(const char* ptr) noexcept;

#if !NDEBUG
	std::u16string& dbgView= *(std::u16string*)this;
#endif

public:
	UString()noexcept = default;
	explicit constexpr UString(uint32_t size, UChar ch = ' ') :src_str(size, ch) {}
	explicit constexpr UString(const UChar* str) : src_str(str) {}
	constexpr UString(const UChar* str, uint32_t count) : src_str(str, count) {}
	constexpr UString(const UChar* startStr, const UChar* endStr) : src_str(startStr, endStr) {}
	constexpr UString(const char* str, uint32_t count);
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

	constexpr UString& operator=(UString&& val)noexcept { 
		src_str::operator=(std::move(val));
		return *this;
	}
	constexpr UString& operator=(const UString& val) { src_str::operator=(val); return *this; }
	constexpr UString& operator=(const UChar* val) { src_str::operator=(val); return *this; }
	constexpr UString& operator=(const char* val) { src_str::operator=(UString(val)); return *this; }
	constexpr UString& operator=(UChar val) { src_str::operator=(val); return *this; }

	constexpr UString& operator+=(UString&& val) { src_str::operator+=(std::move(val)); return *this; }
	constexpr UString& operator+=(const UString& val) { src_str::operator+=(val); return *this; }
	constexpr UString& operator+=(const char* val);
	constexpr UString& operator+=(const UChar* val) { src_str::operator+=(val); return *this; }
	constexpr UString& operator+=(UChar val) { src_str::operator+=(val); return *this; }

	constexpr UString operator+(UString&& val) const {
		UString str; str.reserve(size() + val.size()); str = *this; str += val; return str;
	}
	constexpr UString operator+(const UString& val) const {
		UString str; str.reserve(size() + val.size()); str = *this; str += val; return str;
	}
	constexpr UString operator+(const char* val) const { UString str(*this); str += val; return str; }
	constexpr UString operator+(const UChar* val) const { UString str(*this); str += val; return str; }

	constexpr bool operator==(UString&& val) const noexcept;
	constexpr bool operator!=(UString&& val) const noexcept { return !operator==(std::move(val)); }

	constexpr bool operator==(const UString& val) const noexcept { return *((src_str*)this) == *((src_str*)&val); }
	constexpr bool operator!=(const UString& val)const noexcept { return !operator==(val); }

	constexpr bool operator==(const UChar* val) const noexcept { return *((src_str*)this) == val; }
	constexpr bool operator!=(const UChar* val)const noexcept { return !operator==(val); }

	constexpr bool operator==(const char* val) const noexcept;
	constexpr bool operator!=(const char* val)const noexcept { return !operator==(val); }

	constexpr void popFront() { src_str::erase(0, 1); }

	constexpr size_t find(UChar c, size_t off = 0, bool ignoreCase = false)const noexcept;
	constexpr size_t find(const UString& str, size_t off = 0, bool ignoreCase = false)const noexcept;
	constexpr size_t rfind(UChar c, size_t off = 0, bool ignoreCase = false)const noexcept;
	constexpr size_t rfind(const UString& str, size_t off = 0, bool ignoreCase = false)const noexcept;

	constexpr UString substr(size_t off, size_t count)const;
	constexpr std::vector<UString> split(const UString& separator, bool ignoreCase = false, bool saveEmpty = true)const;
	constexpr void insert(size_t offset, const char* str);
	constexpr void insert(size_t offset, const UChar* str);
	constexpr void insert(size_t offset, const UString& str);
	constexpr void insert(size_t offset, UString&& str);
	constexpr void insert(size_t offset, UChar c);

	constexpr bool startsWith(const UString& val, bool ignoreCase = false)const noexcept;
	constexpr bool startsWith(const UChar* valPtr, bool ignoreCase = false)const noexcept;
	constexpr bool startsWith(const char* valPtr, bool ignoreCase = false)const noexcept;
	constexpr bool endsWith(const UString& val, bool ignoreCase = false)const noexcept;
	constexpr bool endsWith(const UChar* valPtr, bool ignoreCase = false)const noexcept;
	constexpr bool endsWith(const char* valPtr, bool ignoreCase = false)const noexcept;

	constexpr bool contains(const UString& str, bool ignoreCase = false)const noexcept { return find(str,0, ignoreCase) != npos; }

	constexpr size_t count(const UString& subStr, bool ignoreCase = false)const noexcept;
	constexpr size_t count(const UChar c, bool ignoreCase = false)const noexcept;
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
	std::string toString(const std::locale& locale = std::locale(), UChar _default='-')const;
	std::string toStringUtf8()const;
	std::u8string toUtf8()const;
	constexpr std::u16string toUtf16()const;
	std::u32string toUtf32()const;
	std::wstring toWString()const;
	std::filesystem::path toPath()const;
#ifdef QT_CORE_LIB
	QString toQString()const { return QString::fromStdU16String(*(std::u16string*)this); }
#endif

	template <class T>
	static std::enable_if_t<std::is_integral<T>::value, UString> fromIntegral(T num, int base = 10);
	//format: 'g'- general; 'f' - fixed; 's' - scientific; 'h' - hex
	//if precision < 0: auto precision
	template <class T>
	static std::enable_if_t<std::is_floating_point<T>::value, UString> 
		fromFloatingPoint(T num, uint8_t format = 'g', int8_t precision = -1);
	static UString fromString(std::string_view str, const std::locale& locale = std::locale());
	static constexpr UString fromLatin(std::string_view str);
	static UString fromUtf8(std::string_view str);
	static UString fromUtf8(std::u8string_view str);
	static constexpr UString fromUtf16(std::u16string_view str);
	static UString fromUtf32(std::u32string_view str);
	static UString fromWString(std::wstring_view str);
	static UString fromPath(const std::filesystem::path& path);
#ifdef QT_CORE_LIB
	static UString fromQString(const QString& str) { return UString::fromUtf16(str.toStdU16String()); }
#endif

	static bool compare(const UString& str0, const UString& str1, bool ignoreCase) noexcept;
};

constexpr UString::UString(const char* str,uint32_t count)
{
	resize(count);

	for (auto currentPtr = data(), end = currentPtr + count; currentPtr < end; ++str, ++currentPtr)
	{
		*currentPtr = *str;
	}
}

constexpr bool UString::isValidSymbolForNum(int c)noexcept
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
		if (isValidSymbolForNum(ptr->toCode()))
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

constexpr size_t UString::find(UChar c, size_t off, bool ignoreCase)const noexcept
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

		if (c.isAlpha())
		{
			auto forCompare = c.toLower();

			while (srcPtr < srcEnd)
			{
				if (srcPtr->toLower() == forCompare)
					return srcPtr - data();

				srcPtr++;
			}
		}
		else
		{
			while (srcPtr < srcEnd)
			{
				if (*srcPtr == c)
					return srcPtr - data();

				srcPtr++;
			}
		}
	}
	return npos;
}

constexpr size_t UString::find(const UString& str, size_t off, bool ignoreCase)const noexcept
{
	auto otherStrSize = str.size();
	if (otherStrSize == 0)
		return npos;

	assert(size() >= off);

	if (ignoreCase == false)
	{
		return src_str::find(str,off);
	}
	else if (otherStrSize <=size() - off)
	{
		auto srcPtr = data() + off;
		const auto srcEnd = srcPtr + size() - off - str.size() + 1;
		const auto otherPtr = str.data();
		const auto otherEnd = otherPtr + str.size();

		auto c = otherPtr->toLower();

		while (srcPtr< srcEnd)
		{
			if (srcPtr->toLower() == c)
			{
				auto tmpPtr0 = otherPtr + 1;
				auto tmpPtr1 = srcPtr + 1;
				while (tmpPtr0 < otherEnd)
				{
					if (tmpPtr0->toLower() != tmpPtr1->toLower())
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

constexpr size_t UString::rfind(UChar c, size_t off, bool ignoreCase)const noexcept
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
		if (c.isAlpha())
		{
			auto forCompare = c.toLower();
			while (srcPtr > srcEnd)
			{
				if (srcPtr->toLower() == forCompare)
					return srcPtr - data();

				--srcPtr;
			}
		}
		else
		{
			while (srcPtr > srcEnd)
			{
				if (*srcPtr == c)
					return srcPtr - data();

				--srcPtr;
			}
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
		auto c = otherPtr->toLower();

		while (srcPtr > srcEnd)
		{
			if (srcPtr->toLower() == c)
			{
				auto tmpPtr0 = otherPtr + 1;
				auto tmpPtr1 = srcPtr + 1;
				while (tmpPtr0 < otherEnd)
				{
					if (tmpPtr0->toLower() != tmpPtr1->toLower())
						break;
					++tmpPtr0;
					++tmpPtr1;
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
	return UString(data()+off, count);
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
				res.emplace_back(UString(data() + oldPos, s));

			if (npos - pos >= sizeSep)
				pos += sizeSep;
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
					isFind = currentPtr->toLower() == *j;

				if (isFind)
					break;
			}
			
			auto s = pos - oldPos;
			if (s || saveEmpty)
				res.emplace_back(UString(data() + oldPos, s));

			if (npos - pos >= sizeSep)
				pos += sizeSep;
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
			if (currentPtr->toLower() != otherPtr->toLower())
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
			if (currentPtr->toLower().toCode() != tolower(*otherPtr))
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
			if (currentPtr->toLower() != otherPtr->toLower())
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
			if (currentPtr->toLower().toCode() != tolower(*otherPtr))
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
		i = i.toUpper();
}

inline void UString::convertToLower()
{
	for (auto& i : *this)
		i = i.toLower();
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
		if (i.isAlpha() &&i.isUpper())
			return false;
	return true;
}

inline bool UString::isUpper()const
{
	for (auto& i : *this)
		if (i.isAlpha() && i.isLower())
			return false;
	return true;
}

constexpr std::string UString::toLatin() const
{
	std::string res;
	res.reserve(size());

	for (auto i = data(), end = i + size(); i < end; i++)
	{
		if (*i >= std::numeric_limits<char>::min() && *i <= std::numeric_limits<char>::max())
			res.push_back(i->toChar());
	}
	return res;
}

inline std::string UString::toString(const std::locale& locale, UChar _default) const
{
	//https://www.codeproject.com/Tips/196097/Converting-ANSI-to-Unicode-and-back

	std::string res(this->size() * 2, 0);

	if constexpr (sizeof(wchar_t)==sizeof(char16_t))
	{
		auto ptr = (wchar_t*)data();
		std::use_facet<std::ctype<wchar_t>>(locale).narrow(ptr, ptr + size(), _default.toCode(), res.data());
	}
	else
	{
		auto wstr = toWString();
		auto ptr = wstr.data();
		std::use_facet<std::ctype<wchar_t>>(locale).narrow(ptr, ptr + wstr.size(), _default.toCode(), res.data());
	}

	auto pos = res.find(char(0), 0);

	if (pos != res.npos)
		res.resize(pos);

	return res;
}

inline std::string UString::toStringUtf8() const
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	auto ptr = (char16_t*)data();
	return converter.to_bytes(ptr, ptr + size());
}

inline std::u8string UString::toUtf8() const
{
	// Convert the u16string to u8string
	auto str = toStringUtf8();

	return *(std::u8string*)&str;
}

constexpr std::u16string UString::toUtf16() const
{
	const char16_t* startPtr = (const char16_t*)data();
	return std::u16string(startPtr, startPtr + size());
}

inline std::u32string UString::toUtf32() const
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	return conv.from_bytes(toStringUtf8());
}

inline std::wstring UString::toWString() const
{
	if constexpr (sizeof(wchar_t)==sizeof(char16_t))
	{
		auto ptr = (wchar_t*)data();
		return std::wstring(ptr, ptr + size());
	}
	else
	{
		std::wstring_convert<std::codecvt_utf16<wchar_t>> converter;

		// Convert the u16string to wstring
		return converter.from_bytes(
			reinterpret_cast<const char*>(this->c_str()),
			reinterpret_cast<const char*>(this->c_str() + this->size())
		);
	}
}

inline std::filesystem::path UString::toPath()const
{
	return std::filesystem::path(*(std::u16string*)this);
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
				if (ptr0->toLower() != ptr1->toLower())
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
	//https://www.codeproject.com/Tips/196097/Converting-ANSI-to-Unicode-and-back

	const auto& _Facet = std::use_facet<std::codecvt<wchar_t, char, mbstate_t>>(locale);

	std::wstring _Output(str.size(), '\0'); // almost always sufficient

	for (int count=0;;++count) {
		mbstate_t _State{};
		const char* const _From_begin = str.data();
		const char* const _From_end = _From_begin + str.size();
		const char* _From_next = nullptr;
		wchar_t* const _To_begin = _Output.data();
		wchar_t* const _To_end = _To_begin + _Output.size();
		wchar_t* _To_next = nullptr;

		const auto _Result = _Facet.in(_State, _From_begin, _From_end, _From_next, _To_begin, _To_end, _To_next);

		if (_From_next < _From_begin || _From_next > _From_end || _To_next < _To_begin || _To_next > _To_end) {
			std::_Throw_system_error(std::errc::invalid_argument);
		}

		switch (_Result) {
		case std::codecvt_base::ok:
			_Output.resize(static_cast<size_t>(_To_next - _To_begin));
			return fromWString(_Output);

		case std::codecvt_base::partial:
			if (count>2)
				std::_Throw_system_error(std::errc::invalid_argument);

			// N4810 28.4.1.4.2 [locale.codecvt.virtuals]/5:
			// "A return value of partial, if (from_next == from_end), indicates that either the
			// destination sequence has not absorbed all the available destination elements,
			// or that additional source elements are needed before another destination element can be produced."
			if ((_From_next == _From_end && _To_next != _To_end) || _Output.size() > _Output.max_size() / 2) {
				std::_Throw_system_error(std::errc::invalid_argument);
			}

			_Output.resize(_Output.size() * 2);
			break; // out of switch, keep looping

		//case codecvt_base::error:
		//case codecvt_base::noconv:
		default:
			std::_Throw_system_error(std::errc::invalid_argument);
		}
	}
}

constexpr UString UString::fromLatin(std::string_view str)
{
	UString res(str.size());
	auto currentPtr = res.data();
	
	for (auto otherPtr = str.data(),end= otherPtr+str.size(); otherPtr < end; ++otherPtr)
	{
		/*if (*currentPtr < '\a' )
		{
			res.pop_back();
		}
		else*/
		{
			*currentPtr = *otherPtr;
			++currentPtr;
		}
	}

	return res;
}

inline UString UString::fromUtf8(std::string_view str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;

	return fromUtf16(converter.from_bytes(str.data(), str.data() + str.size()));
}

inline UString UString::fromUtf8(std::u8string_view str)
{
	return fromUtf8(std::string_view{ reinterpret_cast<const char*>(str.data()), str.size() });
}

constexpr UString UString::fromUtf16(std::u16string_view str)
{
	return UString((UChar*)str.data(), str.size());
}

inline UString UString::fromUtf32(std::u32string_view str)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
	return fromUtf8(converter.to_bytes(str.data(), str.data() + str.size()));
}

inline UString UString::fromWString(std::wstring_view str)
{
	if constexpr (sizeof(wchar_t) == sizeof(char16_t))
	{
		return UString((UChar*)str.data(), str.size());
	}
	else if constexpr (sizeof(wchar_t) == sizeof(char32_t))
	{
		return fromUtf16(std::filesystem::_Convert_wide_to<std::char_traits<char16_t>,
			std::allocator<char16_t>, char16_t>(str, std::allocator<char16_t>()));
	}
	else
	{
		throw std::exception("Invalid size wchar_t");
	}
}

inline UString UString::fromPath(const std::filesystem::path& path)
{
	return UString::fromUtf16(path.u16string());
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

#endif