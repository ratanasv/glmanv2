#include "vir_toolbox.hpp"


std::string vvprintf(const char* format, ...) {
	va_list	args;
	va_start(args, format);

	int		count = vsnprintf(NULL, 0, format, args);
	char*	str = (char*) malloc((count + 1) * sizeof(char));
	vsnprintf(str, count + 1, format, args);

	std::string result(str);
	free(str);

	va_end(args);
	return result;
}

std::shared_ptr<FILE> InitFilePointer(const std::string& name, const char* mode) {
	return std::shared_ptr<FILE>(fopen(name.c_str(), mode), [](FILE* fp) {
		if (fp!=NULL) fclose(fp);	
	});
}

