#include "tgaimage.h"
#include "model.h"

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
void fixedWidthBasedStepLine(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor& color)
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

// Fourth attempt
void bresenhamsLineWithFloatingPoint(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor& color)
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

    const auto width = (float)(x1 - x0);
    const auto height = (float)(y1 - y0);
    const auto derror = height / width;

    float error = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        if (isXYSwapped)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }

        error += derror;
        if (error > 0.5f)
        {
            y += (y1 > y0)? 1 : -1;
            error -= 1.0f;
        }
    }
}

// Fifth attempt:
// Get rid of the use of floating point numbers.
// There are two floating point numbers:
//  1. The derror value.
//  2. The error threshold (0.5) we use in the error comparison.
// To make both of them integers, we multiply both of them with `width * 2`,
//  so derror becomes `height * 2`
//  and error threshold becomes `width`.
// That also means we need to multiply other numbers that were used to compare with those two numbers.
//
void bresenhamsLineFinal(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor& color)
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

    const auto width = x1 - x0;
    const auto height = y1 - y0;
    const auto derror = std::abs(height) * 2; // (height / width) * width * 2 => height * 2

    float error = 0;
    int y = y0;
    if (isXYSwapped)
    {
        for (int x = x0; x <= x1; x++)
        {
            image.set(y, x, color);
            error += derror;
            if (error > width)  // 0.5 * width * 2 => width
            {
                y += (y1 > y0) ? 1 : -1;
                error -= width * 2; // 1.0 * width * 2 => width * 2
            }
        }
    }
    else
    {
        for (int x = x0; x <= x1; x++)
        {
            image.set(x, y, color);
            error += derror;
            if (error > width)  // 0.5 * width * 2 => width
            {
                y += (y1 > y0) ? 1 : -1;
                error -= width * 2; // 1.0 * width * 2 => width * 2
            }
        }
    }
}

void lesson1DrawLine()
{
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
    /*
    {
        fixedWidthBasedStepLine(13, 20, 80, 40, image, white);
        fixedWidthBasedStepLine(20, 13, 40, 80, image, red);
        fixedWidthBasedStepLine(80, 40, 13, 20, image, red);
    }
    */

    // Fourth attempt
    /*
    {
        bresenhamsLineWithFloatingPoint(13, 20, 80, 40, image, white);
        bresenhamsLineWithFloatingPoint(20, 13, 40, 80, image, red);
        bresenhamsLineWithFloatingPoint(80, 40, 13, 20, image, red);
    }
    */

    // Fifth attempt
    {
        bresenhamsLineFinal(13, 20, 80, 40, image, white);
        bresenhamsLineFinal(20, 13, 40, 80, image, red);
        bresenhamsLineFinal(80, 20, 13, 20, image, red);
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
}

void lesson1DrawModel(const char *pathToModelAsset)
{
    Model model(pathToModelAsset);

    const auto width = 800;
    const auto height = 800;
    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<int> faceVertIndices = model.face(i);
        for (int j = 0; j < 3; j++)
        {
            const int vert0Index = faceVertIndices[j];
            const int vert1Index = faceVertIndices[(j + 1) % 3];
            const Vec3f vert0 = model.vert(vert0Index);
            const Vec3f vert1 = model.vert(vert1Index);

            // We ignore depth (z), and use the center of the image as the model space origin.
            // The provided model vertices range from -1 ~ 1. We want to map vert(0, 0) to image(width * 0.5, height * 0.5).
            int x0 = (vert0.x + 1.0f) * width / 2.0f;
            int y0 = (vert0.y + 1.0f) * height / 2.0f;
            int x1 = (vert1.x + 1.0f) * width / 2.0f;
            int y1 = (vert1.y + 1.0f) * height / 2.0f;
            bresenhamsLineFinal(x0, y0, x1, y1, image, white);
        }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
}

int main(int argc, char** argv) {
    //lesson1DrawLine();
    lesson1DrawModel("assets/african_head.obj");
    return 0;
}