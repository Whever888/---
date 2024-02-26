#include "itons.h"
#include "font.h"
#include "dir_search.h"
#include <time.h>
#include "jpg.h"
#include "list.h"
#include "ts.h"

// 显示菜单
void manu(void)
{
	/* 在x=0,y=0,显示图片 */
	lcd_draw_jpg(0, 0, "gd.jpg");
	lcd_draw_jpg(400, 0, "photo.jpg");
	lcd_draw_jpg(0, 240, "game.jpg");
	lcd_draw_jpg(400, 240, "text.jpg");
	lcd_draw_jpg(750, 0, "back.jpg");
}

// 解码刮刮乐中奖的jpg图片
int jm_ggl_jpg(char *jpg_path, int ggl_color_buf_1[800 * 480])
{
	//******************************解码刮刮乐中奖的jpg图片**************************
	static unsigned char g_color_buf[FB_SIZE] = {0};
	char *pjpg_path = jpg_path;
	int x = 0;
	int y = 0;
	/*定义解码对象，错误处理对象*/
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	unsigned char *pcolor_buf = g_color_buf;
	char *pjpg;

	unsigned int i = 0;
	unsigned int color = 0;
	// unsigned int	count =0;

	unsigned int x_s = x;
	unsigned int x_e;
	unsigned int y_e;
	unsigned int y_n = y;
	unsigned int x_n = x;

	int lcd_buf[480][800] = {0};
	int jpg_fd;
	unsigned int jpg_size;

	if (pjpg_path != NULL)
	{
		/* 申请jpg资源，权限可读可写 */
		jpg_fd = open(pjpg_path, O_RDWR);

		if (jpg_fd == -1)
		{
			printf("open %s error\n", pjpg_path);

			return -1;
		}

		/* 获取jpg文件的大小 */
		jpg_size = file_size_get(pjpg_path);
		if (jpg_size < 1000)
			return -1;

		/* 为jpg文件申请内存空间 */
		pjpg = malloc(jpg_size);

		/* 读取jpg文件所有内容到内存 */
		read(jpg_fd, pjpg, jpg_size);
	}
	else
	{
		printf("jpeg path error : %s \n", pjpg_path);
		return -1;
	}

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/
	jpeg_mem_src(&cinfo, pjpg, jpg_size);

	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);

	/*开始解码*/
	jpeg_start_decompress(&cinfo);

	x_e = x_s + cinfo.output_width;
	y_e = y + cinfo.output_height;

	// int ggl_color_buf_1[800*480];
	bzero(ggl_color_buf_1, 800 * 480);
	/*读解码数据*/
	while (cinfo.output_scanline < cinfo.output_height)
	{
		pcolor_buf = g_color_buf;

		/* 读取jpg一行的rgb值 */
		jpeg_read_scanlines(&cinfo, &pcolor_buf, 1);

		for (i = 0; i < cinfo.output_width; i++)
		{

			/* 获取rgb值 */
			color = *(pcolor_buf + 2);
			color = color | *(pcolor_buf + 1) << 8;
			color = color | *(pcolor_buf) << 16;

			ggl_color_buf_1[x_n + y_n * 800] = color; //**************把解码好的图片存到数组中***********

			pcolor_buf += 3;

			x_n++;
		}

		/* 换行 */
		y_n++;

		x_n = x_s;
	}
	//*******************刮刮乐中奖图片解码完成******************************
}

int main(int argc, char const *argv[])
{
	char *xiaosuo[40] =
		{"50年前,长沙镖子岭。四个土夫子正蹲在一个土丘上，",
		 "所有人都不说话，直勾勾盯着地上的洛阳铲。铲子里还",
		 "带着刚从地下带出的土，奇怪的是，这一杯土正不停的",
		 "向外渗着鲜红的液体，就像刚刚在鲜血里蘸过一样。“",
		 "这下子麻烦大喽”老烟头把他的旱烟在地上敲了敲“下",
		 "面是个血尸嘎，弄不好我们这点儿当当，都要撂在下面",
		 "欧。”“下不下去喃？要得要不得，一句话，莫七里八里",
		 "的！”独眼的小伙子说：“你说你个老人家腿脚不方便，",
		 "就莫下去了，我和我弟两个下去，管他什么东西，直接",
		 "给他来一梭子。”老烟头不怒反笑，对边上的一个大胡",
		 "子说：“你屋里二伢子海式撩天的，直不定什么时候就",

		 "给翻盖子了，你得多教育教育，咱这买卖，不是有只",
		 "匣子炮就能喔荷西天。”那大胡子瞪了那年轻人一眼：",
		 "“你崽子，怎么这么跟老太爷讲话，老太爷淘土的时候",
		 "你她妈的还在你娘肚子里咧。”“我咋说...说错了，老",
		 "祖宗不说了嘛，那血尸就是个好东西，下面宝贝肯定不",
		 "少，不下去，走嘎一炉锅汤。”“你他娘的还敢顶嘴！”",
		 "大胡子举手就打，被老烟头用烟枪挡了回去。“你这个",
		 "当爹的也真是地，就知道打来打去，也不看看现在什么",
		 "地方咧，你自己做伢那时候不还是一样，这叫上粱不正",
		 "下粱歪！”那独眼的小伙子看他老爸被数落了，低下头",
		 "偷笑，老烟头咳嗽了一声，又敲了那独眼的少年一记头",

		 "棍“你笑个嘛？碰到血尸，可大可小，上次你二公就是",
		 "在洛阳挖到这东西，结果现在还疯疯颠颠地，你个小伢",
		 "子嘴巴上毛都没有，做事情这么毛里毛糙，嫌脑袋多是",
		 "喽？”“那到底是要得还是要不得嘛？”独眼的青年不耐",
		 "烦的直挠头。老烟头吧嗒吧嗒抽了几口，看了看天，似",
		 "乎笃定了主意，对大胡子说道：“那要还是要的地，等",
		 "一下我先下去，你跟在我后面，二伢子你带个土耗子殿",
		 "后，三伢子你就别下去了，四个人，想退都来不及退，",
		 "你就拉着土耗子的尾巴，我们在里面一吆喝你就把东西",
		 "拉出来。”年纪最小的那少年不服气了：“我不依，你们",
		 "偏心，我告诉我娘去！”老烟头大笑：“你看你看，三伢"};

	int gd_tmp = 0;								  // 功德计数
	int ggl_color_buf[800 * 480];				  // 图片刮刮乐解码后的像素信息
	char gd_buf[20] = {0};						  // 功德转换字符数组
	FILE *gd_count = fopen("gd_count.txt", "r+"); // 功德文件
	if (gd_count == NULL)
	{
		printf("打开“gd_count”失败\n");
	}

	//********************************************初始化相册*************************************************
	// FILE *file1 = fopen("dir.txt", "w+");
	P_Node Head = NodeInit(NULL);
	if (argc != 2)
	{
		printf("输入错误！\n");
		return -1;
	}
	dt_dir(argv[1], ".jpg", Head);
	// fclose(file1);
	//********************************************触摸屏初始化********************************************

	int fd_ts = open("/dev/input/event0", O_RDONLY);
	if (fd_ts == -1)
	{
		perror("open event 0 error");
		return -1;
	}

	struct input_event tsEvent = {0};

	xy tmp_xy = {0};
	int direction = 0;

	//********************显示屏初始化**************************
	lcd_open();//(jpg的lcd初始化)
	//****************************************************菜单********************************************************

	manu(); // 显示菜单
	fscanf(gd_count, "%s", gd_buf);

	// 初始化Lcd
	struct LcdDevice *lcd = init_lcd("/dev/fb0");//(font的lcd初始化)

	// 打开字体
	font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");

	// 字体大小的设置
	fontSetSize(f, 50);

	// 创建一个画板（点阵图）
	bitmap *bm = createBitmapWithInit(100, 100, 4, getColor(0, 0, 0, 0)); // 也可使用createBitmapWithInit函数，改变画板颜色

	// 将字体写到点阵图上
	fontPrint(f, bm, 0, 0, gd_buf, getColor(0, 255, 255, 255), 100);

	// 把字体框输出到LCD屏幕上
	show_font_to_lcd(lcd->mp, 0, 0, bm);

	// 关闭画板
	destroyBitmap(bm);

	while (1) //***************************************主循环**************************************************
	{
		tmp_xy = get_xy(fd_ts, tsEvent, 1);

		//****************************************返回*****************************************
		if (tmp_xy.x > 750 && tmp_xy.y < 50)
		{
			manu();
			fseek(gd_count, 0, SEEK_SET);
			fscanf(gd_count, "%s", gd_buf);
			// 初始化Lcd
			struct LcdDevice *lcd = init_lcd("/dev/fb0");

			// 创建一个画板（点阵图）
			bitmap *bm = createBitmapWithInit(100, 100, 4, getColor(0, 0, 0, 0)); // 也可使用createBitmapWithInit函数，改变画板颜色

			// 将字体写到点阵图上
			fontPrint(f, bm, 0, 0, gd_buf, getColor(0, 255, 255, 255), 100); //******把功德显示在屏幕左上角

			// 把字体框输出到LCD屏幕上
			show_font_to_lcd(lcd->mp, 0, 0, bm);

			// 关闭画板
			destroyBitmap(bm);
		}
		//*****************************************************功德1**********************************************************
		if (tmp_xy.x < 400 && tmp_xy.y < 240)
		{

			lcd_draw_jpg(0, 0, "gd2.jpg");

			fseek(gd_count, 0, SEEK_SET);
			fscanf(gd_count, "%s", gd_buf);
			gd_tmp = atoi(gd_buf);
			gd_tmp++;

			fseek(gd_count, 0, SEEK_SET);
			fprintf(gd_count, "%d", gd_tmp);
			fseek(gd_count, 0, SEEK_SET);
			fscanf(gd_count, "%s", gd_buf);

			// 初始化Lcd
			struct LcdDevice *lcd = init_lcd("/dev/fb0");

			// 创建一个画板（点阵图）
			bitmap *bm = createBitmapWithInit(100, 100, 4, getColor(0, 0, 0, 0)); // 也可使用createBitmapWithInit函数，改变画板颜色

			// 将字体写到点阵图上
			fontPrint(f, bm, 0, 0, gd_buf, getColor(0, 255, 255, 255), 100);

			// 把字体框输出到LCD屏幕上
			show_font_to_lcd(lcd->mp, 0, 0, bm);

			// 关闭画板
			destroyBitmap(bm);
		}
		//********************************************功德2*******************************************
		if (tmp_xy.x < 400 && tmp_xy.y < 240)
		{

			lcd_draw_jpg(0, 0, "gd1.jpg");

			fseek(gd_count, 0, SEEK_SET);
			fscanf(gd_count, "%s", gd_buf);
			// 初始化Lcd
			struct LcdDevice *lcd = init_lcd("/dev/fb0");

			// 创建一个画板（点阵图）
			bitmap *bm = createBitmapWithInit(100, 100, 4, getColor(0, 0, 0, 0)); // 也可使用createBitmapWithInit函数，改变画板颜色

			// 将字体写到点阵图上
			fontPrint(f, bm, 0, 0, gd_buf, getColor(0, 255, 255, 255), 100);

			// 把字体框输出到LCD屏幕上
			show_font_to_lcd(lcd->mp, 0, 0, bm);

			// 关闭画板
			destroyBitmap(bm);
		}

		//*********************************相册*************************************

		if (tmp_xy.x > 400 && tmp_xy.y < 240 && tmp_xy.x < 750 && tmp_xy.y > 50)
		{

			P_Node jpg_now = list_entry(Head->ptr.next, Node, ptr);
			printf("现在显示的是%s\n", jpg_now->Data.name);
			lcd_draw_jpg(0, 0, jpg_now->Data.name);

			while (1)
			{
				direction = get_direction(fd_ts, tsEvent);
				if (direction == 3) //左滑
				{
					struct list_head *tmp = NULL;
					tmp = jpg_now->ptr.next;
					if (tmp == &Head->ptr)
					{
						tmp = tmp->next;
					}
					jpg_now = list_entry(tmp, Node, ptr);

					printf("现在显示的是%s\n", jpg_now->Data.name);
					lcd_draw_jpg(0, 0, jpg_now->Data.name);
				}

				if (direction == 4) //右滑
				{
					struct list_head *tmp = NULL;
					tmp = jpg_now->ptr.prev;
					if (tmp == &Head->ptr)
					{
						tmp = tmp->prev;
					}

					jpg_now = list_entry(tmp, Node, ptr);
					printf("现在显示的是%s\n", jpg_now->Data.name);
					lcd_draw_jpg(0, 0, jpg_now->Data.name);
				}

				//**********************************按到退出键退出***************************************
				if (direction == 5)
				{
					break;
				}
			}

			// lcd_close();
		}
		//*************************************小说***************************************************

		if (tmp_xy.x > 400 && tmp_xy.y > 240)
		{
			FILE *txt = fopen("xiaosuo.txt", "r"); // 打开小说文件
			if (txt == NULL)
			{
				printf("打开“txt”失败\n");
			}

			FILE *novel_position = fopen("novel_position.txt", "a+"); // 打开小说位置信息文件
			if (novel_position == NULL)
			{
				printf("打开“novel_position.txt”失败\n");
			}

			int txt_position = 0; // 小说当前位置

			// 初始化Lcd
			struct LcdDevice *lcd = init_lcd("/dev/fb0");

			// 创建一个画板（点阵图）
			bitmap *bm = createBitmapWithInit(800, 480, 4, getColor(0, 255, 255, 255)); // 也可使用createBitmapWithInit函数，改变画板颜色

			// 字体大小的设置
			fontSetSize(f, 44);

			fseek(novel_position, 0, SEEK_SET);
			fscanf(novel_position, "%d", &txt_position); // 读取小说当前位置信息

			fseek(txt, txt_position, SEEK_SET);
			char novel_buf[80] = {0}; // 小说内容缓冲数组
			for (int i = 0; i < 11; i++)
			{
				int k = 0;
				// fscanf(txt,  "%[^\n]\n", novel_buf);
				fgets(novel_buf, sizeof(novel_buf), txt);
				fontPrint(f, bm, 0, k += i * 44, novel_buf, getColor(0, 0, 0, 0), 800); //*********把字写到画布上
			}
			txt_position = ftell(txt);

			if (feof(txt))
			{
				printf("文件内容已读完.\n");
				// txt_position = 0;
			}
			//------↓--------↓---------------↓---以下区域配置小说位置偏移量文件---↓----------↓-----------↓------↓-------------
			int page_in_total = 0; // 总页数(包括上一次的偏移量记录)
			// txt_position = 0;//小说当前位置
			char novel_position_buf[10] = {0}; // 用于计算页数的缓冲区

			fseek(novel_position, 0, SEEK_SET); // 偏移到文件开头
			while (1)							// 遍历位置信息文件计算总页数（如果第一次打开此文件则为0，否则为非0）
			{
				if (fgets(novel_position_buf, 10, novel_position) != NULL)
				{
					page_in_total++;
				}
				else
				{
					break;
				}
			}
			// page_in_total = page_in_total - 1;
			int page_index[page_in_total]; // 小说位置偏移量数组

			bzero(page_index, page_in_total); // 清空小说位置偏移量数组
			printf("total[%d]\n", page_in_total);

			fseek(novel_position, 0, SEEK_SET);			 // 偏移到文件开头
			fscanf(novel_position, "%d", &txt_position); // 读取第一行小说当前位置信息

			if (txt_position == 0 && page_in_total == 0) // 小说当前位置为0且位置信息文件总页数为0表示第一次打开这本小说
			{
				fprintf(novel_position, "%d\n", txt_position); // 把小说当前位置信息写在位置文件第一行

				while (1)
				{
					fprintf(novel_position, "%d\n", txt_position); // 把小说第一页位置信息写在位置文件第二行，并把后续偏移量继续写入位置文件
					char novel_buf[80] = {0};					   // 小说内容缓冲数组
					for (int i = 0; i < 11; i++)				   //***********************************以下是打印区
					{
						int k = 0;
						fscanf(txt, "%[^\n]\n", novel_buf);
						printf("[%s]\n", novel_buf);
					} //********************************************以上是打印区

					txt_position = ftell(txt); // 下一页的起始位置

					if (feof(txt))
					{
						printf("文件内容已读完.\n");
						txt_position = 0;
						break;
					}
				}
			}

			fseek(novel_position, 0, SEEK_SET);		// 偏移到文件开头
			for (int i = 0; i < page_in_total; i++) // 把位置信息文件偏移量加入数组
			{
				fscanf(novel_position, "%d", &page_index[i]);
				printf("page_index[%d]=[%d]\n", i, page_index[i]);
			}

			//-------↑--------↑---------------↑---以上区域配置小说位置偏移量文件---↑----------↑-----------↑------↑-------

			show_font_to_lcd(lcd->mp, 0, 0, bm); //************************************把画板显示到lcd屏幕上

			destroyBitmap(bm); //********************关闭画板****************************

			// printf("*******************************************************\n");
			// bm = createBitmapWithInit(800,480,4,getColor(0,255,255,255));
			// fseek(txt, page_index[2], SEEK_SET);
			// for (int i = 0; i < 11; i++)//***********************************以下是打印区
			// {
			// 	int k = 0;
			// 	fscanf(txt,  "%[^\n]\n", novel_buf);
			// 	fontPrint(f,bm,0,k+=i*44,novel_buf,getColor(0,0,0,0),800);//*********把字写到画布上
			// 	printf("[%s]\n", novel_buf);
			// }//********************************************以上是打印区
			// show_font_to_lcd(lcd->mp,0,0,bm);//**************把画板显示到lcd屏幕上
			// destroyBitmap(bm);//***********************销毁画板**********************
			// printf("*******************************************************\n");

			while (1)
			{
				direction = get_direction(fd_ts, tsEvent);
				if (direction == 3) //***左滑*****
				{
					// 创建一个画板（点阵图）
					bitmap *bm = createBitmapWithInit(800, 480, 4, getColor(0, 255, 255, 255)); // 也可使用createBitmapWithInit函数，改变画板颜色
					fseek(txt, txt_position, SEEK_SET);
					for (int i = 0; i < 11; i++)
					{
						int k = 0;
						fscanf(txt, "%[^\n]\n", novel_buf);
						fontPrint(f, bm, 0, k += i * 44, novel_buf, getColor(0, 0, 0, 0), 800); //*********把字写到画布上
					}
					txt_position = ftell(txt);

					if (feof(txt))
					{
						printf("文件内容已读完.\n");
						txt_position = 0;
					}

					show_font_to_lcd(lcd->mp, 0, 0, bm); //**************把画板显示到lcd屏幕上
					destroyBitmap(bm);					 //***********************销毁画板**********************
				}

				if (direction == 4) //***右滑*****
				{
					// 创建一个画板（点阵图）
					bitmap *bm = createBitmapWithInit(800, 480, 4, getColor(0, 255, 255, 255)); // 也可使用createBitmapWithInit函数，改变画板颜色
					int buf[13] = {0,
								   1470,
								   2246,
								   3028,
								   3847,
								   4655,
								   5394,
								   6161,
								   6923,
								   7682,
								   8450,
								   9229,
								   9992};
					for (int k = 0; k < 13; k++) //*******************实验***************************
					{
						txt_position = buf[k];
						printf("__%d__\n", __LINE__);

						for (int i = 1; i < page_in_total; i++) // 查找当前位置在偏移量数组中的位置，偏移到上一页
						{
							if (page_index[i] == txt_position && i > 2)
							{
								fseek(txt, page_index[i - 2], SEEK_SET);
								printf("__%d__\n", __LINE__);
								printf("end1[%d]-[%d]\n", i, ftell(txt));
								printf("__%d__\n", __LINE__);
							}
							else if (page_index[i] == txt_position && i == 1)
							{
								fseek(txt, page_index[13], SEEK_SET);
								printf("__%d__\n", __LINE__);
								printf("end2[%d]-[%d]\n", i, ftell(txt));
								printf("__%d__\n", __LINE__);
							}
							else if (page_index[i] == txt_position && i == 2)
							{
								fseek(txt, page_index[13], SEEK_SET);
								printf("__%d__\n", __LINE__);
								printf("end3[%d]-[%d]\n", i, ftell(txt));
								printf("__%d__\n", __LINE__);
							}
						}
						//*************************************************

						for (int i = 0; i < 11; i++)
						{
							int k = 0;
							// fscanf(txt,  "%[^\n]\n", novel_buf);
							fgets(novel_buf, sizeof(novel_buf), txt);
							fontPrint(f, bm, 0, k += i * 44, novel_buf, getColor(0, 0, 0, 0), 800); //*********把字写到画布上
						}

						txt_position = ftell(txt);
						// sleep(1);
						if (feof(txt))
						{
							printf("文件内容已读完.\n");
						}

						show_font_to_lcd(lcd->mp, 0, 0, bm);
						printf("__%d__\n", __LINE__); //************************************把画板显示到lcd屏幕上
						destroyBitmap(bm);			  //***********************销毁画板**********************
					}								  //*********************************实验***********************************
				}

				//***********************************如果按到返回键就退出******************************
				if (direction == 5)
				{
					break;
				}
			}

			fclose(txt);
			fclose(novel_position);
		}
		//******************************************************小游戏************************************************************
		if (tmp_xy.x < 400 && tmp_xy.y > 240)
		{
			//****************************************小游戏初始化**************************************
			// srand(time(NULL));
			// int g_x = 0, g_y = 0, g_rand = 0;
			// int game[4][4] = {0};
			// g_x = rand() % 4;
			// g_y = rand() % 4;
			// if (rand() % 100 < 66)
			// {
			// 	g_rand = 2;
			// }
			// g_rand = 4;
			// game[g_x][g_y] = g_rand; //**********************在随机位置生成一个随机数***********

			// g_x = rand() % 4; //****************************************
			// g_y = rand() % 4;
			// if (rand() % 100 < 66)
			// {
			// 	g_rand = 2;
			// }
			// g_rand = 4;
			// game[g_x][g_y] = g_rand; //*************在随机位置生成一个随机数*******************

			lcd_draw_jpg(0, 0, "guaguale.jpg");			// 显示刮刮乐界面
			jm_ggl_jpg("zongjiang.jpg", ggl_color_buf); // 解码中奖图片

			//************************************刮刮乐小游戏开始*******************************************
			int point_x = 0;
			int point_y = 0;
			while (1)
			{
				tmp_xy = get_xy(fd_ts, tsEvent, 2);//由于已经到了最后一个模块，这一轮循环已经不再需要tmp_xy的值作为选择功能的依据，此时利用tmp_xy作为临时的中间变量没有问题

				for (int i = 0; i < 50; i++)
				{
					for (int j = 0; j < 50; j++)
					{

						point_x = tmp_xy.x + i;
						point_y = tmp_xy.y + j;
						if (point_x < 800 && point_y < 480 && point_x >= 0 && point_y >= 0)
						{
							lcd_draw_point(point_x, point_y, ggl_color_buf[point_x + point_y * 800]);
						}

						point_x = tmp_xy.x + i;
						point_y = tmp_xy.y - j;
						if (point_x < 800 && point_y < 480 && point_x >= 0 && point_y >= 0)
						{
							lcd_draw_point(point_x, point_y, ggl_color_buf[point_x + point_y * 800]);
						}

						point_x = tmp_xy.x - i;
						point_y = tmp_xy.y - j;
						if (point_x < 800 && point_y < 480 && point_x >= 0 && point_y >= 0)
						{
							lcd_draw_point(point_x, point_y, ggl_color_buf[point_x + point_y * 800]);
						}

						point_x = tmp_xy.x - i;
						point_y = tmp_xy.y + j;
						if (point_x < 800 && point_y < 480 && point_x >= 0 && point_y >= 0)
						{
							lcd_draw_point(point_x, point_y, ggl_color_buf[point_x + point_y * 800]);
						}
					}
				}

				if (tmp_xy.x > 750 && tmp_xy.y < 50)
				{
					break;
				}
			}

			// printf("%d--%d--%d--%d\n"
			// 		"%d--%d--%d--%d\n"
			// 		"%d--%d--%d--%d\n",
			// 		game[0][0], game[0][1], game[0][2], game[0][3],
			// 		game[1][0], game[1][1], game[1][2], game[1][3],
			// 		game[2][0], game[2][1], game[2][2], game[2][3],
			// 		game[3][0], game[3][1], game[3][2], game[3][3]
			// 		);

			// printf("。。。。。。。。。。初始化。。。。。。。。。。。。\n");

			/*
			while (1)//***********************2048小游戏循环开始************************
			{

				xy xy_tmp = cur_xy(&tsEvent, fd_ts);


				if (tsEvent.type == EV_KEY &&
					tsEvent.code == BTN_TOUCH && tsEvent.value == 1)
				{
					x_start = xy_tmp.x;
					y_start = xy_tmp.y;
					printf("x_start = %d\n",x_start);
					printf("y_start = %d\n",y_start);
					printf("**********************\n");

				}


				if (tsEvent.type == EV_KEY &&
					tsEvent.code == BTN_TOUCH && tsEvent.value == 0)
				{
					x_end = xy_tmp.x;
					y_end = xy_tmp.y;
					printf("x_end = %d\n",x_end);
					printf("y_end = %d\n",y_end);
					printf("**********************\n");

				}
				if (x_start - x_end > 200 && abs(y_start - y_end) < 60)//*************左滑****************
				printf("左滑一次。。。。。\n");


						/*while (1)
						{
							int ret_val = read(fd_ts, &tsEvent, sizeof(tsEvent));
							if (tsEvent.type == EV_ABS)
							{
								if (tsEvent.code == ABS_X)
								{
									x_tmp = tsEvent.value * 800 / 1024;
								}
								else if(tsEvent.code == ABS_Y)
								{
									y_tmp = tsEvent.value *480 / 600;
								}
							}

							if (tsEvent.type == EV_KEY &&
								tsEvent.code == BTN_TOUCH && tsEvent.value == 0)
							{
								x_end = x_tmp;
								y_end = y_tmp;

								printf("x_end = %d\n",x_end);
								printf("y_end = %d\n",y_end);

								if (x_start - x_end > 200 && abs(y_start - y_end) < 60)//*************左滑****************
								{

									printf("左移一次。。。。。\n");

									int i,j,score=0,flag=-1;
									for(i=0;i<4;i++)
									{
										for(j=0;j<4;j++)
										{
											int cell=game[i][j];//cell单词用的不太恰当，表示当前元素，你可以采用更有意义的命名
											if(cell!=0)
											{
												int k=j+1;
												while(k<4)
												{
													int nextcell=game[i][k];
													if(nextcell!=0)
													{
														if(cell==nextcell)
														{
															flag=0;//相邻两个元素相同，就说明能移动，所以改变flag的值
															game[i][j]+=game[i][k];
															score+=game[i][j];
															game[i][k]=0;
														}
														k=4;
														break;
													}
													k++;
												}
											}
										}
									}

									for(i=0;i<4;i++)
									{
										for(j=0;j<4-1;j++)
										{
											int cell=game[i][j];
											if(cell==0)
											{
												int k=j+1;
												while(k<4)
												{
													int nextcell=game[i][k];
													if(nextcell!=0)
													{
														flag=0;//
														game[i][j]=nextcell;
														game[i][k]=0;
														k=4;
													}
													k++;
												}

											}
										}
									}

									printf("%d--%d--%d--%d\n"
										"%d--%d--%d--%d\n"
										"%d--%d--%d--%d\n",
											game[0][0], game[0][1], game[0][2], game[0][3],
											game[1][0], game[1][1], game[1][2], game[1][3],
											game[2][0], game[2][1], game[2][2], game[2][3],
											game[3][0], game[3][1], game[3][2], game[3][3]
											);

									g_x = rand()%4;//****************************************
									g_y = rand()%4;
									if (rand()%100<66)
									{
										g_rand = 2;
									}
									g_rand = 4;
									game[g_x][g_y] = g_rand;//*************在随机位置生成一个随机数*******************


									printf("%d--%d--%d--%d\n"
											"%d--%d--%d--%d\n"
											"%d--%d--%d--%d\n",
												game[0][0], game[0][1], game[0][2], game[0][3],
												game[1][0], game[1][1], game[1][2], game[1][3],
												game[2][0], game[2][1], game[2][2], game[2][3],
												game[3][0], game[3][1], game[3][2], game[3][3]
												);
								}
							}
							break;

						}



				if (tsEvent.type == EV_KEY &&
					tsEvent.code == BTN_TOUCH &&
					tsEvent.value == xy_tmp.x > 750 && xy_tmp.y < 50)
				{
					break;
				}

			}*/
			//*********************************2048小游戏循环结束************************************

			// //初始化Lcd
			// struct LcdDevice* lcd = init_lcd("/dev/fb0");

			// //打开字体
			// font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");

			// //字体大小的设置
			// fontSetSize(f,80);

			// //创建一个画板（点阵图）
			// bitmap *bm = createBitmapWithInit(800,480,4,getColor(0,255,255,255));

			// bitmap *bm01 = createBitmapWithInit(200,120,4,getColor(0,0,255,127));
			// bitmap *bm02 = createBitmapWithInit(200,120,4,getColor(0,0,191,255));
			// bitmap *bm03 = createBitmapWithInit(200,120,4,getColor(0,255,255,255));
			// bitmap *bm04 = createBitmapWithInit(200,120,4,getColor(0,255,246,143));
			// bitmap *bm05 = createBitmapWithInit(200,120,4,getColor(0,255,246,143));
			// bitmap *bm06 = createBitmapWithInit(200,120,4,getColor(0,255,255,255));
			// bitmap *bm07 = createBitmapWithInit(200,120,4,getColor(0,0,191,255));
			// bitmap *bm08 = createBitmapWithInit(200,120,4,getColor(0,0,255,127));
			// bitmap *bm09 = createBitmapWithInit(200,120,4,getColor(0,0,255,127));
			// bitmap *bm10 = createBitmapWithInit(200,120,4,getColor(0,0,191,255));
			// bitmap *bm11 = createBitmapWithInit(200,120,4,getColor(0,255,255,255));
			// bitmap *bm12 = createBitmapWithInit(200,120,4,getColor(0,255,246,143));
			// bitmap *bm13 = createBitmapWithInit(200,120,4,getColor(0,255,246,143));
			// bitmap *bm14 = createBitmapWithInit(200,120,4,getColor(0,255,255,255));
			// bitmap *bm15 = createBitmapWithInit(200,120,4,getColor(0,0,191,255));
			// bitmap *bm16 = createBitmapWithInit(200,120,4,getColor(0,0,255,127));

			// char *output[16] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15"};

			// //将字体写到点阵图上
			// fontPrint(f,bm01,30,20,output[0],getColor(0,0,0,0),200);
			// fontPrint(f,bm02,30,20,output[1],getColor(0,0,0,0),200);
			// fontPrint(f,bm03,30,20,output[2],getColor(0,0,0,0),200);
			// fontPrint(f,bm04,30,20,output[3],getColor(0,0,0,0),200);
			// fontPrint(f,bm05,30,20,output[4],getColor(0,0,0,0),200);
			// fontPrint(f,bm06,30,20,output[5],getColor(0,0,0,0),200);
			// fontPrint(f,bm07,30,20,output[6],getColor(0,0,0,0),200);
			// fontPrint(f,bm08,30,20,output[7],getColor(0,0,0,0),200);
			// fontPrint(f,bm09,30,20,output[8],getColor(0,0,0,0),200);
			// fontPrint(f,bm10,30,20,output[9],getColor(0,0,0,0),200);
			// fontPrint(f,bm11,30,20,output[10],getColor(0,0,0,0),200);
			// fontPrint(f,bm12,30,20,output[11],getColor(0,0,0,0),200);
			// fontPrint(f,bm13,30,20,output[12],getColor(0,0,0,0),200);
			// fontPrint(f,bm14,30,20,output[13],getColor(0,0,0,0),200);
			// fontPrint(f,bm15,30,20,output[14],getColor(0,0,0,0),200);
			// fontPrint(f,bm16,30,20,output[15],getColor(0,0,0,0),200);

			// //把字体框输出到LCD屏幕上
			// show_font_to_lcd(lcd->mp,0,0,bm01);
			// show_font_to_lcd(lcd->mp,200,0,bm02);
			// show_font_to_lcd(lcd->mp,400,0,bm03);
			// show_font_to_lcd(lcd->mp,600,0,bm04);
			// show_font_to_lcd(lcd->mp,0,120,bm05);
			// show_font_to_lcd(lcd->mp,200,120,bm06);
			// show_font_to_lcd(lcd->mp,400,120,bm07);
			// show_font_to_lcd(lcd->mp,600,120,bm08);
			// show_font_to_lcd(lcd->mp,0,240,bm09);
			// show_font_to_lcd(lcd->mp,200,240,bm10);
			// show_font_to_lcd(lcd->mp,400,240,bm11);
			// show_font_to_lcd(lcd->mp,600,240,bm12);
			// show_font_to_lcd(lcd->mp,0,360,bm13);
			// show_font_to_lcd(lcd->mp,200,360,bm14);
			// show_font_to_lcd(lcd->mp,400,360,bm15);
			// show_font_to_lcd(lcd->mp,600,360,bm16);

			// //关闭字体，关闭画板
			// fontUnload(f);
			// destroyBitmap(bm);
			// destroyBitmap(bm01);
			// destroyBitmap(bm02);
			// destroyBitmap(bm03);
			// destroyBitmap(bm04);
			// destroyBitmap(bm05);
			// destroyBitmap(bm06);
			// destroyBitmap(bm07);
			// destroyBitmap(bm08);
			// destroyBitmap(bm09);
			// destroyBitmap(bm10);
			// destroyBitmap(bm11);
			// destroyBitmap(bm12);
			// destroyBitmap(bm13);
			// destroyBitmap(bm14);
			// destroyBitmap(bm15);

		} //**************************小游戏结束**************************

	} //***********************主循环结束******************************************************

	fontUnload(f);	  // 关闭字体
	lcd_close();	  //**********jpg****关闭lcd屏幕***************
	close(fd_ts);	  //**********关闭触摸屏*****************
	fclose(gd_count); //**************关闭功德计数文件*********************
	return 0;
} //*****************************main函数结束*****************************************
