#pragma once

// Base configuration defines.
#ifdef _DEBUG 
	#define PF_DEBUG
#endif

#ifdef VISDEBUG
	#define PF_VISDEBUG
#endif

#ifdef RELEASE 
	#define PF_RELEASE
#endif

#ifdef DIST
	#define PF_DISTRIBUTE
#endif

// Single line debugging configuration defines.
#ifdef PF_DEBUG
	#define DEBUGONLY(expression) expression
#else 
	#define DEBUGONLY(expression) ;;
#endif

#ifdef PF_VISDEBUG
#define VISDEBUGONLY(expression) expression
#else 
#define VISDEBUGONLY(expression) ;;
#endif


// Debugging configuration defines.
#if defined(PF_DEBUG) || defined(PF_RELEASE)
#define Assert(expression, message) if(!(expression))																		\
									{																						\
										printf("%s %s %i %s %s %c", message, " Line:", __LINE__, " File:", __FILE__, '\n'); \
										assert(false);																		\
									}
#define Print(message)		printf("%s %c", message, '\n')
#define PrintInt(value)		printf("%i %c", value, '\n')
#define PrintBool(value)	printf("%s %c", value ? "true" : "false", '\n')
#define PrintFloat(value)   printf("%f %c", value, '\n')
#define PrintFloat2(value)  printf("%f %f %c", value.x, value.y, '\n')
#define PrintInt2(value)    printf("%i %i %c", value.x, value.y, '\n')
#define PrintEndl()			printf("%c", '\n')
#else
#define Assert(expression, message) ;;
#define Print(message)				;;
#define PrintInt(value)				;;
#define PrintBool(value)			;;
#define PrintFloat(value)			;;
#define PrintFloat2(value)			;;
#define PrintInt2(value)            ;;
#define PrintEndl()					;;
#endif

#define Cast static_cast
#define TryCast dynamic_cast

#define RgbColor(r, g, b) (r << 16) + (g << 8) + (b)
#define RgbaColor(r, g, b, a) (a << 24) + (r << 16) + (g << 8) + (b)

namespace Pitfall
{
	struct AABB;
	struct RectFloat;

	namespace Utils
	{
		inline bool HasMatchingBit(uint32_t a, uint32_t b)
		{
			return (a & b) != 0;
		}

		// Resource for finding the size of a file in c
		// https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
		uint32_t GetFileCharCount(FILE* file);

		bool CalculateOverlap(const float2& min1, const float2& max1, const float2& min2, const float2& max2, float2& out);

		AABB RectToAABB(const RectFloat& rect);
		
		inline uint32_t IntegerNumberCount(int in)
		{
			if (in == 0) return 1;

			uint32_t numberCount = 0;
			int n = in;
			while (n >= 1)
			{
				numberCount++;
				n /= 10;
			}

			return numberCount;
		}
	}
}