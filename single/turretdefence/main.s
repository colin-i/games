

format elfobj64microsoft

warning off
include "../actionswf/dev/import.h"
warning on
include "../actionswf/include/prog.h"

include "../files/tool.s"
include "../files/submit.s"

str game_name="Turret Defence"

const texter_height=40
const grid_lat=26
const grid_w=cell*grid_lat
const grid_x=cell
const info_x=grid_x+grid_w+5
const w_road_1=grid_lat-4
const w_road_2=grid_lat/2+1-2
const w_road_sum=w_road_1+w_road_2
const turret_r=4
const fps=12
const half_fps=fps/2

#id
function turret_text(sd font_id,ss initialtext,sd font_height)
    call edittext_font(font_id,font_height)
    call edittext_text(initialtext)
    call edittext_rgba(0x01ffFFff)
    sd e_struct
    setcall e_struct edittext_struct()
        #add some pixels to see all the text
    add font_height (static_text_off_add)
    sd text_id
    setcall text_id swf_text(800,font_height,"",(HasFont|HasText|ReadOnly|NoSelect|HasTextColor),e_struct)
    return text_id
endfunction
#id
function turret_dyn_text(sd font_id,ss v_name,sd font_height)
    call edittext_font(font_id,font_height)
    call edittext_rgba(0x01ffFFff)
    sd e_struct
    setcall e_struct edittext_struct()
        #add some pixels to see all the text
    add font_height (static_text_off_add)
    sd text_id
    setcall text_id swf_text(800,font_height,v_name,(HasFont|ReadOnly|NoSelect|HasTextColor),e_struct)
    return text_id
endfunction

function add_enemy(ss path,ss n1,ss n2)
    sd presprite
    sd sprite
    sd img
    sd en_pre
    sd en_spr
    setcall presprite swf_sprite_new()
        setcall en_pre swf_sprite_new()
        setcall img swf_image(path)
        call swf_sprite_placeobject(en_pre,img,0);call swf_sprite_showframe(en_pre)
        setcall en_spr swf_sprite_done(en_pre);call swf_exports_add(en_spr,n1)
    chars ac#1500;str act^ac
    call actionf_sprite(presprite,act,"var code_name='%s';attachMovie('%s','mov',0);var sens='D';var escaped=0",n2,n1)
    call actionf_sprite(presprite,act,"
        var all_hits=_root.round_data[_root.current_round][2];var hit_points=all_hits;var round_that_is=_root.current_round;
        var line;var pos=0;var pos_line=0;var fire;
        if(_x==_root.en_start_x)
            {if(code_name=='tank')line=_root.left_ground;else line=_root.left_air;}
        else
            {if(code_name=='tank')line=_root.right_ground;else line=_root.right_air;}
        createEmptyMovieClip('hits',getNextHighestDepth());
        hits.lineStyle(2,0x00FF00,100,0,'none','none','miter',1);
        hits.lineTo(_root.cell,0);
        createEmptyMovieClip('broken',getNextHighestDepth());
        broken.lineStyle(2,0x212121,100,0,'none','none','miter',1);
        broken.moveTo(_root.cell,0);
        attachMovie('fire','fire_mc',getNextHighestDepth());fire_mc._visible=false")

    call swf_sprite_showframe(presprite)
    call action_sprite(presprite,"if(_root.move_enemy(this)==false)removeMovieClip()")
    call swf_sprite_showframe(presprite)
    call action_sprite(presprite,"if(round_that_is!=_root.current_round)removeMovieClip()")
    sd i=0;while i<(half_fps/2-1);call swf_sprite_showframe(presprite);inc i;endwhile
    call action_sprite(presprite,"fire_mc._visible=false")
    set i 0;while i<(half_fps/2);call swf_sprite_showframe(presprite);inc i;endwhile
    call action_sprite(presprite,"gotoAndPlay(2)")
    call swf_sprite_showframe(presprite)
    setcall sprite swf_sprite_done(presprite);call swf_exports_add(sprite,n2)
endfunction

const info_text_height=15
const more_add=info_text_height+5
const info_x_off=10

#y size
function add_info_turret(ss file,ss dispName,ss game_name,sd minDmg_ground,sd maxDmg_ground,sd minDmg_air,sd maxDmg_air,sd y,sd font,sd e1,sd e2)
    sd presprite
    sd sprite
    sd txt
    sd intern_y=0
    setcall presprite swf_sprite_new()
    #
    sd new_presprite
    setcall new_presprite swf_sprite_new()
    #
    setcall txt turret_text(font,dispName,(info_text_height))
    add intern_y 10
    call swf_sprite_placeobject_coords(presprite,txt,0,0,intern_y)
    #
    chars data_d#300
    str data^data_d
    if minDmg_ground!=0
        add intern_y (more_add)
        call sprintf(data,"Ground Damage: %u-%u",minDmg_ground,maxDmg_ground)
        setcall txt turret_text(font,data,(info_text_height))
        call swf_sprite_placeobject_coords(presprite,txt,1,0,intern_y)
        call actionf_sprite(new_presprite,data,"var %s_min=%u;var %s_max=%u;",e1,minDmg_ground,e1,maxDmg_ground)
    endif
    #
    if minDmg_air!=0
        add intern_y (more_add)
        call sprintf(data,"Air Damage: %u-%u",minDmg_air,maxDmg_air)
        setcall txt turret_text(font,data,(info_text_height))
        call swf_sprite_placeobject_coords(presprite,txt,2,0,intern_y)
        call actionf_sprite(new_presprite,data,"var %s_min=%u;var %s_max=%u;",e2,minDmg_air,e2,maxDmg_air)
    endif
    #
    add intern_y (more_add)

    sd new_sprite
    sd dbl
    data w#1
    data h#1
    setcall dbl swf_dbl_ex(file,#w)
    sd bitmap
    setcall bitmap swf_shape_bitmap(dbl,w,h)
    call swf_sprite_placeobject(new_presprite,bitmap,0)
    call actionf_sprite(new_presprite,data,"var actionName='%s';stop();var waiting=true",game_name)
    call swf_sprite_showframe(new_presprite)
    #
    sd img
    setcall img swf_image("D:/universe/local/turretdefence/preshot.dbl")
    call swf_sprite_placeobject_coords(new_presprite,img,1,0,-20)
    sd i=0;while i<(half_fps/4);call swf_sprite_showframe(new_presprite);inc i;endwhile
    call swf_sprite_removeobject(new_presprite,1)
    call swf_sprite_showframe(new_presprite)
    #
    setcall new_sprite swf_sprite_done(new_presprite)
    sd turret_y
    set turret_y intern_y
    call swf_exports_add(new_sprite,game_name)
    add intern_y h
    add intern_y 10
    #

    call swf_sprite_showframe(presprite)
    setcall sprite swf_sprite_done(presprite)
    #call swf_sprite_placeobject_coords(pre_spr,sprite,depth,(info_x_off),y)
    call sprintf(data,"%s_turret",game_name)
    call swf_exports_add(sprite,data)
    chars act#256
    str actn^act
    call actionf(actn,"var nm='%s';base.attachMovie(nm,nm,base.getNextHighestDepth());
        var mv=base[nm];mv._x=%u;mv._y=%u",data,(info_x_off),y)
    call actionf(actn,"nm='%s';mv.attachMovie(nm,nm,mv.getNextHighestDepth());
        var %s=mv[nm];mv[nm]._y=%u;",game_name,game_name,turret_y)

    return intern_y
endfunction

entry turretdefence()

ss tk="tank"
ss pl="plane"

sd sprite
sd presprite
sd img
#sd dbl
#sd bitmap
sd shape

str env_path="D:/universe/local/turretdefence/env.dbl"

chars a_code#16384
str ac^a_code

sd env_w
sd env_h
setcall env_w swf_dbl_width(env_path)
setcall env_h swf_dbl_height(env_path)

call sprintf(ac,"%s.swf",game_name)
call swf_new(ac,env_w,env_h,0x0101ff,(fps))

#env>
setcall presprite swf_sprite_new()
setcall img swf_image(env_path)
call swf_sprite_placeobject(presprite,img,0)
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"env")
call action("attachMovie('env','env',0)")
#<env

sd texter_y
set texter_y env_h
sub texter_y (texter_height)

sd deffont
setcall deffont swf_font_basic("_sans")
sd txt

#
call actionf(ac,"attachMovie('base','base',getNextHighestDepth());base._x=%u",(info_x))
setcall presprite swf_sprite_new()
const info_line_w=3
data info_struct=no_fill
data *lineheight=info_line_w
    data *lcolor=0x010101FF
data *={0,StateLineStyle}
data *={1,1}
    data *=0;data info_h#1
    set info_h env_h
data *=-1
setcall shape swf_shape((info_line_w),info_h,#info_struct)
call swf_sprite_placeobject(presprite,shape,0)
#
sd info_turret_y
sd turret_font
setcall turret_font swf_font("_sans",(FontFlagsBold))
setcall info_turret_y add_info_turret("D:/universe/local/turretdefence/turrets/War_Tower.dbl","War Tower","mixt",90,100,90,100,0,turret_font,tk,pl)
addcall info_turret_y add_info_turret("D:/universe/local/turretdefence/turrets/Heavy_Turret.dbl","Heavy Turret","ground",100,110,0,0,info_turret_y,turret_font,tk,0)
addcall info_turret_y add_info_turret("D:/universe/local/turretdefence/turrets/Rocket_Launcher.dbl","Rocket Launcher","air",0,0,100,110,info_turret_y,turret_font,0,pl)
#Round
setcall txt turret_dyn_text(turret_font,"round_text",(info_text_height))
call swf_sprite_placeobject_coords(presprite,txt,4,(info_x_off),info_turret_y)
add info_turret_y (more_add)
#Score
setcall txt turret_dyn_text(turret_font,"score_text",(info_text_height))
call swf_sprite_placeobject_coords(presprite,txt,5,(info_x_off),info_turret_y)
add info_turret_y (more_add)
add info_turret_y (more_add)
#Turrets left
sd pre_turretsLeft
sd turretsLeft
setcall pre_turretsLeft swf_sprite_new()
call swf_sprite_showframe(pre_turretsLeft)
setcall turretsLeft swf_sprite_done(pre_turretsLeft)
call swf_exports_add(turretsLeft,"turretsLeft")
#
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"base")
call actionf(ac,"base.attachMovie('turretsLeft','turretsLeft',base.getNextHighestDepth());
                 base.turretsLeft._x=%u;base.turretsLeft._y=%u",(info_x_off),info_turret_y)
#

call declare_grid((grid_lat),(grid_lat),"map",0,"-1")
call declare_grid((grid_lat),(grid_lat),"side",0,"undefined")
call declare_grid((grid_lat),(grid_lat),"senses",0,"undefined")
call declare_grid_zero((2*turret_r),(2*turret_r),"placer_map",0)
call init_reset_grid_value()

#road
call actionf(ac,"var cell=%u;var grid_lat=%u;var grid_x=%u",(cell),(grid_lat),(grid_x))
call action("createEmptyMovieClip('road',getNextHighestDepth());road._x=grid_x;road._y=cell")
#grid
call action("
function fill_grid()
{
    grid.lineStyle(0,0);
    grid.beginFill(0x0101FF,10);
    var row=0;var y=0;
    while(row<grid_lat)
    {
        var col=0;var x=0;
        while(col<grid_lat)
        {
            grid.moveTo(x,y);grid.lineTo(x+cell,y);grid.lineTo(x+cell,y+cell);grid.lineTo(x,y+cell);grid.lineTo(x,y);
            x=x+cell;col++;
        }
        y=y+cell;row++;
    }
    grid.endFill();
}
createEmptyMovieClip('grid',getNextHighestDepth());grid._x=road._x;grid._y=road._y;grid._visible=false;
fill_grid()
")
#used grid
call action("createEmptyMovieClip('used_grid',getNextHighestDepth());used_grid._x=grid._x;used_grid._y=grid._y;used_grid._visible=false")
#grid placer
call actionf(ac,"var turret_r=%u",(turret_r))
call action("
function drawCircle(mc,r,off){
    mc.moveTo(r+r+off,r+off);
    mc.curveTo(r+r+off,Math.tan(Math.PI/8)*r+r+off,Math.sin(Math.PI/4)*r+r+off,Math.sin(Math.PI/4)*r+r+off);
    mc.curveTo(Math.tan(Math.PI/8)*r+r+off,r+r+off,r+off,r+r+off);
    mc.curveTo(0-Math.tan(Math.PI/8)*r+r+off,r+r+off,0-Math.sin(Math.PI/4)*r+r+off,Math.sin(Math.PI/4)*r+r+off);
    mc.curveTo(off,Math.tan(Math.PI/8)*r+r+off,off,r+off);
    mc.curveTo(off,0-Math.tan(Math.PI/8)*r+r+off,0-Math.sin(Math.PI/4)*r+r+off,0-Math.sin(Math.PI/4)*r+r+off);
    mc.curveTo(0-Math.tan(Math.PI/8)*r+r+off,off,r+off,off);
    mc.curveTo(Math.tan(Math.PI/8)*r+r+off,off,Math.sin(Math.PI/4)*r+r+off,0-Math.sin(Math.PI/4)*r+r+off);
    mc.curveTo(r+r+off,0-Math.tan(Math.PI/8)*r+r+off,r+r+off,r+off);
}
function fill_grid_placer()
{
    grid_placer.lineStyle(0,0);
    var m=cell/2;var n=m/2+1;var d=2*turret_r;var rd_w=turret_r*cell-n;

    grid_placer.beginFill(0x01ff01,0);
    drawCircle(grid_placer,rd_w,n);
    grid_placer.endFill();

    var row=0;var y=m;
    var k1=turret_r-1;var k2=turret_r+1;
    while(row<d)
    {
        var col=0;var x=m;
        while(col<d)
        {
            if(grid_placer.hitTest(x,y,true)==true)
            {
                var color=0xFFff01;
                if(row>=k1)if(row<k2)if(col>=k1)if(col<k2)color=0x0101FF;
                grid_placer.beginFill(color,20);
                var a=x-m;var b=y-m;grid_placer.moveTo(a,b);grid_placer.lineTo(a+cell,b);grid_placer.lineTo(a+cell,b+cell);grid_placer.lineTo(a,b+cell);grid_placer.lineTo(a,b);
                grid_placer.endFill();
                placer_map[row][col]=1;
            }
            x=x+cell;col++;
        }
        y=y+cell;row++;
    }
}
createEmptyMovieClip('grid_placer',getNextHighestDepth());grid_placer._visible=false;
grid_placer.createEmptyMovieClip('roadHit',grid_placer.getNextHighestDepth());
fill_grid_placer()
")
#const h_road_sum=8+7+8  const road_sum=w_road_sum+h_road_sum
#;var road_sum=%u       ,(road_sum)      var round_score;      round_score=road_sum;
call actionf(ac,"var w_road_sum=%u",(w_road_sum))
call action("
var current_round;var all_rounds=2;var score;
var turret_place_index;var prev_turrets;var round_turrets;
var round1=new Array(10,12,920);var round2=new Array(10,22,1100);
var round_data=new Array(round1,round2);
var used_color=0xff0101;var used_transp=20;
var en_start_x;var placed_turrets=new Array();
var farest_pos;var maximum_pos;
var left_counter;var right_counter;
function newgame()
{
    road.clear();
    used_grid.clear();
    road.enemies_ground.removeMovieClip();
    road.turrets.removeMovieClip();
    road.enemies_air.removeMovieClip();
    reset_grid_value(grid_lat,grid_lat,map,-1);
    game_start();
}
function game_start()
{
    current_round=0;score=0;placed_turrets.length=0;prev_turrets=0;
    left_counter=0;right_counter=0;

    left_air.length=0;left_ground.length=0;right_air.length=0;right_ground.length=0;

    draw_road();
    road.createEmptyMovieClip('enemies_ground',road.getNextHighestDepth());
    road.createEmptyMovieClip('turrets',road.getNextHighestDepth());
    road.createEmptyMovieClip('enemies_air',road.getNextHighestDepth());

    new_round();
}
function new_round()
{
    _text.message='Turrets Set';
    take_turrets_set();
    round_turrets=round_data[current_round][0];
    round_up();score_up();

    var set_w=env._width-base._x-base.turretsLeft._x-base.turretsLeft._x;var one_w=set_w/round_turrets;
    var i=0;while(i<round_turrets){
        var nm='turret'+i;
        base.turretsLeft.createEmptyMovieClip(nm,base.turretsLeft.getNextHighestDepth());
        var mv=base.turretsLeft[nm];
        var x=one_w*i;mv._x=x;
        mv.beginFill(0x00ff00);mv.lineTo(4,0);mv.lineTo(4,10);mv.lineTo(0,10);mv.lineTo(0,0);mv.endFill();
        i++;
    }
    turret_place_index=prev_turrets+i-1;
}
function round_up(){var r=current_round+1;base.round_text='Round: '+r+'/'+all_rounds;}
function score_up(){base.score_text='Score: '+score;}
function draw_road()
{
    //x x r r x x ->-2,-2,-1 ; then random(1)
    var start_x_pos=2+random(grid_lat-2-2-1);
    en_start_x=start_x_pos*cell;
    var x_left1=en_start_x;var x_right1=x_left1+cell+cell;
    var x_left2;var x_right1;var x_left3;var x_right3;
    var more_y=2*cell;
    var neg_y=0-more_y;
    var y1=8*cell;
    var line_x1=x_left1+cell;var line_y1=y1+cell;var line_x2;var line_y2;var line_x3;var step;
    var half=grid_lat/2;var y2=y1+cell+cell;var w_sum_1;
    var y_left1;var y_right1;var y_left2;var y_right2;
    //h1
    if(start_x_pos<half)
        {x_left2=cell*grid_lat-cell-cell-cell-cell;
        x_right2=x_left2+cell+cell;
        w_sum_1=grid_lat-2-start_x_pos;
        y_left1=y2;y_right1=y1;
        step=1;}
    else
        {x_right2=4*cell;
        x_left2=x_right2-cell-cell;
        w_sum_1=start_x_pos+2-2;
        y_left1=y1;y_right1=y2;
        step=-1;}
    var y4=11*cell+y1;var y3=y4-cell-cell;
    line_x2=x_left2+cell;line_y2=y4-cell;
    var w_sum_2=w_road_sum-w_sum_1;var sum2=w_sum_2-2*cell;
    //v2 h2
    if(start_x_pos<half)
        {x_right3=x_right2-sum2;
        x_left3=x_left2-sum2;
        y_left2=y3;y_right2=y4;}
    else
        {x_left3=x_left2+sum2;
        x_right3=x_right2+sum2;
        y_left2=y4;y_right2=y3;}
    var y5=8*cell+y4;var last_y=y5+more_y;
    line_x3=x_left3+cell;

    road.beginFill(0xccCCcc);
    road.lineStyle(0,0);
    road.moveTo(x_left1,neg_y);

    road.lineTo(x_left1,y_left1);road.lineTo(x_left2,y_left1);road.lineTo(x_left2,y_left2);road.lineTo(x_left3,y_left2);
    road.lineTo(x_left3,last_y);road.lineTo(x_right3,last_y);
    road.lineTo(x_right3,y_right2);road.lineTo(x_right2,y_right2);road.lineTo(x_right2,y_right1);road.lineTo(x_right1,y_right1);
    road.lineTo(x_right1,neg_y);road.lineTo(x_left1,neg_y);
    road.endFill();

    var grid_step=step;
    road.lineStyle(0,0xffFFff);
    road.moveTo(line_x1,neg_y);
    vert_line(line_x1,neg_y,line_y1);
    horiz_line(line_y1,line_x1,line_x2,step);
    vert_line(line_x2,line_y1,line_y2);
    step=step*-1;
    horiz_line(line_y2,line_x2,line_x3,step);
    vert_line(line_x3,line_y2,last_y);

    used_grid.beginFill(used_color,used_transp);
    x_right1=x_right1-cell;
    if(start_x_pos<half)y_left1=y_left1-cell;else y_right1=y_right1-cell;
    vert_grid(x_left1,0,y_left1,'L');vert_grid(x_right1,0,y_right1,'R');
    x_right2=x_right2-cell;
    horiz_grid(y_left1,x_left1,x_left2,grid_step,'L');horiz_grid(y_right1,x_right1,x_right2,grid_step,'R');
    if(start_x_pos<half)y_right2=y_right2-cell;else y_left2=y_left2-cell;
    vert_grid(x_left2,y_left1,y_left2,'L');vert_grid(x_right2,y_right1,y_right2,'R');
    grid_step=grid_step*-1;
    x_right3=x_right3-cell;
    horiz_grid(y_left2,x_left2,x_left3,grid_step,'L');horiz_grid(y_right2,x_right2,x_right3,grid_step,'R');
    vert_grid(x_left3,y_left2,grid_lat*cell,'L');vert_grid(x_right3,y_right2,grid_lat*cell,'R');
    maximum_pos=left_counter;
    used_grid.endFill();
}
function vert_line(x,y,y_end)
{
    while(y<y_end)
    {
        var n=y+10;
        road.lineTo(x,n);y=n+10;road.moveTo(x,y);
    }
}
function horiz_line(y,x,x_end,step)
{
    var half_cell=cell/2;
    while(x!=x_end)
    {
        var m=10*step;var n=x+m;
        road.lineTo(n,y);x=n+m;road.moveTo(x,y);
    }
}
function vert_grid(x,y,y_end,side_value)
{
    var col=x/cell;
    while(y<y_end)
    {
        var row=y/cell;
        senses[row][col]='D';
        side[row][col]=side_value;
        map_grid(row,col,side_value);
        used_grid_set(x,y);
        y=y+cell;
    }
}
function horiz_grid(y,x,x_end,step,side_value)
{
    var row=y/cell;var val;if(step==1)val='R';else val='L';
    while(x!=x_end)
    {
        var col=x/cell;
        senses[row][col]=val;
        side[row][col]=side_value;
        map_grid(row,col,side_value);
        used_grid_set(x,y);
        x=step*cell+x;
    }
}
function map_grid(row,col,side_value){
    var pos;
    if(side_value=='L'){pos=left_counter;left_counter++;}else{pos=right_counter;right_counter++;}
    map[row][col]=pos;
}
function used_grid_set(x,y){used_grid.moveTo(x,y);used_grid.lineTo(x+cell,y);used_grid.lineTo(x+cell,y+cell);used_grid.lineTo(x,y+cell);used_grid.lineTo(x,y);}
function take_turrets_set(){mixt.onPress=take_turret;ground.onPress=take_turret;air.onPress=take_turret;mixt.useHandCursor=true;ground.useHandCursor=true;air.useHandCursor=true;}
function take_turrets_unset(){mixt.onPress=undefined;ground.onPress=undefined;air.onPress=undefined;mixt.useHandCursor=false;ground.useHandCursor=false;air.useHandCursor=false;}
function take_turret()
{
    take_turrets_unset();
    var turret=this._name;attachMovie(turret,'up_turret',getNextHighestDepth());
    grid._visible=true;grid_placer._visible=true;used_grid._visible=true;
    onMouseMove=move_turret;onMouseDown=set_turret;
    move_turret();
}
function move_turret(){
    move_turret_coords(_xmouse,_ymouse);
}
function move_turret_coords(x_pos,y_pos){
    up_turret._x=x_pos-cell;up_turret._y=y_pos-cell-cell;
    //find a way for grid_placer
    var a=cell/2;
    var gridPos_x=int(x_pos+a/cell);var gridPos_y=int(y_pos/cell);
    var left=gridPos_x*cell;var top=gridPos_y*cell;var half_placer=turret_r*cell;
    grid_placer._x=left-half_placer;grid_placer._y=top-half_placer+cell;
    grid_placer.roadHit.clear();grid_placer_tests(0);
}
function set_turret(){
    if(turret_set()==false)take_turrets_set();
    up_turret.removeMovieClip();
    grid._visible=false;grid_placer._visible=false;used_grid._visible=false;
    onMouseMove=undefined;onMouseDown=undefined;
}
//bool true when all turrets are set
function turret_set(){
    var half_placer=turret_r*cell;
    var x_R=grid_placer._x+half_placer;var y_B=grid_placer._y+half_placer;
    var x_L=x_R-cell;var y_T=y_B-cell;
    //wrong results if testing with topLeft point
    var half_cell=cell/2;
    var x_L_test=x_L+half_cell;var y_T_test=y_T+half_cell;
    var x_R_test=x_R+half_cell;var y_B_test=y_B+half_cell;
    if(grid.hitTest(x_L_test,y_T_test,true)==false)return false;
    if(grid.hitTest(x_R_test,y_T_test,true)==false)return false;
    if(grid.hitTest(x_L_test,y_B_test,true)==false)return false;
    if(grid.hitTest(x_R_test,y_B_test,true)==false)return false;
    if(used_grid.hitTest(x_L_test,y_T_test,true)==true)return false;
    if(used_grid.hitTest(x_R_test,y_T_test,true)==true)return false;
    if(used_grid.hitTest(x_L_test,y_B_test,true)==true)return false;
    if(used_grid.hitTest(x_R_test,y_B_test,true)==true)return false;

    x_L=x_L-road._x;x_R=x_R-road._x;
    y_T=y_T-road._y;y_B=y_B-road._y;

    var t_index=turret_place_index-prev_turrets;
    base.turretsLeft['turret'+t_index].removeMovieClip();
    var name=up_turret.actionName;
    var field_nm=name+turret_place_index;
    road.turrets.attachMovie(name,field_nm,road.turrets.getNextHighestDepth());var mv=road.turrets[field_nm];
    used_grid.beginFill(used_color,used_transp);
    used_grid_set(x_L,y_T);used_grid_set(x_R,y_T);used_grid_set(x_L,y_B);used_grid_set(x_R,y_B);
    used_grid.endFill();
    mv._x=x_L;mv._y=y_T-cell;
    place_turret(mv);

    grid_placer_tests(mv,name);

    if(turret_place_index!=prev_turrets){turret_place_index--;return false;}
    start_round();
    return true;
}
function grid_placer_tests(mv,name){
    var delta_row=grid_placer._y-grid._y/cell;var delta_col=grid_placer._x-grid._x/cell;
    var row=0;var d=turret_r*2;
    while(row<d){
        var gr_row=delta_row+row;
        if(gr_row>=0)if(gr_row<grid_lat){
            var col=0;
            while(col<d){
                if(placer_map[row][col]==1){
                    var gr_col=delta_col+col;
                    if(gr_col>=0)if(gr_col<grid_lat){
                        var map_pos=map[gr_row][gr_col];
                        if(map_pos!=-1){
                            if(mv!=0){
                                var side_value=side[gr_row][gr_col];var ground=0;var air=0;
                                if(name=='ground')ground=1;if(name=='mixt')ground=1;
                                if(name=='air')air=1;if(name=='mixt')air=1;
                                if(ground==1){if(side_value=='L')add_zone(map_pos,left_ground,mv);else add_zone(map_pos,right_ground,mv);}
                                if(air==1){if(side_value=='L')add_zone(map_pos,left_air,mv);else add_zone(map_pos,right_air,mv);}
                            }
                            else{
                                var wr_x=col*cell;var wr_y=row*cell;
                                var des_mv=grid_placer.roadHit;
                                des_mv.beginFill(0x01FF01,20);
                                des_mv.moveTo(wr_x,wr_y);des_mv.lineTo(wr_x+cell,wr_y);des_mv.lineTo(wr_x+cell,wr_y+cell);des_mv.lineTo(wr_x,wr_y+cell);des_mv.lineTo(wr_x,wr_y);
                                des_mv.endFill();
                            }
                        }
                    }
                }
                col++;
            }
        }
        row++;
    }
}
function place_turret(mv){
    var i=0;while(i<placed_turrets.length){
        if(placed_turrets[i]._y>mv._y){
            placed_turrets.splice(i,0,mv);
            var max=placed_turrets.length-1;
            while(i<max){placed_turrets[i].swapDepths(placed_turrets[i+1]);i++;}
            return undefined;
        }
        i++;
    }
    placed_turrets.push(mv);
}
function add_zone(map_pos,line,turret)
{
    var i=get_zone(map_pos,line);
    line[i][1].push(turret);
}
function get_zone(map_pos,line)
{
    var i=0;
    while(i<line.length){
        if(line[i][0]>=map_pos){
            if(line[i][0]>map_pos)new_zone_pos(i,map_pos,line);
            return i;
        }
        i++;
    }
    new_zone_pos(i,map_pos,line);
    return i;
}
function new_zone_pos(i,map_pos,line){
    line.splice(i,0,undefined);
    line[i]=new Array(map_pos,undefined);
    line[i][1]=new Array();
}
")

include "moving.s"

call default_text_ex((0x01ffFFff),deffont,texter_y,(texter_height))

call submit_form(deffont,env_w,env_h,game_name)

#call init_debug_grid()

call action("game_start()")

#include "debugging.s"

call swf_exports_done()
call swf_showframe()
call swf_done()
return 1
