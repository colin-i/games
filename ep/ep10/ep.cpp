

#include<windows.h>
#include<stdio.h>

#include<math.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

int _start(){
    char* ep_nm=On_the_Rail_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    action("_root['episode']=this");
    init_episod(ep_nm);

    //env
    //rail
    int rail_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/10/rail.dbl",rail_sz);
    swf_exports_add(dbl,"rail_texture");
    //
    int bg_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/10/bg.dbl",bg_sz);
    swf_exports_add(dbl,"bg_texture");
    //
    presprite=swf_sprite_new();
    #define rail_x 0-(rail_sz[0]/2)
    actionf_sprite(presprite,buf,"var height=%u;var rail_w=%u;var rail_h=%u;var bg_h=%u",height,rail_sz[0],rail_sz[1],bg_sz[1]);
    action_sprite(presprite,R"(
        var hg=3*height;
        //
        var rail_bmp=flash.display.BitmapData.loadBitmap('rail_texture');
        var r_mx=new flash.geom.Matrix();
        //
        var hg_half=hg/2;
        //
        var bg_bmp=flash.display.BitmapData.loadBitmap('bg_texture');
        var b_mx=new flash.geom.Matrix();
        //
        function draw_rail_bg(distance){
            clear();
            //
            reset_mx(b_mx,distance,bg_h);
            beginBitmapFill(bg_bmp,b_mx,true);
            moveTo(0-hg_half,0);
            lineTo(hg_half,0);lineTo(hg_half,hg);
            lineTo(0-hg_half,hg);lineTo(0-hg_half,0);
            endFill();
            //
            reset_mx(r_mx,distance,rail_h);
            beginBitmapFill(rail_bmp,r_mx,true);
            lineTo(rail_w,0);lineTo(rail_w,hg);
            lineTo(0,hg);lineTo(0,0);
            endFill();
        }
        function reset_mx(mx,distance,bmp_h){
            mx.ty+=distance;
            if(mx.ty>bmp_h)mx.ty-=bmp_h;
        }
        //asta oricum din prima enterframe la inaintare apare
        //draw_rail_bg(0);
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"rail_bg");

	automatic(1);
    //railRobot
    //gun
    int gun_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/10/gun.dbl",gun_sz);
    #define gun_x 0-(gun_sz[0]/2)
    #define gun_y 0-gun_sz[1]
    #define bullet_r gun_sz[0]/2
    swf_sprite_placeobject_coords(presprite,bmp,0,gun_x,gun_y);
    actionf_sprite(presprite,buf,"var shot_interval=%u", shot_interval);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
		function shot_func(){
			if(shot_timer==undefined){
				var d=ep.env.bullets.getNextHighestDepth();
                var mc=ep.env.bullets.attachMovie('bullet','bullet'+d,d);
                //
                createEmptyMovieClip('shot_timer',getNextHighestDepth());
                shot_timer['shot_interval']=shot_interval;
                shot_timer.onEnterFrame=function(){
                    this.shot_interval--;if(this.shot_interval<0)this.removeMovieClip();
                }
            }
		}
        onEnterFrame=function(){
            if(Key.isDown(com.action_key)==true)shot_func();
			else if(ep.auto_shot==true)shot_func();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"gun");
    //bullet
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    actionf_sprite(presprite,buf,"var height=%u",height);
    actionf_sprite(presprite,buf,"var bullet_r=%u",bullet_r);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //aicea ex: step 580/32=18.125 si 20 este latura unei target
        var step=height/(2*fps);
        beginFill(0xff8000);
        com.drawCircle_offset(this,bullet_r,0-bullet_r);
        endFill();
        //
        var gun_rot=ep.railRobot.gun._rotation;
        var x_step=Math.sin(gun_rot*Math.PI/180)*step;
        var y_step=Math.cos(gun_rot*Math.PI/180)*step;
        //e 2*height ca pe diagonala daca se duce e mai mult decat height
        var dist_max=2*height;
        var frames_max=dist_max/step;
        onEnterFrame=function(){
            _y-=y_step;_x+=x_step;
            frames_max--;if(frames_max<0)removeMovieClip();
            else{
                var mv=ep.env.zones.clips;
                for(var mc_name in mv){
                    var mc=mv[mc_name];
                    if(hitTest(mc)==true){
                        mc.removeMovieClip();
                        var last_target=true;
                        for(var mc_name in mv){
                            last_target=false;break;
                        }
                        if(last_target==true)ep.env.zones.new_zone();
                        removeMovieClip();
                        break;
                    }
                }
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bullet");
    //utilaj
    int utilaj_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/10/utilaj.dbl",utilaj_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"railRobot_main");
    //robot
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    actionf_sprite(presprite,buf,"var railRobot_w=%u;var railRobot_h=%u",utilaj_sz[0],utilaj_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        ep.add_smokes(this);
        attachMovie('railRobot_main','railRobot_main',getNextHighestDepth());
        attachMovie('gun','gun',getNextHighestDepth());
        gun._x=railRobot_w/2;gun._y=railRobot_h/2;
        var x_step=0;
        //aicea int ca nu da bine la up-down cand nu e fix
        var y_step=int(0-(railRobot_h/fps));
        var rot_step=16/fps;
        onEnterFrame=function(){
            ep.env.rail_bg.draw_rail_bg(0-y_step);
            if(Key.isDown(com.left_arrow)==true){
                var rt=fine_rotation();
                ep.env._rotation+=rt;
                gun._rotation-=rt;ep.map.draw_gun();
            }
            else if(Key.isDown(com.right_arrow)==true){
                var rt=fine_rotation();
                ep.env._rotation-=rt;
                gun._rotation+=rt;ep.map.draw_gun();
            }
        }
        //rotation+
        function fine_rotation(){
            if(Key.isDown(com.jump_aux_key)==true)return 5*rot_step;
            return rot_step;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"railRobot");
    //smokes
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var smoke_step=fps;
        var smoke_index=0;
        onEnterFrame=function(){
            smoke_index++;if(smoke_index<smoke_step)return null;
            smoke_index=0;
            //aicea cand sta pe loc
            if(clip.x_step==0)if(clip.y_step==0)return null;
            var mvClip=ep.env.smokes;
            var d=0;while(mvClip.getInstanceAtDepth(d)!=undefined)d++;
            var mc=mvClip.attachMovie('smoke','smoke'+d,d);
            mc._x=clip._x+(clip._width/2);mc._y=clip._y+(clip._height/2);
            mc['clip']=clip;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"smokes");
    action(R"(
        function add_smokes(mc){
            var smk=mc.attachMovie('smokes','smokes',mc.getNextHighestDepth());
            smk['clip']=mc;
        }
    )");
    //smoke
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    action_sprite(presprite,R"(
        var com=_root['shared_level'];
        beginFill(0,30);
        var r=clip._height/6;
        com.drawCircle_offset(this,r,0-r);
        endFill();
        var dist=r*20;
        var step_frames=2*fps;
        var step=dist/step_frames;
        var x_step=0-clip.x_step;
        var y_step=0-clip.y_step;
        onEnterFrame=function(){
            _x+=x_step;
            _y+=y_step;
            step_frames--;if(step_frames<0)removeMovieClip();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"smoke");

    //target
    int target_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/10/target.dbl",target_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"target_mc");
    //
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        ep.add_smokes(this);
        attachMovie('target_mc','target_mc',getNextHighestDepth());
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"target");
    //zones
int wd=width;
wd/=4;
int x_base=0-wd;
int y_start=0-height;
int targets_move_h=2*height;
//                               offset sa nu mearga in margini
#define backOff 200
#define frontOff 50
//las 50 aicea
int start_y=y_start-50;
int end_y=start_y-targets_move_h+backOff+frontOff;
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var height=%u",height);
    actionf_sprite(presprite,buf,"var x_base=%u;var y_start=%u",x_base,y_start);
    actionf_sprite(presprite,buf,"var start_y=%u;var end_y=%u;var targets_move_h=%u;var frontOff=%u;var backOff=%u;",start_y,end_y,targets_move_h,frontOff,backOff);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var zoneNr=0;var z;
        createEmptyMovieClip('clips',getNextHighestDepth());
        function new_zone(){
            if(zoneNr==0)add_targets();
            else if(zoneNr==1)z=attachMovie('lr_targets','lr_targets',getNextHighestDepth());
            else{
                z.removeMovieClip();
                if(zoneNr==2)z=attachMovie('ud_targets','ud_targets',getNextHighestDepth());
                else if(zoneNr==3)z=attachMovie('circle_targets','circle_targets',getNextHighestDepth());
                else if(zoneNr==4)z=attachMovie('boss_shapeTarget','boss_target',getNextHighestDepth());
                else if(zoneNr==5)z=attachMovie('circleOne_shapeTarget','circleOne_target',getNextHighestDepth());
                else if(zoneNr==6)z=attachMovie('star_shapeTarget','star_target',getNextHighestDepth());
                else if(zoneNr==7)z=attachMovie('line_allScreenTargets','line_target',getNextHighestDepth());
                else if(zoneNr==8)z=attachMovie('diag_shapeTarget','diag_target',getNextHighestDepth());
                else if(zoneNr==9)z=attachMovie('arrow_allScreenTargets','arrow_target',getNextHighestDepth());
                else _root.end_scenario();
            }
            zoneNr++;
        }
        var y_min=y_start+frontOff;var y_max=height-backOff;
        var y;
        var bottom_Cursor;var bottomCursor=y_start-backOff+targets_move_h;
        function add_targets(){
            y=start_y;bottom_Cursor=bottomCursor;
            while(end_y<y){
                add_mc(x_base);add_mc(x_base*-1);
                y-=50;bottom_Cursor-=50;
            }
        }
        function add_mc(x){
            var mc=new_mc();mc._x=x;mc._y=y;
            mc['bottom_y']=bottom_Cursor;
            z.zone_set_fn(mc);
            mc.onEnterFrame=function(){
                this._y-=ep.railRobot.y_step;
                this.zone_f();
                if(this.bottom_y<this._y){
                    this.y_step=ep.railRobot.y_step;
                    this.onEnterFrame=this.zone_f;
                }
            }
        }
        //mc
        function new_mc(){
            var d=clips.getNextHighestDepth();
            var mc=clips.attachMovie('target','target'+d,d);
            mc.y_step=0;mc.x_step=0;
            return mc;
        }
        new_zone();
        //
        function add_shape_targets(level){
            var next_y=y_max;var string_val='yMax';var step=0-ep.railRobot.y_step;
            level.onEnterFrame=function(){
                var rect=clips.getBounds();
                var a=rect[string_val]<next_y;
                for(var mc_name in clips){
                    var mc=clips[mc_name];mc._y+=step;
                }
                rect=clips.getBounds();
                if(a!=(rect[string_val]<next_y)){
                    if(next_y==y_max){
                        next_y=y_min;string_val='yMin';step=ep.railRobot.y_step;
                        for(var mc_name in clips){
                            var mc=clips[mc_name];mc.y_step=2*ep.railRobot.y_step;
                        }
                    }else{
                        next_y=y_max;string_val='yMax';step=0-ep.railRobot.y_step;
                        for(var mc_name in clips){
                            var mc=clips[mc_name];mc.y_step=0;
                        }
                    }
                }
            }
        }
        function add_allScreenTargets(){
            var down_step=0-ep.railRobot.y_step;
            for(var mc_name in clips){
                var mc=clips[mc_name];
                mc.y_step=down_step;
                mc['next_y']=y_max;
                mc.onEnterFrame=function(){
                    var a=this.next_y<this._y;
                    this._y+=this.y_step-ep.railRobot.y_step;
                    if(a!=(this.next_y<this._y)){
                        if(this.next_y==y_max){
                            this.next_y=y_min;this.y_step=3*ep.railRobot.y_step;
                        }
                        else{
                            this.next_y=y_max;this.y_step=down_step;
                        }
                    }
                }
            }
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"zones");
    //left right moving targets
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var min_x=_x-30;var max_x=_x+30;
        function zone_set_fn(mc){
            mc.x_step=ep.railRobot.y_step*((random(2)*2)-1);
            mc['min_x']=mc._x-30;mc['max_x']=mc._x+30;
            mc['zone_f']=zone_f;
        }
        function zone_f(){
            this._x+=this.x_step;
            if(this.max_x<this._x)this.x_step*=-1;
            else if(this._x<this.min_x)this.x_step*=-1;
        }
        _parent.add_targets();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"lr_targets");
    //up down moving targets
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var stable_step=2*ep.railRobot.y_step;
        var move_frames=(2*30/Math.abs(stable_step-ep.railRobot.y_step));
        function zone_set_fn(mc){
            mc.y_step=0-ep.railRobot.y_step;
            mc['zone_f']=zone_f;
        }
        function zone_f(){
            if(this.onEnterFrame!=zone_f)this._y+=this.y_step;
            else{
                this.frames=move_frames/2;
                //aicea daca pun random se vor cocni intre ele cand e cu 0 si cand e cu 1
                //random(2)
                this.y_step=stable_step;
                this.onEnterFrame=up_down_f;
            }
        }
        function up_down_f(){
            this._y+=this.y_step-ep.railRobot.y_step;
            this.frames++;if(this.frames<move_frames)return null;
            this.frames=0;
            //asa devine 0 si -catTrebuie
            this.y_step=(this.y_step-stable_step)*-1;
        }
        _parent.add_targets();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ud_targets");
    //circle targets
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var r=60;var rot_step=90/fps;
        function zone_set_fn(mc){
            mc['angle']=0;
            set_offsets(mc);
            mc['zone_f']=zone_f;
        }
        function set_offsets(mc){
            //este int ca sa se intoarca exact pe aceeasi directie fara marja de eroare
            mc['x_off']=int(Math.sin(mc.angle*Math.PI/180)*r);
            mc['y_off']=int(Math.cos(mc.angle*Math.PI/180)*r);
            mc._x+=mc.x_off;mc._y+=mc.y_off;
        }
        function zone_f(){
            this.angle+=rot_step;
            var x1=this._x;var y1=this._y;
            this._x-=this.x_off;this._y-=this.y_off;
            set_offsets(this);
            this.x_step=this._x-x1;this.y_step=this._y-y1;
        }
        _parent.add_targets();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"circle_targets");
    //boss
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var x_base=%u;var start_y=%u",x_base,start_y);
    actionf_sprite(presprite,buf,"var target_w=%u;var target_h=%u",target_sz[0],target_sz[1]);
    action_sprite(presprite,R"(
        var cols_nr=5;
        var x_val=(target_w*cols_nr)/2;
        var x;var y;
        var parts=0;while(parts<2){
            var x_set=x_base-x_val;
            y=start_y;
            var row=0;while(row<5){
                x=x_set;
                var col=0;while(col<cols_nr){
                    var mc=_parent.new_mc();mc._y=y;mc._x=x;
                    x+=target_w;col++;
                }
                y-=target_h;row++;
            }
            x_base*=-1;parts++;
        }
        _parent.add_shape_targets(this);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"boss_shapeTarget");
    //circle
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var x_base=%u;var start_y=%u",x_base,start_y);
    actionf_sprite(presprite,buf,"var target_lat=%u",target_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var r=60;
        var y_base=start_y-r;
        var dif_x=Math.sqrt((r*r)-(target_lat*target_lat));
        var angle=Math.atan2(target_lat,dif_x)*180/Math.PI;
        var total_nr=int(360/angle);
        function add_circle(){
            var rot=0;var nr=total_nr;
            while(0<nr){
                var mc=_parent.new_mc();
                mc._x=x_base+(Math.cos(rot*Math.PI/180)*r);
                mc._y=y_base+(Math.sin(rot*Math.PI/180)*r);
                rot+=angle;nr--;
            }
        }
        var parts=0;while(parts<2){
            add_circle();
            parts++;x_base*=-1;
        }
        _parent.add_shape_targets(this);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"circleOne_shapeTarget");
    //star_target
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var x_base=%u;var start_y=%u",x_base,start_y);
    actionf_sprite(presprite,buf,"var target_lat=%u",target_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var r=4;
        var y_base=start_y-(4*target_lat);
        function add_star(){
            var rot=0;
            while(rot<360){
                var on_x=Math.round(Math.cos(rot*Math.PI/180));
                var on_y=Math.round(Math.sin(rot*Math.PI/180));
                var nr=r;var x=x_base;var y=y_base;
                while(nr>0){
                    x+=target_lat*on_x;y+=target_lat*on_y;
                    var mc=_parent.new_mc();mc._x=x;mc._y=y;
                    nr--;
                }
                rot+=45;
            }
        }
        var parts=0;while(parts<2){
            var mc=_parent.new_mc();mc._x=x_base;mc._y=y_base;
            add_star();
            parts++;x_base*=-1;
        }
        _parent.add_shape_targets(this);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"star_shapeTarget");
    //diag_target
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var x_base=%u;var start_y=%u",x_base,start_y);
    actionf_sprite(presprite,buf,"var target_lat=%u",target_sz[1]);
    action_sprite(presprite,R"(
        var diags=5;var d_height=5;var sn=1;
        function add_diag(){
            var x_start=x_base;
            var nr=diags;
            while(nr>0){
                var x=x_start;var y=start_y;
                var n=0;while(n<d_height){
                    var mc=_parent.new_mc();mc._x=x;mc._y=y;
                    x+=target_lat*sn;y-=target_lat;n++;
                }
                x_start-=2*target_lat*sn;
                nr--;
            }
        }
        var parts=0;while(parts<2){
            add_diag();
            parts++;x_base*=-1;sn*=-1;
        }
        _parent.add_shape_targets(this);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"diag_shapeTarget");
    //arrow_allScreenTargets
    #define all_screenSegments 6
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var x_base=%u;var start_y=%u",x_base,start_y);
    actionf_sprite(presprite,buf,"var target_lat=%u;var segments_set=%u",target_sz[1],all_screenSegments);
    action_sprite(presprite,R"(
        function add_arrow(){
            var segments=segments_set;
            var y=start_y;var x_left=x_base;var x_right=x_base;
            while(segments>0){
                x_left-=target_lat;x_right+=target_lat;y-=target_lat;
                var mc=_parent.new_mc();mc._x=x_left;mc._y=y;
                mc=_parent.new_mc();mc._x=x_right;mc._y=y;
                segments--;
            }
        }
        var parts=0;while(parts<2){
            var mc=_parent.new_mc();mc._x=x_base;mc._y=start_y;
            add_arrow();
            parts++;x_base*=-1;
        }
        _parent.add_allScreenTargets();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"arrow_allScreenTargets");
    //line_allScreenTargets
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var x_base=%u;var start_y=%u",x_base,start_y);
    actionf_sprite(presprite,buf,"var target_lat=%u;var segments_set=%u",target_sz[1],all_screenSegments);
    action_sprite(presprite,R"(
        function add_line(){
            var segments=segments_set;var x_left=x_base;var x_right=x_base;
            while(segments>0){
                x_left-=target_lat;x_right+=target_lat;
                var mc=_parent.new_mc();mc._x=x_left;mc._y=start_y;
                mc=_parent.new_mc();mc._x=x_right;mc._y=start_y;
                segments--;
            }
        }
        var parts=0;while(parts<2){
            var mc=_parent.new_mc();mc._x=x_base;mc._y=start_y;
            add_line();
            parts++;x_base*=-1;
        }
        _parent.add_allScreenTargets();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"line_allScreenTargets");

    //map
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var hg=150;var all_h=2*height;
        _y=height-hg-ep._y;
        var wd=width*hg/all_h;_x=width-wd-ep._x;
        var line_sz=2;_x-=line_sz+1;_y-=line_sz+1;
        lineStyle(line_sz,0xff0000);
        lineTo(wd,0);lineTo(wd,hg);lineTo(0,hg);lineTo(0,0);
        attachMovie('map_screen','map_screen',getNextHighestDepth());
        map_screen['wd']=wd;map_screen['hg']=hg;
        //
        function ep_x_to_map_x(x){
            x+=ep._x;
            return wd*x/width;
        }
        function ep_y_to_map_y(y){
            y+=ep._y;
            return hg*y/all_h;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"map");
    //map_screen
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var target_w=%u;var target_h=%u",target_sz[0],target_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        function fill(name){
            var mc=createEmptyMovieClip(name,getNextHighestDepth());
            mc.beginFill(0);
            mc.lineTo(wd,0);mc.lineTo(wd,hg);mc.lineTo(0,hg);mc.lineTo(0,0);
            mc.endFill();
        }
        fill('scr');fill('msk');setMask(msk);
        attachMovie('map_gun','map_gun',getNextHighestDepth());
        attachMovie('map_targets','targets',getNextHighestDepth());
        targets['trg_w']=wd*target_w/width;targets['trg_h']=hg*target_h/(2*height);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"map_screen");
    //map_targets
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var map=ep.map;
        var mClips=ep.env.zones.clips;
        onEnterFrame=function(){
            clear();
            for(var mc_name in mClips){
                var mc=mClips[mc_name];
                var x=map.ep_x_to_map_x(mc._x);var y=map.ep_y_to_map_y(mc._y);
                moveTo(x,y);
                beginFill(0x00ff00);
                lineTo(x+trg_w,y);lineTo(x+trg_w,y+trg_h);
                lineTo(x,y+trg_h);lineTo(x,y);
                endFill();
            }
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"map_targets");
    //map gun
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var map=ep.map;
        _x=map.ep_x_to_map_x(0);_y=map.ep_y_to_map_y(0);
        var r=4;
        lineStyle(1,0xff0000);
        com.drawCircle_offset(this,r,0-r);
        createEmptyMovieClip('arrow',getNextHighestDepth());
        function draw_gun(){
            arrow.clear();
            arrow.lineStyle(2,0xff0000);
            arrow.lineTo(0,0-(2*r));
            arrow._rotation=ep.railRobot.gun._rotation;
        }
        draw_gun();
        map['draw_gun']=draw_gun;
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"map_gun");

    //
    actionf(buf,"var width=%u;var height=%u",width,height);
    actionf(buf,"var railRobot_w=%u;var railRobot_h=%u",utilaj_sz[0],utilaj_sz[1]);
    actionf(buf,"var rail_x=%u",rail_x);
    action(R"(
        var yOff=20+(railRobot_h/2);
        _x=width/2;_y=height-yOff;
        createEmptyMovieClip('env',getNextHighestDepth());
        env.attachMovie('rail_bg','rail_bg',env.getNextHighestDepth());
        env.createEmptyMovieClip('smokes',env.getNextHighestDepth());
        env.createEmptyMovieClip('bullets',env.getNextHighestDepth());
        env.attachMovie('zones','zones',env.getNextHighestDepth());
        env.attachMovie('railRobot','railRobot',env.getNextHighestDepth());
        //asta e scurtatura ca peste tot las ep.railRobot
        var railRobot=env.railRobot;
        attachMovie('map','map',getNextHighestDepth());
        //
        railRobot._x=0-(railRobot_w/2);railRobot._y=0-(railRobot_h/2);
        //e 3/2 , railul e 3*height ca sa acopere si diagonalele si aicea e jumate
        env.rail_bg._x=rail_x;env.rail_bg._y=0-(3/2*height);
    )");

    swf_exports_done();
    swf_showframe();
    swf_done();
	printf("\ndone");
    return 1;
}

#ifdef __cplusplus
}
#endif
