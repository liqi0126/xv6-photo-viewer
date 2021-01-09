//Desktop

#include "msg.h"
#include "user.h"
#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
#include "stat.h"
#include "image_utils.h"

#define PI 3.1415926536
#define MOUSE_SPEED_X 0.6f
#define MOUSE_SPEED_Y -0.6f

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
RGB* image_list_up_icon;
RGB* image_list_down_icon;
RGB* bmp_icon;
RGB* rollover_icon;
RGB* turnaround_icon;
RGB* edit_img;
RGB* edit_img_test;
RGB* edit_img_origin;
RGB* cut_img_save;
RGB* image_list_origin;
RGB* image_title_origin;
RGB* image_in_content;
RGB* image_in_preview;
RGB* image_in_preview_scaled;
RGB* image_in_content_scaled;
RGB* image_in_content_turned;
RGB* image_in_content_rolledover;
RGB* image_in_content_turnedaround;
RGB* image_in_content_torollover;
RGB* image_in_content_toturnaround;
PBitmap* image_origin;
PBitmap* image_origin_preview;
PBitmap* image_origin_mirror;
PBitmap* image_scale;
PBitmap* image_scale_preview;
PBitmap* image_turn;
PBitmap* image_rollover;
PBitmap* image_turnaround;
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
char image_list_up_filename[] = "up-w3.bmp";
char image_list_down_filename[] = "down-w3.bmp";
char bmp_filename[] = "bmp-w7.bmp";
char rollover_filename[] = "rollover-w11.bmp";
char turnaround_filename[] = "turnaround-w5.bmp";
char filename[] = "testtest.bmp";

RGB pencil_color={0,0,255};
RGB cut_box_color={9,9,10};
Size edit_img_size={410,500};
Point edit_img_pos={140,30};
Size image_list_size={465,140};
Point image_list_pos={0,80};
Size image_title_size={18,200};
Point image_title_pos={140,6};
Size content_size={410,500};
Size cutbox_size={410,500};
Point content_pos={140,30};
Point cutbox_pos={140,30};
Point lastMousePos = {0, 0};
Point mousePos = {0, 0};
const int pointSize = 5;
const int cut_box_dash_width = 6;
const int cut_box_dash_corner_width = 10;
const int cut_box_dash_height = 2;
const int cut_box_dash_corner_height = 5;
int mouse_down = 0;
int is_pencil = 0;
int is_rubber = 0;
int is_cut = 0;
int is_file = 0;
int image_item = 0;
float scale_degree = 1;
float turn_degree = 0;

ImageList *image_list;
Image** image_show;

void ImageListInit(ImageList *image_list)
{
	image_list->head=0;
	image_list->tail=0;
}

void ImageListAppend(char *filename, int size, int filename_len, ImageList *image_list, char* type, int type_len)
{
	Image *append_image=(Image*)malloc(sizeof(Image));
	append_image->image_name=(char*)malloc(sizeof(char)*filename_len);
    int len=0;
    for (int i = 0; i < filename_len; i++)
    {
        if(filename[i]!=' ')
        {
            append_image->image_name[i]=filename[i];
            len++;
        }
    }
    append_image->image_name[len]='\0';
    len=0;
    append_image->image_type=(char*)malloc(sizeof(char)*(type_len+1));
    for (int i = 0; i < type_len; i++)
    {
        if(type[i]!=' ')
        {
            append_image->image_type[i]=type[i];
            len++;
        }
    }
    append_image->image_type[len]='\0';
    append_image->image_size=size;
    append_image->data=(RGB*)malloc(sizeof(RGB)*append_image->image_size);
    int h,w;
    read24BitmapFile(append_image->image_name, append_image->data, &h, &w);
    append_image->h=h;
    append_image->w=w;
    append_image->scale_needed=0;
    image_origin_preview->height=h;
    image_origin_preview->width=w;
    image_origin_preview->data=append_image->data;
    if(h>80 || w>80)
    {
        append_image->scale_needed=1;
        float scale_tem=1;
        if(h>=w)
        {
            scale_tem=(float)80/h;
        }
        else
        {
            scale_tem=(float)80/w;
        }
        image_scale_preview->height=image_origin_preview->height*scale_tem;
        image_scale_preview->width=image_origin_preview->width*scale_tem;
        append_image->scale_data=(RGB*)malloc(sizeof(RGB)*image_scale_preview->height*image_scale_preview->width);
        image_scale_preview->data=append_image->scale_data;
        picScale(image_origin_preview, image_scale_preview);
        append_image->scale_h=image_scale_preview->height;
        append_image->scale_w=image_scale_preview->width;
    }
    append_image->save_time=0;
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
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
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
      printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

void
ls_new(char *path)
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
          // adjust image size here
          if(st.size<1000000) ImageListAppend(filename, st.size, strlen(filename), image_list, tem, strlen(tem));
      }
    }
    break;
  }
  close(fd);
}

int isMouseInCutBoxLeft(int x, int y) {
   if (cutbox_pos.x-cut_box_dash_corner_width <= x && x <= cutbox_pos.x 
        && cutbox_pos.y <= y && y <= cutbox_pos.y + cutbox_size.h ){
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInCutBoxRight(int x, int y) {
   if (cutbox_pos.x + cutbox_size.w <= x && x <= cutbox_pos.x + cutbox_size.w + cut_box_dash_corner_width 
        && cutbox_pos.y <= y && y <= cutbox_pos.y + cutbox_size.h ){
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInCutBox(int x, int y) {
   if ( content_pos.x <= x && x <= content_pos.x + content_size.w
        && content_pos.y <= y && y <= content_pos.y + content_size.h ){
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInContent(int x, int y) {
   if (140 + (500-content_size.w)/2 < x && x < 640-(500-content_size.w)/2 && 30 + (410-content_size.h)/2 < y && y < 440 - (410-content_size.h)/2){
       return 1;
   }
   else {
        return 0;
   }
}

int isMouseInPencilColorButton(int x, int y) {
    if (555 <= x && x <= 575 && 5 <= y && y <= 25){
        pencil_color = (RGB){0,0,255};
        return 1;
    }
    else if(525 <= x && x <= 545 && 5 <= y && y <= 25)
    {
        pencil_color = (RGB){0,255,0};
        return 1;
    }
    else if(495 <= x && x <= 515 && 5 <= y && y <= 25)
    {
        pencil_color = (RGB){255,0,0};
        return 1;
    }
    else if(465 <= x && x <= 485 && 5 <= y && y <= 25)
    {
        pencil_color = (RGB){128,0,128};
        return 1;
    }
    else {
        return 0;
    }
}

int isMouseInRubberButton(int x, int y) {
   if (610 < x && x <= 640 && 0 <= y && y <= 30){
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
   if (580 <= x && x <= 610 && 0 <= y && y <= 30){
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
        printf(1, "Save begin.\n");
        edit_img = malloc(content_size.h*content_size.w*3);
        int max_line = content_size.w;
        for (int i = 0; i < content_size.h; i++) {
            o = wnd.content + (content_pos.y + i) * wnd.size.w + content_pos.x;
            t = edit_img + i * content_size.w;
            memmove(t, o, max_line * 3);
        }
        image_show[image_item-1]->save_time=image_show[image_item-1]->save_time+1;
        int filename_len = strlen(image_show[image_item-1]->image_name);
        int filetype_len = strlen(image_show[image_item-1]->image_type);
        char* save_filename;
        save_filename=malloc(sizeof(char)*(filename_len+10));
        for(int i=0;i<filename_len-filetype_len-1;i++)
        {
            save_filename[i]=image_show[image_item-1]->image_name[i];
        }
        save_filename[filename_len-filetype_len-1]='-';
        save_filename[filename_len-filetype_len]='v';
        char s[100];
        int k=0;
        int save_time_tem=image_show[image_item-1]->save_time;
        do{
           s[k++]=save_time_tem%10+'0';
        }
        while ((save_time_tem/=10)>0);
        s[k]='.';
        int index = filename_len-filetype_len+1;
        for(int j=k-1;j>=0;j--)
        {
            save_filename[index] = s[j];
            index++;
        }
        save_filename[index]='.';
        index++;
        for(int i=index;i<index+filetype_len;i++)
        {
            save_filename[i]=image_show[image_item-1]->image_type[i-index];
        }
        save_filename[index+filetype_len]='\0';
        //printf(1, "%d %s %s %s %d %d\n", image_show[image_item-1]->save_time, save_filename, image_show[image_item-1]->image_name, image_show[image_item-1]->image_type, filename_len, filetype_len);
        write24BitmapFile(save_filename, edit_img, content_size.h, content_size.w);
        printf(1, "Save succeed.\n");
        ImageListAppend(save_filename, content_size.h*content_size.w*3, strlen(save_filename), image_list, "bmp", 3);
        free(edit_img);
        free(save_filename);
        api_repaint(&wnd);
        return 1;
   }
   else {
        return 0;
   }
}

void drawCutBoxWidth(int y_corner, int y_dash, int width) {
    api_drawRect(&wnd, (Point) {cutbox_pos.x, y_corner}, (Size) {cut_box_dash_corner_height, cut_box_dash_corner_width}, cut_box_color);
    api_update(&wnd, (Rect){cutbox_pos.x, y_corner, cut_box_dash_corner_width, cut_box_dash_corner_height});
    int image_width = width - cut_box_dash_corner_width*2 - cut_box_dash_width*2;
    if(image_width > 0)
    {
        int cut_box_width = image_width / cut_box_dash_width;
        for(int i=0;i<cut_box_width;i++)
        {
            if(i%2==0)
            {
                api_drawRect(&wnd, (Point) {cutbox_pos.x+cut_box_dash_corner_width+cut_box_dash_width+i*cut_box_dash_width, y_dash}, (Size) {cut_box_dash_height, cut_box_dash_width}, cut_box_color);
                api_update(&wnd, (Rect){cutbox_pos.x+cut_box_dash_corner_width+cut_box_dash_width+i*cut_box_dash_width, y_dash, cut_box_dash_width, cut_box_dash_height});
            }
        }
        if(cut_box_width%2==0)
        {
            api_drawRect(&wnd, (Point) {cutbox_pos.x +cut_box_dash_corner_width+cut_box_dash_width+ cut_box_width*cut_box_dash_width, y_dash}, (Size) {cut_box_dash_height, image_width - cut_box_width*cut_box_dash_width}, cut_box_color);
            api_update(&wnd, (Rect){cutbox_pos.x+cut_box_dash_corner_width+cut_box_dash_width+cut_box_width*cut_box_dash_width, y_dash, image_width - cut_box_width*cut_box_dash_width, cut_box_dash_height});
        }
    }
    api_drawRect(&wnd, (Point) {cutbox_pos.x+image_width+cut_box_dash_width*2+cut_box_dash_corner_width, y_corner}, (Size) {cut_box_dash_corner_height, cut_box_dash_corner_width}, cut_box_color);
    api_update(&wnd, (Rect){cutbox_pos.x+image_width+cut_box_dash_width*2+cut_box_dash_corner_width, y_corner, cut_box_dash_corner_width, cut_box_dash_corner_height});
}

void drawCutBoxHeight(int x_corner, int x_dash, int height) {
    api_drawRect(&wnd, (Point) {x_corner, cutbox_pos.y}, (Size) {cut_box_dash_corner_width, cut_box_dash_corner_height}, cut_box_color);
    api_update(&wnd, (Rect){x_corner, cutbox_pos.y, cut_box_dash_corner_height, cut_box_dash_corner_width});
    int image_height = height - cut_box_dash_corner_width*2 - cut_box_dash_width*2;
    if(image_height > 0)
    {
        int cut_box_width = image_height / cut_box_dash_width;
        for(int i=0;i<cut_box_width;i++)
        {
            if(i%2==0)
            {
                api_drawRect(&wnd, (Point) {x_dash, cutbox_pos.y+cut_box_dash_corner_width+cut_box_dash_width+i*cut_box_dash_width}, (Size) {cut_box_dash_width, cut_box_dash_height}, cut_box_color);
                api_update(&wnd, (Rect){x_dash, cutbox_pos.y+cut_box_dash_corner_width+cut_box_dash_width+i*cut_box_dash_width, cut_box_dash_height, cut_box_dash_width});
            }
        }
        if(cut_box_width%2==0)
        {
            api_drawRect(&wnd, (Point) {x_dash, cutbox_pos.y+cut_box_dash_corner_width+cut_box_dash_width+cut_box_width*cut_box_dash_width}, (Size) {image_height - cut_box_width*cut_box_dash_width, cut_box_dash_height}, cut_box_color);
            api_update(&wnd, (Rect){x_dash, cutbox_pos.y+cut_box_dash_corner_width+cut_box_dash_width+cut_box_width*cut_box_dash_width, cut_box_dash_height, image_height - cut_box_width*cut_box_dash_width});
        }
    }
    api_drawRect(&wnd, (Point) {x_corner, cutbox_pos.y+image_height+cut_box_dash_width*2+cut_box_dash_corner_width}, (Size) {cut_box_dash_corner_width, cut_box_dash_corner_height}, cut_box_color);
    api_update(&wnd, (Rect){x_corner, cutbox_pos.y+image_height+cut_box_dash_width*2+cut_box_dash_corner_width, cut_box_dash_corner_height, cut_box_dash_corner_width});
}

void drawCutBoxCorner(Point pos) {
    api_drawRect(&wnd, pos, (Size) {cut_box_dash_corner_height, cut_box_dash_corner_height}, cut_box_color);
    api_update(&wnd, (Rect){pos.x, pos.y, cut_box_dash_corner_height, cut_box_dash_corner_height});
}

void drawCutBox(Point pos, int width, int height) {
    api_paint24BitmapToContent(&wnd, cut_img_save, (Point){content_pos.x - cut_box_dash_corner_height,content_pos.y - cut_box_dash_corner_height}, (Point){0,0}, (Size){content_size.h+2*cut_box_dash_corner_height, content_size.w+2*cut_box_dash_corner_height},(Size){content_size.h+2*cut_box_dash_corner_height,content_size.w+2*cut_box_dash_corner_height});
    drawCutBoxWidth(pos.y - cut_box_dash_corner_height, pos.y - cut_box_dash_height, width);
    drawCutBoxWidth(pos.y + height, pos.y + height, width);
    drawCutBoxHeight(pos.x - cut_box_dash_corner_height, pos.x - cut_box_dash_height, height);
    drawCutBoxHeight(pos.x + width, pos.x + width, height);
    drawCutBoxCorner((Point){pos.x - cut_box_dash_corner_height, pos.y - cut_box_dash_corner_height});
    drawCutBoxCorner((Point){pos.x + width, pos.y - cut_box_dash_corner_height});
    drawCutBoxCorner((Point){pos.x - cut_box_dash_corner_height, pos.y + height});
    drawCutBoxCorner((Point){pos.x + width, pos.y + height});
}

int isMouseInCutButton(int x, int y) {
    if (60 < x && x <= 90 && 0 <= y && y <= 30){
        if(is_cut == 0) 
        {
            is_cut = 1;
            cutbox_pos.x = content_pos.x;
            cutbox_pos.y = content_pos.y;
            cutbox_size.w = content_size.w;
            cutbox_size.h = content_size.h;
            struct RGB *t;
            struct RGB *o;
            cut_img_save = malloc((cutbox_size.w+cut_box_dash_corner_height*2)*(cutbox_size.h+cut_box_dash_corner_height*2)*3);
            int max_line = cutbox_size.w+cut_box_dash_corner_height*2;
            for (int i = 0; i < cutbox_size.h+cut_box_dash_corner_height*2; i++) {
                o = wnd.content + (cutbox_pos.y - cut_box_dash_corner_height + i) * wnd.size.w + cutbox_pos.x - cut_box_dash_corner_height;
                t = cut_img_save + i * (cutbox_size.w+cut_box_dash_corner_height*2);
                memmove(t, o, max_line * 3);
            }
            drawCutBox(cutbox_pos, cutbox_size.w, cutbox_size.h);
            // api_paint24BitmapToContent(&wnd, cut_img_save, (Point){cutbox_pos.x - cut_box_dash_corner_height,cutbox_pos.y - cut_box_dash_corner_height}, (Point){0,0}, (Size){cutbox_size.h+2*cut_box_dash_corner_height,cutbox_size.w+2*cut_box_dash_corner_height},(Size){cutbox_size.h+2*cut_box_dash_corner_height,cutbox_size.w+2*cut_box_dash_corner_height});
            // api_repaint(&wnd);
        }
        return 1;
    }
    else {
        return 0;
    }
}

void setImageList()
{
    RGBA image_name_color = {255,0,0,0};
    struct RGB *t;
    struct RGB *o;
    int max_line = image_list_size.w;
    for (int i = 0; i < image_list_size.h; i++) {
        o = wnd.content + (image_list_pos.y + i) * wnd.size.w + image_list_pos.x;
        t = image_list_origin + i * image_list_size.w;
        memmove(o, t, max_line * 3);
    }
    for(int i=0;i<3;i++)
    {
        if(image_show[i]->scale_needed==1)
        {
            int pos_y=120+130*i-image_show[i]->scale_h/2;
            int pos_x=70-image_show[i]->scale_w/2;
            api_paint24BitmapToContent(&wnd, image_show[i]->scale_data, (Point){pos_x,pos_y}, (Point){0,0}, (Size){image_show[i]->scale_h,image_show[i]->scale_w},(Size){image_show[i]->scale_h,image_show[i]->scale_w});
        }
        else
        {
            int pos_y=120+130*i-image_show[i]->h/2;
            int pos_x=70-image_show[i]->w/2;
            api_paint24BitmapToContent(&wnd, image_show[i]->data, (Point){pos_x,pos_y}, (Point){0,0}, (Size){image_show[i]->h,image_show[i]->w},(Size){image_show[i]->h,image_show[i]->w});
        }
        int name_len=strlen(image_show[i]->image_name);
        int offset_x;
        if(name_len>15)
        {
            char* image_name_slug;
            image_name_slug=(char*)malloc(sizeof(char)*name_len);
            for(int j=0;j<6;j++) 
            {
                image_name_slug[j]=image_show[i]->image_name[j];
                image_name_slug[14-j]=image_show[i]->image_name[name_len-1-j];
            }
            image_name_slug[6]='.';
            image_name_slug[7]='.';
            image_name_slug[8]='.';
            name_len=15;
            offset_x=(140-name_len*9)/2;
            api_drawString(&wnd, offset_x, 80+130*i+90, image_name_slug, image_name_color);
        }
        else if(name_len<=15 && name_len>=0)
        {
            offset_x=(140-name_len*9)/2;       
            api_drawString(&wnd, offset_x, 80+130*i+90, image_show[i]->image_name, image_name_color);
        }
        api_repaint(&wnd);
    }
}

int isMouseInListUpButton(int x, int y) {
   if (47 <= x && x <= 82 && 45 <= y && y <= 65){
        if(image_show[0]->prev==0) return 1;
        else
        {
            image_show[2]=image_show[1];
            image_show[1]=image_show[0];
            image_show[0]=image_show[0]->prev;
            setImageList();
            api_repaint(&wnd);
        }
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInListDownButton(int x, int y) {
   if (47 <= x && x <= 82 && 465 <= y && y <= 485){
        if(image_show[2]->next==0) return 1;
        else
        {
            image_show[0]=image_show[1];
            image_show[1]=image_show[2];
            image_show[2]=image_show[2]->next;
            setImageList();
            api_repaint(&wnd);
        }
        return 1;
   }
   else {
        return 0;
   }
}

void showImageInContent()
{
    RGBA image_name_color = {255,0,0,0};
    struct RGB *t;
    struct RGB *o;
    int max_line = edit_img_size.w;
    for (int i = 0; i < edit_img_size.h; i++) {
        o = wnd.content + (edit_img_pos.y + i) * wnd.size.w + edit_img_pos.x;
        t = edit_img_origin + i * edit_img_size.w;
        memmove(o, t, max_line * 3);
    }
    max_line = image_title_size.w;
    for (int i = 0; i < image_title_size.h; i++) {
        o = wnd.content + (image_title_pos.y + i) * wnd.size.w + image_title_pos.x;
        t = image_title_origin + i * image_title_size.w;
        memmove(o, t, max_line * 3);
    }
    int pos_y=235-image_show[image_item-1]->h/2;
    int pos_x=390-image_show[image_item-1]->w/2;
    api_paint24BitmapToContent(&wnd, image_show[image_item-1]->data, (Point){pos_x,pos_y}, (Point){0,0}, (Size){image_show[image_item-1]->h,image_show[image_item-1]->w},(Size){image_show[image_item-1]->h,image_show[image_item-1]->w});
    api_drawString(&wnd, 140, 6, image_show[image_item-1]->image_name, image_name_color);
    content_size=(Size){image_show[image_item-1]->h,image_show[image_item-1]->w};
    content_pos=(Point){140 + (500-content_size.w)/2,30 + (410-content_size.h)/2};
    scale_degree=1;
    turn_degree=0;
    image_origin->data=image_show[image_item-1]->data;
    image_origin->height=content_size.h;
    image_origin->width=content_size.w;
    image_origin_mirror->height=content_size.h;
    image_origin_mirror->width=content_size.w;
    image_in_content_torollover=(RGB*)malloc(sizeof(RGB)*image_origin->height*image_origin->width);
    memmove(image_in_content_torollover, image_show[image_item-1]->data, image_origin->height*image_origin->width*3);
    image_in_content_toturnaround=(RGB*)malloc(sizeof(RGB)*image_origin->height*image_origin->width);
    memmove(image_in_content_toturnaround, image_show[image_item-1]->data, image_origin->height*image_origin->width*3);
    api_repaint(&wnd);
}

int isMouseInListItem(int x, int y) {
   if (30 <= x && x <= 110 && 80 <= y && y <= 188){
        image_item = 1;
        showImageInContent();
        return 1;
   }
   else if (30 <= x && x <= 110 && 210 <= y && y <= 318)
   {
        image_item = 2;
        showImageInContent();
        return 1;
   }
   else if (30 <= x && x <= 110 && 340 <= y && y <= 448)
   {
        image_item = 3;
        showImageInContent();
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
        max_line = image_title_size.w;
        for (int i = 0; i < image_title_size.h; i++) {
            o = wnd.content + (image_title_pos.y + i) * wnd.size.w + image_title_pos.x;
            t = image_title_origin + i * image_title_size.w;
            memmove(o, t, max_line * 3);
        }
        image_item = 0;
        content_size=(Size){410,380};
        content_pos=(Point){140,30};
        api_repaint(&wnd);
        return 1;
   }
   else {
        return 0;
   }
}

void image_scale_process(float zoom_degree)
{
    scale_degree=scale_degree*zoom_degree;
    image_scale->height=image_origin->height*scale_degree;
    image_scale->width=image_origin->width*scale_degree;
    image_in_content_scaled=(RGB*)malloc(sizeof(RGB)*image_scale->height*image_scale->width);
    image_scale->data=image_in_content_scaled;
    struct RGB *t;
    struct RGB *o;
    int max_line = edit_img_size.w;
    for (int i = 0; i < edit_img_size.h; i++) {
        o = wnd.content + (edit_img_pos.y + i) * wnd.size.w + edit_img_pos.x;
        t = edit_img_origin + i * edit_img_size.w;
        memmove(o, t, max_line * 3);
    }
    picScale(image_origin, image_scale);
    int pos_y=235-image_scale->height/2;
    int pos_x=390-image_scale->width/2;
    api_paint24BitmapToContent(&wnd, image_scale->data, (Point){pos_x,pos_y}, (Point){0,0}, (Size){image_scale->height,image_scale->width},(Size){image_scale->height,image_scale->width});
    content_size=(Size){image_scale->height,image_scale->width};
    content_pos=(Point){140 + (500-content_size.w)/2,30 + (410-content_size.h)/2};
    free(image_in_content_scaled);
    api_repaint(&wnd);
}

int isMouseInZoominButton(int x, int y) {
   if (140 < x && x <= 200 && 440 <= y && y <= 500){
        image_scale_process(2);
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInZoomoutButton(int x, int y) {
   if (200 < x && x <= 260 && 440 <= y && y <= 500){
        image_scale_process(0.5);
        return 1;
   }
   else {
        return 0;
   }
}

void image_turn_process(float rotate_degree)
{
    turn_degree=turn_degree+rotate_degree;
    image_turn->height=image_origin->height;
    image_turn->width=image_origin->width;
    printf(1, "0 %d, %d", image_turn->width, image_turn->height);
    getTurnSize(&(image_turn->width), &(image_turn->height), turn_degree);
    printf(1, "1 %d, %d", image_turn->width, image_turn->height);
    image_in_content_turned=(RGB*)malloc(sizeof(RGB)*image_turn->height*image_turn->width);
    image_turn->data=image_in_content_turned;
    struct RGB *t;
    struct RGB *o;
    int max_line = edit_img_size.w;
    for (int i = 0; i < edit_img_size.h; i++) {
        o = wnd.content + (edit_img_pos.y + i) * wnd.size.w + edit_img_pos.x;
        t = edit_img_origin + i * edit_img_size.w;
        memmove(o, t, max_line * 3);
    }
    picTurn(image_origin, image_turn, turn_degree);
    int pos_y=235-image_turn->height/2;
    int pos_x=390-image_turn->width/2;
    api_paint24BitmapToContent(&wnd, image_turn->data, (Point){pos_x,pos_y}, (Point){0,0}, (Size){image_turn->height,image_turn->width},(Size){image_turn->height,image_turn->width});
    content_size=(Size){image_turn->height,image_turn->width};
    content_pos=(Point){140 + (500-content_size.w)/2,30 + (410-content_size.h)/2};
    free(image_in_content_turned);
    api_repaint(&wnd);
}

int isMouseInRotateLeftNinetyButton(int x, int y) {
   if (260 < x && x <= 325 && 440 <= y && y <= 500){
        image_turn_process(-PI/2);
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInRotateLeftThirtyButton(int x, int y) {
   if (325 < x && x <= 390 && 440 <= y && y <= 500){
        image_turn_process(-PI/6);
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInRotateRightThirtyButton(int x, int y) {
   if (390 < x && x <= 455 && 440 <= y && y <= 500){
        image_turn_process(PI/6);
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInRotateRightNinetyButton(int x, int y) {
   if (455 < x && x <= 520 && 440 <= y && y <= 500){
        image_turn_process(PI/2);
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInRolloverButton(int x, int y) {
   if (520 < x && x <= 580 && 440 <= y && y <= 500){
        image_origin_mirror->data=image_in_content_torollover;
        image_rollover->height=image_origin_mirror->height;
        image_rollover->width=image_origin_mirror->width;
        image_in_content_rolledover=(RGB*)malloc(sizeof(RGB)*image_rollover->height*image_rollover->width);
        image_rollover->data=image_in_content_rolledover;
        struct RGB *t;
        struct RGB *o;
        int max_line = edit_img_size.w;
        for (int i = 0; i < edit_img_size.h; i++) {
            o = wnd.content + (edit_img_pos.y + i) * wnd.size.w + edit_img_pos.x;
            t = edit_img_origin + i * edit_img_size.w;
            memmove(o, t, max_line * 3);
        }
        picRollingOver(image_origin_mirror, image_rollover);
        int pos_y=235-image_rollover->height/2;
        int pos_x=390-image_rollover->width/2;
        api_paint24BitmapToContent(&wnd, image_rollover->data, (Point){pos_x,pos_y}, (Point){0,0}, (Size){image_rollover->height,image_rollover->width},(Size){image_rollover->height,image_rollover->width});
        content_size=(Size){image_rollover->height,image_rollover->width};
        content_pos=(Point){140 + (500-content_size.w)/2,30 + (410-content_size.h)/2};
        free(image_in_content_torollover);
        image_in_content_torollover=(RGB*)malloc(sizeof(RGB)*image_rollover->height*image_rollover->width);
        memmove(image_in_content_torollover, image_in_content_rolledover, image_rollover->height*image_rollover->width*3);
        free(image_in_content_rolledover);
        api_repaint(&wnd);
        return 1;
   }
   else {
        return 0;
   }
}

int isMouseInTurnaroundButton(int x, int y) {
   if (580 < x && x <= 640 && 440 <= y && y <= 500){
        image_origin_mirror->data=image_in_content_toturnaround;
        image_turnaround->height=image_origin_mirror->height;
        image_turnaround->width=image_origin_mirror->width;
        image_in_content_turnedaround=(RGB*)malloc(sizeof(RGB)*image_turnaround->height*image_turnaround->width);
        image_turnaround->data=image_in_content_turnedaround;
        struct RGB *t;
        struct RGB *o;
        int max_line = edit_img_size.w;
        for (int i = 0; i < edit_img_size.h; i++) {
            o = wnd.content + (edit_img_pos.y + i) * wnd.size.w + edit_img_pos.x;
            t = edit_img_origin + i * edit_img_size.w;
            memmove(o, t, max_line * 3);
        }
        picTurnAround(image_origin_mirror, image_turnaround);
        int pos_y=235-image_turnaround->height/2;
        int pos_x=390-image_turnaround->width/2;
        api_paint24BitmapToContent(&wnd, image_turnaround->data, (Point){pos_x,pos_y}, (Point){0,0}, (Size){image_turnaround->height,image_turnaround->width},(Size){image_turnaround->height,image_turnaround->width});
        content_size=(Size){image_turnaround->height,image_turnaround->width};
        content_pos=(Point){140 + (500-content_size.w)/2,30 + (410-content_size.h)/2};
        free(image_in_content_toturnaround);
        image_in_content_toturnaround=(RGB*)malloc(sizeof(RGB)*image_turnaround->height*image_turnaround->width);
        memmove(image_in_content_toturnaround, image_in_content_turnedaround, image_turnaround->height*image_turnaround->width*3);
        free(image_in_content_turnedaround);
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
        if(isMouseInListUpButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInListDownButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInListItem(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInZoominButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInZoomoutButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInRotateLeftNinetyButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInRotateLeftThirtyButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInRotateRightNinetyButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInRotateRightThirtyButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInRolloverButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInTurnaroundButton(msg->params[0], msg->params[1]))
        {
            break;
        }
        if(isMouseInCutButton(msg->params[0], msg->params[1]))
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
        if(mouse_down == 0 && is_cut==1 && (isMouseInCutBoxLeft(msg->params[0], msg->params[1]) || isMouseInCutBoxRight(msg->params[0], msg->params[1])))
        {
            mouse_down = 1;
            mousePos.x = msg->params[0];
            mousePos.y = msg->params[1];
        }
        break;
    case M_MOUSE_MOVE:
        if(mouse_down == 1 && is_cut==1 && isMouseInCutBox(cutbox_pos.x, cutbox_pos.y)) {
            lastMousePos.x = mousePos.x;
            lastMousePos.y = mousePos.y;
            mousePos.x = msg->params[0];
            mousePos.y = msg->params[1];
            int dx = mousePos.x - lastMousePos.x;
            int dy = mousePos.y - lastMousePos.y;
            cutbox_pos.x = cutbox_pos.x + dx;
            printf(1, "mousePos, %d, %d\n", mousePos.x, mousePos.y);
            if(isMouseInCutBox(cutbox_pos.x, cutbox_pos.y))
            {
                // cutbox_pos.y = cutbox_pos.y + dy;
                if(isMouseInCutBoxLeft(lastMousePos.x, lastMousePos.y))
                {
                    cutbox_size.w = cutbox_size.w - dx;
                    drawCutBox(cutbox_pos, cutbox_size.w, cutbox_size.h);
                    printf(1, "40, %d, %d, %d\n", cutbox_pos.x, cutbox_pos.y, isMouseInCutBox(cutbox_pos.x, cutbox_pos.y));
                }
                if(isMouseInCutBoxRight(msg->params[0], msg->params[1]))
                {
                    cutbox_pos.x = cutbox_pos.x - dx;
                    cutbox_size.w = cutbox_size.w + dx;
                    drawCutBox(cutbox_pos, cutbox_size.w, cutbox_size.h);
                }
                api_repaint(&wnd);
            }
            else
            {
                cutbox_pos.x = cutbox_pos.x - dx;
            }
            // cutbox_size.h = cutbox_size.h - dy;
        }
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
        free(image_list_up_icon);
        free(image_list_down_icon);
        free(bmp_icon);
        free(edit_img_origin);
        free(image_title_origin);
        free(image_list_origin);
        api_destroywindow(&wnd);
        break;
    }
}
int
main(int argc, char *argv[])
{
    //int pra = argv[1][0] - '0';
    wnd.pos.x = 100;
    wnd.pos.y = 60;
    //wnd.pos.x = 160;
    //wnd.pos.y = 30;
    wnd.size.w = 640;
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
    image_list_up_icon = malloc(35*20*3);
    image_list_down_icon = malloc(35*20*3);
    bmp_icon = malloc(80*80*3);
    rollover_icon = malloc(60*60*3);
    turnaround_icon = malloc(60*60*3);
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
    read24BitmapFile(image_list_down_filename, image_list_down_icon, &h, &w);
    read24BitmapFile(image_list_up_filename, image_list_up_icon, &h, &w);
    read24BitmapFile(bmp_filename, bmp_icon, &h, &w);
    read24BitmapFile(rollover_filename, rollover_icon, &h, &w);
    read24BitmapFile(turnaround_filename, turnaround_icon, &h, &w);
    
    // memset(wnd.content, pra * 50, wnd.size.w * wnd.size.h * 3);

    api_paint24BitmapToContent(&wnd, save_icon, (Point){0,0}, (Point){0,0}, (Size){30,30},(Size){30,30});
    api_paint24BitmapToContent(&wnd, delete_icon, (Point){30,0}, (Point){0,0}, (Size){30,30},(Size){30,30});
    api_paint24BitmapToContent(&wnd, pen_icon, (Point){580,0}, (Point){0,0}, (Size){30,30},(Size){30,30});
    api_paint24BitmapToContent(&wnd, rubber_icon, (Point){610,0}, (Point){0,0}, (Size){30,30},(Size){30,30});
    api_paint24BitmapToContent(&wnd, red_icon, (Point){555,5}, (Point){0,0}, (Size){20,20},(Size){20,20});
    api_paint24BitmapToContent(&wnd, green_icon, (Point){525,5}, (Point){0,0}, (Size){20,20},(Size){20,20});
    api_paint24BitmapToContent(&wnd, blue_icon, (Point){495,5}, (Point){0,0}, (Size){20,20},(Size){20,20});
    api_paint24BitmapToContent(&wnd, purple_icon, (Point){465,5}, (Point){0,0}, (Size){20,20},(Size){20,20});
    api_paint24BitmapToContent(&wnd, cut_icon, (Point){60,0}, (Point){0,0}, (Size){30,30},(Size){30,30});
    api_paint24BitmapToContent(&wnd, zoomin_icon, (Point){140,440}, (Point){0,0}, (Size){60,60},(Size){60,60});
    api_paint24BitmapToContent(&wnd, zoomout_icon, (Point){200,440}, (Point){0,0}, (Size){60,60},(Size){60,60});
    api_paint24BitmapToContent(&wnd, rotate_left_90_icon, (Point){260,440}, (Point){0,0}, (Size){60,63},(Size){60,63});
    api_paint24BitmapToContent(&wnd, rotate_left_30_icon, (Point){325,440}, (Point){0,0}, (Size){60,65},(Size){60,65});
    api_paint24BitmapToContent(&wnd, rotate_right_30_icon, (Point){390,440}, (Point){0,0}, (Size){60,65},(Size){60,65});
    api_paint24BitmapToContent(&wnd, rotate_right_90_icon, (Point){455,440}, (Point){0,0}, (Size){60,65},(Size){60,65});
    api_paint24BitmapToContent(&wnd, rollover_icon, (Point){520,440}, (Point){0,0}, (Size){60,60},(Size){60,60});
    api_paint24BitmapToContent(&wnd, turnaround_icon, (Point){580,440}, (Point){0,0}, (Size){60,60},(Size){60,60});
    api_paint24BitmapToContent(&wnd, image_list_up_icon, (Point){47,45}, (Point){0,0}, (Size){20,35},(Size){20,35});
    api_paint24BitmapToContent(&wnd, image_list_down_icon, (Point){47,465}, (Point){0,0}, (Size){20,35},(Size){20,35});
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

    image_list_origin = malloc(image_list_size.h*image_list_size.w*3);
    max_line = image_list_size.w;
    for (int i = 0; i < image_list_size.h; i++) {
        o = wnd.content + (image_list_pos.y + i) * wnd.size.w + image_list_pos.x;
        t = image_list_origin + i * image_list_size.w;
        memmove(t, o, max_line * 3);
    }

    image_title_origin = malloc(image_title_size.h*image_title_size.w*3);
    max_line = image_title_size.w;
    for (int i = 0; i < image_title_size.h; i++) {
        o = wnd.content + (image_title_pos.y + i) * wnd.size.w + image_title_pos.x;
        t = image_title_origin + i * image_title_size.w;
        memmove(t, o, max_line * 3);
    }

    image_origin=(PBitmap*)malloc(sizeof(PBitmap));
    image_origin_preview=(PBitmap*)malloc(sizeof(PBitmap));
    image_origin_mirror=(PBitmap*)malloc(sizeof(PBitmap));
    image_scale=(PBitmap*)malloc(sizeof(PBitmap));
    image_scale_preview=(PBitmap*)malloc(sizeof(PBitmap));
    image_turn=(PBitmap*)malloc(sizeof(PBitmap));
    image_turnaround=(PBitmap*)malloc(sizeof(PBitmap));
    image_rollover=(PBitmap*)malloc(sizeof(PBitmap));
    image_in_content = malloc(edit_img_size.w*edit_img_size.h*3);

    image_list=(ImageList* )malloc(sizeof(ImageList));
	ImageListInit(image_list);
    ls_new(".");
    ls(".");
    Image *header;
    header = image_list->head;
    image_show=(Image**)malloc(sizeof(Image*)*3);
    image_show[0] = header;
    image_show[1] = header->next;
    image_show[2] = header->next->next;
    setImageList();

    api_repaint(&wnd);
    api_exec(&wnd, &MsgProc);
    return 0;
}
