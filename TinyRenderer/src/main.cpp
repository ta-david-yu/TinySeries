#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

// First attempt
void inefficientConstantStepLine(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor& color)
{
    const float step = 0.01f;
    for (float t = 0; t < 1.0f; t += step)
    {
        const int x = x0 + (x1 - x0) * t;
        const int y = y0 + (y1 - y0) * t;
        image.set(x, y, color);
    }
}

// Second attempt
void widthBasedStepLine(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor& color)
{
    for (int x = x0; x <= x1; x++)
    {
        const float t = (x - x0) / (float)(x1 - x0);
        const int y = y0 * (1.0f - t) + y1 * t;
        image.set(x, y, color);
    }
}

// Third attempt
void widthBasedStepLineFixingAliasingAndLoopOrder(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor& color)
{

    const bool isLineTooSteep = std::abs(x0 - x1) < std::abs(y0 - y1);
    const bool isXYSwapped = isLineTooSteep;
    if (isLineTooSteep)
    {
        // If the line is too steep, we loop through the y-axis instead of the x.
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        // We want to make sure the loop goes from left to right.
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x <= x1; x++)
    {
        const float t = (x - x0) / (float)(x1 - x0);
        const int y = y0 * (1.0f - t) + y1 * t;

        if (isXYSwapped)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);

    // First attempt
    /*
    {
        inefficientConstantStepLine(13, 20, 80, 40, image, white);
    }
    */

    // Second attempt
    /*
    {
        widthBasedStepLine(13, 20, 80, 40, image, white);
        widthBasedStepLine(20, 13, 40, 80, image, red);
        widthBasedStepLine(80, 40, 13, 20, image, red);
    }
    */

    // Third attempt
    {
        widthBasedStepLineFixingAliasingAndLoopOrder(13, 20, 80, 40, image, white);
        widthBasedStepLineFixingAliasingAndLoopOrder(20, 13, 40, 80, image, red);
        widthBasedStepLineFixingAliasingAndLoopOrder(80, 40, 13, 20, image, red);
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}