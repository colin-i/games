
format elfobj64microsoft

warning off
include "../actionswf/dev/import.h"
warning on
include "../actionswf/include/prog.h"

include "../files/tool.s"
include "../files/submit.s"

const texter_height=40
const nr_of_horses=12
const x_pos=10
const fps=12
const play_width=800

str game_name="Racecourse"

function add_horse(sd i)
    sd presprite
    sd sprite
    sd img

    chars n#300
    str name^n
    call sprintf(name,"horse%u",i)

    chars d#300
    str data^d

sd phases
#
setcall phases swf_sprite_new()
call sprintf(data,"D:/universe/local/racecourse/%u/stay.dbl",i)
setcall img swf_image(data)
call swf_sprite_placeobject(phases,img,1)
call swf_sprite_showframe(phases)
setcall sprite swf_sprite_done(phases)
chars stay_#20
str stay^stay_
call sprintf(stay,"stay%u",i)
call swf_exports_add(sprite,stay)
#
setcall phases swf_sprite_new()
call sprintf(data,"D:/universe/local/racecourse/%u/run1.dbl",i)
setcall img swf_image(data)
call swf_sprite_placeobject(phases,img,1)
call swf_sprite_showframe(phases)
setcall sprite swf_sprite_done(phases)
chars run1_#20
str run1^run1_
call sprintf(run1,"run%u_1",i)
call swf_exports_add(sprite,run1)
#
setcall phases swf_sprite_new()
call sprintf(data,"D:/universe/local/racecourse/%u/run2.dbl",i)
setcall img swf_image(data)
call swf_sprite_placeobject(phases,img,1)
call swf_sprite_showframe(phases)
setcall sprite swf_sprite_done(phases)
chars run2_#20
str run2^run2_
call sprintf(run2,"run%u_2",i)
call swf_exports_add(sprite,run2)

    setcall presprite swf_sprite_new()
    #1#init
    call actionf_sprite(presprite,data,"var stay='%s';attachMovie(stay,stay,1)",stay)
    call actionf_sprite(presprite,data,"var run1='%s';attachMovie(run1,run1,getNextHighestDepth())",run1)
    call actionf_sprite(presprite,data,"var run2='%s';attachMovie(run2,run2,getNextHighestDepth())",run2)
    call action_sprite(presprite,"
        this[run1]._visible=false;this[run2]._visible=false;var ended=0;
        var start_speed=10;var part_run_size=_root.part_run_size;var all_run=_root.all_run")
    call actionf_sprite(presprite,data,"var picture_change=%u;var horse_nr=%u",(fps/4),i)
    call swf_sprite_showframe(presprite)
    #2#staying
    call action_sprite(presprite,"var min_speed=start_speed;var current_run=0;var move_env=0;
        var part_start_bool=true;var subpart_start_bool=true;stop()")
    call swf_sprite_showframe(presprite)
    #3#
        #part start
    call action_sprite(presprite,"
        if(part_start_bool==true){
            var step_size=min_speed+random(10);var part_run=part_run_size;
            part_start_bool=false;}")
        #subpart start
    call action_sprite(presprite,"
        if(subpart_start_bool==true){
            var picture_ind=picture_change+random(2);
            if(this[run1]._visible==false){this[run1]._visible=true;this[run2]._visible=false;}
            else{this[run1]._visible=false;this[run2]._visible=true;}
            subpart_start_bool=false;
        }")
        #run
    call action_sprite(presprite,"_x=_x+step_size;part_run=part_run-step_size;picture_ind--")
    call action_sprite(presprite,"
        if(move_env==1){_root.advance_all(step_size)}")
    call action_sprite(presprite,"
        current_run=current_run+step_size;
        if(current_run>=all_run)
        {
            if(ended==0)
            {
                ended=1;
                _root.position++;
                min_speed=2;
                if(move_env==1){
                    move_env=0;
                    _root.race_end();
                }
            }
        }
        else
        {
            if(_root.horse_number==horse_nr){
                var sc=_root.score;var f=_root.fps;var s=_root.score/f;s=int(s);var d=s*f;d=sc-d;
                d=d*100/f;d=int(d);if(d<10)d='0'+d;
                _root.score=sc+1;
                _root._text.message=s+':'+d;
            }
        }")
    call swf_sprite_showframe(presprite)
    #4#run
    call action_sprite(presprite,"
        if(part_run>0)
        {
            if(picture_ind<1)subpart_start_bool=true;
            gotoAndPlay(3);
        }
        else
        {
            if(min_speed==start_speed){
                min_speed=min_speed+10;
                if(_root.horse_number==horse_nr)move_env=1;
            }
            if(min_speed!=1){part_start_bool=true;gotoAndPlay(3);if(min_speed==2)min_speed=1;}
            else
            {
                this[stay]._visible=true;this[run1]._visible=false;this[run2]._visible=false;
                gotoAndStop(2);
            }
        }")
    call swf_sprite_showframe(presprite)

    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,name)
endfunction

entry racecourse()

sd presprite
sd sprite
sd img

str env_path="D:/universe/local/racecourse/env.dbl"

chars a_code#16384
str ac^a_code

sd env_w
sd env_h
setcall env_w swf_dbl_width(env_path)
setcall env_h swf_dbl_height(env_path)
sd horse_w
sd horse_h
setcall horse_w swf_dbl_height("D:/universe/local/racecourse/0/stay.dbl")
setcall horse_h swf_dbl_height("D:/universe/local/racecourse/0/stay.dbl")

sd texter_y
set texter_y env_h
sub texter_y (texter_height)

sd stable_h
set stable_h (nr_of_horses)
mult stable_h horse_h
sd stable_y
set stable_y texter_y
sub stable_y stable_h

#

call sprintf(ac,"%s.swf",game_name)
call swf_new(ac,(play_width),env_h,0x112233,(fps))

#env|
setcall presprite swf_sprite_new()
setcall img swf_image(env_path)
call swf_sprite_placeobject(presprite,img,0)
#
str spectators_path="D:/universe/local/racecourse/spectators.dbl"
sd spectators_h
setcall spectators_h swf_dbl_height(spectators_path)
sd spectators_y
set spectators_y stable_y
sub spectators_y 20
sub spectators_y spectators_h
sd spectators_img
setcall spectators_img swf_image(spectators_path)
call swf_sprite_placeobject_coords(presprite,spectators_img,1,0,spectators_y)
#
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"env")
call action("attachMovie('env','env1',0)")
#
call action("attachMovie('env','env2',1)")
#|env

sd deffont
setcall deffont swf_font_basic("_sans")
call default_text_withHeight(2,(0xff0011FF),deffont,texter_y,(texter_height))

setcall presprite swf_sprite_new()
sd sec_text
setcall sec_text dyn_text((play_width),"text",deffont,(texter_height-10))
call swf_sprite_placeobject(presprite,sec_text,0)
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"sec_text")
call action("attachMovie('sec_text','sec_text',3)")

#horses|
sd i=0
while i<(nr_of_horses)
    call add_horse(i)
    inc i
endwhile
#|horses

sd dbl

#stable|
sd stable_depths=0
str stablePart_path="D:/universe/local/racecourse/stable.dbl"
setcall presprite swf_sprite_new()
data stablePart_width#1
data stablePart_height#1
setcall dbl swf_dbl_ex(stablePart_path,#stablePart_width)
#
setcall img swf_shape_bitmap(dbl,stablePart_width,stablePart_height)
set i 0
sd y_pos
set y_pos horse_h
while i<(nr_of_horses)
    sd y_stable
    set y_stable y_pos
    sub y_stable stablePart_height

    call swf_sprite_placeobject_coords(presprite,img,stable_depths,0,y_stable)
    inc stable_depths
    add y_pos horse_h
    inc i
endwhile
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"stable")
call actionf(ac,"var stable_x=%u;var stable_y=%u",(x_pos),stable_y)
    #keep stable part for the start
setcall presprite swf_sprite_new()
#
const line_dif=5
sd h_dif
set h_dif horse_h
sub h_dif (line_dif)
data door_line=no_fill
data *line_width=2
data *color=0xcc7711FF
#
data *={0,StateLineStyle|StateMoveTo}
    data *=0
    data *=line_dif
data *={1,1}
data *=0;data down#1
    set down h_dif
data *=-1
sd closed_door
setcall closed_door swf_shape(2,horse_h,#door_line)
#
call swf_sprite_placeobject(presprite,closed_door,0)
call action_sprite(presprite,"stop()")
call swf_sprite_showframe(presprite)
call swf_sprite_removeobject(presprite,0)
sd stablePart_y
set stablePart_y horse_h
sub stablePart_y stablePart_height
call swf_sprite_placeobject_coords(presprite,img,0,0,stablePart_y)
call action_sprite(presprite,"stop()")
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"stable_part")
#|stable

#end line|
#
data line=no_fill
data l_w=2
data *color=0xffFFffFF
#
data *={0,StateLineStyle}
data *={1,1}
data *=0;data sud#1
    set sud stable_h
data *=-1
sd race_line
setcall race_line swf_shape(2,stable_h,#line)
sd l_x
set l_x l_w
add l_x stablePart_width
#
setcall presprite swf_sprite_new()
call swf_sprite_placeobject(presprite,race_line,0)
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"finish")
call action("attachMovie('finish','finish',getNextHighestDepth())")
#|end line

call submit_form(deffont,(play_width),env_h,game_name)

call actionf(ac,"var horse_w=%u;var horse_h=%u;var horse_total=%u;var fps=%u",horse_w,horse_h,(nr_of_horses),(fps))
call action("
var horse_number;var score;var position;
var part_run_size=400;var all_run=part_run_size*8;finish._y=stable_y;
function newgame()
{
    var i=0;while(i<horse_total){
        _root['horse'+i].removeMovieClip();
        i++;
    }
    stable.removeMovieClip();
    game_start();

    sec_text.text='';
}
function game_start()
{
    env1._x=0;env2._x=env1._width;

    var y=stable_y;
    var i=0;while(i<horse_total){
        var name='horse'+i;
        attachMovie(name,name,getNextHighestDepth());
        var horse=_root[name];
        horse._x=stable_x;
        horse._y=y;y=y+horse_h;
        i++;
    }

    attachMovie('stable','stable',getNextHighestDepth());
    stable._x=stable_x;stable._y=stable_y;
    y=0;i=0;while(i<horse_total){
        var st='stable_part'+i;stable.attachMovie('stable_part',st,stable.getNextHighestDepth());
        stable[st]._x=horse_w;stable[st]._y=y;y=y+horse_h;
        i++;
    }

    finish._x=stable_x+all_run;

    score=0;position=0;
    _text.message='Select a horse';
    stable.onMouseMove=horse_see_move;
    stable.onPress=horse_sel;
}
function horse_see_in()
{
    stable.createEmptyMovieClip('horse_square',stable.getNextHighestDepth());
    var sq=stable.horse_square;
    sq.beginFill(0x0101FF,20);
    sq.lineTo(horse_w,0);
    sq.lineTo(horse_w,horse_h);
    sq.lineTo(0,horse_h);
    sq.lineTo(0,0);
    sq.endFill();
}
function horse_see_out()
{
    stable.horse_square.removeMovieClip();
}
function horse_see_move()
{
    var y=_ymouse-stable._y;y=y/horse_h;
    var is_in=0;
    if(y>=0)if(y<horse_total){
        var max_x=stable._x+stable._width;
        if(_xmouse>=stable._x)if(_xmouse<max_x)is_in=1;
    }
    if(stable.horse_square==undefined){if(is_in==1)horse_see_in();}
    else if(is_in==0){horse_see_out();return undefined;}
    y=int(y)*horse_h;stable.horse_square._y=y;
}
function horse_sel()
{
    horse_number=stable.horse_square._y/horse_h;
    horse_see_out();
    stable.onMouseMove=function(){}
    stable.onPress=function(){}

    var horse='horse'+horse_number;
    _root[horse].createEmptyMovieClip('horse_view_easy',0);
    var easy=_root[horse].horse_view_easy;
    easy.lineStyle(3,0x0101FF,100,0,'none','round','miter',1);
    easy.moveTo(-10,0);
    var max_line=horse_w+10;
    easy.lineTo(max_line,0);
    easy._y=horse_h-3;

    var i=0;while(i<horse_total){
        stable['stable_part'+i].play();
        var horse='horse'+i;
        _root[horse]['stay'+i]._visible=false;
        _root[horse]['run'+i+'_1']._visible=true;
        _root[horse].play();i++;
    }
}
function race_end()
{
    var horse='horse'+horse_number;
    var easy=_root[horse].horse_view_easy;
    easy.removeMovieClip();
    sec_text.text='Position '+_root.position;
    score=score+_root.position;
    submitform(score);
}
function advance_all(step_size)
{
    env1._x=env1._x-step_size;
    env2._x=env2._x-step_size;
    if(env1._x<0)if(env2._x<0)
    {
        if(env1._x<env2._x)env1._x=env2._x+env2._width;
        else env2._x=env1._x+env1._width;
    }
    stable._x=stable._x-step_size;
    var i=0;while(i<horse_total){
        var horse=_root['horse'+i];
        horse._x=horse._x-step_size;i++;
    }
    finish._x=finish._x-step_size;
}
game_start()
")

call swf_exports_done()
call swf_showframe()
call swf_done()
return 1
