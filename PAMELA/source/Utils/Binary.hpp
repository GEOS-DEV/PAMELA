#pragma once

#define LEVEL_LOG_FILE "DEBUG"
#define LEVEL_LOG_SCREEN "BRIEF"
#ifdef __clang__
#if __clang_major__ > 4
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-template"
#endif
#endif

const int i = 1;
//#define IS_BIG_ENDIAN() ( (*(char*)&i) == 0 )

namespace PAMELA
{

	namespace utils
	{
		
		template <class T>
		static void bites_swap(T *objp)
		{
			if (true)
			{
				unsigned char *memp = reinterpret_cast<unsigned char*>(objp);
				std::reverse(memp, memp + sizeof(T));
			}
		}

	}

}

#ifdef __clang__
#if __clang_major__ > 4
#pragma clang diagnostic pop
#endif
#endif
