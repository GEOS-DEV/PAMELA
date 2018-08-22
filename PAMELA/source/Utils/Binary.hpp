#pragma once

const int i = 1;
#define IS_BIG_ENDIAN() ( (*(char*)&i) == 0 )

namespace PAMELA
{

	namespace utils
	{
		
		template <class T>
		static void bites_swap(T *objp)
		{
			if (!IS_BIG_ENDIAN())
			{
				unsigned char *memp = reinterpret_cast<unsigned char*>(objp);
				std::reverse(memp, memp + sizeof(T));
			}
		}

	}

}
