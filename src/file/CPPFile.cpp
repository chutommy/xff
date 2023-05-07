/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "CPPFile.h"
#include "../DataFileCorrupted.h"

#include <utility>
#include <unordered_set>
#include <fstream>

CPPFile::CPPFile(std::filesystem::path new_path,
		const Timestamp& new_last_write_time,
		size_t new_size,
		size_t new_keyword_count,
		std::set<std::string> new_includes)
		: File(std::move(new_path), new_last_write_time, new_size),
		  keyword_count(new_keyword_count),
		  includes(std::move(new_includes))
{
}

CPPFile::CPPFile(const std::filesystem::path& file_path)
		: CPPFile(file_path,
		Timestamp(std::filesystem::last_write_time(file_path)),
		file_size(file_path),
		get_keyword_count(file_path),
		get_includes(file_path))
{
}

std::ostream& CPPFile::print(std::ostream& os) const
{
	File::print(os) << std::setw(LABEL_WIDTH) << std::right << "Keywords: " << keyword_count << "\n"
					<< std::setw(LABEL_WIDTH) << std::right << "Includes: ";
	for (auto it = includes.begin(); it != includes.end();)
	{
		for (size_t i = 0; i < 5 && it != includes.end(); ++i)
		{
			os << *it << " ";
			++it;
		}
		if (it != includes.end())
			os << "\n" << std::setw(LABEL_WIDTH) << " ";
	}

	return os << "\n";
}

std::ostream& CPPFile::store(std::ostream& os) const
{
	File::store(os) << keyword_count << "\n";
	for (const auto& name: includes)
		os << name << " ";
	return os << "\n";
}

CPPFile::CPPFile(File& file, std::istringstream& iss) : File(file)
{
	std::string keyword_count_str, includes_str;
	if (!std::getline(iss, keyword_count_str)
		|| !std::getline(iss, includes_str))
		throw DataFileCorrupted("Invalid format");

	if (!onlyDigits(keyword_count_str))
		throw DataFileCorrupted("Invalid keyword count");
	keyword_count = std::stoi(keyword_count_str);

	std::stringstream includes_ss(includes_str);
	while (includes_ss.rdbuf()->in_avail())
	{
		std::string include;
		includes_ss >> include;
		includes.insert(include);
	}
}

bool CPPFile::MatchKeywordCount(const IntTerm& term) const
{
	switch (term.opt)
	{
	case lt:
		return keyword_count < term.value;
	case lte:
		return keyword_count <= term.value;
	case eq:
		return keyword_count == term.value;
	case gte:
		return keyword_count >= term.value;
	case gt:
		return keyword_count > term.value;
	default:
		throw std::runtime_error("Unexpected integer term option value");
	}
}

bool CPPFile::MatchInclude(const StringTerm& term) const
{
	for (const std::string& s: includes)
		if (s == term.value)
			return true;
	return false;
}

const std::unordered_set<std::string>& get_keywords()
{
	static const std::unordered_set<std::string> keywords{
			"alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept", "auto",
			"bitand", "bitor", "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t", "class",
			"compl", "concept", "const", "consteval", "constexpr", "constinit", "const_cast", "continue", "co_await",
			"co_return", "co_yield", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
			"explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long",
			"mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private",
			"protected", "public", "reflexpr", "register", "reinterpret_cast", "requires", "return", "short", "signed",
			"sizeof", "static", "static_assert", "static_cast", "struct", "switch", "synchronized", "template", "this",
			"thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
			"virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq",
			"final", "override", "transaction_safe", "transaction_safe_dynamic", "import", "module", "elif", "ifdef",
			"ifndef", "elifdef", "elifndef", "define", "undef", "include", "line", "error", "warning", "pragma",
	};

	return keywords;

}

size_t get_keyword_count(const std::filesystem::path& path)
{
	const std::unordered_set<std::string>& keywords = get_keywords();

	std::ifstream file(path);
	std::string word;
	int count = 0;
	while (file >> word)
		if (keywords.count(word))
			++count;

	return count;
}

std::set<std::string> get_includes(const std::filesystem::path& path)
{
	std::set<std::string> includes;
	std::ifstream file(path);
	std::string line;
	while (std::getline(file, line))
		if (line.rfind("#include", 0) == 0)
		{
			std::stringstream ss(line);
			std::string include_direct, name;
			ss >> include_direct >> name;
			includes.insert(name.substr(1, name.size() - 2));
		}

	return includes;
}