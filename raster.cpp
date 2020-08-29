/*Rasterization main algorithm: object centric algorithm
Step1: Project vertices of triangle using perspective projection
Step2: Is the pixel contained in the projected image of the triangle
created by Suriya on 11th July, 2020*/

#include <iostream> //std::cout
#include <fstream>  //file reading and writing
#include "helper_functions.h"
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//Step 1 - Projection using the z-coordinate (Depth Buffer?)

//Step 2 - Pixel Inside-outside triangle test

//Step 3 - Barycentric coordinates and Shading

typedef Eigen::Vector2f Vec2;
typedef Eigen::Vector3f Vec3;
typedef unsigned char Rgb[3];

inline float edgeFunction(const Vec2& a, const Vec2& b, const Vec2& c)
{
    return ((c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]));
}

int main(int argc, char** argv)
{
    //vertex coordinates
    Vec2 v0 = { 491.407, 411.407 };
    Vec2 v1 = { 148.593, 68.5928 };
    Vec2 v2 = { 148.593, 411.407 };

    //vectex colors
    Vec3 c0 = { 1,0,0 };    //red
    Vec3 c1 = { 0,1,0 };    //green
    Vec3 c2 = { 0,0,1 };    //blue

    //dimension of image
    const uint32_t width = 512;
    const uint32_t height = 512;

    //framebuffer
    Rgb* framebuffer = new Rgb[width * height];
    memset(framebuffer, 0x0, width * height * 3);
    //std::cout << "Framebuffer: "<<framebuffer;

    //framebuffer for the stb image
    #define CHANNEL_NUM 3
    uint8_t* stb_framebuffer = new uint8_t[width * height * CHANNEL_NUM];

    float area = edgeFunction(v0, v1, v2);

    int index = 0;
    for (uint32_t j = 0; j < height; ++j)
    {
        for (uint32_t i = 0; i < width; ++i)
        {
            Vec2 pix = { i + 0.5f, j + 0.5f };
            //edge function in CCW like OpenGL and DirectX implementation
            float w0 = edgeFunction(v1, v2, pix);
            float w1 = edgeFunction(v2, v0, pix);
            float w2 = edgeFunction(v0, v1, pix);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) //if pix inside triangle
            {
                //Barycentric Coordinates
                w0 /= area;
                w1 /= area;
                w2 /= area;

                float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
                float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
                float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];

                /*framebuffer[j * width + i][0] = (unsigned char)(r * 255);
                framebuffer[j * width + i][1] = (unsigned char)(g * 255);
                framebuffer[j * width + i][2] = (unsigned char)(b * 255);*/

                stb_framebuffer[(j * width + i) * 3] = int(r * 255.99);
                stb_framebuffer[(j * width + i) * 3 + 1] = int(g * 255.99);
                stb_framebuffer[(j * width + i) * 3 + 2] = int(b * 255.99);
            }
            /*else
            {
                float r = 0;
                float g = 0;
                float b = 0;

                stb_framebuffer[index++] = int(r * 255.99);
                stb_framebuffer[index++] = int(g * 255.99);
                stb_framebuffer[index++] = int(b * 255.99);
            }*/
        }
    }

    //stb image write
    stbi_write_jpg("./raster.jpg", width, height, 3, stb_framebuffer, 100);
    delete[] stb_framebuffer;

    std::ofstream ofs;
    ofs.open("./raster2d.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";
    ofs.write((char*)framebuffer, width * height * 3);
    ofs.close();
    delete[] framebuffer;

    return 0;
}