

format elfobj64microsoft

warning off
include "../actionswf/dev/import.h"
warning on
include "../actionswf/include/prog.h"

include "../files/tool.s"
include "../files/submit.s"

const nr_of_balls=9
const all_nr_of_balls=29
const balls_on_row=3
const texter_height=40
const fps=24
const pozitie=1
chars data_d#1000
str data^data_d

str game_name="Lotto"

entry lotto()

sd sprite
sd presprite
sd img
#sd dbl
#sd bitmap

str env_path="D:/universe/local/lotto/env.dbl"
str slot_path="D:/universe/local/lotto/shape.dbl"

chars a_code#16384
str ac^a_code

sd env_w
sd env_h
setcall env_w swf_dbl_width(env_path)
setcall env_h swf_dbl_height(env_path)
sd ball_w
sd ball_h
setcall ball_w swf_dbl_width(slot_path)
setcall ball_h swf_dbl_height(slot_path)

call sprintf(ac,"%s.swf",game_name)
call swf_new(ac,env_w,env_h,0x112233,(fps))

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
call default_text_withHeight(1,(0x0101FFff),deffont,texter_y,(texter_height))

#machine>
setcall presprite swf_sprite_new()
const machine_r=150
sd machine_w=2*machine_r
#sd machine_h=2*machine_r
call actionf(ac,"machine_r=%u",(machine_r))
sd machine_x
set machine_x env_w
sub machine_x machine_w
div machine_x 2
#
call action("
function drawCircle(mc,r){
    mc.moveTo(r+r,r);
    mc.curveTo(r+r,Math.tan(Math.PI/8)*r+r,Math.sin(Math.PI/4)*r+r,Math.sin(Math.PI/4)*r+r);
    mc.curveTo(Math.tan(Math.PI/8)*r+r,r+r,r,r+r);
    mc.curveTo(0-Math.tan(Math.PI/8)*r+r,r+r,0-Math.sin(Math.PI/4)*r+r,Math.sin(Math.PI/4)*r+r);
    mc.curveTo(0,Math.tan(Math.PI/8)*r+r,0,r);
    mc.curveTo(0,0-Math.tan(Math.PI/8)*r+r,0-Math.sin(Math.PI/4)*r+r,0-Math.sin(Math.PI/4)*r+r);
    mc.curveTo(0-Math.tan(Math.PI/8)*r+r,0,r,0);
    mc.curveTo(Math.tan(Math.PI/8)*r+r,0,Math.sin(Math.PI/4)*r+r,0-Math.sin(Math.PI/4)*r+r);
    mc.curveTo(r+r,0-Math.tan(Math.PI/8)*r+r,r+r,r);
}
createEmptyMovieClip('machine',getNextHighestDepth());
machine.beginFill(0x010101,10);
machine.lineStyle(0,0x000000);
drawCircle(machine,machine_r);
machine.endFill();
")
call actionf(ac,"
    machine._x=%u;
    machine._y=40",machine_x)
#<machine
#machine base>
setcall presprite swf_sprite_new()
setcall img swf_image("D:/universe/local/lotto/machine_base.dbl")
call swf_sprite_placeobject(presprite,img,0)
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"machine_base")
call action("attachMovie('machine_base','machine_base',getNextHighestDepth());
    machine_base._x=machine._x;
    machine_base._y=machine._y+machine._height")
#<machine base

#timer>
setcall presprite swf_sprite_new()
    #1
call action_sprite(presprite,"var frames_left;stop()")
call swf_sprite_showframe(presprite)
    #2
call action_sprite(presprite,"
    _root.update_ball_line(frames_left);
    if(frames_left==0)
    {
        if(_root.time_out()==false)stop();
    }
    else frames_left--")
call swf_sprite_showframe(presprite)
    #3
call action_sprite(presprite,"gotoAndPlay(2)")
call swf_sprite_showframe(presprite)
    #
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"timer")
call action("attachMovie('timer','timer',getNextHighestDepth())")
#<timer

#slots>
setcall presprite swf_sprite_new()
setcall img swf_image(slot_path)
sd slot_depths=0
sd slots_x
sd slots_all_w
set slots_all_w ball_w
mult slots_all_w (nr_of_balls)
sd all_free
set all_free env_w
sub all_free slots_all_w
sd slots_free_w
set slots_free_w all_free
div slots_free_w (nr_of_balls+1)
set slots_x slots_free_w
sd i=0
while i<(nr_of_balls)
    call swf_sprite_placeobject_coords(presprite,img,slot_depths,slots_x,0)
    inc slot_depths
    add slots_x ball_w
    add slots_x slots_free_w
    inc i
endwhile
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"slots")
sd slots_y
set slots_y texter_y
sub slots_y 1
sub slots_y ball_h
call actionf(ac,"attachMovie('slots','slots',getNextHighestDepth());
    slots._y=%u",slots_y)
call actionf(ac,"
    var slots_free_w=%u",slots_free_w)
#<slots

#balls>
call action("var ball_x_ar=new Array();var ball_y_ar=new Array()")
sd all_rows
set all_rows (all_nr_of_balls+balls_on_row-1)
div all_rows (balls_on_row)
sd half_rows=1
add half_rows all_rows
div half_rows 2
sd row
sd col
const marg_off=10
const inter_off=10
sd right_off
set right_off env_w
sub right_off (marg_off)
sub right_off ball_w
sub right_off (inter_off)
sub right_off ball_w
sub right_off (inter_off)
sub right_off ball_w
sd last_row
set last_row all_rows
dec last_row
set i 0
while i<(all_nr_of_balls)
    set row i
    div row (balls_on_row)
    setcall col rest(i,(balls_on_row))
    sd x=marg_off
    sd y
    set y row
    if row>=half_rows
        set x right_off
        sub y half_rows
    endif
    #
    sd current_col
    set current_col col
    #
    while col>0
        add x ball_w
        add x (inter_off)
        dec col
    endwhile
    sd add
    set add (inter_off)
    mult add y
    mult y ball_h
    add y (marg_off)
    add y add
    #
    if row==last_row
        if current_col==0
            sd all_off_size
            set all_off_size env_w
            sub all_off_size 10
            sub all_off_size x
            sd inter_off_w
            set inter_off_w all_off_size
            sub inter_off_w ball_w
            div inter_off_w (nr_of_balls-1)
            call actionf(ac,"var off_balls_x=%u;var off_balls_y=%u;var off_inter_w=%u",x,y,inter_off_w)
        endif
    endif
    #
    sd pos=pozitie
    add pos i
    setcall presprite swf_sprite_new()
    call sprintf(ac,"D:/universe/local/lotto/balls/%u.dbl",pos)
    setcall img swf_image(ac)
    call swf_sprite_placeobject(presprite,img,0)
    #1
    call actionf_sprite(presprite,data,"var nr=%u",pos)
    call action_sprite(presprite,"if(_parent!=_root.machine_area)stop()")
    call swf_sprite_showframe(presprite)
    #2
    call action_sprite(presprite,"var step=4;var on_x=0;var on_y=step;var urn=1")
    call action_sprite(presprite,"var shut_down=0;var start_x=_x;var start_y=_y;var frames_left=-1;")
#call action_sprite(presprite,"if(nr!=1)stop()")
    call swf_sprite_showframe(presprite)
    #3
    call action_sprite(presprite,"
        if(shut_down==0)
        {
            _root.urnPart_update(this);
            if(_root.margin_test(_x+on_x,_y+on_y)==false)_root.new_sense(this);
        }
        else
        {
            if(frames_left==-1)
            {
                //calculate to move the balls to the bottom
                var dif_to_end_x;var x_sense;
                if(_x>start_x){dif_to_end_x=_x-start_x;x_sense=-1;}
                else{dif_to_end_x=start_x-_x;x_sense=1;}
                var dif_to_end_y;var y_sense;
                if(_y>start_y){dif_to_end_y=_y-start_y;y_sense=-1;}
                else{dif_to_end_y=start_y-_y;y_sense=1;}

                if(dif_to_end_x>dif_to_end_y)frames_left=int(dif_to_end_x/step);
                else frames_left=int(dif_to_end_y/step);
                if(frames_left!=0)
                {
                    on_x=int(dif_to_end_x/frames_left)*x_sense;
                    on_y=int(dif_to_end_y/frames_left)*y_sense;
                }
            }
            if(frames_left==0)
            {
                //this will be the last frame
                stop();
                on_x=start_x-_x;
                on_y=start_y-_y;
            }
            frames_left--;
        }
        _x=_x+on_x;_y=_y+on_y;
		")
    call swf_sprite_showframe(presprite)
    #4
    call action_sprite(presprite,"gotoAndPlay(3)")
    call swf_sprite_showframe(presprite)

#include "debugging.s"

    #
    setcall sprite swf_sprite_done(presprite)
    call sprintf(data,"ball%u",pos)
    call swf_exports_add(sprite,data)
    call actionf(ac,"ball_x_ar.push(%u);ball_y_ar.push(%u)",x,y)
    #
    inc i
endwhile
#<balls

call actionf(ac,"var balls_total=%u;var sel_balls_total=%u;var ball_lat=%u",(all_nr_of_balls),(nr_of_balls),ball_w)
call actionf(ac,"fps=%u",(fps))
call action("
var selection=new Array();
var new_lat;
var urn_balls=new Array();
//
var all_balls_max_speed;
var normal_timeout=3*fps;
//
var balls_left;
var off_balls_index;
var score;
function game_start()
{
    _text.message='Select the numbers';
    var i=0;while(i<balls_total)
    {
        var pos=i+1;var ball='ball'+pos;
        attachMovie(ball,ball,getNextHighestDepth());var mv=_root[ball];
        mv.onPress=ball_sel;
        mv._x=ball_x_ar[i];mv._y=ball_y_ar[i];
        i++;
    }
    selection.length=0;
    score=0;
    all_balls_max_speed=0;
    createEmptyMovieClip('top_slots',getNextHighestDepth());top_slots._y=slots._y;
}
function ball_sel()
{
    var nr=this.nr;var sel_pos=selection.length;
    var ball='ball'+nr;
    top_slots.attachMovie(ball,ball,top_slots.getNextHighestDepth());
    var mv=top_slots[ball];
    this.removeMovieClip();
    selection.push(mv);
    var x=sel_pos*mv._width;mv._x=sel_pos+1*slots_free_w+x;
    mv._alpha=20;
    if(selection.length==sel_balls_total)launch();
}
function launch()
{
    _text.message='';
    createEmptyMovieClip('machine_area',getNextHighestDepth());machine_area._x=machine._x;machine_area._y=machine._y;

    createEmptyMovieClip('machine_base_area',getNextHighestDepth());machine_base_area._x=machine_base._x;machine_base_area._y=machine_base._y;
    machine_base_area.lineStyle(0,0xff0101);machine_base_area.moveTo(2,machine_base._height-2)

    createEmptyMovieClip('off_balls',getNextHighestDepth());off_balls._x=off_balls_x;off_balls._y=off_balls_y;
    off_balls_index=0;

    var base_h=machine_base._height-4;var half_base_h=base_h/2;var one_base_w=machine_base._width-4/sel_balls_total;
    var base_x=2;
    var i=0;while(i<sel_balls_total){
        var info='info_'+i;
        machine_base_area.createEmptyMovieClip(info,machine_base_area.getNextHighestDepth());
        var info_mv=machine_base_area[info];
        info_mv.lineStyle(0,0x00ffFF);
        info_mv.moveTo(0,half_base_h);
        info_mv.lineTo(one_base_w,half_base_h);
        info_mv.moveTo(one_base_w,0);
        info_mv.lineTo(one_base_w,base_h);
        info_mv._x=base_x;info_mv._y=2;
        i++;base_x=base_x+one_base_w;
    }
    balls_left=sel_balls_total;

    i=1;while(i<=balls_total)
    {
        var ball='ball'+i;if(_root[ball]!=undefined)_root[ball].removeMovieClip();
        machine_area.attachMovie(ball,ball,machine_area.getNextHighestDepth());
        i++;
    }

    urnPart[0]=0;urnPart[1]=balls_total;urnPart[2]=0;urnPart[3]=0;
    urn_balls.length=0;

    new_lat=30;var lat_scale=new_lat*100/ball_lat;
    var middle=machine._width/2;var x_step=16;
    var on_x;var starting_y=machine._height-new_lat-8;var on_row=0;var n;
    i=1;while(i<=balls_total)
    {
        on_row++;
        var x_width=on_row-1*x_step+new_lat;var half_x_width=x_width/2;
        on_x=middle-half_x_width;
        n=i;var max_i=n+on_row;if(max_i>balls_total)max_i=balls_total+1;
        while(n<=max_i){
            var bl_mv=machine_area['ball'+i];
            urn_balls.push(bl_mv);
            bl_mv._xscale=lat_scale;bl_mv._yscale=lat_scale;
            bl_mv._x=on_x;bl_mv._y=starting_y;
            n++;i++;on_x=on_x+x_step;
        }
        starting_y=starting_y-2;
    }
	onEnterFrame=pack_and_swap;
}
//bool
function margin_test(left,top)
{
    left=left+machine._x;top=top+machine._y;
    if(machine.hitTest(left,top,true)==false)return false;
    else if(machine.hitTest(left,top+new_lat,true)==false)return false;
    else if(machine.hitTest(left+new_lat,top,true)==false)return false;
    else if(machine.hitTest(left+new_lat,top+new_lat,true)==false)return false;
    return true;
}
var urnPart=new Array();urnPart.length=4;
//min
function find_new_direction(index1,index2)
{
    var pos1=urnPart[index1];var pos2=urnPart[index2];
    if(pos1<pos2)return index1;
    else if(pos1>pos2)return index2;
    var rand=random(2);
    if(rand==0)return index1;
    return index2;
}
function new_sense(ball)
{
    //get sub-part of current part
    var half_new_lat=new_lat/2;var middle_x=ball._x+half_new_lat;var middle_y=ball._y+half_new_lat;
    var half_machine_w=machine._width/2;var half_machine_h=machine._height/2;

    var center_x=middle_x-half_machine_w;
    var center_y=middle_y-half_machine_h;
    var modulo_x=center_x;
    if(modulo_x<0)modulo_x=modulo_x*-1;
    var modulo_y=center_y;
    if(modulo_y<0)modulo_y=modulo_y*-1;

    var sub_part=0;
    if(modulo_x>modulo_y)sub_part=1;

    //get next part
    var current_urnPart=ball.urn;
    var viewParts=new Array(0,1,2,3);
    viewParts.splice(current_urnPart,1);
    var next_sense=find_new_direction(viewParts[0],viewParts[1]);
    next_sense=find_new_direction(next_sense,viewParts[2]);

    //increase step
    var max_step=10;
    if(ball.step<max_step)
    {
        ball.step=ball.step+2;
		//de la 4 pana la 10
        if(ball.step==max_step)time_out_start_verify();
    }

    //set new steps
    new_sense_set(ball,ball.urn,sub_part,next_sense);
}
function new_sense_set(ball,current_urnPart,sub_part,next_sense)
{
    var step=ball.step;var half_step=step/2;var half_step_random=random(half_step);
    if(current_urnPart==0)
    {
        if(sub_part==0)
        {
            if(next_sense==2){ball.on_x=step;ball.on_y=half_step_random;}
            else if(next_sense==3){ball.on_x=half_step+half_step_random;ball.on_y=step;}
            else{ball.on_x=0-half_step_random;ball.on_y=step;}
        }
        else
        {
            if(next_sense==2){ball.on_x=step;ball.on_y=0-half_step_random;}
            else if(next_sense==3){ball.on_x=step;ball.on_y=half_step+half_step_random;}
            else{ball.on_x=half_step_random;ball.on_y=step;}
        }
    }
    else if(current_urnPart==1)
    {
        if(sub_part==0)
        {
            if(next_sense==3){ball.on_x=step;ball.on_y=0-half_step_random;}
            else if(next_sense==2){ball.on_x=half_step+half_step_random;ball.on_y=0-step;}
            else{ball.on_x=0-half_step_random;ball.on_y=0-step;}
        }
        else
        {
            if(next_sense==3){ball.on_x=step;ball.on_y=half_step_random;}
            else if(next_sense==2){ball.on_x=step;ball.on_y=0-half_step-half_step_random;}
            else{ball.on_x=half_step_random;ball.on_y=0-step;}
        }
    }
    else if(current_urnPart==2)
    {
        if(sub_part==0)
        {
            if(next_sense==0){ball.on_x=0-step;ball.on_y=half_step_random;}
            else if(next_sense==1){ball.on_x=0-half_step-half_step_random;ball.on_y=step;}
            else{ball.on_x=half_step_random;ball.on_y=step;}
        }
        else
        {
            if(next_sense==0){ball.on_x=0-step;ball.on_y=0-half_step_random;}
            else if(next_sense==1){ball.on_x=0-step;ball.on_y=half_step+half_step_random;}
            else{ball.on_x=0-half_step_random;ball.on_y=step;}
        }
    }
    else
    {
        if(sub_part==0)
        {
            if(next_sense==1){ball.on_x=0-step;ball.on_y=0-half_step_random;}
            else if(next_sense==0){ball.on_x=0-half_step-half_step_random;ball.on_y=0-step;}
            else{ball.on_x=half_step_random;ball.on_y=0-step;}
        }
        else
        {
            if(next_sense==1){ball.on_x=0-step;ball.on_y=half_step_random;}
            else if(next_sense==0){ball.on_x=0-step;ball.on_y=0-half_step-half_step_random;}
            else{ball.on_x=0-half_step_random;ball.on_y=0-step;}
        }
    }
}
function urnPart_update(ball)
{
    var half_new_lat=new_lat/2;var middle_x=ball._x+half_new_lat;var middle_y=ball._y+half_new_lat;
    var half_machine_w=machine._width/2;var half_machine_h=machine._height/2;
    var current_urn=ball.urn;
    if(ball.urn==0)
    {
        if(middle_x>half_machine_w)
            {if(middle_y>half_machine_h)current_urn=3;else current_urn=2;}
        else if(middle_y>half_machine_h)
            {if(middle_x>half_machine_w)current_urn=3;else current_urn=1;}
    }
    else if(ball.urn==1)
    {
        if(middle_x>half_machine_w)
            {if(middle_y>half_machine_h)current_urn=3;else current_urn=2;}
        else if(middle_y<half_machine_h)
            {if(middle_x>half_machine_w)current_urn=2;else current_urn=0;}
    }
    else if(ball.urn==2)
    {
        if(middle_x<half_machine_w)
            {if(middle_y>half_machine_h)current_urn=1;else current_urn=0;}
        else if(middle_y>half_machine_h)
            {if(middle_x>half_machine_w)current_urn=3;else current_urn=0;}
    }
    else
    {
        if(middle_x<half_machine_w)
            {if(middle_y>half_machine_h)current_urn=1;else current_urn=0;}
        else if(middle_y<half_machine_h)
            {if(middle_x>half_machine_w)current_urn=2;else current_urn=0;}
    }
    if(ball.urn!=current_urn)
    {
        urnPart[ball.urn]--;
        urnPart[current_urn]++;
        ball.urn=current_urn;
    }
}
function time_out_start_verify()
{
    all_balls_max_speed++;
    var visual_x=machine_base._width-4*all_balls_max_speed/balls_total;
    machine_base_area.lineTo(visual_x,machine_base._height-2);
    if(all_balls_max_speed<balls_total)return undefined;
    machine_base_area.moveTo(2,2);
    timer.frames_left=normal_timeout;
    timer.play();
}
//bool
function time_out()
{
    verify_balls();

    balls_left--;
    var mv='info_'+balls_left;
    machine_base_area[mv].removeMovieClip();
    if(balls_left==0)
    {
		//opreste swap dep..urile
		onEnterFrame=undefined;
        var i=0;while(i<urn_balls.length){
            urn_balls[i].shut_down=1;
            i++;
        }
        submitform(score);
        return false;
    }
    timer.frames_left=normal_timeout;
    return true;
}
function verify_balls()
{
    var rand_pos=random(urn_balls.length);
    var rand_ball=urn_balls[rand_pos];
    var nr=rand_ball.nr;
    rand_ball.removeMovieClip();
    urn_balls.splice(rand_pos,1);

    var ball_name='ball'+nr;
    var i=0;while(i<selection.length){
        if(selection[i].nr==nr){
            score++;
            var new_name=ball_name+'_on_top';
            top_slots.attachMovie(ball_name,new_name,top_slots.getNextHighestDepth());
            top_slots[new_name]._x=selection[i]._x;
            selection.splice(i,1);
            return undefined;
        }
        else i++;
    }
    var off_x=off_balls_index*off_inter_w;
    off_balls_index++;
    off_balls.attachMovie(ball_name,ball_name,off_balls.getNextHighestDepth());
    off_balls[ball_name]._x=off_x;
}
game_start();
function newgame()
{
    top_slots.removeMovieClip();
    machine_area.removeMovieClip();
    machine_base_area.removeMovieClip();
    off_balls.removeMovieClip();
    game_start();
}
function update_ball_line(frames_left)
{
    var frames_passed=normal_timeout-frames_left;
    var pos=balls_left-1;
    var mv=machine_base_area['info_'+pos];
    var passed_w=mv._width*frames_passed/normal_timeout;
    mv.lineTo(mv._width-passed_w,mv._height);
}
")
#data aicea are doar 1000
call actionf(data,"
function pack_and_swap(){
//se opreste la shut_down, se porneste la start
	var a=%u+random(balls_total);
	for(var i=0;i<urn_balls.length;i++){
		if(urn_balls[i].nr==a){
			new_sense(urn_balls[i]);
			var b=random(urn_balls.length)-1;
			if(b>=i)b++;
			//
			urn_balls[i].swapDepths(urn_balls[b]);
			break;
		}
	}
}
",(pozitie))
#var urn_balls=_root.urn_balls;var swapPos=random(urn_balls.length);var swapBall=urn_balls[swapPos];
#if(swapBall==this){if(swapPos==0)swapPos++;else swapPos--;swapBall=urn_balls[swapPos];}

call submit_form(deffont,env_w,env_h,game_name)

call swf_exports_done()
call swf_showframe()
call swf_done()
return 1
