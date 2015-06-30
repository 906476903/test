#include "texture.h"
#include "global_variables.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


using namespace std;

 void * font=GLUT_BITMAP_TIMES_ROMAN_24;//字体
 int B[4]={5,6,8,10};//B[i]是模式按钮[i]，它的值表示对应的贴图代码

 GLuint theTexture[80];
char* textureLists[49] = {
     "image\\load.bmp",//0
     "image\\restart.bmp",//1
     "image\\reference.bmp",//2
     "image\\exit.bmp",//3
     "image\\1a.bmp",//4
     "image\\1b.bmp",//5
     "image\\2a.bmp",//6
     "image\\2b.bmp",//7
     "image\\3a.bmp",//8
     "image\\3b.bmp",//9
     "image\\4a.bmp",//10
     "image\\4b.bmp",//11
     "image\\mode.bmp",//12
     "image\\totalnum.bmp",//13
     "image\\part_r.bmp",//14
     "image\\E.bmp",//15
     "image\\boundary.bmp",//16
     "image\\friction.bmp",//17
     "image\\dt.bmp",//18
     "image\\epsilon.bmp",//19
     "image\\back_dark.bmp",//20
     "image\\procedure.bmp",//21
     "image\\control_bar.bmp",//22
     "image\\system_settings.bmp",//23
     "image\\background.bmp",//24
     "image\\intro.bmp",//25
     "image\\up.bmp",//26
     "image\\down.bmp",//27
     "image\\start_dark.bmp",//28
     "image\\logo.bmp",//29
     "image\\save.bmp",//30
     "image\\continue_dark.bmp",//31
     "image\\pause_dark.bmp",//32
     "image\\leave_dark.bmp",//33
     "image\\y_dark.bmp",//34
     "image\\n_dark.bmp",//35
     "image\\continue_bright.bmp",//36
     "image\\pause_bright.bmp",//37
     "image\\leave_bright.bmp",//38
     "image\\up_bright.bmp",//39
     "image\\down_bright.bmp",//40
     "image\\load_dark.bmp",//41
     "image\\restart_dark.bmp",//42
     "image\\reference_dark.bmp",//43
     "image\\exit_dark.bmp",//44
     "image\\y_bright.bmp",//45
     "image\\n_bright.bmp",//46
     "image\\start_bright.bmp",//47
     "image\\back_bright.bmp",//48
};
/*0-3,41-44是menu场景的dark，bright状态按钮，4-11是setting场景的调控模式的按钮，12-19是setting场景选项的按钮，
20,48是back按钮的dark，bright状态，21是控制条，22是控制bar，23是setting的标题，24是menu的背景，25是参考资料的图片
26-27,39-40分别是up,down的dark和bright状态，28,47分别是back按钮的dark，bright状态，29是最开始的logo
30是maindisplay询问是否保存的背景，31-33,36-38分别是maindisplay界面右侧三个按钮，34-35,45-46分别是y,n
*/
int ImageLoad(const char *filename, Image *image) {

    FILE *file;
    unsigned long size;
    unsigned long i;
    unsigned short int planes;
    unsigned short int bpp;
    char temp, finalName[80];

	//strcpy(finalName, "E:\\程序\\opengl\\纹理贴图2\\" );
	//strcat(finalName, filename );
    //finalName=filename;


    if ((file = fopen(filename, "rb"))==NULL) {	//(finalName, "rb"))==NULL) {			//当没有找到文件时报错
		cout<<"File Not Found : "<<filename;
		return 0;
    }


    fseek(file, 0x0012, SEEK_CUR);			//找到位图的第18个字节以读取宽度

    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {			//读取宽度
		cout<<"Error reading width from "<< finalName;
		return 0;
    }

    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {				//读取高度
		cout<<"Error reading height from \n"<<finalName;
		return 0;
    }

    size = image->sizeX * image->sizeY * 3;

    if ((fread(&planes, 2, 1, file)) != 1) {
		cout<<"Error reading planes from \n"<< finalName;
		return 0;
    }

    if (planes != 1) {
		cout<<"Planes from xx is not 1:\n"<<finalName<< planes;
		return 0;
    }

    if ((i = fread(&bpp, 2, 1, file)) != 1) {
		cout<<"Error reading bpp from \n"<<finalName;
		return 0;
    }

    if (bpp != 24) {
		cout<<"Bpp from xx is not 24: \n"<<finalName<< bpp;
	return 0;
    }

    fseek(file, 24, SEEK_CUR);

    image->data = (char *) malloc(size);
    if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", finalName);
		return 0;
    }

    for (i=0;i<size;i+=3) {
		temp = image->data[i];
		image->data[i] = image->data[i+2];
		image->data[i+2] = temp;
    }

    return 1;
}
// 读取bmp图
Image myTextureData[80];//图品数据的存储地址

void LoadGLTextures(int k) {

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if (!ImageLoad(textureLists[k], &myTextureData[k])) exit(1);

		// 分配纹理对象
		glGenTextures(1, &theTexture[k]);
	    glBindTexture(GL_TEXTURE_2D, theTexture[k]);
	   	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);			//
	   	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);      //纹理像素的重复 详细参看教程65
	   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	   	gluBuild2DMipmaps(GL_TEXTURE_2D, 3/*表示RGB*/, myTextureData[k].sizeX, myTextureData[k].sizeY, GL_RGB, GL_UNSIGNED_BYTE, myTextureData[k].data/*告诉OpenGL纹理数据的来源*/);			//将载入的位图文件(*.bmp)转换成纹理贴图
}

void glprint(float x, float y, char *string)//屏幕上显示文字的函数
{
     int len, i; glRasterPos2f(x, y);
     for (i = 0; i < (int) strlen(string); i++)
     {
        glutBitmapCharacter(font, string[i]);
     }
}



