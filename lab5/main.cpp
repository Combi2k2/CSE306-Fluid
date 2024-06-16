#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>
#include <algorithm>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef std::vector<int>        ImageRow;
typedef std::vector<ImageRow>   Image;

void seamCarving(const int *img, int H, int W, int *result) {
    Image E(H, ImageRow(W, 0));
    Image C(H, ImageRow(W, 0));

    for (int i = 0 ; i < H*W ; ++i) {
        int x = i / W;
        int y = i % W;
        int val1 = 0, val2 = 0;

        if (x < H-1)    val1 += (img[(i+W)*3] + img[(i+W)*3+1] + img[(i+W)*3+2]);
        if (x > 0)      val1 -= (img[(i-W)*3] + img[(i-W)*3+1] + img[(i-W)*3+2]);

        if (y < W-1)    val2 += (img[(i+1)*3] + img[(i+1)*3+1] + img[(i+1)*3+2]);
        if (y > 0)      val2 -= (img[(i-1)*3] + img[(i-1)*3+1] + img[(i-1)*3+2]);

        E[x][y] = abs(val1) + abs(val2);
    }
    
    for (int i = 0 ; i < H ; ++i)
    for (int j = 0 ; j < W ; ++j) {
        if (i) {
            C[i][j] = C[i-1][j];

            if (j > 0)      C[i][j] = std::min(C[i][j], C[i-1][j-1]);
            if (j < W-1)    C[i][j] = std::min(C[i][j], C[i-1][j+1]);
        } else {
            C[i][j] = 0;
        }
        C[i][j] += E[i][j];
    }

    std::vector<int> path;

    path.push_back(std::min_element(C[H-1].begin(), C[H-1].end()) - C[H-1].begin());

    while (path.size() < H) {
        int x = H - path.size();
        int y = path.back();

        if (y > 0 && C[x][y] == C[x-1][y-1] + E[x][y]) {
            path.push_back(y-1);
            continue;
        }
        if (y < W-1 && C[x][y] == C[x-1][y+1] + E[x][y]) {
            path.push_back(y+1);
            continue;
        }
        path.push_back(y);
    }
    std::reverse(path.begin(), path.end());

    for (int i = 0 ; i < H ; ++i) {
        int idx = i * (W-1);

        for (int j = 0 ; j < W ; ++j) if (j != path[i]) {
            result[idx * 3 + 0] = img[(i*W+j) * 3 + 0];
            result[idx * 3 + 1] = img[(i*W+j) * 3 + 1];
            result[idx * 3 + 2] = img[(i*W+j) * 3 + 2];
            idx++;
        }
    }
}

int main() {
    int W, H, C;
    
    //stbi_set_flip_vertically_on_load(true);
    unsigned char *image = stbi_load("8733654151_b9422bb2ec_k.jpg",
                                 &W,
                                 &H,
                                 &C,
                                 STBI_rgb);
    
    std::vector<int> image_int(W*H*3);
    std::vector<int> image_result(W*H*3);

    for (int i = 0 ; i < H*W ; ++i) {
        image_int[i*3+0] = image[i*3+0];
        image_int[i*3+1] = image[i*3+1];
        image_int[i*3+2] = image[i*3+2];
    }
    for (int t = 0 ; t < 500 ; ++t, --W) {
        seamCarving(&image_int[0], H, W, &image_result[0]);
        image_int.swap(image_result);
    }
    
    std::vector<unsigned char> result(W*H*3, 0);

    for (int i = 0; i < H; i++)
    for (int j = 0; j < W; j++) {
        result[(i*W + j) * 3 + 0] = (unsigned char)image_int[(i*W+j)*3+0];
        result[(i*W + j) * 3 + 1] = (unsigned char)image_int[(i*W+j)*3+1];
        result[(i*W + j) * 3 + 2] = (unsigned char)image_int[(i*W+j)*3+2];
    }
    stbi_write_png("image.png", W, H, 3, &result[0], 0);

    return 0;
}