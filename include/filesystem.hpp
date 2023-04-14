#ifndef OPENGL_RENDERER_FILESYSTEM_HPP
#define OPENGL_RENDERER_FILESYSTEM_HPP

#include <cstdlib>
#include <string>

struct FileSystem {

	static const char* GetRoot() {
		const char* env = std::getenv("OPENGL_RENDERER_ROOT");
		if (env)
			return env;
		return NULL;
	}

	static const char* GetPath(const std::string& path) {
		const char* root = GetRoot();
		if(root)
			return std::string(root + path).c_str();
		return NULL;
	}
};

#endif