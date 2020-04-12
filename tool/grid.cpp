
#include<stdio.h>
//#include<strings.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
//#include "../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

char *strcat(
   char *strDestination,
   const char *strSource 
);
size_t strlen(
   const char *str
);

void declare_grid(int grid_width,int grid_height,char* grid_name,char* init_value){
    //[row][col]
    char str[8192];
    //actionscript the col
    char* col_frm="var %s_%u=new Array(";
    int row=0;char* ptr;
    while(row<grid_height){
        sprintf(str,col_frm,grid_name,row);
        ptr=str+strlen(str);
        strcat(ptr,init_value);ptr+=strlen(ptr);
        int col=1;
        while(col<grid_width){
            strcat(ptr,",");strcat(ptr,init_value);ptr+=strlen(ptr);col++;
        }
        ptr[0]=')';ptr[1]=0;
        action(str);
        row++;
    }
    //and the rows
    char* row_frm="var %s=new Array(";
    sprintf(str,row_frm,grid_name);
    ptr=str+strlen(str);
    sprintf(ptr,"%s_0",grid_name);
    ptr+=strlen(ptr);
    row=1;
    while(row<grid_height){
        sprintf(ptr,",%s_%u",grid_name,row);ptr+=strlen(ptr);row++;
    }
    ptr[0]=')';ptr[1]=0;
    action(str);
};

void debug_grid(char* container,int grid_width,int grid_height,char* grid,int dist,int color){
    char str[300];
    actionf(str,"var container_debug=%s;var width_debug=%u;var height_debug=%u;var grid_debug=%s;var dist=%u;var color=%u",container,grid_width,grid_height,grid,dist,color);
    action(R"(
        container_debug.createEmptyMovieClip('debug',container_debug.getNextHighestDepth());
        var dbg_txt_x;
        var dbg_txt_y=0;
        var row=0;var col;
        var i=0;var cnt_debug=container_debug.debug;
        var fmt=new TextFormat();
        fmt.size=40;fmt.color=color;
        while(row<height_debug)
        {
            col=0;
            dbg_txt_x=0;
            while(col<width_debug){
                var debug_name='debg_'+row+'_'+col;
                cnt_debug.createTextField(debug_name,cnt_debug.getNextHighestDepth(),dbg_txt_x,dbg_txt_y,dist,dist);
                cnt_debug[debug_name].text=grid_debug[row][col];
                cnt_debug[debug_name].setTextFormat(fmt);
                col=col+1;
                dbg_txt_x=dbg_txt_x+dist;
            }
            row=row+1;
            dbg_txt_y=dbg_txt_y+dist;
        }
    )");
}
#ifdef __cplusplus
}
#endif
