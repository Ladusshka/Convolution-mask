# Convolution-mask


Image Editor Core Enhancement: Convolution-Based Sharpening and Histogram Calculation
Imagine you're a developer at a computer company and a member of a team building a high-performance image editor (think Photoshop or GIMP). In today’s competitive market, performance and clarity are paramount. This project focuses on implementing a convolution operation for image sharpening and computing a grayscale histogram—all while optimizing memory usage and cache performance.

Project Overview
This project consists of two main components:

Image Sharpening via Convolution:
Task: Implement a convolution filter using the following kernel to sharpen an image:
Копировать
0   -1    0
-1   5   -1
0   -1    0

- **Details:**
- Apply the convolution operation separately to each color channel (R, G, B).
- For pixels at the image edges (where the kernel would extend beyond the image boundaries), simply copy the original pixel value.
- Focus on memory optimization and efficient cache usage.

Grayscale Histogram Calculation:
Conversion: Convert the sharpened image from RGB to grayscale using the luminance formula:
ini
Копировать
Y = round(0.2126 * R + 0.7152 * G + 0.0722 * B)
Histogram Binning: Divide the grayscale range [0, 255] into 5 intervals:
[0, 50]
[51, 101]
[102, 152]
[153, 203]
[204, 255]
Note: All intervals are left-closed and right-open, except the last one which is right-closed.
Output: Write the frequency counts (separated by spaces) to an output file.
Input/Output Specifications
Input Format:

The input image is provided in the binary-encoded Portable Pixmap (PPM) format.
The PPM file starts with the header P6, followed by the image width, height, the maximum intensity value (255), and then the binary data for each pixel (each RGB component occupies one byte).
The input filename is passed as a command-line argument (refer to main(int argc, char **argv)).
Output Files:

output.ppm: Contains the sharpened image.
output.txt: Contains the computed grayscale histogram (brightness frequencies separated by a space).
