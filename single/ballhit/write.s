
format elfobj64microsoft

const env=1
const ball=1
const bat=1
const act=1
const submit=1

#import "printf" printf
warning off
include "../actionswf/dev/import.h"
warning on
include "../actionswf/include/prog.h"

str game_name="Ball Hit"

const landoffset=10
const balltop=20
const width=800
const height=400

include "../files/tool.s"
include "../files/submit.s"

entry ballhit()

sd image
str env_path="D:/universe/local/ballhit/env1.dbl"
str env2_path="D:/universe/local/ballhit/env2.dbl"
str ballpath="D:/universe/local/ballhit/ball.dbl"
str bat_format="D:/universe/local/ballhit/baseballbat%u.dbl"
str a_bat="D:/universe/local/ballhit/baseballbat0.dbl"
sd bat_x
set bat_x (width)
subcall bat_x swf_dbl_width(a_bat)
div bat_x 2
sd bat_y
set bat_y (height)
subcall bat_y swf_dbl_height(a_bat)
sub bat_y (landoffset)
sd ball_height
setcall ball_height swf_dbl_height(ballpath)
#
sd envheight
setcall envheight swf_dbl_height(env_path)
sd envwidth
setcall envwidth swf_dbl_width(env_path)
sd ballheight
setcall ballheight swf_dbl_height(ballpath)
#
sd minfall
set minfall (balltop)
sd maxfall
set maxfall envheight
sub maxfall ballheight
#sub maxfall 10
#sd medfall;set medfall bat_y;sub medfall 70
#sd intfall;set intfall medfall;sub intfall minfall

sd cursor;const step=2
#sd dif=3;sd framesfall=0;set cursor minfall
#while cursor<medfall
#    sd valuefall;    set valuefall cursor;    sub valuefall minfall;    mult valuefall dif;    div valuefall intfall;    sub valuefall dif;    mult valuefall -1;    add framesfall valuefall;    add cursor (step)
#endwhile

#sd restfall;set restfall maxfall;sub restfall cursor;div restfall (step)

#add framesfall restfall

#sd framerate;set framerate framesfall;div framerate 4
#74 dadea framerateul

chars a_code#16384
str ac^a_code

#D:/oapp/games/
call sprintf(ac,"%s.swf",game_name)

call swf_new(ac,(width),(height),0x0011ff,100)
sd deffont
setcall deffont swf_font_basic("_sans")


sd depths=1

#env begin
if (env)==1
sd envsprite
setcall envsprite swf_sprite_new()
setcall image swf_image(env_path)
call swf_sprite_placeobject(envsprite,image,depths)
call swf_sprite_showframe(envsprite)
call action_sprite(envsprite,"_root.move();")
call swf_sprite_showframe(envsprite)
#
str envname1="env1"
sd env
setcall env swf_sprite_done(envsprite)
call swf_exports_add(env,envname1)
call sprintf(ac,"this.attachMovie('env1','_%s',%u)",envname1,depths)
call action(ac)
inc depths
#
str envname2="env2"
setcall envsprite swf_sprite_new()
setcall image swf_image(env2_path)
call swf_sprite_placeobject(envsprite,image,depths)
call swf_sprite_showframe(envsprite)
setcall env swf_sprite_done(envsprite)
call swf_exports_add(env,envname2)
call sprintf(ac,"this.attachMovie('env2','_%s',%u)",envname2,depths)
call action(ac)
inc depths
#
endif
#env end
#ball begin
if (ball)==1
str ballname="ball"
sd ballsprite
setcall ballsprite swf_sprite_new()
setcall image swf_image(ballpath)
call swf_sprite_placeobject(ballsprite,image,1)
#
chars ac1#100
#chars ac2#100
str f1="_y=_y+%u"
#str f2="_y=_y+%u"
str mathstr^ac1
#str mathstr2^ac2
call sprintf(#ac1,f1,(step))
#call sprintf(#ac2,f2,(step))
set cursor minfall
while cursor<maxfall
    call action_sprite(ballsprite,mathstr)
	#call action_sprite(ballsprite,"if(_y==(400-60-30-10))_root.bat_hit()")
	#call action_sprite(ballsprite,"if(_y==(400-60+30-10))_root.bat_hit()")
    call swf_sprite_showframe(ballsprite)
	add cursor (step)
endwhile
call action_sprite(ballsprite,"stop();_root.start_msg();_root.onMouseDown=_root.newfall")
call swf_sprite_showframe(ballsprite)

#while cursor<medfall
#    set valuefall cursor;    sub valuefall minfall;    mult valuefall dif;    div valuefall intfall;    sub valuefall dif;    mult valuefall -1
#    sd n=0;    dec valuefall
#    while n<valuefall
#        call swf_sprite_showframe(ballsprite);        inc n
#    endwhile
#    call action_sprite(ballsprite,mathstr)
#    call swf_sprite_showframe(ballsprite)
#    add cursor (step)
#endwhile

#sd last
#set last maxfall
#sub last (step)
#while cursor<maxfall
#    if cursor>=last
#        call action_sprite(ballsprite,"stop();_root.start_msg();_root.onMouseDown=_root.newfall")
#    endif
#    call action_sprite(ballsprite,mathstr2)
#    call swf_sprite_showframe(ballsprite)
#    add cursor (step)
#endwhile

sd ball
setcall ball swf_sprite_done(ballsprite)
call swf_exports_add(ball,ballname)
call sprintf(ac,"attachMovie('ball','_%s',%u)",ballname,depths)
call action(ac)
inc depths
endif
#ball end
#bat begin
if (bat)==1
sd spritefreeid
setcall spritefreeid swf_sprite_new()
#
str bat="bat"
chars b_path#300
str bat_path^b_path
sd i=0
while i!=10
    call sprintf(bat_path,bat_format,i)
    setcall image swf_image(bat_path)
    if i>0
        call swf_sprite_removeobject(spritefreeid,1)
    endif
    call swf_sprite_placeobject(spritefreeid,image,1)
    if i==0
        call action_sprite(spritefreeid,"stop()")
    elseif i==8
        call action_sprite(spritefreeid,"_root.bat_hit()")
    elseif i==9
        call action_sprite(spritefreeid,"stop()")
    endelseif
    call swf_sprite_showframe(spritefreeid)
    inc i
endwhile
#
sd sprite
setcall sprite swf_sprite_done(spritefreeid)
call swf_exports_add(sprite,bat)
call sprintf(ac,"attachMovie('bat','_%s',%u)",bat,depths)
call action(ac)
inc depths
endif
#bat end
setcall depths default_text(depths,(0xff0011FF),deffont,0)
#actions begin
if (act)==1
#hitul normal e la -10
str go_format="
function bat_hit()
{
    var a=_ball._y-_bat._y;
	var is_h=true;
	//off e jumate de ball
	var margin=30;var off=10;
	var v1=a+margin+off;if(v1<0){is_h=false;a=v1;}
	else{var v2=a-margin+off;if(v2>0){is_h=false;a=v2;}}
    if(is_h==true)
    {
        onMouseDown=undefined;
        _ball.stop();
        var b=start_fly(v1);
        _text.message='Angle: '+b;
        _miss.fail='';
        _env1.play();
        return undefined;
    }
    #_miss.fail=a+(era la sprintf);
	_miss.fail=a;
}"
#sd plus
#set plus ballheight
#div plus 2
#call sprintf(ac,go_format,plus)
call action(go_format)
str format="
    var score;
    function game_at_load(){
        _ball.stop();
        newgame_instance();
    }
    function start_msg(){_text.message='Click to start';}
    function newgame_instance(){
        score=0;
        _env1._x=0;
        _env2._x=%u;
        _bat._x=%u;
        _bat._y=%u;
        //
        _ball._x=%u;
        _ball._y=%u;
        start_msg();
        onMouseDown=atready;
    }
    function newgame()
    {
        _ball.gotoAndStop(1);
        _bat.gotoAndStop(1);
        newgame_instance();
    }
    function atready()
    {
        _text.message='';
        _miss.fail='';
        _ball.play();
        onMouseDown=movebat;
    }
    function movebat()
    {
        _bat.play();
    }
    function newfall()
    {
        _ball._y=%u;
        _bat.gotoAndStop(1);
        atready();
    }
    _env1.stop();
    game_at_load();
"
call sprintf(ac,format,envwidth,bat_x,bat_y,bat_x,(balltop),(balltop))
call action(ac)
endif
#actions end
#miss begin
sd presprite
str text_fail="miss"
setcall presprite swf_sprite_new()
#
sd ed_str
sd txt
setcall ed_str edittext_struct()
call edittext_font(deffont,40)
call edittext_rgba(0xff0011FF)
setcall txt swf_text((font_rect_width),(font_rect_height),"fail",(HasFont|HasTextColor|ReadOnly|NoSelect),ed_str)
call swf_sprite_placeobject_coords(presprite,txt,1,(width-100),(height-font_rect_height))
call swf_sprite_showframe(presprite)
#
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,text_fail)
call sprintf(ac,"this.attachMovie('miss','_%s',%u)",text_fail,depths)
call action(ac)
inc depths
#miss end

if (submit)==1
call submit_form(deffont,(width),(height),game_name)
endif

#include "../files/base.s"

include "score.s"

call swf_exports_done()
call swf_showframe()
call swf_done()

#call printf("\ndone")

return 1
