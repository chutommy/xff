/**
 * @author Tommy Chu <chutommy@fit.cvut.cz>
 * @date 3.5.23
 */

#include "CPPFile.h"

#include <utility>
#include <unordered_set>
#include <fstream>

CPPFile::CPPFile(std::filesystem::path new_path,
		std::filesystem::file_time_type new_last_write_time,
		size_t new_size,
		size_t new_keyword_count,
		std::set<std::string> new_includes)
		: File(std::move(new_path), new_last_write_time, new_size),
		  keyword_count(new_keyword_count),
		  includes(std::move(new_includes))
{
}

std::ostream& CPPFile::print(std::ostream& os, int width) const
{
	File::print(os, width) << std::setw(width) << std::right << "Keywords: " << keyword_count << "\n"
						   << std::setw(width) << std::right << "Includes: ";
	for (auto it = includes.begin(); it != includes.end(); )
	{
		for (size_t i = 0; i < 5 && it != includes.end(); ++i)
		{
			os << *it << " ";
			++it;
		}
		if (it != includes.end())
			os << "\n" << std::setw(width) << " ";
	}

	return os << "\n";
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
