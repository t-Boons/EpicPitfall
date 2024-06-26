// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#pragma once

namespace Tmpl8
{

// basic sprite class
class Sprite
{
public:
	// structors
	Sprite( Surface* surface, unsigned int frameCount );
	~Sprite();
	// methods
	void Draw( Surface* target, int x, int y );

	// Safe function allows for off-screen drawing. But that does come at a performance cost.
	// Modified function to allow drawing negative sprites.
	void DrawScaledSafe( int x, int y, int width, int height, Surface* target ) const;

	// Modified function to allow drawing negative sprites.
	void DrawScaled(int x, int y, int width, int height, Surface* target) const;

	// Safe function allows for off-screen drawing. But that does come at a performance cost.
	// Modified function to allow spriteatlas drawing.
	void DrawCroppedSafe(int xi, int yi, int wi, int hi, int x, int y, Surface* target);

	void SetFlags( unsigned int f ) { flags = f; }
	void SetFrame( unsigned int i ) { currentFrame = i; }
	unsigned int GetFlags() const { return flags; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	uint* GetBuffer() const { return surface->pixels; }
	unsigned int Frames() { return numFrames; }
	unsigned int CurrentFrame() const { return currentFrame; }
	Surface* Screen() const { return surface; }
	void InitializeStartData();
	inline uint ColorAtPixel(int x, int y) const { return surface->pixels[x + (y * width) * numFrames + width * currentFrame]; } // Big: O(1)
private:
	// attributes
	int width, height;
	unsigned int numFrames;
	unsigned int currentFrame;
	unsigned int flags;
	unsigned int** start;
	Surface* surface;
};

}