#pragma once

class i_localize {
public:
	wchar_t* find(const char* token_name) {
		using original_fn = wchar_t* (__thiscall*)(i_localize*, const char*);
		return (*(original_fn * *)this)[12](this, token_name);
	}
};


struct i_base_file_system
{

	int Read(void* pOutput, int size, void* file)
	{
		using original_fn = int(__thiscall*)(void*, void*, int, void*);
		return (*(original_fn**)this)[0](this, pOutput, size, file);
	}

	// if pathID is NULL, all paths will be searched for the file
	void* Open(const char* pFileName, const char* pOptions, const char* pathID = nullptr)
	{
		using original_fn = void* (__thiscall*)(void*, const char*, const char*, const char*);
		return (*(original_fn**)this)[2](this, pFileName, pOptions, pathID);
	}

	void Close(void* file)
	{
		using original_fn = void(__thiscall*)(void*, void*);
		return (*(original_fn**)this)[3](this, file);
	}

	unsigned int Size(void* file)
	{
		using original_fn = unsigned int(__thiscall*)(void*, void*);
		return (*(original_fn**)this)[7](this, file);
	}
};