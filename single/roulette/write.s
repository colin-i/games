
format elfobj64microsoft

warning off
include "../actionswf/dev/import.h"
warning on

include "../actionswf/include/prog.h"

const env=1
const roulette=1
const info=1

str game_name="Roulette"

include "../files/tool.s"
include "../files/submit.s"

const width=800
const roulette_margins=10
const roulette_y=roulette_margins+font_rect_height
const height=600+roulette_y+roulette_margins
const dist=50
const power_wd=30
const power_hg=150

entry roulette()

sd left
sd info_width
sd depths=1

chars a_code#16384
str ac^a_code
#D:/oapp/games/
call sprintf(ac,"%s.swf",game_name)

call swf_new(ac,(width),(height),0xff1100,32)

str r_path="D:/universe/local/roulette/roulette.dbl"
str bg_path="D:/universe/local/roulette/bg.dbl"
sd img
sd presprite
sd sprite
sd r_width
sd r_height
setcall r_width swf_dbl_width(r_path)
setcall r_height swf_dbl_height(r_path)
sd bg_width
sd bg_height
setcall bg_width swf_dbl_width(bg_path)
setcall bg_height swf_dbl_height(bg_path)
#
set left (roulette_margins)
add left r_width
add left (dist)
set info_width (width-10)
sub info_width left

sd deffont
setcall deffont swf_font_basic("_sans")

#include "../files/base.s"

#env begin
if (env)==1
setcall presprite swf_sprite_new()
setcall img swf_image(bg_path)
call swf_sprite_placeobject(presprite,img,1)
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"bg")
call sprintf(ac,"
attachMovie('bg','_bg',%u);
",depths)
inc depths
call action(ac)
endif
#env end

setcall depths default_text(depths,(0x1100ffFF),deffont,0)

#roulette begin
if (roulette)==1
sd x
set x r_width
div x -2
sd y
set y r_height
div y -2

setcall presprite swf_sprite_new()
setcall img swf_image(r_path)
call swf_sprite_placeobject_coords(presprite,img,1,x,y)
const slow_frames=10
sd i=slow_frames
while i>0
    call swf_sprite_showframe(presprite)
    dec i
endwhile
call action_sprite(presprite,"
_root.spin()
")
call swf_sprite_showframe(presprite)
sd r_sprite
setcall r_sprite swf_sprite_done(presprite)
call swf_exports_add(r_sprite,"roulette")
mult x -1
add x (roulette_margins)
mult y -1
add y (roulette_y)
call sprintf(ac,"
attachMovie('roulette','_roulette',%u);
_roulette._x=%u+10;
_roulette._y=%u;
",depths,x,y)
inc depths
call action(ac)
endif
#roulette end

sd shape
#info begin
if (info)==1
#power
const spin_start_y=power_hg/2
setcall presprite swf_sprite_new()
    #white
sd white
sd prewhite
setcall prewhite swf_sprite_new()
data nopower={solid_fill,0xFFffFFff,0}
data *={0,StateFillStyle0}
data *={1,1,power_wd,0,   1,1,0,power_hg,   1,1,0-power_wd,0,   1,1,0,0-power_hg}
data *=-1
setcall shape swf_shape((power_wd),(power_hg),#nopower)
call swf_sprite_placeobject(prewhite,shape,1)
call swf_sprite_showframe(prewhite)
setcall white swf_sprite_done(prewhite)
call swf_exports_add(white,"white")
    #powerup
sd powerup
sd prepowerup
setcall prepowerup swf_sprite_new()
call swf_sprite_showframe(prepowerup)
call action_sprite(prepowerup,"
function pw_up()
{
_root.power_current_height=_root.power_current_height+4;
if(_root.power_current_height>_root.power_height)
{
    _root.powerup_reset();
    return undefined;
}
empty_power();
createEmptyMovieClip('powerup',1);
powerup.beginFill(0xFF0000);
powerup.lineTo(_root.power_width,0);
powerup.lineTo(_root.power_width,_root.power_current_height);
powerup.lineTo(0,_root.power_current_height);
powerup.lineTo(0,0);
powerup.endFill();
powerup._y=_root.power_height-_root.power_current_height;
}
function empty_power()
{
    if(powerup!=undefined)powerup.removeMovieClip();
}
pw_up()
")
call swf_sprite_showframe(prepowerup)
setcall powerup swf_sprite_done(prepowerup)
call swf_exports_add(powerup,"powerup")
    #lines
sd lines
sd prelines
setcall prelines swf_sprite_new()
data bar={no_fill,10,0x0000FFff}
data *={0,StateLineStyle}
data *={1,1,power_wd,0,   1,1,0,power_hg,   1,1,0-power_wd,0,   1,1,0,0-power_hg}
data *={0,StateMoveTo,0,spin_start_y}
data *={1,1,power_wd,0}
data *=-1
setcall shape swf_shape((power_wd),(power_hg),#bar)
call swf_sprite_placeobject(prelines,shape,1)
call swf_sprite_showframe(prelines)
setcall lines swf_sprite_done(prelines)
call swf_exports_add(lines,"lines")
    #
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"powerbar")
call sprintf(ac,"
attachMovie('powerbar','_powerbar',%u);
_powerbar._x=%u;
_powerbar._y=%u;
_powerbar.attachMovie('white','_white',1);
_powerbar.attachMovie('powerup','_powerup',2);
_powerbar.attachMovie('lines','_lines',3);
",depths,left,(font_rect_height))
inc depths
call action(ac)
#
const inf_height=70
sd e_struct
setcall e_struct edittext_struct()
#select
setcall presprite swf_sprite_new()
data struct={solid_fill,0xffFFffFF,10,0x0000FFff}
data *={0,StateFillStyle0|StateLineStyle}
data *={1,1}
    data w1#1;data *=0
    set w1 info_width
data *={1,1,0,inf_height,    1,1}
    data w2=-1;data *=0
    mult w2 info_width
data *={1,1,0,inf_height*-1}
const left_arrow=17
data *={0,StateMoveTo,0-left_arrow,inf_height/2}
data *={1,1,left_arrow,0}
data *=-1
setcall shape swf_shape(info_width,(inf_height),#struct)
call swf_sprite_placeobject(presprite,shape,1)
#
sd txt
call edittext_font(deffont,30)
setcall txt swf_text(info_width,(30+10),"sel_info",(HasFont|ReadOnly|NoSelect),e_struct)
call swf_sprite_placeobject_coords(presprite,txt,2,30,(inf_height/2-20))
#
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"select")
sd select_x
set select_x left
sub select_x 20
sd select_y
set select_y y
sub select_y (inf_height/2)
call sprintf(ac,"
attachMovie('select','_select',%u);
_select._x=%u;
_select._y=%u;
",depths,select_x,select_y)
inc depths
call action(ac)
#details
const details_height=2*font_rect_height+20
const to_left=40
sd det_wd=to_left
add det_wd info_width
sd det_left=0-to_left
add det_left left
setcall presprite swf_sprite_new()
setcall shape swf_shape_basic(det_wd,(details_height),0xffFFffFF,0x0000FFff)
call swf_sprite_placeobject(presprite,shape,1)
#
const font_sz=20
call edittext_font(deffont,(font_sz))
const hg=font_sz+10
const left_off=10
setcall txt swf_text(det_wd,(hg),"spin_info",(HasFont|ReadOnly|NoSelect),e_struct)
call swf_sprite_placeobject_coords(presprite,txt,2,(left_off),(details_height/2-font_sz-font_sz))
setcall txt swf_text(det_wd,(hg),"score_info",(HasFont|ReadOnly|NoSelect),e_struct)
call swf_sprite_placeobject_coords(presprite,txt,3,(left_off),(details_height/2))
#
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"info")
call sprintf(ac,"
attachMovie('info','_info',%u);
_info._x=%u;
_info._y=%u;
",depths,det_left,(height-font_rect_height-details_height))
inc depths
call action(ac)
endif
#info end

call sprintf(ac,"var power_width=%u;var power_height=%u",(power_wd),(power_hg))
call action(ac)
call sprintf(ac,"var total_pos=%u;var spin_start_y=%u;var slow_frames=%u
",64,(spin_start_y),(slow_frames))
call action(ac)
call action("
var current_round;
var total_rounds;
var score;
var power_current_height;
var round_spins;
var current_pos;
function powerup_reset()
{
    power_current_height=0;
    _powerbar._powerup.stop();
    _powerbar._powerup.empty_power();
    onMouseDown=get_power;
    _text.message='Click to play';
}
function get_power()
{
    _text.message='';
    _powerbar._powerup.play();
    onMouseDown=use_power;
}
function use_power()
{
    var current_y=power_height-power_current_height;
    if(current_y>spin_start_y)
    {
        powerup_reset();
        return undefined;
    }
    onMouseDown=undefined;
    _powerbar._powerup.stop();
    var total_power=(current_y-spin_start_y)*-1;
    var launch_spins=total_power*total_pos/spin_start_y;
//3773 msec minim    7622 msec maxim        5697.5 medie
    round_spins=total_pos+int(launch_spins)+random(total_pos);
    _roulette.play();
}
//value
function get_a_value(nr)
{
    if(current_pos==0)
    {
        return 1000;
    }
    var test;
    test=current_pos&1;
    if(test!=0)
    {
        if(nr==1)
            if(total_rounds<6)
                total_rounds=total_rounds+1;
        return 25;
    }
    test=current_pos&2;
    if(test!=0)
    {
        return 50;
    }
    test=current_pos&4;
    if(test!=0)
    {
        return 100;
    }
    test=current_pos&8;
    if(test!=0)
    {
        return 250;
    }
    if(current_pos==32)
    {
        if(nr==0)return 'x2';
        return score;
    }
    return 500;
}
function reset_sel()
{
    _select.sel_info=get_a_value(0);
}
function spin()
{
    var f=slow_frames;
    if(round_spins<slow_frames)
    {
        f=round_spins;
    }
    if(f==0)
    {
        _roulette.stop();
        score=score+get_a_value(1);
        current_round=current_round+1;
        round_ready();
        if(current_round==total_rounds)
        {
            submitform(score);
            return undefined;
        }
        powerup_reset();
        update_net_score(score);
        return undefined;
    }
    current_pos=current_pos+1;
    if(current_pos==total_pos)current_pos=0;
    reset_sel();
    var angle=360*current_pos/total_pos;
    angle=int(angle);
    if(angle>180)angle=angle-360;
    _roulette._rotation=angle;
    round_spins=round_spins-1;
    _roulette.gotoAndPlay(f);
}
function round_ready()
{
    _info.spin_info='Rounds: '+current_round+'/'+total_rounds;
    _info.score_info='Score: '+score;
}
current_pos=0;
reset_sel();
_roulette.stop();
function newgame()
{
    current_round=0;
    total_rounds=3;
    score=0;
    powerup_reset();
    round_ready();
}
newgame();
")

call submit_form(deffont,(width),(height),game_name)

call swf_exports_done()
call swf_showframe()
call swf_done()

return 1
