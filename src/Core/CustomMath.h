#pragma once

namespace Pitfall
{
	class Math
	{
	public:
		inline static constexpr float Pi = 3.14159f;
		inline static constexpr float Pi2 = Math::Pi * 2.0f;

		// Resource for Rad2Deg constant https://www.bilsen.com/agkpas/documentation/RAD2DEG.html
		// Resource for Deg2Rad constant https://www.bilsen.com/agkpas/documentation/DEG2RAD.html
		inline static constexpr float Rad2Deg = 57.29578f;
		inline static constexpr float Deg2Rad = 0.0174532f;

		template<typename InType>
		inline static InType Floor(InType in)
		{
			return Cast<int>(in - (in < 0 ? 1.0 : 0.0));
		}

		template<typename InType>
		inline static int FloorToInt(InType in)
		{
			return Cast<int>(in - (in < 0 ? 1.0 : 0.0));
		}

		template<typename InType>
		inline static InType Ceil(InType in)
		{
			return Cast<int>(in - (in < 0 ? 1.0 : 0.0)) + 1;
		}

		template<typename InType>
		inline static int CeilToInt(InType in)
		{
			return Cast<int>(in - (in < 0 ? 1.0 : 0.0)) + 1;
		}

		template<typename InType>
		inline static InType Round(InType in)
		{
			return in + 0.5f - (in < 0 ? 1 : 0);
		}

		template<typename InType>
		inline static int RoundToInt(InType in)
		{
			return Cast<int>(in + 0.5f - (in < 0 ? 1 : 0));
		}

		template<typename InType>
		inline static InType Min(InType a, InType b)
			{
			return a < b ? a : b;
		}

		template<typename InType>
		inline static InType Max(InType a, InType b)
		{
			return a < b ? b : a;
		}

		template<typename InType>
		inline static float Sin(InType in)
		{
			return sin(in);
		}

		template<typename InType>
		inline static float Cos(InType in)
		{
			return cos(in);
		}

		template<typename InType>
		inline static float Tan(InType in)
		{
			return tan(in);
		}

		inline static float Pow(float in, float mul)
		{
			return powf(in, mul);
		}

		template<typename InType>
		inline static InType Pow(InType in, int mul)
		{
			return pow(in, mul);
		}

		template<typename InType>
		inline static InType Clamp(InType in, InType min, InType max)
		{
			if (in < min)
				in = min;
			if (in > max)
				in = max;
			return in;
		}

		template<typename InType>
		inline static InType Sqrt(InType in)
		{
			return sqrtf(in);
		}

		template<typename InType>
		inline static InType Abs(const InType& in)
		{
			return in < 0 ? -in : in;
		}

		template<typename InType>
		inline static float Length(const InType& in)
		{
			return Math::Sqrt(in.x * in.x + in.y * in.y);
		}

		template<typename InType>
		inline static InType Normalize(const InType& in)
		{
			return in / Length(in);
		}

		template<typename InType>
		inline static InType Lerp(InType a, InType b, float t)
		{
			return a + (b - a) * t;
		}

		template<typename InType>
		inline static InType LerpFloat2(InType a, InType b, float t)
		{
			return { Math::Lerp<float>(a.x, b.x, t), Math::Lerp<float>(a.y, b.y, t) };
		}


		// Resource for inverse lerp function.
		// https://www.gamedev.net/articles/programming/general-and-gameplay-programming/inverse-lerp-a-super-useful-yet-often-overlooked-function-r5230/ 

		template<typename InType>
		inline static InType InvLerp(InType a, InType b, InType v)
		{
			return (v - a) / (b - a);
		}

		template<typename InType>
		inline static InType InvLerpFloat2(InType a, InType b, InType v)
		{
			return { Math::InvLerp(a.x, b.x, v), Math::InvLerp(a.y, b.y, v) };
		}

		/// <summary>
		/// Reduces value to 0 with a linear speed.
		/// </summary>
		template<typename InType>
		inline static InType Reduce(InType a, InType b, InType speed)
		{
			a = a > b ? Math::Clamp(a, b, a - speed) : Math::Clamp(a, a + speed, b);
			return a;
		}


		/// <summary>
		/// Reduces value to 0 with a linear speed.
		/// </summary>
		template<typename InType>
		inline static InType ReduceFloat2(const InType& a, const InType& b, float speed)
		{
			return { Reduce<float>(a.x, b.x, speed), Reduce<float>(a.y, b.y, speed) };
		}



		// Resource for learning Dot product formula.
		// ChatGPT

		/// Returns the dot product of the 2 vectors.
		inline static float Dot(const float2& a, const float2& b)
		{
			return a.x * b.x + a.y * b.y;
		}

		// Resource for learning about vector reflection.
		// ChatGPT

		// Returns the reflected vector.
		inline static float2 Reflect(const float2& a, const float2& norm)
		{
			const float dot = Dot(a, norm);
			return a - (2 * dot) * norm;
		}


		// Beziercurve method uses a static list because it eliminates the need to create a dynamically heap-allocated DynamicList (vector) which is way slower.
		// Resource: https://www.matecdev.com/posts/c-heap-vs-stack-allocation.html#:~:text=Slower%20performance%3A%20Heap%20allocation%20involves,deallocated%20to%20prevent%20memory%20leaks. 
		// 
		// By using a template for its InSize the algorithm is scalable and can be applied for any static array size.
		
		// Resource for learning about spline bezier curves and code implementation.
		// ChatGPT

		// Returns t% position on a bezier curve between points.
		template<uint32_t InSize>
		inline static float2 BezierCurve(StaticList<float2, InSize> points, float t) // Big: O(n2)
		{
			static_assert(InSize != 0, "Bezier curve size can not be 0");

			int n = points.Size();												// O(1)
																				
			for (int k = 1; k <= n; k++) {										// O(n2)
				for (int i = 0; i < n - k; i++) {								
					points[i].x = (1 - t) * points[i].x + t * points[i + 1].x;	// O(1)
					points[i].y = (1 - t) * points[i].y + t * points[i + 1].y;	// O(1)
				}
			}

			return points[0];									            	// O(1)
		}
	};
}