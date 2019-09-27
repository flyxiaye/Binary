// Binary.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
using namespace std;

char otsuThreshold(unsigned char* image, int col, int row);

int main()
{
	for (int order = 0; order < 500; order++)
	{
		string name1 = "C:\\Users\\ChxxxXL\\Desktop\\grayImage\\image\\1\\";
		string name3 = "C:\\Users\\ChxxxXL\\Desktop\\grayImage\\image\\binary\\1\\";
		string name2 = "0000.BMP";
		unsigned char header[54];
		unsigned char Color_Palette[1024];
		unsigned char* imageData = new unsigned char[120 * 188];
		int tmp = order;
		name2[3] = tmp % 10 + '0';
		tmp /= 10;
		name2[2] = tmp % 10 + '0';
		tmp /= 10;
		name2[1] = tmp % 10 + '0';
		name1 += name2;
		name3 += name2;
		FILE* fp = NULL;
		fopen_s(&fp, (char*)name1.data(), "rb");
		if (!fp) continue;
		fread_s(header, 54, 1, 54, fp);
		fread_s(Color_Palette, 1024, 1, 1024, fp);
		fread_s(imageData, 120 * 188, 1, 120 * 188, fp);
		char th = otsuThreshold(imageData, 188, 120);
		for (int i = 0; i < 120; i++)
		{
			for (int j = 0; j < 188; j++)
			{
				if (imageData[i * 188 + j] > th)
					imageData[i * 188 + j] = 255;
				else imageData[i * 188 + j] = 0;
			}
		}
		for (int i = 0; i < 49; i++)
		{
			for (int j = 0; j < 188; j++)
				imageData[i * 188 + j] = 0;
		}
		for (int i = 100; i < 120; i++)
			for (int j = 0; j < 188; j++)
				imageData[i * 188 + j] = 0;
		fclose(fp);
		cout << name3 << endl;
		fopen_s(&fp, (char*)name3.data(), "wb");
		if (!fp) continue;
		fwrite((const unsigned char*)header, 1, 54, fp);
		fwrite((const unsigned char*)Color_Palette, 1, 1024, fp);
		fwrite((const unsigned char*)imageData, 1, 120 * 188, fp);
		fclose(fp);
		delete[] imageData;
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件


//================================================================//
//  @brief  :		大津法二值化
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
char otsuThreshold(unsigned char* image, int col, int row)
{
#define GrayScale 256
	int width = col;
	int height = row;
	int pixelCount[GrayScale] = { 0 };
	float pixelPro[GrayScale] = { 0 };
	int i, j, pixelSum = width * height;
	char threshold = 0;
	unsigned char* data = image;  //指向像素数据的指针
	for (i = 0; i < GrayScale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//统计灰度级中每个像素在整幅图像中的个数  
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
		}
	}

	//计算每个像素在整幅图像中的比例  
	float maxPro = 0.0;
	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
		if (pixelPro[i] > maxPro)
		{
			maxPro = pixelPro[i];
		}
	}

	//遍历灰度级[0,255]  
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < GrayScale; i++)     // i作为阈值
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < GrayScale; j++)
		{
			if (j <= i)   //背景部分  
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //前景部分  
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}

	return threshold;
}