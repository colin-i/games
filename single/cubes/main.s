

format elfobj64microsoft

warning off
include "../actionswf/dev/import.h"
warning on
include "../actionswf/include/prog.h"

include "../files/tool.s"
include "../files/submit.s"

str game_name="Cubes"

const texter_height=40
const fps=24
const grid_width=7
const grid_height=18
const matchnumber=3
const all_requirements=matchnumber*10+2
const all_req_plus=all_requirements+8

chars more_cubes#2000;str multe_cuburi^more_cubes

function add_combatant(sd y,ss point,sd font,sd mover_pre,ss multe_cuburi)
    sd sprite
    sd presprite
    setcall presprite swf_sprite_new()
    chars f#300
    str data^f
    #
    sd competitor_wd;setcall competitor_wd competitor_w()
    const fnt_sz=15
        #sd sz;setcall sz strlen(name);inc sz;call memcpy(data,name,sz);sub data# 0x20
        #static_text(competitor_wd#,data,font,(fnt_sz));call swf_sprite_placeobject(presprite,txt,0)
        sd txt_w=50;add txt_w competitor_wd#
        sd txt;setcall txt dyn_text(txt_w,"mover_name",font,(fnt_sz));call swf_sprite_placeobject(presprite,txt,0)
    #
    const text_part_size=fnt_sz+10
    sd rect_w;set rect_w competitor_wd#
    sd rect_h=text_part_size;addcall rect_h draw_competitor(presprite,point,(text_part_size),multe_cuburi)
    #
    const brd_off=5
    call actionf_sprite(presprite,data,"var rect_w=%u;var rect_h=%u;var brd_off=%u",rect_w,rect_h,(brd_off))
    call actionf_sprite(presprite,data,"var text_part_size=%u",(text_part_size))
    call action_sprite(presprite,"
		function cr_brd(hgt){
			createEmptyMovieClip('brd',getNextHighestDepth());
			brd.lineStyle(2,0x01aa01,100,0,'none','round','round');brd.moveTo(0-brd_off,text_part_size-brd_off);
			brd.lineTo(rect_w+brd_off,text_part_size-brd_off);brd.lineTo(rect_w+brd_off,hgt+brd_off);
			brd.lineTo(0-brd_off,hgt+brd_off);brd.lineTo(0-brd_off,text_part_size-brd_off);
		}
        cr_brd(rect_h)
        ")
    call action_sprite(presprite,"if(_root.can_start==1)_parent.mover_start()")
    call swf_sprite_showframe(presprite)
    #
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,"table")
    call actionf_sprite(mover_pre,data,"attachMovie('table','table',getNextHighestDepth());table._y=%u;table.mover_name=_name",y)
endfunction
const Y_add=20+5
#height
function draw_competitor(sd presprite,ss point,sd y,ss multe_cuburi)
    sd competitor_wd
    setcall competitor_wd competitor_w()
    sd max_x;set max_x competitor_wd#;sub max_x 10
    chars f#300
    str data^f
    sd i=0
	sd pnt_re
    sd points_y;set points_y 0
    call actionf_sprite(presprite,data,"createEmptyMovieClip('points',getNextHighestDepth());points._y=%u",y)
    while 1==1
        sd points_x=0
        while points_x<max_x
			call sprintf(data,"var nm='%s';var inst=nm+%u;points.attachMovie(nm,inst,points.getNextHighestDepth());points[inst]._x=%u;points[inst]._y=%u",point,i,points_x,points_y)
			if i<(all_requirements)
				call action_sprite(presprite,data)
			else
				call strcat(multe_cuburi,";")
				call strcat(multe_cuburi,data)
			endelse
            add points_x 11
            inc i;if i==(all_requirements)
				set pnt_re points_y
				add pnt_re (Y_add)
            elseif i==(all_req_plus)
				add points_y (Y_add*2)
				call sprintf(data,";var rect_h=%u",points_y)
				call strcat(multe_cuburi,data)
				return pnt_re
			endelseif
        endwhile
        add points_y (Y_add)
    endwhile
endfunction
function competitor_w();data q#1;return #q;endfunction

entry cubes()

sd sprite
sd presprite
sd img
#sd dbl
#sd bitmap
sd shape

str env_path="D:/universe/local/cubes/env.dbl"

chars a_code#16384
str ac^a_code

sd env_w
sd env_h
setcall env_w swf_dbl_width(env_path)
setcall env_h swf_dbl_height(env_path)

call sprintf(ac,"%s.swf",game_name)
call swf_new(ac,env_w,env_h,0x212121,(fps))

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

#cubes>
str path="D:/universe/local/cubes/cubes/%s.dbl"
chars cb="ananas";chars *="apple";chars *="banana";chars *="bar";chars *="cherries";chars *="plums";chars *="seven"
chars *="basket";chars *="cocktail";chars *="grape";chars *="mellon";chars *="orange";chars *="pear";chars *="strawberry"
chars *=0
ss cubes_names^cb
call sprintf(ac,path,cubes_names);
sd cube_w;setcall cube_w swf_dbl_width(ac)
sd cube_h;setcall cube_h swf_dbl_height(ac)
while cubes_names#!=0
sd j=0;while j<2
    chars dt#300;str data^dt
    if j==0
        setcall presprite swf_sprite_new()
        call actionf_sprite(presprite,data,"var matched=0;var type='%s'",cubes_names)
        call swf_sprite_showframe(presprite)
        setcall sprite swf_sprite_done(presprite)
        call sprintf(data,"vars_%s",cubes_names)
        call swf_exports_add(sprite,data)
    endif
    #
    setcall presprite swf_sprite_new()
    if j==0
        call sprintf(ac,path,cubes_names);setcall img swf_image(ac)
    endif
    call swf_sprite_placeobject(presprite,img,0)
    #
    call action_sprite(presprite,"var ancestor=_parent._parent")
        #this for having vars attached before the next functions, at player at at comp with max_thinking=0 required
    call swf_sprite_showframe(presprite)
    sd i=0
    while i<(fps)
        if j==0
            call action_sprite(presprite,"ancestor.player_cube_frame(this)")
        else
            call action_sprite(presprite,"ancestor.comp_cube_frame(this)")
        endelse
        call swf_sprite_showframe(presprite);inc i
    endwhile
    call action_sprite(presprite,"ancestor.move_cube_y(this)")
    call swf_sprite_showframe(presprite)
    #
    setcall sprite swf_sprite_done(presprite)
    if j==0
        call swf_exports_add(sprite,cubes_names)
    else
        call sprintf(ac,"%s_comp",cubes_names);call swf_exports_add(sprite,ac)
        addcall cubes_names strlen(cubes_names);inc cubes_names
    endelse
inc j;endwhile;endwhile
set cubes_names #cb;call sprintf(ac,"var cubes=new Array('%s'",cubes_names);addcall cubes_names strlen(cubes_names);inc cubes_names
ss cursor;set cursor ac;addcall cursor strlen(ac)
while cubes_names#!=0
    call sprintf(cursor,",'%s'",cubes_names);addcall cubes_names strlen(cubes_names);inc cubes_names;addcall cursor strlen(cursor)
endwhile
call sprintf(cursor,")");call action(ac)
#<cubes

set more_cubes 0
call strcat(multe_cuburi,"function add_more_cubes(comb){var tab_p=comb.table;var points=tab_p.points")

sd mover_pre
sd mover_sprite
setcall mover_pre swf_sprite_new()

#sides>
ss pnt="point"
setcall presprite swf_sprite_new()
#
data struct=solid_fill
    data color=0xff0101FF
data *=0
data *={0,StateFillStyle0|StateMoveTo}
    data *=3
    data *=0
data *={1,1}
    data *=5;data *=0
data *={1,1}
    data *=0;data *=20
data *={1,1}
    data *=-5;data *=0
data *={1,1}
    data *=0;data *=-20
data *=-1
setcall shape swf_shape(11,20,#struct)
call swf_sprite_placeobject(presprite,shape,0)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,pnt)
#
setcall presprite swf_sprite_new()
set color 0x01FF01FF
setcall shape swf_shape(11,20,#struct)
call swf_sprite_placeobject(presprite,shape,0)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"point_done")
#
const grid_w=grid_width*cell;const grid_h=grid_height*cell
const off_to_competitor=30
const inter_grid=5
const gr_off=10
const full_grid_xMargin=gr_off+inter_grid
const full_grid_w=grid_w+full_grid_xMargin+full_grid_xMargin
sd side_w;set side_w env_w;sub side_w (2*full_grid_w);div side_w 2;
sd competitor_wd;setcall competitor_wd competitor_w();set competitor_wd# side_w;sub competitor_wd# (2*off_to_competitor)
sd side_h;set side_h env_h;sub side_h (grid_h);div side_h 2;sd y_off;set y_off side_h
sub y_off 20
call add_combatant(y_off,pnt,deffont,mover_pre,multe_cuburi)
#sd comp_x;set comp_x side_w;add comp_x (grid_w);add comp_x (off_to_competitor);;;;;;;;comp_x
#call add_combatant(y_off,p,deffont)
#<sides

call actionf_sprite(mover_pre,ac,"var grid_width=%u;var grid_height=%u;var cell=%u",(grid_width),(grid_height),(cell))
sd x_offset;set x_offset cube_w;sub x_offset (cell)
sd y_offset;set y_offset cube_h;sub y_offset (cell)
call actionf_sprite(mover_pre,ac,"var xOff_3d=%u;var yOff_3d=%u",x_offset,y_offset)
call action_sprite(mover_pre,"var grid_w=grid_width*cell;var grid_h=grid_height*cell")
call actionf_sprite(mover_pre,ac,"var all_points=%u",(all_requirements))
call actionf_sprite(mover_pre,ac,"var competitor_w=%u",competitor_wd#)
call actionf_sprite(mover_pre,ac,"var off_to_competitor=%u;var inter_grid=%u;var gr_off=%u",(off_to_competitor),(inter_grid),(gr_off))
#
#call actionf_sprite(mover_pre,ac,"var half_fps=%u",(fps/2))
call actionf_sprite(mover_pre,ac,"var fps=%u",(fps))
                     #

#grid>
call action_sprite(mover_pre,"
createEmptyMovieClip('grid',getNextHighestDepth());
var player_grid_x=off_to_competitor+competitor_w+off_to_competitor+inter_grid+gr_off;
if(_name=='Player'){
    grid._x=player_grid_x;
    table._x=off_to_competitor;
}else{
    _x=player_grid_x+grid_w+gr_off+inter_grid;
    grid._x=inter_grid+gr_off;
    table._x=inter_grid+gr_off+grid_w+gr_off+inter_grid+off_to_competitor;
}
grid._y=(_root.env._height-grid_h)/2;

//adjustments
var half_xOff_3d=xOff_3d/2;grid._x=grid._x+half_xOff_3d;
var half_yOff_3d=yOff_3d/2;grid._y=grid._y+half_yOff_3d;
var grid_x_left_border_off=0-gr_off-xOff_3d;

grid.lineStyle(2,0x0101FF,100,0,'none','round','round');
grid.moveTo(grid_x_left_border_off,0);grid.lineTo(grid_x_left_border_off,grid_h+gr_off);grid.lineTo(grid_w+gr_off,grid_h+gr_off);grid.lineTo(grid_w+gr_off,0);

grid.createEmptyMovieClip('grid_lines',grid.getNextHighestDepth());
var grid_lines=grid.grid_lines;
grid_lines.lineStyle(1,0x0101FF,50,0,'none','none','miter');
var row_off;var col_off;var cell_part=cell/4;
var row=0;while(row<grid_height){
    row_off=row*cell;
    var col=0;while(col<grid_width){
        col_off=col*cell;
        var is_not=1;
        var x;var y=0;var on_x=cell_part;while(y<=cell){
            if(row==0)if(y==0)is_not=0;
            if(is_not==1)is_not=0;
            else{
                x=y;
                grid_lines.moveTo(col_off+x,row_off+y);grid_lines.lineTo(col_off+x+on_x,row_off+y);
                x=x+on_x+on_x;
                grid_lines.moveTo(col_off+x,row_off+y);grid_lines.lineTo(col_off+x+on_x,row_off+y);
            }
            on_x=on_x*-1;y=y+cell;
        }
        var is_not_2=0;
        var x=cell;var on_y=cell_part;while(x>=0){
            if(is_not_2==1)is_not_2=0;
            else{
                y=x-cell*-1;
                grid_lines.moveTo(col_off+x,row_off+y);grid_lines.lineTo(col_off+x,row_off+y+on_y);
                y=y+on_y+on_y;
                grid_lines.moveTo(col_off+x,row_off+y);grid_lines.lineTo(col_off+x,row_off+y+on_y);
                is_not_2=1;
                if(col==0)if(x==cell)is_not_2=0;
            }
            on_y=on_y*-1;x=x-cell;
        }
        col++;
    }
    row++;
}

var grid_w_off=grid_w+yOff_3d;y=0;
row=0;while(row<grid_height){
    grid_lines.moveTo(grid_w,y);y=y+yOff_3d;
    grid_lines.lineTo(grid_w_off,y);y=y+cell_part;
    grid_lines.lineTo(grid_w_off,y);y=y+cell_part;
    grid_lines.moveTo(grid_w_off,y);y=y+cell_part;
    grid_lines.lineTo(grid_w_off,y);y=y+cell_part-yOff_3d;
    row++;
}
var grid_h_off=grid_h+xOff_3d;x=0;
col=0;while(col<grid_width){
    grid_lines.moveTo(x,grid_h);x=x+xOff_3d;
    grid_lines.lineTo(x,grid_h_off);x=x+cell_part;
    grid_lines.lineTo(x,grid_h_off);x=x+cell_part;
    grid_lines.moveTo(x,grid_h_off);x=x+cell_part;
    grid_lines.lineTo(x,grid_h_off);x=x+cell_part-xOff_3d;
    col++;
}
grid_lines.moveTo(grid_w,grid_h);grid_lines.lineTo(grid_w_off,grid_h_off);
grid_lines._x=grid_lines._x-xOff_3d;grid_lines._y=grid_lines._y-yOff_3d;

")
#<grid

#timer>
setcall presprite swf_sprite_new()
call action_sprite(presprite,"stop()")
set i 0
while i<(fps);call swf_sprite_showframe(presprite);inc i;endwhile
call action_sprite(presprite,"_parent.timer_end()")
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"timer")
call action_sprite(mover_pre,"attachMovie('timer','timer',getNextHighestDepth())")
#<timer

call declare_grid((grid_width),(grid_height),"map",mover_pre,"0")
#call init_reset_grid()

call action_sprite(mover_pre,"
var simple_counter;
function mover_start(){
    done_points=0;simple_counter=0;
    createEmptyMovieClip('movies',getNextHighestDepth());movies._x=grid._x;movies._y=grid._y;
    table.createEmptyMovieClip('done_points_movies',table.getNextHighestDepth());table.done_points_movies._x=table.points._x;table.done_points_movies._y=table.points._y;
    //reset_grid(grid_width,grid_height,map);
    got_all_points=0;
    cube_in();
}

var cube;var can_go_down;var right_frames;var total_playing;var done_points;
var comp_thinking;
var got_all_points;

function cube_in(){
    if(got_all_points==1)return undefined;
    var pos=random(_root.cubes.length);

    /*if(_name=='Player'){
	//_root._text.message=_root.debugging_player.length;
	pos=_root.debugging_player.splice(0,1);pos=pos[0];
	}
    else{
		pos=_root.debugging_comp.splice(0,1);pos=pos[0];
	}*/

    var cube_rand=_root.cubes[pos];var inst=cube_rand+simple_counter;simple_counter++;
    var exp_name=cube_rand;
    can_go_down=0;total_playing=1;
    if(_name!='Player'){
        exp_name=exp_name+'_comp';
		comp_thinking=fps/2;
		if(map[0][0]==0){
			comp_thinking+=fps;
			if(map[1][0]==0)comp_thinking+=fps;
		}
		//comp_thinking=0;
		//if(_root.debugging_comp.length<2)comp_thinking=fps/2+fps;
    }else{_root.score++;}
    movies.attachMovie(exp_name,inst,movies.getNextHighestDepth());
    cube=movies[inst];cube._x=0-xOff_3d;cube._y=0-yOff_3d-cell;
    if(_name=='Player'){
		_root._text.message=_root.score;
        cube.onKeyDown=move_cube;
        Key.addListener(cube);
    }
    cube.attachMovie('vars_'+cube_rand,'vars',cube.getNextHighestDepth());
}
function move_cube(){
    if(Key.getCode()==Key.LEFT)move_cube_left();
    else if(Key.getCode()==Key.RIGHT)move_cube_right();
    else if(Key.getCode()==Key.DOWN)can_go_down=1;
}
function move_cube_y(entry){
    var cube_at_ground=false;
    var row=(entry._y+yOff_3d)/cell+1;
    if(row==grid_height)cube_at_ground=true;
    else{
        var col=(entry._x+xOff_3d)/cell;
        if(map[row][col]!=0)cube_at_ground=true;
    }
    if(cube_at_ground==true){
        if(row==0){
			//if this cube comes again it will wait until the space clear
            if(can_go_down!=2){
				if(_name=='Player')Key.removeListener(cube);
                var last_row=grid_height-1;
                var last_brick=map[last_row][col];map[last_row][col]=0;last_brick.removeMovieClip();
                start_a_fall(map[last_row-1][col]);
            }
        }
        else{
			if(_name=='Player')Key.removeListener(cube);
			set_cube(entry);
		}
        return undefined;
    }

    entry._y=entry._y+cell;
    map[row][col]=entry;
    row--;
    if(row>=0){
        map[row][col]=0;
        if(can_go_down==2){
            row--;
            if(row>=0){
                var fall_cube=map[row][col];
                if(fall_cube!=0)fall_cube.play();
            }
        }
    }
}
function move_cube_left(){
    var x=cube._x+xOff_3d;
    if(x>0){
        var row=(cube._y+yOff_3d)/cell;
        var col=x/cell;

        //can't go left
        if(row>=0){
            if(map[row][col-1]!=0)return undefined;
            map[row][col]=0;
        }

        col--;

        var row_cursor=grid_height-1;var swap_mv;
        while(row_cursor>row){
            var swap_mv=map[row_cursor][col];
            if(swap_mv==0)row_cursor=-1;
            else{
                swap_mv.swapDepths(cube);row_cursor--;
            }
        }

        if(row>=0)map[row][col]=cube;

        cube._x=cube._x-cell;
    }
}
function move_cube_right(){
    var max=grid_w-cell;var x=cube._x+xOff_3d;
    if(x<max){
        var row=(cube._y+yOff_3d)/cell;
        var col=x/cell;

        //can't go right
        if(row>=0)if(map[row][col+1]!=0)return undefined;

        var row_cursor=grid_height-1;
        if(row!=row_cursor){
            var swap_mv=map[row_cursor][col];
            if(swap_mv!=0){
                swap_mv.swapDepths(cube);row_cursor--;var swap_prev;
                while(row_cursor>row){
                    swap_prev=map[row_cursor][col];
                    if(swap_prev==0)row_cursor=-1;
                    else{
                        swap_mv.swapDepths(swap_prev);swap_mv=swap_prev;row_cursor--;
                    }
                }
            }
        }

        if(row>=0)map[row][col]=0;
        col++;
        if(row>=0)map[row][col]=cube;

        cube._x=cube._x+cell;
    }
}
var pool=new Array();var arrived_cubes=new Array();var matched_cubes=new Array();
function set_cube(entry){
    entry.stop();
    arrived_cubes.push(entry);
    total_playing--;
    if(total_playing==0){
        var i=0;while(i<arrived_cubes.length){
            calculate_pos(arrived_cubes[i]);
            i++;
        }
        arrived_cubes.length=0;

        if(matched_cubes.length!=0)timer.play();
        else cube_in();
    }
}
function calculate_pos(entry){
    var row=(entry._y+yOff_3d)/cell;
    var col=(entry._x+xOff_3d)/cell;
    var type=entry.vars.type;

    pool.push(entry);
    get_matches(row,col,0,-1,0,type);
    get_matches(row,col,grid_width-1,1,0,type);
    write_matches();

    pool.push(entry);
    get_matches(row,col,grid_height-1,0,1,type);
    write_matches();
}
function get_matches(row,col,lim,c_step,r_step,type){
    var c=col;var r=row;
    while(1==1){
        if(c_step!=0){if(c==lim)return undefined;c=c+c_step;}
        else{if(r==lim)return undefined;r=r+r_step;}
        var mv=map[r][c];
        if(mv==0)return undefined;
        if(mv.vars.type!=type)return undefined;
        pool.push(mv);
    }
}
function write_matches(){
    if(pool.length>=3){
        var i=0;while(i<pool.length){add_match(pool[i]);i++;}
    }
    pool.length=0;
}
function add_match(match){
    //was matched by another call
    if(match.vars.matched==1)return undefined;

    match.createEmptyMovieClip('mark',match.getNextHighestDepth());
    var mv=match.mark;mv._x=xOff_3d;mv._y=yOff_3d;
    mv.lineStyle(2,0x01FF01,100,0,'none','round','round');
    mv.lineTo(cell,0);mv.lineTo(cell,cell);mv.lineTo(0,cell);mv.lineTo(0,0);

    matched_cubes.push(match);match.vars.matched=1;
}
var possible_starting_fallers_x=new Array();var possible_starting_fallers_y=new Array();
function timer_end(){
    var i=0;possible_starting_fallers_x.length=0;possible_starting_fallers_y.length=0;
    while(i<matched_cubes.length){
        var cube=matched_cubes[i];
        var row=(cube._y+yOff_3d)/cell;
        var col=(cube._x+xOff_3d)/cell;
        cube.removeMovieClip();
        map[row][col]=0;

        row--;
        if(row>=0){
            possible_starting_fallers_y.push(row);
            possible_starting_fallers_x.push(col);
        }

        i++;
    }

    //calculate the new falling cubes
    i=0;while(i<possible_starting_fallers_x.length){
        var fall_cube=map[possible_starting_fallers_y[i]][possible_starting_fallers_x[i]];
        if(fall_cube!=0){
        //undefined, can be above but then discovered as match and now is undefined
        //           or is the case where vertical matches come from above on a vertical match
            start_a_fall(fall_cube);
        }
        i++;
    }

    i=0;while(i<matched_cubes.length){
        var pt_mv=table.points['point'+done_points];var dones=table.done_points_movies;
        var next_depth=dones.getNextHighestDepth();var done_name='point_done'+next_depth;
        dones.attachMovie('point_done',done_name,next_depth);
        var completed_mv=dones[done_name];completed_mv._x=pt_mv._x;completed_mv._y=pt_mv._y;
        done_points++;i++;
    }

    matched_cubes.length=0;

    if(done_points>=all_points){
        got_all_points=1;
        if(_name=='Player'){
            _root.submitform(_root.score);
        }else{
			//_root._text.message=_root.add_more_cubes+' '+_root['Player'];
			_root.add_more_cubes(_root['Player']);
		}
    }
    else if(total_playing==0)cube_in();
}
function start_a_fall(fall_cube){
    fall_cube.play();
    can_go_down=2;
    var row=(fall_cube._y+yOff_3d)/cell;
    var col=(fall_cube._x+xOff_3d)/cell;
    while(row>=0){
    /*count the above remaining cubes
        the play is for the starting ones here and when the starters go down,
                                               the play for above cube is invoked*/
        if(map[row][col]!=0){total_playing++;row--;}
        else row=-1;
    }
}

function player_cube_frame(entry){
    if(can_go_down!=0)move_cube_y(entry);
}
function comp_cube_frame(entry){
    if(can_go_down>=1)move_cube_y(entry);
    else if(comp_thinking>=0){if(comp_thinking==0)computer_set();comp_thinking--;}
    else if(right_frames!=0){right_frames--;move_cube_right();}
    else can_go_down=1;
}

function computer_set(){
    //a computer function, sets the right_frames value to move the cube
    var row=(cube._y+yOff_3d)/cell;
	right_frames=0;
	var col=1;
	var type=cube.vars.type;
	var best_match=-1;
	var max_row=grid_height-1;
	while(col<grid_width){
		if(map[max_row][col]==0){if(right_frames==0)right_frames=col;}
		else if(map[max_row][col].vars.type==type){right_frames=col;break;}
		col++;
	}
}
/*function computer_set(){
    //a computer function, sets the right_frames value to move the cube
    var row=(cube._y+yOff_3d)/cell;
	right_frames=0;
	var col_to=grid_width;
	if(row>=0){
		col_to=1;while(col_to<grid_width){
			//is wall, can't go right
			if(map[row][col_to]!=0){
				//_root._text.message='g'+row+' '+col_to;
				break;
			}
			col_to++;
		}
	}
	var col=0;
	var type=cube.vars.type;
    var best_match=-1;var second_match=-1;var low_match=-1;
    while(col<col_to){
		var row_ground=row;var row_cursor=row+1;
        while(row_ground!=row_cursor){
			if(row_cursor!=grid_height)if(map[row_cursor][col]==0)row_ground++;
            if(row_cursor==row_ground)row_cursor++;
            else row_cursor--;
        }
        if(row_ground>=0){
			var cube_match;
			if(row_ground>best_match){
				var max_row=grid_height-1;
				if(row_ground<max_row){
					cube_match=map[row_ground+1][col];
					if(cube_match!=0)if(cube_match.vars.type==type){
						//_root._text.message='a';
						right_frames=col;best_match=row_ground;
					}
				}
			}
			//left and right will block the brick one level down. down match is best in most cases
			if(best_match==-1)if(row_ground>second_match){
				var bln=comp_act(row_ground,col,type);
				if(bln==true){
					//_root._text.message='b';
					right_frames=col;second_match=row_ground;
				}else if(row_ground>low_match){
					right_frames=col;low_match=row_ground;
					//_root._text.message='c';
				}
			}
        }
		col++;
	}
}
function comp_act(r,c,t){
	var max_col=grid_width-1;var cube_match;
    if(c>0){
		cube_match=map[r][c-1];
        if(cube_match!=0)if(cube_match.vars.type==t){
			//_root._text.message='d';
			return true;
		}
    }
	if(c<max_col){
		cube_match=map[r][c+1];
		if(cube_match!=0)if(cube_match.vars.type==t){
			//_root._text.message='e';
			return true;
		}
	}
	//_root._text.message='f';
	return false;
}*/
")

#

call swf_sprite_showframe(mover_pre)
setcall mover_sprite swf_sprite_done(mover_pre)
call swf_exports_add(mover_sprite,"mover")

call action("
var score;
var can_start;
function newgame(){
    Player.removeMovieClip();
    Computer.removeMovieClip();
    can_start=1;
    game_start();
    //movies.removeMovieClip();
    //player.done_points_movies.removeMovieClip();
}
function at_load(){
    _text.message='Click to play';
    can_start=0;
    game_start();
    onMouseDown=a_start;
}
function a_start(){
//add_more_cubes('Player');
    onMouseDown=undefined;
    Player.mover_start();
    Computer.mover_start();
}
function game_start(){
    score=0;
    attachMovie('mover','Player',getNextHighestDepth());
    attachMovie('mover','Computer',getNextHighestDepth());
}
")

call sprintf(ac,";comb.all_points=%u",(all_req_plus))
call strcat(multe_cuburi,ac)
call strcat(multe_cuburi,";tab_p.brd.removeMovieClip();tab_p.cr_brd(rect_h);}")
call action(multe_cuburi)

sd texter_y
set texter_y env_h
sub texter_y (texter_height)
call default_text_ex((0xff0101FF),deffont,texter_y,(texter_height))
call submit_form(deffont,env_w,env_h,game_name)

#include "debugging.s"

call action("at_load()")


call swf_exports_done()
call swf_showframe()
call swf_done()
return 1
