
//vin si modific ore_lat care e 6 si vine 6*6 patrate pentru un metal      sau nr_ores_lat care este 4 si vine 4*4 total ores

#include<Windows.h>
#include<stdio.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#define nr_ores_lat 4
//#define nr_of_ores nr_ores_lat*nr_ores_lat
#define ore_lat 6
#define grid_width ore_lat*nr_ores_lat
#define grid_height grid_width
#define drill_lat 4*cell

#ifdef __cplusplus
extern "C" {
#endif

int _ep1(){
    char* ep_nm=Ores_for_Ship_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    //swf_new("ep.swf",width,height,0x01ff01,fps);
    action("_root['episode']=this");
    init_episod(ep_nm);

    //drill sprite
    int drill_pre=swf_sprite_new();

    int drill_bmp_pre=swf_sprite_new();
    int bmp;

    bmp=swf_image("C:/Desktop/universe/img/1/drill1.dbl");
    swf_sprite_placeobject_coords(drill_bmp_pre,bmp,0,cell,0);swf_sprite_showframe(drill_bmp_pre);
    for(int i=0;i<fps/5;i++){
        swf_sprite_showframe(drill_bmp_pre);}
    swf_sprite_removeobject(drill_bmp_pre,0);
    bmp=swf_image("C:/Desktop/universe/img/1/drill2.dbl");
    swf_sprite_placeobject_coords(drill_bmp_pre,bmp,0,cell,0);
    for(int i=0;i<fps/5;i++){
        swf_sprite_showframe(drill_bmp_pre);}

    int front_pre=swf_sprite_new();

    int front_part_pre=swf_sprite_new();
    for(int i=0;i<fps;i++){
        int b=i&1;
        if(b==0)action_sprite(front_part_pre,"_parent.draw_front_part();");
        swf_sprite_showframe(front_part_pre);
    }
    int front_part=swf_sprite_done(front_part_pre);swf_exports_add(front_part,"front_part");

    action_sprite(front_pre,"var drill_part_lat=_root['episode'].drill_lat/2");
    action_sprite(front_pre,R"(
        var diag_height=5;
        createEmptyMovieClip('front_metal',getNextHighestDepth());
        front_metal.beginFill(0xd3d3d3);
        front_metal.lineTo(drill_part_lat,0);
        front_metal.lineTo(drill_part_lat,drill_part_lat);
        front_metal.lineTo(0,drill_part_lat);
        front_metal.lineTo(0,0);
        front_metal.endFill();
        var move_mark=0;
        var stop_after;
        function draw_front_part(){
            if(stop_after!=-1){
                if(stop_after==0){front_part.stop();return undefined;}
                stop_after--;
            }
            var y=move_mark;
            var max_y=drill_part_lat+diag_height;
            front_part.clear();
            front_part.lineStyle(0,0);
            while(y<max_y){
                front_part.moveTo(0,y);
                front_part.lineTo(drill_part_lat,y-diag_height);
                y=y+diag_height;
            }
            move_mark--;if(move_mark<0)move_mark=move_mark+diag_height;
        }
        attachMovie('front_part','front_part',getNextHighestDepth());_root['episode'].drill_stop();
    )");
    swf_sprite_showframe(front_pre);
    int front=swf_sprite_done(front_pre);
    swf_exports_add(front,"front");

    int drill_bmp=swf_sprite_done(drill_bmp_pre);
    swf_exports_add(drill_bmp,"drill_img");

    actionf_sprite(drill_pre,buf,"var cell=%u",cell);
    action_sprite(drill_pre,R"(
        attachMovie('drill_img','img',getNextHighestDepth());
        var drill_part_lat=_root['episode'].drill_lat/2;
        img.attachMovie('front','front',img.getNextHighestDepth());img.front._x=cell;img.front._y=drill_part_lat;
        img.createEmptyMovieClip('front_mask',img.getNextHighestDepth());var f_mask=img.front_mask;
        f_mask._x=cell;f_mask._y=drill_part_lat;
        f_mask.beginFill(0x0011FF);
        f_mask.lineTo(drill_part_lat,0);
        f_mask.lineTo(cell,drill_part_lat);
        f_mask.lineTo(0,0);
        f_mask.endFill();
        img.front.setMask(f_mask);
        var is_playing=0;
    )");
    swf_sprite_showframe(drill_pre);
    int drill=swf_sprite_done(drill_pre);swf_exports_add(drill,"drill");

    //action

    actionf(buf,"var fps=%u",fps);
    actionf(buf,"var ore_lat=%u;var grid_width=%u;var grid_height=%u;",ore_lat,grid_width,grid_height);
    actionf(buf,"var cell=%u;var drill_lat=%u",cell,drill_lat);

    action("var grid_w=drill_lat*grid_width;var grid_h=drill_lat*grid_height");

    //grass and land

    int wh[2];
    int dbl=swf_dbl_ex("C:/Desktop/universe/img/1/grass.dbl",wh);
    swf_exports_add(dbl,"grass_img");
    actionf(buf,"var map_y=%u",wh[1]);

    dbl=swf_dbl("C:/Desktop/universe/img/1/land.dbl");
    swf_exports_add(dbl,"land_img");
    dbl=swf_dbl_ex("C:/Desktop/universe/img/1/land_top.dbl",wh);
    swf_exports_add(dbl,"land_top_img");
    actionf(buf,"var top_marg=%u",wh[1]);

    action(R"(
        var matrix=new flash.geom.Matrix();
        var BitmapData=flash.display.BitmapData;
        function bitmap_till_mc(container,bm_name,wd,hg){
            var bmp=BitmapData.loadBitmap(bm_name+'_img');
            container.createEmptyMovieClip(bm_name,container.getNextHighestDepth());
            var mv=container[bm_name];
            mv.beginBitmapFill(bmp,matrix,true);
            mv.lineTo(wd,0);mv.lineTo(wd,hg);mv.lineTo(0,hg);mv.lineTo(0,0);
            mv.endFill();
        }

        bitmap_till_mc(this,'grass',grid_w,map_y+top_marg);

        function land_and_fog(zone){
            createEmptyMovieClip(zone,getNextHighestDepth());var mv=this[zone];mv._y=map_y;

            bitmap_till_mc(mv,zone,grid_w,grid_h);
            mv[zone]._y=top_marg;

            bitmap_till_mc(mv,zone+'_top',grid_w,top_marg);
        }
        land_and_fog('land');
    )");

    //drill
	int eroare_altfel = width;
	int screen_half_w = eroare_altfel /2;
	eroare_altfel = height;
	int screen_half_h = eroare_altfel /2;
	//printf("\n%u %u", screen_half_w, screen_half_h);
    actionf(buf,"var screen_half_w=%u;var screen_half_h=%u", screen_half_w, screen_half_h);
	action("var drill_frames=fps;var frames_go");
	action(R"(
var x_to_go=0;var y_to_go=0;var is_x;
//var x_to_start;var y_to_start;
var tested_type;
function move_drill(){
    if(command==0){
        set_the_left();drill_play();
    }
    else if(command==1){
        set_the_right();drill_play();
    }
    else if(command==2){
        set_the_up();drill_play();
    }
    else if(command==3){
        set_the_down();drill_play();
    }
}
function set_the_left(){drill.img._rotation=90;drill.img._x=drill_lat;drill.img._y=0;x_to_go=x_to_go-drill_lat;is_x=true;}
function set_the_right(){drill.img._rotation=-90;drill.img._x=0;drill.img._y=drill_lat;x_to_go=x_to_go+drill_lat;is_x=true;}
function set_the_up(){drill.img._rotation=180;drill.img._x=drill_lat;drill.img._y=drill_lat;y_to_go=y_to_go-drill_lat;is_x=false;}
function set_the_down(){drill.img._rotation=0;drill.img._x=0;drill.img._y=0;y_to_go=y_to_go+drill_lat;is_x=false;}
function next_destination(){
    tested_type=stone_or_margin_test();
	if(tested_type==2){frames_go=drill_frames;one_tip=true;}
}
//0 stone,1 margin,2 can go
function stone_or_margin_test()
{
    //map margins
    if(y_to_go<0)return 1;
    if(y_to_go==all_h)return 1;
    if(x_to_go<0)return 1;
    if(x_to_go==all_w)return 1;
    //grass zone
    if(y_to_go<map_y)return 2;
    //stone test
    var next_row=(y_to_go-map_y)/drill_lat;
    var next_col=x_to_go/drill_lat;
    var flag=map[next_row][next_col]&stone_flag;
    if(flag==0)return 2;
    return 0;
}

var all_w=grid_w;var all_h=map_y+grid_h;
//_root.createTextField('qwe',_root.getNextHighestDepth(),0,0,200,100);
var screen_max_w=all_w-screen_half_w;var screen_max_h=all_h-screen_half_h;
//var screen_right_x=0-all_w+width;var screen_bottom_y=0-all_h+height;
//_root.qwe.text=screen_max_w+' '+screen_half_w+' '+drill_lat;
function drill_moving(){
  get_input();
  if(drill.is_playing==1){
    if(tested_type==2){
		if(one_tip==true){one_tip=false;arrow_tip();}
		frames_go--;var n=drill_lat*frames_go/drill_frames;
        if(is_x==true){
			var prev_x=drill._x;var pos;
			if(x_to_go<prev_x)pos=x_to_go+n;
			else pos=x_to_go-n;
			move_easy(prev_x,pos,screen_half_w,screen_max_w,x_move);
			drill._x=pos;
        }
        else{
			var prev_y=drill._y;var pos;
			if(y_to_go<prev_y)pos=y_to_go+n;
			else pos=y_to_go-n;
			move_easy(prev_y,pos,screen_half_h,screen_max_h,y_move);
			drill._y=pos;
        }
		if(frames_go>0)return undefined;
        reveal();
        if(points_x.length==0)_root.end_scenario();
    }
    else{
		if(tested_type==0)reveal();
        //back from reveal or margin to where the drill is staying
        x_to_go=drill._x;
        y_to_go=drill._y;
    }
    drill_stop();
  }
  move_drill();
}
function drill_stop(){
    drill.is_playing=0;
    drill.img.front.stop_after=fps;
    drill.img.stop();
}
var one_tip;
function drill_play(){
	next_destination();
	command=-1;command_mc.removeMovieClip();
    drill.is_playing=1;
    drill.img.front.stop_after=-1;
    drill.img.front.front_part.play();
    drill.img.play();
}
attachMovie('drill','drill',getNextHighestDepth());
drill.onEnterFrame=drill_moving;
    )");

    dbl=swf_dbl("C:/Desktop/universe/img/1/fog.dbl");
    swf_exports_add(dbl,"fog_img");

    dbl=swf_dbl("C:/Desktop/universe/img/1/fog_top.dbl");
    swf_exports_add(dbl,"fog_top_img");

    //fog

	action(R"(
    land_and_fog('fog');fog.blendMode='layer';
    fog.createEmptyMovieClip('masker',fog.getNextHighestDepth());
    fog.masker.blendMode='erase';

var revealed_flag=1;
var stone_flag=revealed_flag<<1;
var coal_flag=stone_flag<<1;var first_flag=coal_flag;
var iron_flag=coal_flag<<1;
var gold_flag=iron_flag<<1;var last_flag=gold_flag;
var ore_flags=coal_flag|iron_flag|gold_flag;

function reveal(){
    var x=x_to_go-fog._x;var y=y_to_go-fog._y;
    if(y<0)return undefined;
    var x_pos=x/drill_lat;var y_pos=y/drill_lat;
    var flag=map[y_pos][x_pos];

    var flag_rev=flag&1;
    if(flag_rev==revealed_flag)return undefined;
    map[y_pos][x_pos]|=revealed_flag;

    var off=10;
    fog.masker.beginFill(0x0101ff);
    fog.masker.moveTo(x-off,y);
    fog.masker.curveTo(x,y+cell,x-off,y+drill_lat);y=y+drill_lat;fog.masker.lineTo(x,y+off);
    fog.masker.curveTo(x+cell,y,x+drill_lat,y+off);x=x+drill_lat;fog.masker.lineTo(x+off,y);
    fog.masker.curveTo(x,y-cell,x+off,y-drill_lat);y=y-drill_lat;fog.masker.lineTo(x,y-off);
    fog.masker.curveTo(x-cell,y,x-drill_lat,y-off);x=x-drill_lat;fog.masker.lineTo(x-off,y);
    fog.masker.endFill();

    //free or stone or ore
    if(flag==0)return undefined;
    var next_depth=land.getNextHighestDepth();
    var name='mc'+next_depth;

    if(flag==stone_flag){
        land.attachMovie('stone',name,next_depth);
        land[name]._x=x_to_go;land[name]._y=y_to_go-map_y;
        return undefined;
    }

    if(flag==coal_flag)land.attachMovie('coal',name,next_depth);
    else if(flag==iron_flag)land.attachMovie('iron',name,next_depth);
    else land.attachMovie('gold',name,next_depth);
    land[name]._x=x_to_go;land[name]._y=y_to_go-map_y;
    land[name].attachMovie('marker','instance',1);
    //remove the ore from the tip points
    i=0;
	while(false==false){
		if(points_x[i]==x_pos)if(points_y[i]==y_pos){points_x.splice(i,1);points_y.splice(i,1);break;}
		i++;
	}
}
    )");

    //ores and stones

    int presprite;int sprite;
    presprite=swf_sprite_new();
    bmp=swf_image("C:/Desktop/universe/img/1/stone.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"stone");
    presprite=swf_sprite_new();
    bmp=swf_image("C:/Desktop/universe/img/1/coal.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"coal");
    presprite=swf_sprite_new();
    bmp=swf_image("C:/Desktop/universe/img/1/iron.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"iron");
    presprite=swf_sprite_new();
    bmp=swf_image("C:/Desktop/universe/img/1/gold.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"gold");

    presprite=swf_sprite_new();
    bmp=swf_image("C:/Desktop/universe/img/1/mark1.dbl");swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/2;i++)swf_sprite_showframe(presprite);
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/1/mark2.dbl");swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/2;i++)swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"marker");

    declare_grid(grid_width,grid_height,"map","0");
    action("var points_x=new Array();var points_y=new Array()");

    action(R"(
        //set the stones
        var row=0;
        while(row<grid_height){
            var col=0;
            while(col<grid_width){
                var one_ore_row=0;
                while(one_ore_row<ore_lat){
                    var one_ore_col=0;
                    while(one_ore_col<ore_lat){
                        map[row+one_ore_row+random(3)][col+one_ore_col+random(3)]=stone_flag;
                        one_ore_col=one_ore_col+3;
                    }
                    one_ore_row=one_ore_row+3;
                }
                col=col+ore_lat;
            }
            row=row+ore_lat;
        }
        //set the ores
        var ore_part_lat=ore_lat/3;var ore_row;var ore_col;
        row=0;var pos=0;
        while(row<grid_height){
            col=0;
            while(col<grid_width){
                var r_row=random(3);
                var r_col=random(3);
                ore_row=random(ore_part_lat)*3+row;
                ore_col=random(ore_part_lat)*3+col;
                if(map[ore_row+r_row][ore_col+r_col]==stone_flag){
                    //can be stone
                    r_row=r_row+1+random(2);if(r_row>=3)r_row=r_row-3;
                    r_col=r_col+1+random(2);if(r_col>=3)r_col=r_col-3;
                }
                points_x[pos]=ore_col+r_col;points_y[pos]=ore_row+r_row;pos++;
                col=col+ore_lat;
            }
            row=row+ore_lat;
        }
		//seteaza tipurile, egale cantitativ, acum, dupa ce le-a adunat pe toate, si in mod random
        var x_shuffle=points_x.slice();var y_shuffle=points_y.slice();var flag=first_flag;
        while(x_shuffle.length!=0){
            var rand=random(x_shuffle.length);
            map[y_shuffle[rand]][x_shuffle[rand]]=flag;flag<<=1;if(flag>last_flag)flag=first_flag;
            x_shuffle.splice(rand,1);y_shuffle.splice(rand,1);
        }
    )");

    //info

    int info_mc_pre=swf_sprite_new();
    for(int i=0;i<fps;i++)swf_sprite_showframe(info_mc_pre);
    action_sprite(info_mc_pre,"removeMovieClip()");
    swf_sprite_showframe(info_mc_pre);
    int info_mc=swf_sprite_done(info_mc_pre);swf_exports_add(info_mc,"info_mc");

    char* sign_chars[]={"straight_red","straight_yellow","straight_green","diag_red","diag_yellow","diag_green",0};
    char** signs=sign_chars;
    while(signs[0]!=0){
        presprite=swf_sprite_new();sprintf(buf,"D:/universe/local/treasurefinder/%s.dbl",signs[0]);
        int img=swf_image(buf);swf_sprite_placeobject(presprite,img,0);
        sprite=swf_sprite_done(presprite);swf_exports_add(sprite,signs[0]);
        signs++;
    }

	action(R"(
    var arrow_frames=1;
    function arrow_tip(){
        arrow_frames--;
        if(arrow_frames>0)return undefined;

        var x=(x_to_go-fog._x)/drill_lat;var y=(y_to_go-fog._y)/drill_lat;
		//mai bine decat Number.MAX_VALUE
		var max_val=grid_width+grid_height;
        var min_dist=max_val;
		var pos;
        i=0;while(i<points_x.length){
            var d_x=x-points_x[i];var d_y=y-points_y[i];
            d_x=d_x*d_x;d_y=d_y*d_y;var d=Math.sqrt(d_x+d_y);
			//ca 0 e ghicita deja
			if(d>0){
				if(d<min_dist){
					min_dist=d;pos=i;
				}
			}
            i++;
        }
		//ca ultima nu are rost
		if(min_dist<max_val){
			arrow_frames=3;

			var ts_row=points_y[pos];var ts_col=points_x[pos];var half_cell=cell/2;var at_center=drill_lat/2-half_cell;
			if(y<ts_row){
				if(x<ts_col)get_sign('diag',min_dist,0,0,0,drill_lat-cell,drill_lat-cell);
				else if(x==ts_col)get_sign('straight',min_dist,90,cell,0,at_center,drill_lat-cell);
				else get_sign('diag',min_dist,90,cell,0,0,drill_lat-cell);
			}
			else if(y>ts_row){
				if(x<ts_col)get_sign('diag',min_dist,-90,0,cell,drill_lat-cell,0);
				else if(x==ts_col)get_sign('straight',min_dist,-90,0,cell,at_center,0);
				else get_sign('diag',min_dist,180,cell,cell,0,0);
			}
			else if(x<ts_col)get_sign('straight',min_dist,0,0,0,drill_lat-cell,at_center);
			else get_sign('straight',min_dist,180,cell,cell,0,at_center);
		}
    }
    function get_sign(pre_name,min_dist,rot,fix_x,fix_y,x_off,y_off)
    {
        attachMovie('info_mc','info_mc',getNextHighestDepth());info_mc._x=x_to_go+x_off;info_mc._y=y_to_go+y_off;
        var color;
        if(min_dist<2)color='green';
        else if(min_dist<5)color='yellow';
        else color='red';
        var name=pre_name+'_'+color;
        info_mc.attachMovie(name,'arrow',info_mc.getNextHighestDepth());
        info_mc.arrow._rotation=rot;info_mc.arrow._x=fix_x;info_mc.arrow._y=fix_y;
    }
    )");

	presprite = swf_sprite_new();
	action_sprite(presprite, R"(
lineStyle(2,0xffFFff);
var half=side/2;var q=half/2;
moveTo(0,half);lineTo(side,half);lineTo(side-q,half-q);
moveTo(side,half);lineTo(side-q,half+q);
_alpha=50;
)");
	swf_sprite_showframe(presprite);//bine de notat: nu mai merge onEnterFrame la gnash fara asta
	sprite = swf_sprite_done(presprite); swf_exports_add(sprite, "command_mc");
	action(R"(
var command;
function get_input(){
	if(Key.isDown(com.left_arrow)==true)new_input(0,180,1,1);
	else if(Key.isDown(com.right_arrow)==true)new_input(1,0,0,0);
	else if(Key.isDown(com.up_arrow)==true)new_input(2,-90,0,1);
	else if(Key.isDown(com.down_arrow)==true)new_input(3,90,1,0);
}
function new_input(a,rot,x,y){
	if(command_mc!=undefined){
		if(command_mc._rotation==rot)return undefined;
		command_mc.removeMovieClip();
	}
	command=a;
	attachMovie('command_mc','command_mc',getNextHighestDepth());command_mc._rotation=rot;
	command_mc.side=40;
	command_mc._x=command_mc.side*x;command_mc._y=command_mc.side*y;
	command_mc._y+=0-_y;command_mc._x+=0-_x;
}
function y_move(a){
	//if(y_to_go<screen_half_h)if(y_to_start<screen_half_h)return undefined;
	//if(y_to_go>screen_max_h)if(y_to_start>screen_max_h)return undefined;
	_y+=a;command_mc._y-=a;
}
function x_move(a){
	//if(x_to_go<screen_half_w)if(x_to_start<screen_half_w)return undefined;
	//if(x_to_go>screen_max_w)if(x_to_start>screen_max_w)return undefined;
	_x+=a;command_mc._x-=a;
}
)");
	int w_test = (screen_half_w%drill_lat) != 0;
	int h_test = (screen_half_h%drill_lat) != 0;
	char* s1 = ""; char*s2 = ""; char*s3 = "";
	if (w_test || h_test) { s1 = "if(prev>min)fn(prev-min);"; s2 = "if(prev<smax)fn(prev-max);"; s3 = "if(prev<min)fn(min-pos);else if(prev>max)fn(max-pos);else "; }
actionf(buf,R"(
function move_easy(prev,pos,min,max,fn){
	//intors stanga/sus
	if(pos<min){%s}
	//dus dreapta/jos
	else if(pos>max){%s}
	else{
		//dus stanga/sus, intors dreapta/jos
		%s
		//mijloc
		fn(prev-pos);
	}
}
)",s1,s2,s3);
    //debug_grid("fog",grid_width,grid_height,"map",drill_lat,0x0101ff);
    //action("_xscale=50;_yscale=50");

    swf_exports_done();
    swf_showframe();
    swf_done();
	printf("\ndone");
    return 1;
}

#ifdef __cplusplus
}
#endif
