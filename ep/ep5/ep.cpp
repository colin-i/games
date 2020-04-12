
#include<windows.h>
#include<stdio.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

#define launcher_w 40
#define launcher_h 40

#define circle_lat 30

int _start(){
    char* ep_nm=Balls_Play_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    action("_root['episode']=this");
    init_episod(ep_nm);

    //launcher
    int launcher_texture_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/5/launcher.dbl",launcher_texture_sz);
    swf_exports_add(dbl,"solid_texture");

    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var launcher_w=%u;var launcher_h=%u",launcher_w,launcher_h);
    actionf_sprite(presprite,buf,"var launcher_texture_w=%u",launcher_texture_sz[0]);
    actionf_sprite(presprite,buf,"var fps=%u;var width=%u;var circle_lat=%u",fps,width,circle_lat);

    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var launcher_texture=flash.display.BitmapData.loadBitmap('solid_texture');
        var matrix=new flash.geom.Matrix();
        var main_color=0x8080ff;
        var line_color=main_color;var line_size=1;var line_sz=circle_lat/2;
        //
        createEmptyMovieClip('mc_left',getNextHighestDepth());
            //
        mc_left.moveTo(launcher_w/4,0-line_sz);
        mc_left.lineStyle(line_size,line_color);
        mc_left.lineTo(launcher_w/4,launcher_h);
        mc_left.lineStyle();
            //
        mc_left.beginBitmapFill(launcher_texture,matrix,true);
        mc_left.moveTo(launcher_texture_w,0);
        mc_left.curveTo(0,0,0,launcher_h/2);
        mc_left.lineTo(0,launcher_h);mc_left.lineTo(launcher_texture_w,launcher_h);mc_left.lineTo(launcher_texture_w,0);
        mc_left.endFill();
        //
        var pusher_step=launcher_h/(2*fps);
        var pusher_max=launcher_h/2;var pusher_line=pusher_max;
        var pusher_w=launcher_w-(2*launcher_texture_w);
        createEmptyMovieClip('pusher',getNextHighestDepth());pusher._x=launcher_texture_w;
        function draw_pusher(){
            pusher.clear();
            //
            pusher.beginFill(main_color);
            pusher.moveTo(0,pusher_line);
            pusher.lineTo(pusher_w,pusher_line);pusher.lineTo(pusher_w,launcher_h);pusher.lineTo(0,launcher_h);pusher.lineTo(0,pusher_line);
            pusher.endFill();
        }
        draw_pusher();
        //
        createEmptyMovieClip('mc_right',getNextHighestDepth());mc_right._x=pusher._x+pusher_w;
            //
        mc_right.moveTo(launcher_texture_w-(launcher_w/4),0-line_sz);
        mc_right.lineStyle(line_size,line_color);
        mc_right.lineTo(launcher_texture_w-(launcher_w/4),launcher_h);
        mc_right.lineStyle();
            //
        mc_right.moveTo(0,0);
        mc_right.beginBitmapFill(launcher_texture,matrix,true);
        mc_right.curveTo(launcher_texture_w,0,launcher_texture_w,launcher_h/2);
        mc_right.lineTo(launcher_texture_w,launcher_h);mc_right.lineTo(0,launcher_h);mc_right.lineTo(0,0);
        mc_right.endFill();

        var circle_x_off=ep.fix_dist;
        onEnterFrame=function(){
            if(Key.isDown(com.left_arrow)==true){
                //pun asta de fine pressed
                if(Key.isDown(com.jump_aux_key)==false){
                    left_move(ep.step);
                }
            }
            else if(Key.isDown(com.right_arrow)==true){
                //
                if(Key.isDown(com.jump_aux_key)==false){
                    right_move(ep.step);
                }
            }
        }
        function moving_slow(){
            if(Key.isDown(com.jump_aux_key)==true){
                if(Key.isDown(com.left_arrow)==true)left_move(1);
                else if(Key.isDown(com.right_arrow)==true)right_move(1);
            }
        }
        function left_move(dist){
            _x-=dist;
            if(_x<0)_x=0;
            circle_obj._x=_x+circle_x_off;
        }
        function right_move(dist){
            _x+=dist;
            if(_x>(width-launcher_w))_x=width-launcher_w;
            circle_obj._x=_x+circle_x_off;
        }

        Key.addListener(this);
        function ready_to_shot(){
            onKeyDown=function(){
                moving_slow();
                if(Key.getCode()==com.action_key){
                    //Key.removeListener(this);
                    onKeyDown=moving_slow;
                    pusher_line=0-line_sz;
                    circle_obj.go();
                    pusher.onEnterFrame=function(){
                        pusher_line+=pusher_step;
                        if(pusher_line>pusher_max){
                            pusher_line=pusher_max;
                            pusher.onEnterFrame=undefined;
                        }
                        draw_pusher();
                    }
                    ep.add_circle();
                }
            }
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"launcher");

    //circle
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var circle_lat=%u",circle_lat);
    actionf_sprite(presprite,buf,"var launcher_w=%u",launcher_w);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        ep.draw_circle(this,0xff0000);
        _x=ep.launcher._x+ep.fix_dist;_y=ep.height;
        var step=ep.circle_at_launcher_step;var y_dest=ep.launcher_y_coord;
        onEnterFrame=function(){
            _y-=step;
            if(_y<y_dest){
                _y=y_dest;
                onEnterFrame=undefined;
                ep.launcher.ready_to_shot();
            }
        }
        function go(){
            x_step=0;
            y_step=0-ep.circle_step;
            ep.circle_slowSpeed_set(this);
            onEnterFrame=function(){
                ep.circle_move(this);
                ep.main_target.circle_test(this);
            }
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"circle");

    //hole
    int hole_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/5/galaxy.dbl",hole_sz);
    swf_exports_add(dbl,"galaxy");

    //circle_target
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var hole_w=%u;var hole_h=%u;var circle_lat=%u",hole_sz[0],hole_sz[1],circle_lat);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var color=0x00ff00;
        ep.draw_circle(this,color);

        var circle_test;
        function target_ready(bullet_circle){
            if(circle_hitTest(bullet_circle)==true){
                ep.next_target_overview();
                //
                var factor_step=circle_lat;
                //
                var x_factor=(bullet_circle._x-_x)*-1;
                var angle=90-(90*x_factor/factor_step);
                this.x_step=Math.cos(angle*Math.PI/180)*factor_step;
                this.y_step=Math.sin(angle*Math.PI/180)*factor_step*-1;
                ep.circle_slowSpeed_set(this);
                onEnterFrame=this[ep.targets.target_motion];
                bullet_circle.removeMovieClip();
            }
        }
        function circle_hitTest(bullet_circle){
            var circle_bounds=bullet_circle.getBounds(_root);
            var target_bounds=getBounds(_root);
            if(overlap_allowMargins(circle_bounds.xMin,circle_bounds.xMax,target_bounds.xMin,target_bounds.xMax)==false)return false;
                //asta era: if(circle_bounds.yMin<target_bounds.yMax)return true;
            //asta e cand trag pe langa si repede trag bine si vine targetul si cea rosie de sus sa nu mai faca hittestul
            return overlap_allowMargins(circle_bounds.yMin,circle_bounds.yMax,target_bounds.yMin,target_bounds.yMax);
        }
        //bool
        function overlap_allowMargins(pA1,pA2,pB1,pB2){
            //aicea e ok cu marginile
            if(pA2<=pB1)return false;
            //if(pB1<pA1)????????????
            if(pB2<=pA1)return false;
            return true;
        }

        function galaxy_zone(){
            if(hitTest(ep.galaxy)==true){
                var dist_to_hole=ep.fps;
                onEnterFrame=function(){
                    if(dist_to_hole!=1)dist_to_hole--;
                    else{
                        _xscale-=ep.shrink_step;_yscale-=ep.shrink_step;
                        if(_xscale<0)removeMovieClip();
                    }
                    //aicea se poate misca si galaxia si cercul in acelasi timp
                    var x_dist=ep.galaxy._x+(hole_w/2)-((circle_lat*_xscale/100)/2)-_x;
                    var y_dist=ep.galaxy._y+(hole_h/2)-((circle_lat*_yscale/100)/2)-_y;
                    _x+=x_dist/dist_to_hole;_y+=y_dist/dist_to_hole;
                }
            }
            else ep.circle_move(this);
        }
        //var x_center;var y_center;
        var color_gone_alpha=100;var color_gone_alpha_step=100/ep.fps;
        function defender_zone(){
            if(hitTest(ep.defender)==true)ep.target_reset();
            else if(hitTest(ep.gate)==true){
                ep.defender['circle_done']=true;
                onEnterFrame=function(){
                    clear();
                    ep.draw_circle_alpha(this,color,color_gone_alpha);
                    color_gone_alpha-=color_gone_alpha_step;
                    if(color_gone_alpha<0)removeMovieClip();
					else{
						lineStyle(1,0xffFFff);
						var circle_off=0;
						while(circle_off<(circle_lat/2)){
							com.drawCircle_offset(this,circle_lat/2-circle_off,circle_off);
							circle_off+=5;
						}
					}
                }
            }
            else ep.circle_move(this);
        }
        var motion_min_y=ep.motion_min_y;
        var motion_max_y=ep.motion_max_y;
        function do_motion(){
            if(_y<motion_min_y){_y=motion_min_y;y_step*=-1;}
            else if(_y>motion_max_y){_y=motion_max_y;y_step*=-1;}
            _y+=y_step;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"target");

    actionf(buf,"var step=%u;var launcher_w=%u;var launcher_h=%u;var circle_lat=%u",int(launcher_w*2/fps),launcher_w,launcher_h,circle_lat);
    actionf(buf,"var width=%u;var height=%u;var fps=%u",width,height,fps);
    actionf(buf,"var hole_w=%u;var hole_h=%u",hole_sz[0],hole_sz[1]);

    action(R"(
        com.space_env(this,'env',this.getNextHighestDepth());
        createEmptyMovieClip('rail',getNextHighestDepth());rail._y=height-(launcher_h/2);
        rail.lineStyle(1,0xffFFff);rail.lineTo(width,0);
        attachMovie('launcher','launcher',getNextHighestDepth());
        launcher._x=width/2;launcher._y=height-launcher_h;

        createEmptyMovieClip('galaxy',getNextHighestDepth());
        com.mc_rotation_bmp(flash.display.BitmapData.loadBitmap('galaxy'),galaxy,'hole',galaxy.getNextHighestDepth(),360/(12*fps));
        galaxy._x=(width-hole_w)/2;
        galaxy.hole.onEnterFrame=galaxy.hole.rotate_bmp;

        createEmptyMovieClip('targets',getNextHighestDepth());
        createEmptyMovieClip('circles',getNextHighestDepth());
        attachMovie('aux','aux',getNextHighestDepth());

        function add_circle(){
            var d=circles.getNextHighestDepth();
            launcher['circle_obj']=circles.attachMovie('circle','circle'+d,d);
        }
        function draw_circle(mc,color){
            draw_circle_alpha(mc,color,100);
        }
        function draw_circle_alpha(mc,color,alpha){
            var off=0;
            //fara lineStyle la primul ca devine cercul 30+0.5+0.5 width si da peste cap hitTestul, iar pe urma sunt calculele cu 30
            mc.beginFill(color,alpha);
            com.drawCircle_offset(mc,circle_lat/2,0);
            mc.endFill();
            //
            mc.lineStyle(1,0x0000ff);
            while(off<(circle_lat/2)){
                off+=5;
                com.drawCircle_offset(mc,circle_lat/2-off,off);
            }
        }
        var circle_step=circle_lat*(fps-2)/fps;var circle_step_slow=4/fps/circle_step;
        function circle_slowSpeed_set(mc){
            mc['x_slow']=circle_step_slow*mc.x_step;
            mc['y_slow']=circle_step_slow*mc.y_step;
        }
        function circle_move(mc){
            mc._x+=mc.x_step;mc.x_step-=mc.x_slow;
            mc._y+=mc.y_step;mc.y_step-=mc.y_slow;
            //nu e nevoie de asta dar daca se va face ceva si nu se rescrie aicea atunci vor fi buguri
            //          e dupa y negativ spre 0 dar poate fi invers si 0(dupa x)
            //de asta e nevoie doar la cercurile rosii
            if(mc.y_step>0)mc.removeMovieClip();
        }

        var circle_at_launcher_step=(height-launcher._y)/fps;
        var launcher_y_coord=launcher._y-circle_lat;
        var motion_min_y=galaxy._y+hole_h+circle_lat;
        var motion_max_y=launcher_y_coord-(2*circle_lat);var motion_step=2*circle_lat/fps;
        var fix_dist=(launcher_w-circle_lat)/2;
        var fix_dist_plus_circle=fix_dist+circle_lat;;
        function place_circles(pack_nr,motion){
            var pack_total=Math.ceil(nr_of_circles/pack_nr);
            var free_space=width-(circle_lat*pack_total)-(2*fix_dist_plus_circle);var free_space_per_circle=free_space/(pack_total-1);
            var i=0;
            var right_left=levelNr&1;
            if(right_left==0)var x=width-fix_dist_plus_circle;
            else var x=0;
            var j=0;while(j<pack_total){
                if(motion!=true)var prepare_y=height/2;
                else{
                    var prepare_y=motion_min_y+random(motion_max_y-motion_min_y);
                    if(random(2)==1)var prepare_y_step=0-motion_step;
                    else var prepare_y_step=motion_step;
                }
                if(right_left==0)x-=circle_lat;
                else x+=circle_lat;
                var k=0;while(k<pack_nr){
                    //
                    var mc=targets.attachMovie('target','target_'+levelNr+'_'+i,i);
                    mc._x=x;mc._y=prepare_y;
                    if(motion==true){mc.y_step=prepare_y_step;mc['is_moving']=true;}
                    //la gnash nu merge cu 0
                    mc._xscale=1;mc._yscale=1;
                    coords_basedOn_scale(mc._x+(circle_lat/2),mc._y+(circle_lat/2),mc);
                    set_raiseScale(mc,50);
                    //
                    k++;
                    i++;
                    if(i==nr_of_circles)return null;
                }
                if(right_left==0)x-=free_space_per_circle;
                else x+=free_space_per_circle;
                j++;
            }
        }

        function coords_basedOn_scale(x_center,y_center,obj){
            var x_sz=circle_lat*obj._xscale/100;
            obj._x=x_center-(x_sz/2);
            var y_sz=circle_lat*obj._yscale/100;
            obj._y=y_center-(y_sz/2);
        }

        var store_target_enterframe;
        var store_target_x;var store_target_y;
        //asta e la cand se misca
        var store_target_y_step;
        function next_target_overview(){
            store_target_enterframe=main_target.onEnterFrame;
            store_target_x=main_target._x;
            store_target_y=main_target._y;
            //asta e la cand se misca
            store_target_y_step=main_target.y_step;
            aux.reset_tip();
            targets.onEnterFrame=function(){
                                    //e gata cercul asta la <100 la galaxye
                if(main_target._xscale<100)put_next_target();
                                    //e gata cercul asta la <100 la gate hit
                else if(main_target.color_gone_alpha<100){put_next_target();defender.circle_done=true;}
                else if(main_target.hitTest(env)==false)target_reset();
            }
        }
        function put_next_target(){
            distanceStyle_set_next_target();
            targets.onEnterFrame=undefined;
        }
        function target_reset(){
            main_target.onEnterFrame=store_target_enterframe;
            main_target._x=store_target_x;
            main_target._y=store_target_y;
            //asta e la cand se misca
            main_target.y_step=store_target_y_step;
            aux.set_tip();
            targets.onEnterFrame=undefined;
        }
        var main_target;
        function distanceStyle_set_next_target(){
            distanceStyle_pos--;
            if(distanceStyle_pos>=0){
                main_target=targets['target_'+levelNr+'_'+distanceStyle_pos];
                //asta trebuie la cand se misca
                current_center(main_target,circle_lat,circle_lat);
                main_target._xscale=100;main_target._yscale=100;
                coords_basedOn_scale(main_target.x_center,main_target.y_center,main_target);
                //cand trece la 100 si cercul este deasupra la main target nu conteaza
                main_target.circle_test=main_target.target_ready;
                aux.set_tip();
            }
            else current_circles_done();
        }
        function current_circles_done(){
            targets.onEnterFrame=undefined;
            new_circles();
        }

        //se calculeaza stepul in functie de cat de repede vine cercu urmator si viteza hiturilor sa fie de asa natura incat atunci cand a venit cercul urmator sa nu faca nici un contact cu urmatorul
        var standby_scale=50;
        var target_to_full_size_frames=(height-launcher_y_coord)/circle_at_launcher_step;
        var target_to_full_size_step=(100-standby_scale)/target_to_full_size_frames;
        function set_raiseScale(mc,scale_number){
            mc.scale_number=scale_number;
            mc.onEnterFrame=function(){
                current_center(this,circle_lat,circle_lat);
                //doar cu scale+= si verific scale poate e 99.9999 si mai trebuie inca o frame sa mai astept
                this._xscale+=target_to_full_size_step;
                if(this._xscale>this.scale_number){
                    this._xscale=this.scale_number;this._yscale=this.scale_number;
                    if(this.is_moving!=true)this.onEnterFrame=undefined;
                    else this.onEnterFrame=this.do_motion;
                    if(distanceStyle_pos==nr_of_circles){distanceStyle_set_next_target();defender.circle_to_stop(main_target);}
                }
                else this._yscale+=target_to_full_size_step;
                coords_basedOn_scale(this.x_center,this.y_center,this);
                if(this.is_moving==true)this.do_motion();
            }
        }

        function current_center(mc,w,h){
            var x_sz=w*mc._xscale/100;
            var y_sz=h*mc._yscale/100;
            mc.x_center=mc._x+(x_sz/2);
            mc.y_center=mc._y+(y_sz/2);
        }

        var shrink_step=100/fps;
        function shrink_galaxy(){
            galaxy.onEnterFrame=function(){
                current_center(this,hole_w,hole_h);
                this._xscale-=shrink_step;this._yscale-=shrink_step;
                if(this._xscale<0)this.removeMovieClip();
                else{
                    var x_sz=hole_w*this._xscale/100;this._x=this.x_center-(x_sz/2);
                    var y_sz=hole_h*this._yscale/100;this._y=this.y_center-(y_sz/2);
                }
            }
        }

        var defender_w=3*circle_lat;var defender_h=10;
        function add_defender(){
            var defender_zone_step=2*fps;
            attachMovie('defender','defender',getNextHighestDepth());
            defender._x=(width-defender_w)/2;defender._y=0-defender_h;
            var defender_dest=5;var defender_step=(defender_h+defender_dest)/defender_zone_step;
            //
            createEmptyMovieClip('gate',getNextHighestDepth());
            var gate_line_sz=2;
            gate.lineStyle(gate_line_sz,0xff0000);
            var gate_off=1+(gate_line_sz/1);
            gate.moveTo(0,gate_off);
            gate.lineTo(width,gate_off);
            //pare ok si 100/.. chiar de incepe de la 1, ca 99/.. nu e mare chestie
            var gate_step=100/defender_zone_step;gate._xscale=1;
            //trebuie calculat prosteste 800/fps si da cu virgula si pleaca de la 1 si vine mai mult ca 100 ca altfel nu as mai pune gate._xscale=100;
            gate.onEnterFrame=function(){
                gate._xscale+=gate_step;
                if(gate._xscale>100){gate.onEnterFrame=undefined;gate._xscale=100;defender._y=defender_dest;}
                else defender._y+=defender_step;
            }
        }

        var nr_of_circles=10;
        var levelNr=-1;
        //
        var distanceStyle_pos;
        function new_circles(){
            levelNr++;
            if(levelNr==0){
                targets.target_motion='galaxy_zone';
                place_circles(1);
            }
            else if(levelNr==1)place_circles(1,true);
            else if(levelNr==2)place_circles(3,true);
            //
            else if(levelNr==3){
                galaxy['step']=hole_w/(2*fps);
                galaxy.onEnterFrame=function(){
                    this._x+=this.step;
                    if(this._x>(width-hole_w))this.step*=-1;
                    else if(this._x<0)this.step*=-1;
                }
                place_circles(1);
            }
            else if(levelNr==4)place_circles(1,true);
			else if(levelNr==5)place_circles(3);
            else if(levelNr==6)place_circles(3,true);
            //
            else if(levelNr==7){
                targets.target_motion='defender_zone';
                shrink_galaxy();
                add_defender();
                place_circles(1);
            }
            else if(levelNr==8)place_circles(1,true);
            else if(levelNr==9)place_circles(3,true);
            else _root.end_scenario();
            distanceStyle_pos=nr_of_circles;
        }
        add_circle();
        new_circles();
    )");

    //defender
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var circle_lat=%u;var width=%u",circle_lat,width);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var x_dif=(ep.defender_w-circle_lat)/2;
        var step=circle_lat*4/ep.fps;

        var defender_texture=flash.display.BitmapData.loadBitmap('solid_texture');
        var defender_matrix=new flash.geom.Matrix();
        beginBitmapFill(defender_texture,defender_matrix,true);
        lineTo(ep.defender_w,0);lineTo(ep.defender_w,ep.defender_h);
        lineTo(0,ep.defender_h);lineTo(0,0);
        endFill();

        var screen_circle;
        function circle_to_stop(circle){
            screen_circle=circle;
            onEnterFrame=function(){
                if(circle_done==true){circle_done=false;see_next_circle();}
                var ball_x=screen_circle._x;
                var defender_best_x=ball_x-x_dif;
                if(defender_best_x<_x){
                    _x-=step;if(defender_best_x>_x)_x=defender_best_x;
                    //ok cu stepul si egalul
                    if(ball_x+circle_lat<=0)see_next_circle();
                }
                else if(defender_best_x>_x){
                    _x+=step;if(defender_best_x<_x)_x=defender_best_x;
                    //ok = si cu stepul
                    if(ball_x>=width)see_next_circle();
                }
            }
        }
        function see_next_circle(){
            if(ep.main_target==undefined)onEnterFrame=undefined;
            else{screen_circle=ep.main_target;}
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"defender");

    //aux
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var circle_lat=%u",circle_lat);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var last_x;var last_y;
        //var mover=0;
        function draw_tip(){
            var launcher_circle_x=ep.launcher._x+ep.fix_dist;
            var target_x=ep.main_target._x;
            //
            clear();
            lineStyle(2,0xffFFff,50);
            //ok cu egalul si cand nu mai este tip
            if(launcher_circle_x<target_x)if(launcher_circle_x+circle_lat<=target_x)return null;
            if(target_x+circle_lat<=launcher_circle_x)return null;
            //
            var target_y=ep.main_target._y;
            //
            var x_factor=(launcher_circle_x-target_x)*-1;
            var angle=90-(90*x_factor/circle_lat);
            var tip_w=Math.cos(angle*Math.PI/180)*(circle_lat/2);
            var tip_h=Math.sin(angle*Math.PI/180)*(circle_lat/2);
            //
            _x=target_x+(circle_lat/2);_y=target_y+(circle_lat/2);
            moveTo(tip_w,0-tip_h);
            lineTo(tip_w+tip_w,0-tip_h-tip_h);
        }
        function reset_tip(){
            clear();
            onEnterFrame=undefined;
        }
        function set_tip(){
            onEnterFrame=draw_tip;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"aux");

    swf_exports_done();
    swf_showframe();
    swf_done();
	printf("\ndone");
    return 1;
}

#ifdef __cplusplus
}
#endif
