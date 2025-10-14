#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <signal.h>

/*
滑动检测程序
方向编码: 
0 上
1 下
2 左
3 右
4 点击

实现原理：
- 记录触碰按下时的坐标(start_x, start_y)，和松开触摸屏的坐标(end_x, end_y)。
- 计算位移：
        delta_x = abs(end_x - start_x)
        delta_y = abs(end_y - start_y)

- 判断逻辑：
1.如果 delta_x 和 delta_y 都小于一个预设的阈值（例如 10 个像素）：
    我们可以认为用户的手指在按下和松开之间几乎没有移动。这是一个 "点击" (Tap) 事件。

2.如果 delta_x 或 delta_y 大于等于阈值：我们认为用户进行了一次 "滑动" (Swipe/Flick)。
    1）如果 delta_x > delta_y，则认为是 水平滑动。
        如果 end_x > start_x，是 向右滑动。
        如果 end_x < start_x，是 向左滑动。
    2）如果 delta_y > delta_x，则认为是 垂直滑动。
        如果 end_y > start_y，是 向下滑动。
        如果 end_y < start_y，是 向上滑动。
*/



// 定义手势判断的阈值（像素）
#define THRESHOLD 50

int directionSwipe()
{
    int fd =open("/dev/input/event6",O_RDONLY);
    if(fd < 0)
    {
        perror("open input fd fail");
        return -1;
    }

    struct input_event ev;
    int start_x=-1,start_y=-1,end_x=-1,end_y=-1;
    int touch_active =0 ;
    printf(" touch now------");
    while (1)
    {
        if (read(fd,&ev,sizeof(ev))!=sizeof(ev))
        {
            perror("read ev fail");
            close(fd);
            return -1;
        }

        //按下/离开屏幕
        if(ev.type == EV_KEY && ev.code  == BTN_TOUCH)
        {
            if(ev.value == 1)
            {
                touch_active =1 ;//按下了
                printf("start touch\n");
            }else
            if(ev.value == 0&&touch_active)
            {
                touch_active =0;
                break;
            }
        }

        //获取坐标
        if(touch_active&& ev.type==EV_ABS)
        {
            if (ev.code ==ABS_X)
            {
                if (start_x ==-1)
                {
                    start_x=ev.value;
                }
                end_x =ev.value;
                
            }else
            if(ev.code==ABS_Y)
            {
                if (start_y==-1)
                {
                    start_y =ev.value;
                }
                end_y =ev.value;
                
            }
            
        }
        
    }

    //检测是否有效的坐标
    if (start_x ==-1|| start_y == -1|| end_x==-1||end_y==-1)
    {
        printf("Invalid data\n");
        close(fd);
        return -1;
    }
    printf("Start=(%d,%d) \nEnd=(%d,%d)",start_x,start_y,end_x,end_y);

    //计算位移
    int delta_x =abs(end_x -start_x);
    int delta_y =abs (end_y-start_y);
    printf("delta=[%d][%d]",delta_x,delta_y);
    
    //判断

    if (delta_x <THRESHOLD&&delta_y <THRESHOLD)
    {
        printf("tap");
        close(fd);
        return 4;
    }
    if (delta_x>delta_y&&delta_x>THRESHOLD)
    {
       //水平移动
       printf("d_X:%d",delta_x);
       close(fd);
       return (end_x>start_x)?3:2;//右：左
    }else
    if (delta_y>THRESHOLD)
    {
        printf("d_Y=%d",delta_y);
        close(fd);
        return (end_y>start_y)?1:0;//下：上

    }


    printf("-----too little");

    close(fd);
    return 4;//都不符合就视为tap
    
    
    
    

}

int main()
{
    while (1)
    {
        int result=directionSwipe();
        if (result>=0)
        {
            printf("this is result:%d\n",result);
        }else
        {
            printf("fail\n");
        }
        usleep(500000);
    }
    return 0;
    
}