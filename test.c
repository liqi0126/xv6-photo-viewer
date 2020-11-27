//Desktop

#include "types.h"
#include "color.h"
#include "user.h"
#include "fcntl.h"
#include "msg.h"
#include "stat.h"
#include "fs.h"


//define the xv6 param
#include "xv6_api.h"
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
RGB* edit_img;
RGB* edit_img_test;
RGB* edit_img_origin;
RGB* image_list_preview;
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
char filename[] = "testtest.bmp";

RGB pencil_color={0,0,255};
Size edit_img_size={410,380};
Point edit_img_pos={120,30};
const int pointSize = 5;
int mouse_down = 0;
int is_pencil = 0;
int is_rubber = 0;
int is_file = 0;

ImageList *image_list;

void ImageListInit(ImageList *image_list)
{
	image_list->head=0;
	image_list->tail=0;
}

void ImageListAppend(char *filename, int size, int filename_len, ImageList *image_list)
{
	Image *append_image=(Image*)malloc(sizeof(Image));
    //memmove(append_image->image_name, filename, filename_len);
	append_image->image_name=(char*)malloc(sizeof(char)*filename_len);
    for (int i = 0; i < filename_len; i++)
    {
        if(filename[i]!=' ')
        {
            append_image->image_name[i]=filename[i];
        }
    }
    append_image->image_size=size;
	if(image_list->tail==0)
	{
		image_list->head=append_image;
		image_list->tail=append_image;
		image_list->head->next=0;
		image_list->head->prev=0;
	}
	else{
		Image* ptr=image_list->tail;
		image_list->tail->next=append_image;
		image_list->tail=append_image;
		image_list->tail->prev=ptr;
		image_list->tail->next=0;
	}
}

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;
  
  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  
  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  
  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }
  
  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  
  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d %s\n", fmtname(path), st.type, st.ino, st.size);
    break;
  
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      is_file = 0;
      char *filetype;
      char *filename;
      filename=fmtname(buf);
      for(filetype=filename+strlen(filename); filetype >= filename && *filetype != '.'; filetype--)
          ;
      if(*filetype != '.' || filename[0]=='.') is_file=0;
      else if(*filetype == '.') is_file=1;
      if(is_file == 0)
      {
          char tem[]="not a file";
          printf(1, "%s %d %d %d %s\n", filename, st.type, st.ino, st.size, tem);
      }
      else if(is_file == 1)
      {
          filetype++;
          char tem[DIRSIZ+1];
          memmove(tem, filetype, strlen(filetype));
          printf(1, "%s %d %d %d %s\n", filename, st.type, st.ino, st.size, tem);
          ImageListAppend(filename, st.size, strlen(filename), image_list);
          //ImageListAppend(filename, st.size, image_list);
          //printf(1, "image %s", filename);
      }
    }
    break;
  }
  close(fd);
}

int isMouseInContent(int x, int y) {
   if (120 < x && x < 490 && 30 < y && y < 440){
       return 1;
   }
   else {
        return 0;
   }
}

int isMouseInPencilColorButton(int x, int y) {
    if (415 <= x && x <= 435 && 5 <= y && y <= 25){
        pencil_color = (RGB){0,0,255};
        return 1;
    }
    else if(385 <= x && x <= 405 && 5 <= y && y <= 25)
    {
        pencil_color = (RGB){0,255,0};
        return 1;
    }
    else if(355 <= x && x <= 375 && 5 <= y && y <= 25)
    {
        pencil_color = (RGB){255,0,0};
        return 1;
    }
    else if(325 <= x && x <= 345 && 5 <= y && y <= 25)
    {
        pencil_color = (RGB){128,0,128};
        return 1;
    }
    else {
        return 0;
    }
}

int isMouseInRubberButton(int x, int y) {
   if (470 <= x && x <= 500 && 0 <= y && y <= 30){
        pencil_color = (RGB){255,255,255};
        if(is_rubber == 0) is_rubber = 1;
        else if(is_rubber == 1) is_rubber = 0;
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInPencilButton(int x, int y) {
   if (440 <= x && x <= 470 && 0 <= y && y <= 30){
        pencil_color = (RGB){0,0,255};
        if(is_pencil == 0) is_pencil = 1;
        else if(is_pencil == 1) is_pencil = 0;
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInSaveButton(int x, int y) {
   if (0 <= x && x <= 30 && 0 <= y && y <= 30){
        struct RGB *t;
        struct RGB *o;
        edit_img = malloc(edit_img_size.h*edit_img_size.w*3);
        int max_line = edit_img_size.w;
        for (int i = 0; i < edit_img_size.h; i++) {
            o = wnd.content + (edit_img_pos.y + i) * wnd.size.w + edit_img_pos.x;
            t = edit_img + i * edit_img_size.w;
            memmove(t, o, max_line * 3);
        }
        write24BitmapFile(filename, edit_img, edit_img_size.h, edit_img_size.w);
        free(edit_img);
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInDeleteButton(int x, int y) {
   if (30 < x && x <= 60 && 0 <= y && y <= 30){
        struct RGB *t;
        struct RGB *o;
        int max_line = edit_img_size.w;
        for (int i = 0; i < edit_img_size.h; i++) {
            o = wnd.content + (edit_img_pos.y + i) * wnd.size.w + edit_img_pos.x;
            t = edit_img_origin + i * edit_img_size.w;
            memmove(o, t, max_line * 3);
        }
        api_repaint(&wnd);
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
        if(isMouseInPencilColorButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInPencilButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInRubberButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInSaveButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInDeleteButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(mouse_down == 0 && (is_pencil==1 ||  is_rubber == 1) && isMouseInContent(msg->params[0], msg->params[1]))
        {
            mouse_down = 1;
            api_drawRect(&wnd, (Point) {msg->params[0] - pointSize / 2, msg->params[1] - pointSize / 2},
                                 (Size) {pointSize, pointSize}, pencil_color);
            api_update(&wnd, (Rect){msg->params[0] - pointSize / 2, msg->params[1] - pointSize /2, pointSize, pointSize});
            is_pencil=1;
        }
        break;
    case M_MOUSE_MOVE:
        if(mouse_down == 1 && (is_pencil==1 ||  is_rubber == 1) && isMouseInContent(msg->params[0], msg->params[1]))
        {
            api_drawRect(&wnd, (Point) {msg->params[0] - pointSize / 2, msg->params[1] - pointSize / 2},
                                 (Size) {pointSize, pointSize}, pencil_color);
            api_update(&wnd, (Rect){msg->params[0] - pointSize / 2, msg->params[1] - pointSize /2, pointSize, pointSize});
        }
        break;
    case M_MOUSE_UP:
        mouse_down = 0;
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
        free(edit_img_origin);
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
    save_icon = malloc(30*30*3);
    delete_icon = malloc(30*30*3);
    cut_icon = malloc(30*30*3);
    pen_icon = malloc(30*30*3);
    rubber_icon = malloc(30*30*3);
    red_icon = malloc(20*20*3);
    green_icon = malloc(20*20*3);
    blue_icon = malloc(20*20*3);
    purple_icon = malloc(20*20*3);
    zoomin_icon = malloc(60*60*3);
    zoomout_icon = malloc(60*60*3);
    rotate_left_90_icon = malloc(60*63*3);
    rotate_left_30_icon = malloc(60*65*3);
    rotate_right_30_icon = malloc(60*65*3);
    rotate_right_90_icon = malloc(60*65*3);
    edit_img_test = malloc(edit_img_size.w*edit_img_size.h*3);
    
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
    api_paint24BitmapToContent(&wnd, rotate_left_90_icon, (Point){240,440}, (Point){0,0}, (Size){60,63},(Size){60,63});
    api_paint24BitmapToContent(&wnd, rotate_left_30_icon, (Point){305,440}, (Point){0,0}, (Size){60,65},(Size){60,65});
    api_paint24BitmapToContent(&wnd, rotate_right_30_icon, (Point){370,440}, (Point){0,0}, (Size){60,65},(Size){60,65});
    api_paint24BitmapToContent(&wnd, rotate_right_90_icon, (Point){435,440}, (Point){0,0}, (Size){60,65},(Size){60,65});
    //api_paint24Bitmap(&wnd, rotate_left_90_icon, (Point){180,440}, (Size){60,65});

    struct RGB *t;
    struct RGB *o;
    edit_img_origin = malloc(edit_img_size.h*edit_img_size.w*3);
    int max_line = edit_img_size.w;
    for (int i = 0; i < edit_img_size.h; i++) {
        o = wnd.content + (edit_img_pos.y + i) * wnd.size.w + edit_img_pos.x;
        t = edit_img_origin + i * edit_img_size.w;
        memmove(t, o, max_line * 3);
    }

    image_list=(ImageList* )malloc(sizeof(ImageList));
	ImageListInit(image_list);
    ls(".");

    //Image *now;
    //now = image_list->head;
    //now = now->next->next;
    //printf(1, "image %s", now->image_name);
    //image_list_preview = malloc(now->image_size);
    //printf(1, "image %d", now->image_size);
    //read24BitmapFile(now->image_name, image_list_preview, &h, &w);
    //api_paint24BitmapToContent(&wnd, image_list_preview, (Point){200,200}, (Point){0,0}, (Size){h,w},(Size){h,w});
    
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
