//Desktop

#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "msg.h"

//define the xv6 param
#include "gui_base.h"
#include "gui_api.h"


Window wnd;

RGB* save_icon;
RGB* delete_icon;
RGB* cut_icon;
RGB* pen_icon;
RGB* rubber_icon;
RGB* red_icon;
RGB* green_icon;
RGB* blue_icon;
RGB* purple_icon;
RGB* zoomin_icon;
RGB* zoomout_icon;
RGB* rotate_left_90_icon;
RGB* rotate_left_30_icon;
RGB* rotate_right_30_icon;
RGB* rotate_right_90_icon;
char save_filename[] = "save-w5.bmp";
char delete_filename[] = "delete-w3.bmp";
char cut_filename[] = "cut-w3.bmp";
char pen_filename[] = "pen-w3.bmp";
char rubber_filename[] = "rubber-w3.bmp";
char red_filename[] = "red-circle-w6.bmp";
char green_filename[] = "green-w2.bmp";
char blue_filename[] = "blue-w2.bmp";
char purple_filename[] = "purple-w2.bmp";
char rotate_left_90_filename[] = "90-left-w3.bmp";
char rotate_left_30_filename[] = "30-left-w3.bmp";
char rotate_right_30_filename[] = "30-right-w3.bmp";
char rotate_right_90_filename[] = "90-right-w4.bmp";
char zoomin_filename[] = "zoomin-w3.bmp";
char zoomout_filename[] = "zoomout-w2.bmp";

RGB pencil_color={0,0,255};
const int pointSize = 5;
int mouse_down = 0;

int isMouseInButton(int x, int y) {
    if (420 < x && x < 490 && 260 < y && y < 290) {
        return 1;
    }
    else {
        return 0;
    }
}

int isMouseInContent(int x, int y) {
   if (120 < x && x < 490 && 30 < y && y < 440){
       return 1;
   }
   else {
        return 0;
   }
}


void MsgProc(struct message * msg)
{
    switch (msg->msg_type)
    {
    case M_MOUSE_DOWN:
        if(mouse_down == 0 && isMouseInContent(msg->params[0], msg->params[1]))
        {
            mouse_down = 1;
            api_drawRect(&wnd, (Point) {msg->params[0] - pointSize / 2, msg->params[1] - pointSize / 2},
                                 (Size) {pointSize, pointSize}, pencil_color);
            api_update(&wnd, (Rect){msg->params[0] - pointSize / 2, msg->params[1] - pointSize /2, pointSize, pointSize});
        }
        break;
    case M_MOUSE_MOVE:
        if(mouse_down == 1 && isMouseInContent(msg->params[0], msg->params[1]))
        {
            api_drawRect(&wnd, (Point) {msg->params[0] - pointSize / 2, msg->params[1] - pointSize / 2},
                                 (Size) {pointSize, pointSize}, pencil_color);
            api_update(&wnd, (Rect){msg->params[0] - pointSize / 2, msg->params[1] - pointSize /2, pointSize, pointSize});
        }
        break;
    case M_MOUSE_UP:
        if(mouse_down == 1)
        {
            mouse_down = 0;
        }
        break;
    case M_CLOSE_WINDOW:
        // printf(1, save_icon);
        printf(1, "USER_CLOSE\n");
        free(save_icon);
        free(delete_icon);
        free(cut_icon);
        free(pen_icon);
        free(rubber_icon);
        free(red_icon);
        free(green_icon);
        free(blue_icon);
        free(purple_icon);
        free(zoomin_icon);
        free(zoomout_icon);
        free(rotate_left_90_icon);
        free(rotate_left_30_icon);
        free(rotate_right_30_icon);
        free(rotate_right_90_icon);
        api_destroywindow(&wnd);
        break;
    }
}
int
main(int argc, char *argv[])
{
    //int pra = argv[1][0] - '0';
    wnd.pos.x = 100;
    wnd.pos.y = 100;
    //wnd.pos.x = 160;
    //wnd.pos.y = 30;
    wnd.size.w = 500;
    wnd.size.h = 500;
    wnd.title = "PhotoViewer";
    
    int h,w;
    save_icon = malloc(200*200*3);
    delete_icon = malloc(200*200*3);
    cut_icon = malloc(200*200*3);
    pen_icon = malloc(200*200*3);
    rubber_icon = malloc(200*200*3);
    red_icon = malloc(200*200*3);
    green_icon = malloc(200*200*3);
    blue_icon = malloc(200*200*3);
    purple_icon = malloc(200*200*3);
    zoomin_icon = malloc(300*300*3);
    zoomout_icon = malloc(300*300*3);
    rotate_left_90_icon = malloc(500*500*3);
    rotate_left_30_icon = malloc(300*300*3);
    rotate_right_30_icon = malloc(300*300*3);
    rotate_right_90_icon = malloc(300*300*3);
    
    api_createwindow(&wnd);
    
    read24BitmapFile(save_filename, save_icon, &h, &w);
    printf(1, "Icon %d, %d", h, w);
    read24BitmapFile(delete_filename, delete_icon,&h,&w);
    read24BitmapFile(cut_filename, cut_icon, &h, &w);
    read24BitmapFile(pen_filename, pen_icon, &h, &w);
    read24BitmapFile(rubber_filename, rubber_icon, &h, &w);
    read24BitmapFile(red_filename, red_icon, &h, &w);
    read24BitmapFile(green_filename, green_icon, &h, &w);
    read24BitmapFile(blue_filename, blue_icon, &h, &w);
    read24BitmapFile(purple_filename, purple_icon, &h, &w);
    read24BitmapFile(zoomin_filename, zoomin_icon, &h, &w);
    read24BitmapFile(zoomout_filename, zoomout_icon, &h, &w);
    read24BitmapFile(rotate_left_90_filename, rotate_left_90_icon, &h, &w);
    read24BitmapFile(rotate_left_30_filename, rotate_left_30_icon, &h, &w);
    read24BitmapFile(rotate_right_30_filename, rotate_right_30_icon, &h, &w);
    read24BitmapFile(rotate_right_90_filename, rotate_right_90_icon, &h, &w);
    
    // memset(wnd.content, pra * 50, wnd.size.w * wnd.size.h * 3);

    api_paint24BitmapToContent(&wnd, save_icon, (Point){0,0}, (Point){0,0}, (Size){30,30},(Size){30,30});
    api_paint24BitmapToContent(&wnd, delete_icon, (Point){30,0}, (Point){0,0}, (Size){30,30},(Size){30,30});
    api_paint24BitmapToContent(&wnd, pen_icon, (Point){440,0}, (Point){0,0}, (Size){30,30},(Size){30,30});
    api_paint24BitmapToContent(&wnd, rubber_icon, (Point){470,0}, (Point){0,0}, (Size){30,30},(Size){30,30});
    api_paint24BitmapToContent(&wnd, red_icon, (Point){415,5}, (Point){0,0}, (Size){20,20},(Size){20,20});
    api_paint24BitmapToContent(&wnd, green_icon, (Point){385,5}, (Point){0,0}, (Size){20,20},(Size){20,20});
    api_paint24BitmapToContent(&wnd, blue_icon, (Point){355,5}, (Point){0,0}, (Size){20,20},(Size){20,20});
    api_paint24BitmapToContent(&wnd, purple_icon, (Point){325,5}, (Point){0,0}, (Size){20,20},(Size){20,20});
    api_paint24BitmapToContent(&wnd, cut_icon, (Point){60,0}, (Point){0,0}, (Size){30,30},(Size){30,30});
    api_paint24BitmapToContent(&wnd, zoomin_icon, (Point){120,440}, (Point){0,0}, (Size){60,60},(Size){60,60});
    api_paint24BitmapToContent(&wnd, zoomout_icon, (Point){180,440}, (Point){0,0}, (Size){60,60},(Size){60,60});
    api_paint24BitmapToContent(&wnd, rotate_left_90_icon, (Point){240,440}, (Point){0,0}, (Size){60,65},(Size){60,65});
    api_paint24BitmapToContent(&wnd, rotate_left_30_icon, (Point){305,440}, (Point){0,0}, (Size){60,65},(Size){60,65});
    api_paint24BitmapToContent(&wnd, rotate_right_30_icon, (Point){370,440}, (Point){0,0}, (Size){60,65},(Size){60,65});
    api_paint24BitmapToContent(&wnd, rotate_right_90_icon, (Point){435,440}, (Point){0,0}, (Size){60,65},(Size){60,65});
    //api_paint24Bitmap(&wnd, rotate_left_90_icon, (Point){180,440}, (Size){60,65});

    
    
    /*struct RGB color;*/
    /*color.R = 255;*/
    /*color.G = 0;*/
    /*color.B = 0;*/
    //api_drawButton(&wnd, (Point){100, 200},(Size) {50, 50},"HAHA" );
    /*RGBA color;*/
    /*color.R =255;*/
    /*color.G =0;*/
    /*color.B =0;*/
    /*color.A =255;*/
    /*api_drawString(&wnd, 50, 50, "2333", color);*/
    api_repaint(&wnd);
    api_exec(&wnd, &MsgProc);
    return 0;
}
