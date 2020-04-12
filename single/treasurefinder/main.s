

format elfobj64microsoft

warning off
include "../actionswf/dev/import.h"
warning on
include "../actionswf/include/prog.h"

include "../files/tool.s"
include "../files/submit.s"

str game_name="Treasure Finder"

const texter_height=40
#const fps=12
const treasures_total=5
const dig_width=36
const dig_height=26

entry treasurefinder()

sd sprite
sd presprite
sd img
#sd dbl
#sd bitmap
sd shape

str env_path="D:/universe/local/treasurefinder/env.dbl"
str ts_path="D:/universe/local/treasurefinder/treasure0.dbl"

chars a_code#16384
str ac^a_code

sd env_w
sd env_h
setcall env_w swf_dbl_width(env_path)
setcall env_h swf_dbl_height(env_path)

call sprintf(ac,"%s.swf",game_name)
call swf_new(ac,env_w,env_h,0x212121,12)

sd deffont
setcall deffont swf_font_basic("_sans")

#env>
setcall presprite swf_sprite_new()
setcall img swf_image(env_path)
call swf_sprite_placeobject(presprite,img,0)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"env")
call action("attachMovie('env','env',0)")
#<env

chars chars="D:/universe/local/treasurefinder/treasure0.dbl";chars *="D:/universe/local/treasurefinder/treasure1.dbl";chars *="D:/universe/local/treasurefinder/treasure2.dbl";chars *="D:/universe/local/treasurefinder/treasure3.dbl";chars *="D:/universe/local/treasurefinder/treasure4.dbl"
ss t^chars
sd ind=0;while ind<(treasures_total)
    setcall presprite swf_sprite_new()
    setcall img swf_image(t);addcall t strlen(t);inc t
    call swf_sprite_placeobject(presprite,img,0)
    call sprintf(ac,"treasure%u",ind)
    setcall sprite swf_sprite_done(presprite);call swf_exports_add(sprite,ac)
inc ind;endwhile

chars sign_chars="straight_red";chars *="straight_yellow";chars *="straight_green";chars *="diag_red";chars *="diag_yellow";chars *="diag_green"
chars *=0
ss signs^sign_chars
while signs#!=0
    setcall presprite swf_sprite_new()
    call sprintf(ac,"D:/universe/local/treasurefinder/%s.dbl",signs)
    setcall img swf_image(ac);
    call swf_sprite_placeobject(presprite,img,0)
    setcall sprite swf_sprite_done(presprite);call swf_exports_add(sprite,signs)
    addcall signs strlen(signs);inc signs
endwhile

sd ts_w
sd ts_h
setcall ts_w swf_dbl_width(ts_path)
setcall ts_h swf_dbl_height(ts_path)

call actionf(ac,"var dig_width=%u;var dig_height=%u;var cell=%u",(dig_width),(dig_height),(cell))
call actionf(ac,"var treasure_w=%u;var treasure_h=%u;var treasures_total=%u",ts_w,ts_h,(treasures_total))
call action("var dig_w=dig_width*cell;var dig_h=dig_height*cell")

setcall presprite swf_sprite_new()
setcall img swf_image("D:/universe/local/treasurefinder/emptydig.dbl")
call swf_sprite_placeobject(presprite,img,0)
setcall sprite swf_sprite_done(presprite);call swf_exports_add(sprite,"sand")

#dig>
call action("
    createEmptyMovieClip('dig',getNextHighestDepth());dig._x=cell;dig._y=cell+cell;
    dig.lineStyle(0,0x212121,40);
    dig.lineTo(60,0);
    dig.moveTo(dig_w-60,0);dig.lineTo(dig_w,0);dig.lineTo(dig_w,60);
    dig.moveTo(dig_w,dig_h-60);dig.lineTo(dig_w,dig_h);dig.lineTo(dig_w-60,dig_h);
    dig.moveTo(60,dig_h);dig.lineTo(0,dig_h);dig.lineTo(0,dig_h-60);
    dig.moveTo(0,60);dig.lineTo(0,0);
    ")
#<dig
#inf>
call action("
    var freeSpace_x=dig._x+dig_w;var a=(env._width-freeSpace_x-treasure_w)/2;
    var b=treasure_h*treasures_total;b=(env._height-dig._y-b)/2;
    var inf_x=freeSpace_x+a;var inf_y=dig._y+b
    ")
setcall presprite swf_sprite_new()
sd inf_h=treasures_total
mult inf_h ts_h
setcall shape swf_shape_border(ts_w,inf_h,1,0xff0101ff)
call swf_sprite_placeobject(presprite,shape,0)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"inf")
#<inf

call declare_grid((dig_width),(dig_height),"map",0,"undefined")
call init_reset_grid()

call action("
var score;
var points_x=new Array();var points_y=new Array();var treasures=new Array();
var treasures=new Array();var treasures_pos=new Array();
var treasures_signs=new Array();
function newgame(){
    inf.removeMovieClip();
    movies.removeMovieClip();
    set_field();
}
function at_load(){
    _text.message='Click to play';
    onMouseDown=game_start;
}
function game_start(){
    onMouseDown=undefined;
    set_field();
}
var rand_colors=new Array();
function set_field(){
    _text.message='';
    reset_grid(dig_width,dig_height,map);
    score=0;
    var i=0;while(i<treasures_total){
        treasures_signs[i]=new Array();
        rand_colors.push(i);
        i++;
    }

    attachMovie('inf','inf',getNextHighestDepth());inf._x=inf_x;inf._y=inf_y;
    var y=0;
    var i=0;while(i<treasures_total){
        var treasure='treasure'+i;
        inf.attachMovie(treasure,treasure,inf.getNextHighestDepth());
        var mv=inf[treasure];mv._y=y;mv._alpha=20;
        i++;y=y+treasure_h;
    }
    createEmptyMovieClip('movies',getNextHighestDepth());movies._x=dig._x;movies._y=dig._y;
    var grid_total=dig_width*dig_height;var grid_part=int(grid_total/treasures_total);var part;
    var ts_nr=0;
    i=0;var rest=treasures_total-1;while(i<treasures_total){
        if(i==rest)part=grid_total;
        else part=grid_part;
        grid_total=grid_total-grid_part;
        var treasure_pos=ts_nr+random(part);ts_nr=ts_nr+part;
        var col=int(treasure_pos/dig_height);var back=col*dig_height;
        var row=treasure_pos-back;

        var ts_rand_color_pos=random(rand_colors.length);
        var ts_rand_color=rand_colors[ts_rand_color_pos];
        var name='treasure'+ts_rand_color;rand_colors.splice(ts_rand_color_pos,1);
        movies.attachMovie(name,name,movies.getNextHighestDepth());
        var mv=movies[name];
        mv._x=col*cell;mv._y=row*cell;mv._visible=false;
        points_x.push(col);points_y.push(row);treasures.push(mv);treasures_pos.push(ts_rand_color);
        i++;
    }
    onMouseDown=calculate_press;
    move_marker();
    onMouseMove=move_marker;
}
function move_marker(){
    if(dig_test()==false){
        if(marker!=undefined)marker.removeMovieClip();
        return undefined;
    }
	var row=int((_ymouse-dig._y)/cell);var col=int((_xmouse-dig._x)/cell);
    if(marker==undefined){
        createEmptyMovieClip('marker',getNextHighestDepth());
        marker.lineStyle(1,0x232323,75,0,'none','round','miter',1);
        var cell_part=cell/4;
        var x;var y=0;var on_x=cell_part;while(y<=cell){
            x=y;marker.moveTo(x,y);marker.lineTo(x+on_x,y);
            x=x+on_x+on_x;marker.moveTo(x,y);marker.lineTo(x+on_x,y);
            on_x=on_x*-1;y=y+cell;
        }
        x=cell;var on_y=cell_part;while(x>=0){
            y=x-cell*-1;marker.moveTo(x,y);marker.lineTo(x,y+on_y);
            y=y+on_y+on_y;marker.moveTo(x,y);marker.lineTo(x,y+on_y);
            on_y=on_y*-1;x=x-cell;
        }
    }
    marker._x=col*cell+dig._x;marker._y=row*cell+dig._y;
}
var dig_right=dig._x+dig_w;var dig_bottom=dig._y+dig_h;
//bool
function dig_test(){
	if(_xmouse<dig._x)return false;
	if(_ymouse<dig._y)return false;
	if(dig_right<=_xmouse)return false;
	if(dig_bottom<=_ymouse)return false;
	return true;
}
function calculate_press(){
    if(marker==undefined)return undefined;
    var y=int(marker._y-dig._y/cell);var x=int(marker._x-dig._x/cell);
    if(map[y][x]!=0)return undefined;
    else{score++;_text.message=score;}
    map[y][x]=1;
    i=0;while(i<points_x.length){if(points_x[i]==x)if(points_y[i]==y){
            var ts_pos=treasures_pos[i];
            var treasure='treasure'+ts_pos;
            var treasure_name=treasure+'_done';
            inf.attachMovie(treasure,treasure_name,inf.getNextHighestDepth());
            inf[treasure_name]._y=treasure_h*ts_pos;
            treasures[i]._visible=true;
            var j=0;while(j<treasures_signs[i].length){
                treasures_signs[i][j].removeMovieClip();
                j++;
            }

            points_x.splice(i,1);points_y.splice(i,1);
            treasures.splice(i,1);treasures_pos.splice(i,1);
            treasures_signs.splice(i,1);
            if(points_x.length==0){
                marker.removeMovieClip();
                onMouseDown=undefined;
                onMouseMove=undefined;
                submitform(score);
            }
            return undefined;
        }
        i++;
    }
    min_dist=dig_w;var pos;
    i=0;while(i<points_x.length){
        var d=dist(x,y,points_x[i],points_y[i]);
        if(d<min_dist){min_dist=d;pos=i;}
        i++;
    }
    var ts_row=points_y[pos];var ts_col=points_x[pos];
    if(y<ts_row){
        if(x<ts_col)get_sign('diag',min_dist,0,x,y,0,0,pos);
        else if(x==ts_col)get_sign('straight',min_dist,90,x,y,cell,0,pos);
        else get_sign('diag',min_dist,90,x,y,cell,0,pos);
    }
    else if(y>ts_row){
        if(x<ts_col)get_sign('diag',min_dist,-90,x,y,0,cell,pos);
        else if(x==ts_col)get_sign('straight',min_dist,-90,x,y,0,cell,pos);
        else get_sign('diag',min_dist,180,x,y,cell,cell,pos);
    }
    else if(x<ts_col)get_sign('straight',min_dist,0,x,y,0,0,pos);
    else get_sign('straight',min_dist,180,x,y,cell,cell,pos);
}
function dist(x,y,ts_x,ts_y)
{
    var d_x=x-ts_x;var d_y=y-ts_y;
    d_x=d_x*d_x;d_y=d_y*d_y;
    return Math.sqrt(d_x+d_y);
}
function get_sign(pre_name,min_dist,rot,x,y,x_off,y_off,pos)
{
    var x_pos=x*cell;var y_pos=y*cell;
    var next_depth=movies.getNextHighestDepth();
    var inst_name='sand'+next_depth;
    movies.attachMovie('sand',inst_name,next_depth);
    var s=movies[inst_name];s._x=x_pos;s._y=y_pos;

    var color;
    if(min_dist<2)color='green';
    else if(min_dist<5)color='yellow';
    else color='red';

    var name=pre_name+'_'+color;
    next_depth=movies.getNextHighestDepth();
    inst_name=name+'_'+next_depth;
    movies.attachMovie(name,inst_name,next_depth);

    var mv=movies[inst_name];
    mv._rotation=rot;
    mv._x=x_pos+x_off;mv._y=y_pos+y_off;
    treasures_signs[pos].push(mv);
}
")

call default_text_ex((0xff0101FF),deffont,0,(texter_height))
call submit_form(deffont,env_w,env_h,game_name)

call action("at_load()")

#include "debugging.s"

call swf_exports_done()
call swf_showframe()
call swf_done()
return 1
