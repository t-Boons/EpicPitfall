// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#include "pfpch.h"

using namespace Tmpl8;

// constructor
Sprite::Sprite( Surface* surface, unsigned int frameCount ) :
	width( surface->width / frameCount ),
	height( surface->height ),
	numFrames( frameCount ),
	currentFrame( 0 ),
	flags( 0 ),
	start( new unsigned int* [frameCount] ),
	surface( surface )
{
	InitializeStartData();
}

// destructor
Sprite::~Sprite()
{
	// Do not delete surface as its used by multiple sprites.
	//delete surface;
	for (unsigned int i = 0; i < numFrames; i++) delete start[i];
	delete start;
}

// draw sprite to target surface
void Sprite::Draw( Surface* target, int x, int y )
{
	if (x < -width || x > ( target->width + width )) return;
	if (y < -height || y > ( target->height + height )) return;
	int x1 = x, x2 = x + width;
	int y1 = y, y2 = y + height;
	uint* src = GetBuffer() + currentFrame * width;
	if (x1 < 0) src += -x1, x1 = 0;
	if (x2 > target->width) x2 = target->width;
	if (y1 < 0) src += -y1 * width * numFrames, y1 = 0;
	if (y2 > target->height) y2 = target->height;
	uint* dest = target->pixels;
	int xs;
	if (x2 > x1 && y2 > y1)
	{
		unsigned int addr = y1 * target->width + x1;
		const int w = x2 - x1;
		const int h = y2 - y1;
		for (int j = 0; j < h; j++)
		{
			const int line = j + (y1 - y);
			const int lsx = start[currentFrame][line] + x;
			xs = (lsx > x1) ? lsx - x1 : 0;
			for (int i = xs; i < w; i++)
			{
				const uint c1 = *(src + i);
				if (c1 & 0xffffff) *(dest + addr + i) = c1;
			}
			addr += target->width;
			src += width * numFrames;
		}
	}
}

// Modified function to allow drawing negative sprites.
void Sprite::DrawScaledSafe( int x1, int y1, int w1, int h1, Surface* target ) const
{
	const int w = Pitfall::Math::Abs(w1);
	const int h = Pitfall::Math::Abs(h1);

	const float hmul = (float)height / (float)h;
	const float wmul = (float)width / (float)w;

	// I am caching the buffers to reduce the need for a memory lookup every pixel.
	const int2 targetSize = { target->width, target->height };
	const int surfWidth = surface->width;
	const int currFrameWidth = width * currentFrame;
	uint* targetBuffer = target->pixels;

	if (w1 > 0 && h1 > 0)
	{
		for (int y = 0; y < h; ++y)
		{
			const int v = (int)((float)y * hmul);
			if (y1 + y <= 0 || y1 + y > targetSize.y - 1) continue;

			const int vww = v * surfWidth + currFrameWidth;
			const int y1yt = (y1 + y) * targetSize.x;

			for (int x = 0; x < w; ++x)
			{
				if (x1 + x < 0 || x1 + x > targetSize.x - 1) continue;

				const int u = (int)((float)x * wmul);

				const uint color = GetBuffer()[u + vww];

				if (color >> 24) targetBuffer[x1 + x + y1yt] = color;
			}
		}
	}
	else if (w1 > 0 && h1 <= 0)
	{
		for (int y = 0; y < h; ++y)
		{
			const int v = height - (int)((float)y * hmul);
			if (y1 + y <= 0 || y1 + y > targetSize.y - 1) continue;

			const int vww = v * surfWidth + currFrameWidth;
			const int y1yt = (y1 + y) * targetSize.x;

			for (int x = 0; x < w; ++x)
			{
				if (x1 + x < 0 || x1 + x > targetSize.x - 1) continue;

				const int u = (int)((float)x * wmul);


				const uint color = GetBuffer()[u + vww];

				if (color >> 24) targetBuffer[x1 + x + y1yt] = color;
			}
		}
	}
	else if (w1 <= 0 && h1 > 0)
	{
		for (int y = 0; y < h; ++y)
		{
			const int v = (int)((float)y * hmul);
			if (y1 + y <= 0 || y1 + y > targetSize.y - 1) continue;

			const int vww = v * surfWidth + currFrameWidth;
			const int y1yt = (y1 + y) * targetSize.x;

			for (int x = 0; x < w; ++x)
			{
				if (x1 + x < 0 || x1 + x > targetSize.x - 1) continue;

				const int u = width - 1 - (int)((float)x * wmul);

				const uint color = GetBuffer()[u + vww];

				if (color >> 24) targetBuffer[x1 + x + y1yt] = color;
			}
		}
	}
	else if (w1 <= 0 && h1 <= 0)
	{
		for (int y = 0; y < h; ++y)
		{
			const int v = height - (int)((float)y * hmul);
			if (y1 + y <= 0 || y1 + y > targetSize.y - 1) continue;

			const int vww = v * surfWidth + currFrameWidth;
			const int y1yt = (y1 + y) * targetSize.x;

			for (int x = 0; x < w; ++x)
			{
				if (x1 + x < 0 || x1 + x > targetSize.x - 1) continue;

				const int u = width - 1 - (int)((float)x * wmul);

				const uint color = GetBuffer()[u + vww];

				if (color >> 24) targetBuffer[x1 + x + y1yt] = color;
			}
		}
	}
}

void Tmpl8::Sprite::DrawScaled(int x1, int y1, int w1, int h1, Surface* target) const
{
	const int w = Pitfall::Math::Abs(w1);
	const int h = Pitfall::Math::Abs(h1);

	const float hmul = (float)height / (float)h;
	const float wmul = (float)width / (float)w;

	// I am caching the buffers to reduce the need for a memory lookup every pixel.
	const int2 targetSize = { target->width, target->height };
	const int surfWidth = surface->width;
	uint* targetBuffer = target->pixels;

	if (w1 > 0 && h1 > 0)
	{
		for (int y = 0; y < h; ++y)
		{
			const int v = (int)((float)y * hmul);

			for (int x = 0; x < w; ++x)
			{

				const int u = (int)((float)x * wmul);

				const uint buff = u + v * surfWidth + width * currentFrame;
				const uint color = GetBuffer()[buff];

				if (color >> 24) targetBuffer[x1 + x + ((y1 + y) * targetSize.x)] = color;
			}
		}
	}
	else if (w1 > 0 && h1 <= 0)
	{
		for (int y = 0; y < h; ++y)
		{
			const int v = height - (int)((float)y * hmul);

			for (int x = 0; x < w; ++x)
			{

				const int u = (int)((float)x * wmul);

				const uint buff = u + v * surfWidth + width * currentFrame;
				const uint color = GetBuffer()[buff];

				if (color >> 24) targetBuffer[x1 + x + ((y1 + y) * targetSize.x)] = color;
			}
		}
	}
	else if (w1 <= 0 && h1 > 0)
	{
		for (int y = 0; y < h; ++y)
		{
			const int v = (int)((float)y * hmul);

			for (int x = 0; x < w; ++x)
			{

				const int u = width - 1 - (int)((float)x * wmul);

				const uint buff = u + v * surfWidth + width * currentFrame;
				const uint color = GetBuffer()[buff];

				if (color >> 24) targetBuffer[x1 + x + ((y1 + y) * targetSize.x)] = color;
			}
		}
	}
	else if (w1 <= 0 && h1 <= 0)
	{
		for (int y = 0; y < h; ++y)
		{
			const int v = height - (int)((float)y * hmul);

			for (int x = 0; x < w; ++x)
			{
				const int u = width - 1 - (int)((float)x * wmul);

				const uint buff = u + v * surfWidth + width * currentFrame;
				const uint color = GetBuffer()[buff];

				if (color >> 24) targetBuffer[x1 + x + ((y1 + y) * targetSize.x)] = color;
			}
		}
	}
}

// Modified method for drawing from sprite atlasses.
void Tmpl8::Sprite::DrawCroppedSafe(int xi, int yi, int wi, int hi, int x, int y, Surface* target)
{
	if (x < -wi || x > target->width) return;
	if (y < -hi || y > target->height) return;

	const int x1 = x, x2 = x + wi;
	const int y1 = y, y2 = y + hi;

	uint* src = GetBuffer() + width * yi;
	uint* dest = target->pixels;

	if (x2 > x1 && y2 > y1)
	{
		if (x2 > x1 && y2 > y1)
		{
			unsigned int addr = x + target->width * y;
			for (int j = 0; j < hi; j++)
			{
				for (int i = 0; i < wi; i++)
				{
					// TODO: Optimize this algorithm so it does NOT use if statements in the for loop.
					// TODO: Profile the current function with and without the if statements.
					// TODO: Optimize so it uses Draw and DrawCropped methods. DrawCropped for the edges and Draw for the non-edges.
					if (x + i < 0 || x + i > target->width ||
						y + j <= 0 || y + j > target->height - 1)
					{
						continue;
					}

					const uint c1 = src[i + xi];
					if (c1 >> 24) *(dest + addr + i) = c1;
				}
				addr += target->width;
				src += width;
			}
		}
	}
}

// prepare sprite outline data for faster rendering
void Sprite::InitializeStartData()
{
	for (unsigned int f = 0; f < numFrames; ++f)
	{
		start[f] = new unsigned int[height];
		for (int y = 0; y < height; ++y)
		{
			start[f][y] = width;
			uint* addr = GetBuffer() + f * width + y * width * numFrames;
			for (int x = 0; x < width; ++x) if (addr[x])
			{
				start[f][y] = x;
				break;
			}
		}
	}
}