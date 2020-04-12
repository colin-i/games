
format elfobj64microsoft

warning off
include "../actionswf/dev/import.h"
warning on

include "../actionswf/include/prog.h"

const env=1
const grid=1

str game_name="Naval War"

include "../files/tool.s"
include "../files/submit.s"

const width=800
const play_height=420
const height=play_height+font_rect_height
const titles_height=30

function ship_on_swf(ss hz_name,ss path,ss vt_name)
    sd presprite
    sd dbl
    setcall presprite swf_sprite_new()
    sd shape
    data w#1
    data h#1
    sd wh^w
    setcall dbl swf_dbl_ex(path,wh)
    setcall shape swf_shape_bitmap(dbl,w,h)
    chars act#256
    call sprintf(#act,"
    var width=%u;var height=%u;var action_name='%s';
    var hz_vt=1;var size=width/_root.cell;var cols_nr=size;var rows_nr=1;
    _root._grid2_map.call_from_loaded_instance(this);
    ",w,h,hz_name)
    call action_sprite(presprite,#act)
    call swf_sprite_placeobject(presprite,shape,1)
    call swf_sprite_showframe(presprite)
    sd sprite
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,hz_name)
    #
    sd aux
    set aux w
    set w h
    set h aux
    setcall presprite swf_sprite_new()
    call sprintf(#act,"
    var width=%u;var height=%u;var action_name='%s';
    var hz_vt=-1;var size=height/_root.cell;var cols_nr=1;var rows_nr=size;
    _rotation=90;
    _root._grid2_map.call_from_loaded_instance(this);
    ",w,h,vt_name)
    call action_sprite(presprite,#act)
    mult w -1
    call swf_sprite_placeobject_coords(presprite,shape,1,0,w)
    call swf_sprite_showframe(presprite)
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,vt_name)
endfunction

entry navalwar()

sd depths=1

chars a_code#16384
str ac^a_code
#D:/oapp/games/
call sprintf(ac,"%s.swf",game_name)

call swf_new(ac,(width),(height),0xff1100,12)

str env_path="D:/universe/local/navalwar/env.dbl"
str grid_path="D:/universe/local/navalwar/grid.dbl"
#
str ac_hz_path="D:/universe/local/navalwar/Aircraft_Carrier.dbl"
#
sd env_w
setcall env_w swf_dbl_width(env_path)
sd grid_w
setcall grid_w swf_dbl_width(grid_path)
sd grid_h
setcall grid_h swf_dbl_height(grid_path)
#
sd boats_w=10
sd ac_hz_w
setcall ac_hz_w swf_dbl_width(ac_hz_path)
add boats_w ac_hz_w
#
const boats_font_height=15
const boats_text_height=boats_font_height+10
sd boats_h=10
sd ac_hz_h
setcall ac_hz_h swf_dbl_height(ac_hz_path)
sd ac_vt_h
setcall ac_vt_h swf_dbl_width(ac_hz_path)
const boats_vert_horiz_sz_h=boats_font_height+static_text_off_add
sd horiz_y=boats_text_height+boats_vert_horiz_sz_h
sd text_vr_y
set text_vr_y horiz_y
add text_vr_y ac_hz_h
sd vert_y
set vert_y text_vr_y
add vert_y (boats_vert_horiz_sz_h)
add boats_h vert_y
add boats_h ac_vt_h
#
sd half_boats_w
set half_boats_w boats_w
div half_boats_w 2
sd half_env
set half_env env_w
div half_env 2
#
sd boats_x
set boats_x half_env
sub boats_x half_boats_w
#
sd grid1_x
set grid1_x boats_x
sub grid1_x grid_w
div grid1_x 2
sd grid2_x
set grid2_x half_env
add grid2_x half_boats_w
add grid2_x grid1_x
#

sd grid_width
set grid_width grid_w
div grid_width (cell)
sd grid_height
set grid_height grid_h
div grid_height (cell)
ss grid_name="grid"
ss comp_grid_name="comp_grid"

sd deffont
setcall deffont swf_font_basic("_sans")

#include "../files/base.s"

sd presprite
sd img
sd sprite
sd txt

#env begin
if (env)==1
setcall presprite swf_sprite_new()
setcall img swf_image(env_path)
call swf_sprite_placeobject(presprite,img,1)
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"env")
call sprintf(ac,"
attachMovie('env','_env',%u);
",depths)
inc depths
call action(ac)
endif
#env end

#titles begin
const titles_y=5
setcall presprite swf_sprite_new()
#
setcall txt static_text(half_env,"Player",deffont,(titles_height))
call swf_sprite_placeobject_coords(presprite,txt,1,grid1_x,(titles_y))
setcall txt static_text(half_env,"Computer",deffont,(titles_height))
call swf_sprite_placeobject_coords(presprite,txt,2,grid2_x,(titles_y))
#
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"names")
call sprintf(ac,"
attachMovie('names','_names',%u);
",depths)
inc depths
call action(ac)
#titles end

sd shape
#grid begin
if (grid)==1
setcall presprite swf_sprite_new()
#
data struct=repeating_bitmap_fill
    data refid#1
    setcall refid swf_dbl(grid_path)
data *=2
data *=0x000000FF
data *={0,StateFillStyle0|StateLineStyle}
data *={1,1}
    data w1#1;data *=0
    set w1 grid_w
data *={1,1}
    data *=0;data h1#1
    set h1 grid_h
data *={1,1}
    data w2=-1;data *=0
    mult w2 grid_w
data *={1,1}
    data *=0;data h2=-1
    mult h2 grid_h
data *=-1
setcall shape swf_shape(grid_w,grid_h,#struct)
call swf_sprite_placeobject(presprite,shape,1)
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"grid")
call sprintf(ac,"
attachMovie('grid','_grid1',%u);
",depths)
inc depths
call action(ac)
call sprintf(ac,"
attachMovie('grid','_grid2',%u);
",depths)
inc depths
call action(ac)
#
setcall presprite swf_sprite_new()
call action_sprite(presprite,"var ship_nr;_x=_root._grid1._x;_y=_root._grid1._y")
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"grid_map1")
#
setcall presprite swf_sprite_new()
call action_sprite(presprite,"var ship_nr;_x=_root._grid2._x;_y=_root._grid2._y")
include "computer.s"
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"grid_map2")
endif
#grid end

#center begin
setcall presprite swf_sprite_new()
#
call sprintf(ac,"var boats_w=%u;var boats_h=%u",boats_w,boats_h)
call action_sprite(presprite,ac)
call sprintf(ac,"var horiz_y=%u;var text_vr_y=%u;var vert_y=%u",horiz_y,text_vr_y,vert_y)
call action_sprite(presprite,ac)
call sprintf(ac,"var grid_w=%u;var grid_h=%u",grid_w,grid_h)
call sprintf(ac,"_x=%u;_y=_root._grid1._y",boats_x)
call action_sprite(presprite,ac)
call action_sprite(presprite,ac)
call action_sprite(presprite,"
function ship_at_boats()
{
    var name=_root.ships[_root._grid1_map.ship_nr];
    if(name==undefined)
    {
        _root._grid2_map.set_comp_ship();
        removeMovieClip();
        return undefined;
    }
    _root._grid1_map.ship_nr=_root._grid1_map.ship_nr+1;

    var disp_name=name;
    var i=0;
    var max=disp_name.length;
    var next;
    while(i<max)
    {
        if(disp_name.charAt(i)=='_')
        {
            next=i+1;
            disp_name=disp_name.substr(0,i)+' '+disp_name.substr(next);
        }
        i=i+1;
    }
    attachMovie('boats_border','_border',1);
    attachMovie('boats_texts','_texts',2);
    _texts.ship_name=disp_name;
    //
    var hz=name+'_horiz';
    var vt=name+'_vert';
    attachMovie(hz,hz,3);
    attachMovie(vt,vt,4);
    hz_inst=this[hz];
    set_prepared(hz_inst,horiz_y,hz_dragShip);
    vt_inst=this[vt];
    set_prepared(vt_inst,vert_y,vt_dragShip);
}
function set_prepared(inst,y,fn)
{
    inst._y=y;
    inst.onPress=fn;
}
var hz_inst;var vt_inst;
function hz_dragShip()
{
    _root.current_inst=hz_inst;
    dragShip(hz_inst,vt_inst);
}
function vt_dragShip()
{
    _root.current_inst=vt_inst;
    dragShip(vt_inst,hz_inst);
}
function dragShip(a,b)
{
    verify_depths(a,b);
    a.startDrag();
    a.onPress=_root.place_ship;
    _root.onMouseMove=_root.move_ship;
}
function verify_depths(a,b)
{
    var m=a.getDepth();
    var n=b.getDepth();
    if(m<n)a.swapDepths(b);
}
function reset_pos(current_inst)
{
    if(current_inst==hz_inst)
    {
        hz_inst._x=0;
        set_prepared(hz_inst,horiz_y,hz_dragShip);
        return undefined;
    }
    vt_inst._x=0;
    set_prepared(vt_inst,vert_y,vt_dragShip);
}

_root._grid1_map.ship_nr=0;
ship_at_boats();
")
#
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"boats")
#
setcall presprite swf_sprite_new()
data brd=no_fill
data *=2
data *=0x000000FF
data *={0,StateLineStyle}
data *={1,1}
    data b_w1#1;data *=0
    set b_w1 boats_w
data *={1,1}
    data *=0;data b_h1#1
    set b_h1 boats_h
data *={1,1}
    data b_w2=-1;data *=0
    mult b_w2 boats_w
data *={1,1}
    data *=0;data b_h2=-1
    mult b_h2 boats_h
data *=-1
setcall shape swf_shape(boats_w,boats_h,#brd)
call swf_sprite_placeobject(presprite,shape,1)
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"boats_border")
#
sd texts_sprite
setcall texts_sprite swf_sprite_new()
#
sd ed_str
setcall ed_str edittext_struct()
call edittext_font(deffont,(boats_font_height))
#
setcall txt swf_text(boats_w,(boats_text_height),"ship_name",(HasFont|ReadOnly|NoSelect),ed_str)
call swf_sprite_placeobject_coords(texts_sprite,txt,1,5,0)
#
setcall txt static_text(boats_w,"Horizontal",deffont,(boats_font_height))
call swf_sprite_placeobject_coords(texts_sprite,txt,2,5,(boats_text_height))
#
setcall txt static_text(boats_w,"Vertical",deffont,(boats_font_height))
call swf_sprite_placeobject_coords(texts_sprite,txt,3,5,text_vr_y)
#
call swf_sprite_showframe(texts_sprite)
setcall sprite swf_sprite_done(texts_sprite)
call swf_exports_add(sprite,"boats_texts")
#
call ship_on_swf("Aircraft_Carrier_horiz",ac_hz_path,"Aircraft_Carrier_vert")
call ship_on_swf("Destroyer_horiz","D:/universe/local/navalwar/Destroyer.dbl","Destroyer_vert")
call ship_on_swf("Battleship_horiz","D:/universe/local/navalwar/Battleship.dbl","Battleship_vert")
call ship_on_swf("Assault_Ship_horiz","D:/universe/local/navalwar/Assault_Ship.dbl","Assault_Ship_vert")
#center end

setcall depths default_text(depths,(0xff0011FF),deffont,(play_height))

sd grid1_y=titles_y+titles_height+titles_y+5

call sprintf(ac,"
_grid1._x=%u;_grid2._x=%u;_grid1._y=%u;_grid2._y=_grid1._y;var cell=%u
",grid1_x,grid2_x,grid1_y,(cell))
call action(ac)
call sprintf(ac,"var grid_w=%u;var grid_h=%u",grid_w,grid_h)
call action(ac)
call sprintf(ac,"var grid_width=%u;var grid_height=%u",grid_width,grid_height)
call action(ac)
sd deltaX
set deltaX boats_x
sub deltaX grid1_x
call sprintf(ac,"var deltaX=%u",deltaX)
call action(ac)
sd max_cells
set max_cells ac_vt_h
div max_cells (cell)
call sprintf(ac,"var max_cells=%u",max_cells)
call action(ac)

call declare_grid_zero(grid_width,grid_height,grid_name,0)
call declare_grid_zero(grid_width,grid_height,comp_grid_name,0)
call init_reset_grid()

call action("
var ships=new Array('Aircraft_Carrier','Destroyer','Battleship','Assault_Ship');

var current_inst;
var on_the_grid='on_grid';
var is_on_grid=0;
var score;

function newgame()
{
    reset_grid(grid_width,grid_height,grid);
    reset_grid(grid_width,grid_height,comp_grid);
    _grid1_map.removeMovieClip();
    delete _grid2_map.comp_grid_hz_test;
    delete _grid2_map.comp_grid_vt_test;
    delete _grid2_map.values;
    _grid2_map.removeMovieClip();
    gamestart();
}
function gamestart()
{
    attachMovie('grid_map1','_grid1_map',getNextHighestDepth());
    attachMovie('grid_map2','_grid2_map',getNextHighestDepth());
    attachMovie('boats','_boats',getNextHighestDepth());
    _text.message='Place the ships on the grid';
    score=grid_width*grid_height;
}

function place_ship()
{
    var shadow=_grid1_map[on_the_grid];
    if(test_wrong_drop()==0)
    {
        _boats.reset_pos(current_inst);
        shadow.removeMovieClip();
        return undefined;
    }

    var name=current_inst.action_name;
    _grid1_map.attachMovie(name,name,_grid1_map.getNextHighestDepth());
    _grid1_map[name]._x=shadow._x;
    _grid1_map[name]._y=shadow._y;


    shadow.removeMovieClip();

    _boats.ship_at_boats();

    current_inst.removeMovieClip();
}
function void_function(){}
//bool
function test_wrong_drop()
{
    is_on_grid=0;
    onMouseMove=void_function;
    current_inst.stopDrag();
    var shadow=_grid1_map[on_the_grid];
    if(shadow==undefined)
    {
        return 0;
    }

    var col=shadow._x/cell;
    var row=shadow._y/cell;
    var ref=current_inst.width;
    if(current_inst.width<current_inst.height)ref=current_inst.height;
    ref=ref/cell;
    var max_col=col+ref;
    var max_row=row+1;
    if(current_inst.width<current_inst.height){max_col=col+1;max_row=row+ref;}
    var c;
    var r=row;
    while(r<max_row)
    {
        c=col;
        while(c<max_col)
        {
            if(grid[r][c]!=0)return 0;
            c=c+1;
        }
        r=r+1;
    }
    while(row<max_row)
    {
        c=col;
        while(c<max_col)
        {
            grid[row][c]=_grid1_map.ship_nr*current_inst.hz_vt;
            c=c+1;
        }
        row=row+1;
    }
    return 1;
}
function move_ship()
{
    var round=cell/2;
    var left=current_inst._x+deltaX+round;
    var top=current_inst._y+round;
    left=left/cell;
    left=int(left)*cell;
    top=top/cell;
    top=int(top)*cell;
    var is_in=ship_grid_hittest(left,top);
    if(is_on_grid==0)
    {
        if(is_in==0)return undefined;
        is_on_grid=1;
        ship_on_grid();
        ship_shadow(left,top);
        return undefined;
    }
    if(is_in==0)
    {
        _grid1_map[on_the_grid].removeMovieClip();
        is_on_grid=0;
        return undefined;
    }
    ship_shadow(left,top);
}
//bool
function ship_grid_hittest(left,top)
{
    var right=left+current_inst.width;
    var bottom=top+current_inst.height;
    if(left<0)return 0;
    if(top<0)return 0;
    if(right>grid_w)return 0;
    if(bottom>grid_h)return 0;
    return 1;
}
function ship_on_grid()
{
    _grid1_map.createEmptyMovieClip(on_the_grid,1);
    //add shadow
    var mv=_grid1_map[on_the_grid];
    mv.lineStyle(1,0x000000,100,0,'none','round','miter',1);
    mv.beginFill(0x0011FF,20);
    var w=current_inst.width;
    var h=current_inst.height;
    mv.lineTo(w,0);
    mv.lineTo(w,h);
    mv.lineTo(0,h);
    mv.lineTo(0,0);
    mv.endFill();
}
function ship_shadow(left,top)
{
    _grid1_map[on_the_grid]._x=left;
    _grid1_map[on_the_grid]._y=top;
}
gamestart();


function water_shot(row,col,container)
{
    draw_shot(row,col,container,0x0010ff);
}
function ship_shot(row,col,container)
{
    draw_shot(row,col,container,0xff0010);
}
function under_water(row,col,container)
{
    var name='shot_'+row+'_'+col;
    if(container[name]!=undefined)container[name].removeMovieClip();
    water_shot(row,col,container);
}
function draw_shot(row,col,container,color)
{
    var name='shot_'+row+'_'+col;
    container.createEmptyMovieClip(name,container.getNextHighestDepth());
    var mv=container[name];
    mv.beginFill(color,20);
    mv.lineTo(cell,0);
    mv.lineTo(cell,cell);
    mv.lineTo(0,cell);
    mv.lineTo(0,0);
    mv.endFill();
    mv._x=col*cell;
    mv._y=row*cell;
}
")

call submit_form(deffont,(width),(height),game_name)

call swf_exports_done()
call swf_showframe()

#include "./debugging.s"

call swf_done()

return 1
