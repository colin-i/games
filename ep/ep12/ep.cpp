

#include <Windows.h>
#include<stdio.h>
#include <Math.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

int _start(){
    char* ep_nm=Space_Zones_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    //swf_new("ep.swf",width,height,0x01ff01,fps);
    action("_root['episode']=this");
    init_episod(ep_nm);

    //buguri cu widthul si gcc
    //int wdt=width;

    #define bullet_step height/(2*fps)
    #define rot_step 90/fps
    #define zone_w width
    //ship
    //gun
    int gun_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/12/gun.dbl",gun_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(gun_sz[0]/2),0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"gun");
    //ship_body
    int ship_body_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/12/ship.dbl",ship_body_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(ship_body_sz[0]/2),0-(ship_body_sz[1]/2));
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_body");
    //
    int step=4*ship_body_sz[0]/fps;
    int bullet_r=(gun_sz[0]-2)/2;
    int gun_dist=ship_body_sz[1]/2;
    int backFuel_nr=ship_body_sz[1]/2;
    //
    int ship_down_dist=backFuel_nr+(ship_body_sz[1]/2);
    int ship_down_y=height-ship_down_dist;
	//
	automatic(1);
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var shot_interval=%u;var step=%u", shot_interval,step);
    actionf_sprite(presprite,buf,"var body_w=%u;var body_h=%u",ship_body_sz[0],ship_body_sz[1]);
    actionf_sprite(presprite,buf,"var backFuel_nr=%u;var rot_step=%u;var gun_dist=%u",backFuel_nr,rot_step,gun_dist);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var body_half_w=body_w/2;var body_half_h=body_h/2;
        //
        com.add_fuel(body_w,backFuel_nr,1,'backFuel',this);
        backFuel._x=0-body_half_w;backFuel._y=body_half_h;
        //                                                    atata e pus cu Paintul
        var front_fuel_w=body_w/4;var front_fuel_h=body_h;var x_inset=2;
        com.add_fuel(front_fuel_w,front_fuel_h,-1,'frontLeftFuel',this);
        frontLeftFuel._x=0-body_half_w+x_inset;
        com.add_fuel(front_fuel_w,front_fuel_h,-1,'frontRightFuel',this);
        frontRightFuel._x=body_half_w-front_fuel_w-x_inset;
        //
        attachMovie('ship_body','body',getNextHighestDepth());
        body.attachMovie('gun','gun',body.getNextHighestDepth());body.gun._y=0-gun_dist;
        //
        var rotVal;
		function shot_func(){
			if(shot_timer==undefined){
				var d=ep.env.getNextHighestDepth();
                var mc=ep.env.attachMovie('bullet','bullet'+d,d);
                //asta ca sa se uite in acest env si nu in urmatorul cand se schimba zonele
                mc['bullet_env']=ep.env;
                createEmptyMovieClip('shot_timer',getNextHighestDepth());
                shot_timer['shot_interval']=shot_interval;
                shot_timer.onEnterFrame=function(){
					this.shot_interval--;if(this.shot_interval<0)this.removeMovieClip();
                }
            }
		}
        function ship_frame_f(){
            if(Key.isDown(com.action_key)==true)shot_func();
			else if(ep.auto_shot==true)shot_func();
            if(Key.isDown(com.up_arrow)==true){
                backFuel.fuelOn();
                move_fn(step);
            }
            else if(Key.isDown(com.down_arrow)==true){
                frontLeftFuel.fuelOn();
                frontRightFuel.fuelOn();
                move_fn(0-step);
            }
            if(Key.isDown(com.left_arrow)==true){
                if(Key.isDown(com.jump_aux_key)==true)rotVal=1;
                else rotVal=rot_step;
                ep.env_container._rotation+=rotVal;
            }
            else if(Key.isDown(com.right_arrow)==true){
                if(Key.isDown(com.jump_aux_key)==true)rotVal=1;
                else rotVal=rot_step;
                ep.env_container._rotation-=rotVal;
            }
        }
        onEnterFrame=ship_frame_f;
        Key.addListener(this);
        onKeyUp=function(){
            if(Key.getCode()==com.up_arrow){
                backFuel.clear();
            }
            else if(Key.getCode()==com.down_arrow){
                frontLeftFuel.clear();frontRightFuel.clear();
            }
        }
        function move_fn(val){
			var y_coef=Math.cos(ep.env_container._rotation*Math.PI/180);
            var x_coef=Math.sin(ep.env_container._rotation*Math.PI/180);
            var on_x=x_coef*val;
            var on_y=y_coef*val;
			//
			ep.rot_0();
			//
			ep.env._x+=on_x;
			var a=ep.margin_tester(body);
			if(a>0){
				if(on_x>0)ep.env._x-=a;
				else ep.env._x+=a;
			}else{
				for(var mc_name in ep.env.testObjects){
                    if(body.hitTest(ep.env.testObjects[mc_name])==true){
						var rct_mc=body.getBounds(ep);
						var rct=ep.env.testObjects[mc_name].getBounds(ep);
//plus 1 ca altfel face hittest si la y ca hitTest functia asta ia si marginile, plus ca e mai bine sa nu fie lipire de obiecte
						if(on_x>0)ep.env._x-=rct.xMax-rct_mc.xMin+1;
						else ep.env._x+=rct_mc.xMax-rct.xMin+1;
						break;
					}
                }
			}
			//
			ep.env._y+=on_y;
			a=ep.margin_tester(body);
			if(a>0){
				if(on_y>0)ep.env._y-=a;
				else ep.env._y+=a;
			}else{
				for(var mc_name in ep.env.testObjects){
                    if(body.hitTest(ep.env.testObjects[mc_name])==true){
						var rct_mc=body.getBounds(ep);
						var rct=ep.env.testObjects[mc_name].getBounds(ep);
						if(on_y>0)ep.env._y-=rct.yMax-rct_mc.yMin+1;
						else ep.env._y+=rct_mc.yMax-rct.yMin+1;
						break;
					}
                }
			}
			//
			ep.rot_back();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship");
    //bullet
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var step=%u",bullet_step);
    actionf_sprite(presprite,buf,"var height=%u",height);
    actionf_sprite(presprite,buf,"var bullet_r=%u;var gun_dist=%u",bullet_r,gun_dist);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var color=0xff0000;
        beginFill(color);
        com.drawCircle_offset(this,bullet_r,0-bullet_r);
        endFill();
        //
        var x_nr=0-Math.sin(ep.env_container._rotation*Math.PI/180);
        var y_nr=0-Math.cos(ep.env_container._rotation*Math.PI/180);
        var dist_val=gun_dist+bullet_r;
        _x=0-bullet_env._x+(x_nr*dist_val);_y=0-bullet_env._y+(y_nr*dist_val);
        var x_step=x_nr*step;var y_step=y_nr*step;
        //compar doar dupa cel mai mare max ca cel mai mic max cand e foarte aproape de 0 mai da cu bug si dispare glontul
        var safe_bool=Math.abs(x_nr)<Math.abs(y_nr);
        var coord_max=safe_bool?(height*y_nr+_y):(height*x_nr+_x);
        var hit_bool=false;
        onEnterFrame=function(){
            var comparation=safe_bool?(_y<coord_max):(_x<coord_max);
            _x+=x_step;_y+=y_step;
            var comparation_again=safe_bool?(_y<coord_max):(_x<coord_max);
            if(comparation!=comparation_again)hit_bool=true;
            //
            if(hit_bool==false){
                ep.rot_0();
                var tg=bullet_env.targets;
                for(var mc_name in tg){
                    var mc=tg[mc_name];
                    if(hitTest(mc)==true){
                        mc.hitTest_function();
                        hit_bool=true;
                        break;
                    }
                }
                ep.rot_back();
            }
            if(hit_bool==true)removeMovieClip();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bullet");

    //margins
    //unu cand e inainte si 2 ca e intre numerele rezolutiei, 800 si 600
    #define outOff 2*height
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        attachMovie('margin','up',getNextHighestDepth());
        up._x=ep.env.x_start;up._y=ep.env.y_start;
        up['hFlip']=true;
        up['horiz']=true;
        attachMovie('margin','right',getNextHighestDepth());
        right._x=up._x*-1;right._y=up._y;
        right['hAdd']=true;
        attachMovie('margin','down',getNextHighestDepth());
        down._x=right._x;down._y=up._y*-1;
        down['wFlip']=true;
        down['horiz']=true;
        attachMovie('margin','left',getNextHighestDepth());
        left._x=up._x;left._y=down._y;
        left['wFlip']=true;left['hFlip']=true;
        left['hAdd']=true;
        ep.env_loaded();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"margins");

    //margin
    dbl=swf_dbl("C:/Desktop/universe/img/12/margin.dbl");
    swf_exports_add(dbl,"margin_texture");
    action("var margin_texture=flash.display.BitmapData.loadBitmap('margin_texture')");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var height=%u",height);
    actionf_sprite(presprite,buf,"var outOff=%u",outOff);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var wd;var hg;
        if(horiz==true){wd=ep.env.zone_w+outOff;hg=outOff;}
        else{wd=outOff;hg=ep.env.zone_h+outOff;}
        if(wFlip==true)wd*=-1;
        if(hFlip==true)hg*=-1;
        var margin_matrix=new flash.geom.Matrix();
        beginBitmapFill(ep.margin_texture,margin_matrix,true);
        lineTo(wd,0);lineTo(wd,hg);
        lineTo(0,hg);lineTo(0,0);
        endFill();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"margin");

    //platform margin
    int platforma_de_zone_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/12/platforma_de_zone.dbl",platforma_de_zone_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(platforma_de_zone_sz[0]/2),0-(platforma_de_zone_sz[1]/2));
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    actionf_sprite(presprite,buf,"var platform_h=%u",platforma_de_zone_sz[1]);
    actionf_sprite(presprite,buf,"var zoneDone_step=%u",4*step);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        _y+=platform_h/2;
        function hit_testing_the_ship(){
            if(hitTest(ep.ship.body)==true){
                var sh=ep.ship;
                sh.onEnterFrame=undefined;
                sh.backFuel.clear();sh.frontLeftFuel.clear();sh.frontRightFuel.clear();
                //
                ep.env_container._rotation=0;
                _x+=(ep.env._x*-1)-_x;
                _y+=(ep.env._y*-1)-_y;
                //
                onEnterFrame=undefined;
                ep.new_zone();
            }
        }
        var y_step;var x_step;var frames_nr=2*fps;var frames;frames=frames_nr;
        function move_platform_set(y_coord){
            y_step=-1*y_coord/frames;
            x_step=-1*ep.prev_env._x/frames;
            onEnterFrame=function(){
                ep.prev_env._x+=x_step;ep.prev_env._y+=y_step;
                ep.env._x+=x_step;ep.env._y+=y_step;
                _x-=x_step;_y-=y_step;
                frames--;
                if(frames==0){
                    ep.env.zone_function();
                    ep.ship.onEnterFrame=ep.ship.ship_frame_f;
                    frames=frames_nr;
                    onEnterFrame=function(){
                        _y+=y_step;
                        frames--;
                        if(frames==0)ep.prev_env.removeMovieClip();
                    }
                }
            }
        }
        if(zone_done!=true)onEnterFrame=hit_testing_the_ship;
        else onEnterFrame=function(){
            ep.rot_0();
            //
            var rect=ep.ship.body.getBounds(this);
            var xPos=(rect.xMin+rect.xMax)/2;var yPos=(rect.yMin+rect.yMax)/2;
            var angle=Math.atan2(yPos,xPos)*180/Math.PI;
            _x+=Math.cos(angle*Math.PI/180)*zoneDone_step;
            _y+=Math.sin(angle*Math.PI/180)*zoneDone_step;
            //
            ep.rot_back();
            hit_testing_the_ship();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"platform_margin");

    //platform room
    int platforma_de_camera_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/12/platforma_in_zona.dbl",platforma_de_camera_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(platforma_de_camera_sz[0]/2),0-(platforma_de_camera_sz[1]/2));
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"platform_room");
    //driver
    int driver_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/12/enemy.dbl",driver_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(driver_sz[0]/2),0-(driver_sz[1]/2));
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"enemy");

    //fullRoad
    #define activator_r 10
    int platforms_allNr=(10*height)/platforma_de_camera_sz[1];
    int fullRoad_h=platforms_allNr*platforma_de_camera_sz[1];
    //fullRoad_h=200;
    actionf(buf,"var fullRoad_h=%u",fullRoad_h);
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var platforms_nr=%u",platforms_allNr-1);
    actionf_sprite(presprite,buf,"var platform_lat=%u",platforma_de_camera_sz[0]);
    actionf_sprite(presprite,buf,"var ship_lat=%u",ship_body_sz[0]);
    actionf_sprite(presprite,buf,"var activator_r=%u",activator_r);
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    #define driver_step 2*driver_sz[0]/fps
    actionf_sprite(presprite,buf,"var height=%u",height);
    actionf_sprite(presprite,buf,"var outOff=%u",outOff);
    actionf_sprite(presprite,buf,"var driver_step=%u",driver_step);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        createEmptyMovieClip('platforms',getNextHighestDepth());
        var y_bottom=y_start+zone_h;
        var platformHalf=platform_lat/2;
        var y_nr=y_bottom-platformHalf;var d;var mc;var i;
        i=0;while(i<platforms_nr){
            d=platforms.getNextHighestDepth();
            mc=platforms.attachMovie('platform_room','platform'+d,d);mc._y=y_nr;
            y_nr-=platform_lat;
            i++;
        }
        var x_off=platformHalf;var sz_h=y_bottom-y_start;
        var testObjects_matrix=new flash.geom.Matrix();
        createEmptyMovieClip('testObjects',getNextHighestDepth());
        function inter_margin(x_coord,wd){
            d=testObjects.getNextHighestDepth();
            mc=testObjects.createEmptyMovieClip('obj'+d,d);
            mc._x=x_coord;mc._y=y_start;
            mc._alpha=50;
            mc.beginBitmapFill(ep.margin_texture,testObjects_matrix,true);
            mc.lineTo(wd,0);mc.lineTo(wd,sz_h);
            mc.lineTo(0,sz_h);mc.lineTo(0,0);
            mc.endFill();
        }
        inter_margin(x_off,ship_lat);
        inter_margin(0-x_off-ship_lat,ship_lat);
        //
        var side_road_x=x_off+ship_lat+platformHalf;
        x_off=side_road_x+platformHalf;
        inter_margin(x_off,platform_lat);
        inter_margin(0-x_off-platform_lat,platform_lat);
        //targhete
        var targets_index=0;
        var move_step=side_road_x/fps;
        createEmptyMovieClip('targets',getNextHighestDepth());
        var mv;
        x_off+=ship_lat+activator_r;
        i=0;while(i<platforms_nr){
            mc=platforms.getInstanceAtDepth(i);
            d=targets.getNextHighestDepth();
            mv=targets.attachMovie('activator_fR','activator'+d,d);
            mv._x=x_off;mv._y=mc._y;
            mv['sense']=1;
            d=targets.getNextHighestDepth();
            mv=targets.attachMovie('activator_fR','activator'+d,d);
            mv._x=0-x_off;mv._y=mc._y;
            mv['sense']=-1;
            i++;
        }
        //masini de inceput
        var y_drivers=y_bottom-height-ship_lat;var y_drivers_start=y_start-outOff;
        var y_driver_center=y_drivers;
        function add_driver(x_pos,y_pos){
            d=getNextHighestDepth();
            mc=attachMovie('driver_fR','driver_fR'+d,d);mc._y=y_pos;
            mc._x=x_pos;
        }
        var inter_drivers=2*height;
        while(y_drivers_start<y_driver_center){
            add_driver(0,y_driver_center);
            y_driver_center-=inter_drivers;
        }
        var y_driver_sides=y_drivers-(inter_drivers/2);
        while(y_drivers_start<y_driver_sides){
            add_driver(side_road_x,y_driver_sides);
            add_driver(0-side_road_x,y_driver_sides);
            y_driver_sides-=inter_drivers;
        }
        onEnterFrame=function(){
            y_driver_center+=driver_step;y_driver_sides+=driver_step;
            if(y_drivers_start<y_driver_center){
                add_driver(0,y_driver_center);
                y_driver_center-=inter_drivers;
            }
            else if(y_drivers_start<y_driver_sides){
                add_driver(side_road_x,y_driver_sides);
                add_driver(0-side_road_x,y_driver_sides);
                y_driver_sides-=inter_drivers;
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fullRoad");
    //activator_fR
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var activator_r=%u",activator_r);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //
        lineStyle(3,0x0000ff);
        var matrix=new flash.geom.Matrix();
        var colors=new Array(0xff0000,0x00ff00);
        var alphas=new Array(100,100);
        var ratios=new Array(0,255*activator_r/Stage.width);
        beginGradientFill('radial',colors,alphas,ratios,matrix);
        com.drawCircle_offset(this,activator_r,0-activator_r);
        endFill();
        //
        var dist=Math.sin(45*Math.PI/180)*activator_r;
        moveTo(dist,dist);lineTo(0-dist,0-dist);
        moveTo(dist,0-dist);lineTo(0-dist,dist);
        //
        var en=ep.env;
        var step=en.move_step*sense;
        function hitTest_function(){
            en.targets_index+=sense;
            //cand se duce la -2 si 2:
            en.targets_index=Math.min(Math.abs(en.targets_index),1)*sense;
            var dest=en.targets_index*en.side_road_x;
            var dist=dest-en.platforms._x;
            //aicea e asa cu !=0, inauntru ajunge la 0, cand e la maxim de stanga si e hit, ar fi 0<0 false si dist-=(-n), dist<0 false si ar pleca spre stanga in decor
            if(dist!=0){
                en.targets.onEnterFrame=function(){
                    var a=dist<0;
                    dist-=step;
                    if(a==(dist<0)){
                        moveItems(step);
                    }
                    else{
                        moveItems(dest-en.platforms._x);
                        this.onEnterFrame=undefined;
                    }
                }
            }
        }
        function moveItems(val){
            en.platforms._x+=val;
            en.testObjects._x+=val;
            ep.env._x-=val;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"activator_fR");
    //driver_fR
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var step=%u;var driver_w=%u;var driver_h=%u",driver_step,driver_sz[0],driver_sz[1]);
    actionf_sprite(presprite,buf,"var outOff=%u",outOff);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var bottom=ep.env.y_start+ep.env.zone_h+outOff;
        attachMovie('enemy','enemy',getNextHighestDepth());
        //
        com.add_fuel(driver_w,driver_h/2,1,'backFuel',this);backFuel._x=0-(driver_w/2);backFuel._y=driver_h/2;
        _rotation=180;
        onEnterFrame=function(){
            backFuel.fuelOn();
            _y+=step;
            if(bottom<_y)removeMovieClip();
            else{
                ep.rot_0();
                var player=ep.ship.body;
                if(enemy.hitTest(player)==true){
                    var comp_rect=enemy.getBounds(ep);
                    var player_rect=player.getBounds(ep);
                    if(player_rect.yMax<comp_rect.yMax){
                        var rc=ep.env.margins.down.getBounds(ep);
                        if(comp_rect.yMax<rc.yMin){
                            ep.env._y-=comp_rect.yMax-player_rect.yMax;
                        }
                    }
                }
                ep.rot_back();
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"driver_fR");

    //holeTrap
    //hole
    int hole_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/5/galaxy.dbl",hole_sz);
    swf_exports_add(dbl,"galaxy");
    int hole_lat=driver_sz[0];
    actionf(buf,"var holeTrap_enterDist=%u",(hole_lat/2)+(ship_body_sz[0]/2));
    actionf(buf,"var holeTrap_h=%u",zone_w);
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    actionf_sprite(presprite,buf,"var hole_lat=%u;var galaxy_lat=%u",hole_lat,hole_sz[0]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        createEmptyMovieClip('testObjects',getNextHighestDepth());
        com.mc_rotation_bmp_coords(flash.display.BitmapData.loadBitmap('galaxy'),testObjects,'hole',testObjects.getNextHighestDepth(),360/(12*fps),0,0);
        var hl=testObjects.hole;
        hl.onEnterFrame=hl.rotate_bmp;
        hl._xscale=100*hole_lat/galaxy_lat;hl._yscale=hl._xscale;
        var ship_number=0;
        function zone_function(){
            if(ship_number<10){
                var d=getNextHighestDepth();
                attachMovie('enemy_holeTrap','enemy'+d,d);
                ship_number++;
            }
            else ep.platform_zone_done();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"holeTrap");
    //enemy_holeTrap
    int wd_pow= width/*wdt*//2;wd_pow*=wd_pow;
    int hg_pow=height/2;hg_pow*=hg_pow;
    double driver_offScreen_dist=sqrt(wd_pow+hg_pow)+(driver_sz[0]/2);
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var driver_w=%u;var driver_h=%u",driver_sz[0],driver_sz[1]);
    actionf_sprite(presprite,buf,"var enemy_offScreen_dist=%u",int(driver_offScreen_dist));
    actionf_sprite(presprite,buf,"var player_ship_step=%u",step);
    actionf_sprite(presprite,buf,"var toHole_frames=%u",fps);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        attachMovie('enemy','enemy',getNextHighestDepth());
        com.add_fuel(driver_w,driver_h/2,1,'backFuel',this);backFuel._x=0-(driver_w/2);backFuel._y=driver_h/2;
        //
        var onX=0-ep.env._x;var onY=0-ep.env._y;
        var ang=Math.atan2(onY,onX)*180/Math.PI;
        _x=onX+(Math.cos(ang*Math.PI/180)*enemy_offScreen_dist);
        _y=onY+(Math.sin(ang*Math.PI/180)*enemy_offScreen_dist);
        //
        var step_factor=2;
        var step=player_ship_step/step_factor;
        var remaining_step=step-player_ship_step;
        var stepTaken=(remaining_step/2)*-1;
        //
        onEnterFrame=function(){
            backFuel.fuelOn();
            //
            ep.rot_0();_rotation=0;
            var rect=ep.ship.body.getBounds(this);
            var xPos=(rect.xMin+rect.xMax)/2;var yPos=(rect.yMin+rect.yMax)/2;
            var angle=Math.atan2(yPos,xPos)*180/Math.PI;
            _rotation=angle+90;
            var y_factor=Math.sin(angle*Math.PI/180);
            _y+=y_factor*step;
            var x_factor=Math.cos(angle*Math.PI/180);
            _x+=x_factor*step;
            //
            if(hitTest(ep.ship.body)==true){
                ep.env._y+=y_factor*stepTaken;
                ep.env._x+=x_factor*stepTaken;
            }
            else if(hitTest(ep.env.testObjects.hole)==true){
                var frames=toHole_frames;var x_anim_start=_x;var y_anim_start=_y;
                onEnterFrame=function(){
                    frames--;
                    if(frames>0){
                        var factor=frames/toHole_frames;
                        _xscale=100*factor;_yscale=_xscale;
                        _x=x_anim_start*factor;
                        _y=y_anim_start*factor;
                    }
                    else removeMovieClip();
                }
                ep.env.zone_function();
            }
            ep.rot_back();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"enemy_holeTrap");

    //curvesRoad
    #define nr_of_curves 10
    #define curvesRoad_vert height/2
    int curvesRoad_bucata=ship_body_sz[1]+curvesRoad_vert;
    int curvesRoad_side_sz=ship_body_sz[1];
    int curvesRoad_exitStation_h=2*curvesRoad_side_sz;
    int curvesRoad_h=ship_down_dist+(ship_body_sz[1]/2)+(nr_of_curves*curvesRoad_bucata)+curvesRoad_exitStation_h;
    actionf(buf,"var curvesRoad_h=%u",curvesRoad_h);
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var nr_of_curves=%u",nr_of_curves);
    actionf_sprite(presprite,buf,"var zone_w=%u;var side_sz=%u",zone_w,curvesRoad_side_sz);
    actionf_sprite(presprite,buf,"var curvesRoad_vert=%u",curvesRoad_vert);
    //off_cut doua: la fastarrow sa duca in a doua cu step de 5 cat e acum, body are 40 ca daca nu era ducea in perete
	actionf_sprite(presprite, buf, "var off_cut=%u", 2 * ship_body_sz[0]);
    //
    actionf_sprite(presprite,buf,"var exitStation_h=%u",curvesRoad_exitStation_h);
    actionf_sprite(presprite,buf,"var max_lat=%u",(width/*wdt*/>height? width/*wdt*/ :height));
    actionf_sprite(presprite,buf,"var y_dist=%u",ship_down_dist);
    //
    actionf_sprite(presprite,buf,"var arrows_step=%u",step/2);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var sense=random(2)*2-1;
        var horiz_randSz=(zone_w/2)-off_cut;horiz_randSz/=2;
        var yPos=y_start+zone_h-y_dist;var xPos=0;var mc;
        var vert_add=(side_sz/2)+curvesRoad_vert+(side_sz/2);
        //
        var fast_off=2*side_sz;
        //
        createEmptyMovieClip('road',getNextHighestDepth());
        //mc
        function add_road(f_off){
            var d=road.getNextHighestDepth();var mc=road.attachMovie('curvesRoad_part','road'+d,d);
            mc._y=yPos;mc._x=xPos;
            if(fasters==true){
                mc['fast_off']=f_off;
            }
            return mc;
        }
        var i=0;while(i<nr_of_curves){
            var rand=random(horiz_randSz)+horiz_randSz;
            var horix_add=rand*sense;
            mc=add_road(0);xPos=horix_add;
            mc['xLine']=xPos;mc['yLine']=0;
            mc=add_road(fast_off);yPos-=vert_add;
            mc['xLine']=0;mc['yLine']=0-vert_add;
            mc=add_road(fast_off);xPos=0;
            mc['xLine']=0-horix_add;mc['yLine']=0;
            sense*=-1;i++;
        }
		//inca una ca e greu la miscatoare la sfarsit
		mc=add_road(undefined);mc['xLine']=2*horiz_randSz*sense;mc['yLine']=0;
        //
        createEmptyMovieClip('exitStation',getNextHighestDepth());exitStation._x=0-(side_sz/2);exitStation._y=yPos-(side_sz/2);
        exitStation.beginFill(0x00ff00);
        exitStation.lineTo(side_sz,0);exitStation.lineTo(side_sz,0-exitStation_h);
        exitStation.lineTo(0,0-exitStation_h);exitStation.lineTo(0,0);
        exitStation.endFill();
        function prepared(){
            ep.rot_0();
            prepared_function();
            ep.rot_back();
        }
        function prepared_function(){
            var sh_body=ep.ship.body;
            if(roadTest()==true){
                if(fasters==true){
                    //poate sa stea intre drumuri
                    for(var mc_name in road){
                        var mv=road[mc_name];var mc=mv.fastArrows;
                        if(mc.hitTest(sh_body)==true){
                            ep.env._x-=Math.cos(mv._rotation*Math.PI/180)*arrows_step;
                            ep.env._y-=Math.sin(mv._rotation*Math.PI/180)*arrows_step;
                            break;
                        }
                    }
                }
                return null;
            }
            if(exitStation.hitTest(sh_body)==true)ep.platform_zone_done();
            else{
                attachMovie('police','police',getNextHighestDepth());
                var rc=sh_body.getBounds(this);
                police._y=rc.yMin-(2*max_lat);
            }
            onEnterFrame=undefined;
        }
        //bool
        function roadTest(){
            var sh_body=ep.ship.body;
            for(var mc_name in road){
                var mc=road[mc_name].testLine;
                if(mc.hitTest(sh_body)==true)return true;
            }
            return false;
        }
        function zone_function(){
            onEnterFrame=prepared;
            if(trap==true){
                //createEmptyMovieClip('testObjects',getNextHighestDepth());
                attachMovie('curvesRoad_trap','lavatrap',getNextHighestDepth());
                createEmptyMovieClip('targets',getNextHighestDepth());
                targets.attachMovie('curvesRoad_target','target',targets.getNextHighestDepth());
            }
        }
        //zone_function();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"curvesRoad");
    //curvesRoad_part
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var side_half=%u",curvesRoad_side_sz/2);
    action_sprite(presprite,R"(
        var sz=Math.max(Math.abs(yLine),Math.abs(xLine));
        //
        beginFill(0x000080,50);
        moveTo(0,0-side_half);
        lineTo(sz,0-side_half);lineTo(sz,side_half);
        lineTo(0,side_half);lineTo(0,0-side_half);
        endFill();
        //
        if(fast_off!=undefined){
            attachMovie('curvesRoad_fastObject','fastArrows',getNextHighestDepth());
            fastArrows['fast_off']=fast_off;fastArrows['sz']=sz;
        }
        //
        createEmptyMovieClip('testLine',getNextHighestDepth());
        testLine.lineStyle(1,0x00ff00);
        testLine.lineTo(sz,0);
        //
        _rotation=Math.atan2(yLine,xLine)*180/Math.PI;
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"curvesRoad_part");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var side_half=%u",curvesRoad_side_sz/2);
    action_sprite(presprite,R"(
        lineStyle(1,0x008000);
        var arrow_w=side_half/2;
        sz-=arrow_w;
        //+arrow_w ca sa nu mai ramana nimic sa nu se poata sta in margine
        while(fast_off<=(sz+arrow_w)){
            moveTo(fast_off,0-side_half);
            lineTo(fast_off+arrow_w,0);lineTo(fast_off,side_half);
            fast_off+=arrow_w;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"curvesRoad_fastObject");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var driver_w=%u;var driver_h=%u",driver_sz[0],driver_sz[1]);
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        attachMovie('enemy','driver',getNextHighestDepth());
        com.add_fuel(driver_w,driver_h/2,1,'backFuel',this);backFuel._x=0-(driver_w/2);backFuel._y=driver_h/2;
        var sh_rect;
        function bounds_get(){
            var old_rotation=_rotation;_rotation=0;
            sh_rect=ep.ship.body.getBounds(this);
            _rotation=old_rotation;
        }
        bounds_get();
        var step=sh_rect.yMin/fps;
        var ySeek;var xSeek;
        function get_coords(){
            bounds_get();
            ySeek=sh_rect.yMin+((sh_rect.yMax-sh_rect.yMin)/2);
            xSeek=sh_rect.xMin+((sh_rect.xMax-sh_rect.xMin)/2);
        }
        function direction(xVal,yVal){
            backFuel.fuelOn();
            _x+=xVal;
            _y+=yVal;
            var angle=Math.atan2(yVal,xVal)*180/Math.PI;
            _rotation=angle+90;
        }
        onEnterFrame=function(){
            ep.rot_0();
            //
            get_coords();
            //compar aicea ca altfel poate sa sara peste si sa nu mai faca hitTest
            var dist_to_ship=Math.sqrt((xSeek*xSeek)+(ySeek*ySeek));
            if(dist_to_ship>step){
                var ang=Math.atan2(ySeek,xSeek)*180/Math.PI;
                direction(Math.cos(ang*Math.PI/180)*step,Math.sin(ang*Math.PI/180)*step);
            }
            else{
                ep.ship.onEnterFrame=undefined;
                get_coords();
                _x+=xSeek;_y+=ySeek;
                var y_back=ep.env.y_start+ep.env.zone_h-ep.enter_dist;
                var frames=fps;
                var x_step=-1*ep.env._x/frames;var y_step=((-1*y_back)-ep.env._y)/frames;
                onEnterFrame=function(){
                    ep.env._x+=x_step;ep.env._y+=y_step;
                    direction(0-x_step,0-y_step);
                    frames--;
                    if(frames==0){
                        ep.ship.onEnterFrame=ep.ship.ship_frame_f;
                        ep.env.onEnterFrame=ep.env.prepared;
                        removeMovieClip();
                    }
                }
            }
            ep.rot_back();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"police");
    //curvesRoad_target
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var activator_r=%u",activator_r);
    actionf_sprite(presprite,buf,"var step=%u",360/(4*fps));
    actionf_sprite(presprite,buf,"var ship_lat=%u",ship_body_sz[0]);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //
        lineStyle(3,0x0000ff);
        var matrix=new flash.geom.Matrix();var colors=new Array(0xff0000,0x00ff00);var alphas=new Array(100,100);
        var ratios=new Array(0,255*activator_r/Stage.width);
        beginGradientFill('radial',colors,alphas,ratios,matrix);com.drawCircle_offset(this,activator_r,0-activator_r);
        endFill();
        var dist=Math.sin(45*Math.PI/180)*activator_r;
        moveTo(dist,dist);lineTo(0-dist,0-dist);
        moveTo(dist,0-dist);lineTo(0-dist,dist);
        //platforma _parent -> prev_env
        //asta _parent         la fel
        //->asta: remove aicea nu asteapta platforma
        var stored_env=ep.env;
        //
        var targetRadius=2*ship_lat;
        onEnterFrame=function(){
            if(stored_env!=ep.env){
                //lava sa ramana asa, adaca e pe sfarsite e ok acolo la remove, daca e sa apara direct nu ar fi bine asa ca e ok
                stored_env.lavatrap.onEnterFrame=stored_env.lavatrap.trap_over;
                removeMovieClip();
            }else{
                _rotation+=step;if(_rotation>360)_rotation-=360;
                var rect=ep.rot_getBounds(ep.ship.body,this._parent);
                var xCenter=rect.xMin+((rect.xMax-rect.xMin)/2);var yCenter=rect.yMin+((rect.yMax-rect.yMin)/2);
                _x=xCenter+(Math.cos(_rotation*Math.PI/180)*targetRadius);
                _y=yCenter+(Math.sin(_rotation*Math.PI/180)*targetRadius);
            }
        }
        function hitTest_function(){
            stored_env.lavatrap.trapDisabled=true;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"curvesRoad_target");
    //curvesRoad_trap
    int fire_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/12/fire.dbl",fire_sz);
    swf_exports_add(dbl,"fire_dbl");
    action("var fire_bmp=flash.display.BitmapData.loadBitmap('fire_dbl')");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var height=%u;var fps=%u",height,fps);
    actionf_sprite(presprite,buf,"var trap_duration=%u",10*fps);
    //
    actionf_sprite(presprite,buf,"var fire_h=%u",fire_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var trap_frames=0;
        var max_h=4*height;var half_h=max_h/2;
        var y_max;
        var trapDisabled;
        var redraw_frames;
        function trap_inactive(){
            trap_frames--;if(trap_frames>0)return null;
            //
            var rect=ep.rot_getBounds(ep.ship.body,this._parent);
            y_max=rect.yMin-half_h;
            _y=y_max-max_h;
            //trap_frames=trap_duration;
            trapDisabled=false;
            redraw_frames=0;
            onEnterFrame=trap_active;
        }
        _x=ep.env.x_start;
        onEnterFrame=trap_inactive;
        var step=max_h/(6*fps);
        var curves_sz=100;var wd=ep.env.zone_w;
        var mx=new flash.geom.Matrix();
        function trap_active(){
            //trap_frames--;if(trap_frames==0){onEnterFrame=trap_done;return null;}
            if(trapDisabled==true){onEnterFrame=trap_done;return null;}
            //
            if(_y<y_max)_y+=step;
            redraw_frames--;
            if(redraw_frames<0){
                var max_line=max_h-curves_sz;
                //
                mx.ty+=20;if(mx.ty>fire_h)mx.ty-=fire_h;
                //
                clear();
                beginBitmapFill(ep.fire_bmp,mx,true);
                lineTo(0,max_line);
                //
                var xPos=0;var nr=curves_sz;var mid;
                while(nr<wd){
                    var mid=(nr-xPos)/2;
                    curveTo(xPos+mid,max_h,nr,max_line);
                    xPos+=curves_sz;nr+=curves_sz;
                }
                mid=(wd-xPos)/2;curveTo(xPos+mid,max_h,wd,max_line);
                //
                lineTo(wd,0);lineTo(0,0);
                endFill();
                redraw_frames=fps;
            }
        }
        var alpha_step=100/fps;
        function trap_done(){
            _alpha-=alpha_step;
            if(_alpha<0){
                trap_frames=trap_duration;
                clear();
                _alpha=100;
                onEnterFrame=trap_inactive;
            }
        }
        function trap_over(){
            _alpha-=alpha_step;
            if(_alpha<0)removeMovieClip();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"curvesRoad_trap");

    //redX
    #define redX_lat 3*height
    actionf(buf,"var redX_lat=%u",redX_lat);
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var height=%u",height);
    actionf_sprite(presprite,buf,"var redX_lat=%u",redX_lat);
    //
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        createEmptyMovieClip('testObjects',getNextHighestDepth());
        var freeZone_to_margin=height;
        ep.stopObject(x_start,y_start,redX_lat,freeZone_to_margin);
        var y_stop_bottom=y_start+zone_h-freeZone_to_margin;
        ep.stopObject(x_start,y_stop_bottom,redX_lat,freeZone_to_margin);
        //
        var y_stop_mid=y_start+freeZone_to_margin;
        ep.stopObject(x_start,y_stop_mid,freeZone_to_margin,y_stop_bottom-y_stop_mid);
        ep.stopObject(x_start+zone_w-freeZone_to_margin,y_stop_mid,freeZone_to_margin,y_stop_bottom-y_stop_mid);
        //
        var radius=redX_lat/2-(height/2);
        var targets_on_90=4;var targets_nr=targets_on_90*4;
        var angle_add=360/targets_nr;var angle=0;
        createEmptyMovieClip('targets',getNextHighestDepth());
        while(targets_nr>0){
            var d=targets.getNextHighestDepth();
            var mc=targets.attachMovie('red_target','red_target'+d,d);
            mc._x=Math.cos(angle*Math.PI/180)*radius;
            mc._y=Math.sin(angle*Math.PI/180)*radius;
            angle+=angle_add;
            targets_nr--;
        }
        if(moving_targets==true){
            var step=angle_add/(2*fps);
            var frames_total=fps;
            var frames=frames_total;
            onEnterFrame=function(){
                targets._rotation+=step;
                frames--;
                if(frames==0){
                    frames=frames_total;
                    step*=-1;
                }
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"redX");
    //red_target
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var activator_r=%u",activator_r);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //
        lineStyle(3,0xff0000);
        var matrix=new flash.geom.Matrix();var colors=new Array(0x0000ff,0x00ff00);var alphas=new Array(100,100);
        var ratios=new Array(0,255*activator_r/Stage.width);
        beginGradientFill('radial',colors,alphas,ratios,matrix);
        com.drawCircle_offset(this,activator_r,0-activator_r);
        endFill();
        var dist=Math.sin(45*Math.PI/180)*activator_r;
        moveTo(dist,dist);lineTo(0-dist,0-dist);
        moveTo(dist,0-dist);lineTo(0-dist,dist);
        //
        function hitTest_function(){
            if(is_end()==true)ep.platform_zone_done();
            removeMovieClip();
        }
        //bool
        function is_end(){
            var test_end=0;
            //unu asta si cu unu si mai trebuie
            for(var mc_name in ep.env.targets){
                test_end++;
                if(test_end==2)return false;
            }
            return true;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"red_target");

    //backOrForward
    int backOrForward_freeZone_h=platforma_de_camera_sz[1];
    int backOrForward_deniedZone_h=ship_body_sz[1];
    #define backOrForward_nrOfZones 10
    int oneZone_h=backOrForward_freeZone_h+backOrForward_deniedZone_h;
    int backOrForward_h=oneZone_h*(1+backOrForward_nrOfZones+2);
    actionf(buf,"var backOrForward_h=%u",backOrForward_h);
    int backOrForward_freeZone_w=platforma_de_camera_sz[0]+(2*ship_body_sz[0])+platforma_de_camera_sz[0];
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var nr=%u",backOrForward_nrOfZones);
    actionf_sprite(presprite,buf,"var freeZone_w=%u;var oneZone_h=%u",backOrForward_freeZone_w,oneZone_h);
    actionf_sprite(presprite,buf,"var stopZone_h=%u",backOrForward_deniedZone_h);
    actionf_sprite(presprite,buf,"var platform_w=%u;var platform_h=%u",platforma_de_camera_sz[0],platforma_de_camera_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        createEmptyMovieClip('testObjects',getNextHighestDepth());
        var stop_margin_w=(zone_w-freeZone_w)/2;
        //
        ep.stopObject(x_start,y_start,stop_margin_w,zone_h);
        var right_stop_x=x_start+zone_w-stop_margin_w;
        ep.stopObject(right_stop_x,y_start,stop_margin_w,zone_h);
        var left_x=x_start+stop_margin_w;
        var right_x=right_stop_x-platform_w;
        //
        createEmptyMovieClip('platforms',getNextHighestDepth());
        var yPos=y_start+zone_h-oneZone_h;
        function add_platform(xPos,sense){
            var d=platforms.getNextHighestDepth();
            var mc=platforms.attachMovie('platform_backOrForward','platform'+d,d);
            mc._x=xPos;mc._y=yPos;mc['sense']=sense;
        }
        var i=0;while(i<nr){
            yPos-=platform_h;
            var rand_sense=random(2)*2-1;
            add_platform(left_x,rand_sense);add_platform(right_x,(rand_sense*-1));
            yPos-=stopZone_h;
            ep.stopObject(left_x,yPos,freeZone_w,stopZone_h);
            i++;
        }
    )");
    action_sprite(presprite,R"(
        function ship_hitTest(){
            ep.rot_0();
            for(var mc_name in platforms){
                if(platforms[mc_name].hitTest(ep.ship.body)==true){
                    platforms[mc_name].envTest_function();
        //asta este ca sa nu se mai uite dupa hitTesturi cand e deja pe o platforma
                    onEnterFrame=undefined;
                    break;
                }
            }
            ep.rot_back();
        }
        onEnterFrame=ship_hitTest;
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"backOrForward");
    //platform_backOrForward
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var oneZone_h=%u",oneZone_h);
    actionf_sprite(presprite,buf,"var frames_total=%u",fps/2);
    actionf_sprite(presprite,buf,"var platform_w=%u;var platform_h=%u",platforma_de_camera_sz[0],platforma_de_camera_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        attachMovie('platform_room','platform_room',getNextHighestDepth());
        _x+=platform_w/2;_y+=platform_h/2;
        //
        var y_end=_y+(oneZone_h*sense);
        var x_step=(0-_x)/frames_total;var y_step=(y_end-_y)/frames_total;
        var frames;
        function envTest_function(){
            //aicea e deja rot_0 ca altfel ar fi ep.rot_getBounds
            var rect=ep.ship.body.getBounds(this);
            var xCenter=rect.xMin+((rect.xMax-rect.xMin)/2);
            var yCenter=rect.yMin+((rect.yMax-rect.yMin)/2);
            ep.env._x+=xCenter;ep.env._y+=yCenter;
            frames=frames_total;
            ep.ship.onEnterFrame=undefined;
            onEnterFrame=function(){
                _x+=x_step;_y+=y_step;
                ep.env._x-=x_step;ep.env._y-=y_step;
                frames--;
                if(frames==0){
                    frames=frames_total;
                    onEnterFrame=function(){
                        _x+=-1*x_step;_y+=-1*y_step;
                        frames--;
                        if(frames==0){
                            ep.ship.onEnterFrame=ep.ship.ship_frame_f;
                            onEnterFrame=undefined;
                            ep.env.onEnterFrame=ep.env.ship_hitTest;
                        }
                    }
                }
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"platform_backOrForward");

    //rounds
    #define roundsOnHalfW 2
    #define roundsOnW roundsOnHalfW*2
    double round_lat=double(zone_w)/double(roundsOnW);
    int rounds_free=2*ship_body_sz[1];
    int roundAndFree_h=round_lat+rounds_free;
    #define rounds_rows 10
    int rounds_h=(roundAndFree_h*rounds_rows)+(2*platforma_de_zone_sz[1]);
    actionf(buf,"var rounds_h=%u",rounds_h);
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var rows=%u",rounds_rows);
    actionf_sprite(presprite,buf,"var free_h=%u",rounds_free);
    actionf_sprite(presprite,buf,"var round_lat=%lf",round_lat);
    actionf_sprite(presprite,buf,"var roundsOnHalfW=%u",roundsOnHalfW);
    actionf_sprite(presprite,buf,"var platform_lat=%u",platforma_de_camera_sz[0]);
    action_sprite(presprite,R"(
        var yPos=y_start+zone_h;
        function add_platform(xPos){
            var d=platforms.getNextHighestDepth();
            var mc=platforms.attachMovie('rounds_platform','platform'+d,d);
            mc._x=xPos;mc._y=yPos;
            mc._xscale=scale_ratio;mc._yscale=scale_ratio;
            mc['sense']=random(2)*2-1;
        }
        var scale_ratio=100*round_lat/platform_lat;
        createEmptyMovieClip('platforms',getNextHighestDepth());
        var i=0;while(i<rows){
            yPos-=free_h+(round_lat/2);
            var xPos=round_lat/2;
            var j=0;while(j<roundsOnHalfW){
                add_platform(xPos);add_platform(0-xPos);
                xPos+=round_lat;
                j++;
            }
            yPos-=round_lat/2;
            i++;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"rounds");
    //rounds_platform
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var step=%u*sense",360/(2*fps));
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        attachMovie('platform_room','platform_room',getNextHighestDepth());
        var radius=_width/2;
        var store_scale=_xscale;
        onEnterFrame=function(){
            ep.rot_0();
            if(hitTest(ep.ship.body)==true){
                var store_rot=_rotation;_rotation=0;
                _xscale=100;_yscale=100;
                var rect=ep.ship.body.getBounds(this);
                _rotation=store_rot;
                _xscale=store_scale;_yscale=store_scale;
                //
                var xCenter=rect.xMin+((rect.xMax-rect.xMin)/2);
                var yCenter=rect.yMin+((rect.yMax-rect.yMin)/2);
                var dist=Math.sqrt((xCenter*xCenter)+(yCenter*yCenter));
                if(dist<=radius){
                    var angle=Math.atan2(yCenter,xCenter)*180/Math.PI;
                    angle+=step;
                    var newX=Math.cos(angle*Math.PI/180)*dist;
                    var newY=Math.sin(angle*Math.PI/180)*dist;
                    ep.env._x-=newX-xCenter;ep.env._y-=newY-yCenter;
                }
            }
            ep.rot_back();
            _rotation+=step;if(Math.abs(_rotation)>360)_rotation=_rotation%360;

        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"rounds_platform");

    //fillingScreen
    //portal
    int portal_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/12/portal1.dbl",portal_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(portal_sz[0]/2),0-(portal_sz[1]/2));
    for(int i=0;i<(fps/2);i++)swf_sprite_showframe(presprite);
    bmp=swf_image("C:/Desktop/universe/img/12/portal2.dbl");
    swf_sprite_removeobject(presprite,0);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(portal_sz[0]/2),0-(portal_sz[1]/2));
    for(int i=0;i<(fps/2);i++)swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"portal");
    //
    //aicea e 1 ca sa nu faca hitTest din prima
    #define off_to_portals 1
    int portals_szW= width/*wdt*/ -(2*off_to_portals);
    int portals_onW=portals_szW/portal_sz[0];
    int portals_allW=portals_onW*portal_sz[0];
    int portals_freeAllW=portals_szW-portals_allW;
    int portals_freeW=portals_freeAllW/(portals_onW-1);
    int fromPortalToPortalOnW=portal_sz[0]+portals_freeW;
    //
    #define portals_h height
    actionf(buf,"var portals_h=%u",portals_h);
    int portals_szH=portals_h-(2*off_to_portals)-(2*portal_sz[1]);
    int portals_onH=portals_szH/portal_sz[1];
    int portals_allH=portals_onH*portal_sz[1];
    int portals_freeAllH=portals_szH-portals_allH;
    int portals_freeH=portals_freeAllH/(portals_onH-1);
    int fromPortalToPortalOnH=portal_sz[1]+portals_freeH;
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var off_to_portals=%u",off_to_portals);
    actionf_sprite(presprite,buf,"var portals_onW=%u;var fromPortalToPortalOnW=%u",portals_onW,fromPortalToPortalOnW);
    actionf_sprite(presprite,buf,"var portals_onH=%u;var fromPortalToPortalOnH=%u",portals_onH,fromPortalToPortalOnH);
    actionf_sprite(presprite,buf,"var portal_w=%u;var portal_h=%u",portal_sz[0],portal_sz[1]);
    action_sprite(presprite,"var ep=_root['episode'];var com=_root['shared_level']");
    action_sprite(presprite,R"(
        createEmptyMovieClip('portals',getNextHighestDepth());
        function add_portal(){
            var d=portals.getNextHighestDepth();
            var mc=portals.attachMovie('portal','portal'+d,d);
            mc._x=xPos+(portal_w/2);mc._y=yPos+(portal_h/2);
            mc._alpha=50;
            //este 1..n frames nu 0..n-1
            mc.gotoAndPlay(random(mc._totalframes)+1);
        }
        var xPos;var yPos;var i;var p;
        var upRowY=y_start+off_to_portals;var leftRowX=x_start+off_to_portals;
        yPos=upRowY;
        i=0;while(i<2){
            xPos=leftRowX;
            p=0;while(p<portals_onW){
                add_portal();
                xPos+=fromPortalToPortalOnW;p++;
            }
            yPos=y_start+zone_h-off_to_portals-portal_h;
            i++;
        }
        xPos=leftRowX;
        i=0;while(i<2){
            yPos=upRowY+portal_h;
            p=0;while(p<portals_onH){
                add_portal();
                yPos+=fromPortalToPortalOnH;p++;
            }
            xPos=x_start+zone_w-off_to_portals-portal_w;
            i++;
        }
    )");
    #define fillingScreen_interFrames fps
    actionf_sprite(presprite,buf,"var frames_inter=%u",fillingScreen_interFrames);
    actionf_sprite(presprite,buf,"var nr_of_drivers=%u",60*fps/fillingScreen_interFrames);
    //
    actionf_sprite(presprite,buf,"var driver_w=%u;var driver_h=%u",driver_sz[0],driver_sz[1]);
    actionf_sprite(presprite,buf,"var driver_step=%u",driver_step);
    action_sprite(presprite,R"(
        var frames=0;var index_drivers=0;
        createEmptyMovieClip('drivers_container',getNextHighestDepth());
        function addShips_testHit(){
            frames--;
            if(frames<0){
                index_drivers++;
                if(index_drivers==nr_of_drivers){
                    ep.platform_zone_done();
                    onEnterFrame=undefined;
                    return null;
                }
                frames=frames_inter;
                //
                var max_depth=portals.getNextHighestDepth();
                var rand_portal=portals.getInstanceAtDepth(random(max_depth));
                var d=drivers_container.getNextHighestDepth();
                var mc=drivers_container.createEmptyMovieClip('enemy'+d,d);
                mc._x=rand_portal._x;mc._y=rand_portal._y;
                driver_init(mc);
            }
            ep.rot_0();
            shipHit();
            ep.rot_back();
        }
        function shipHit(){
            var sh=ep.ship.body;
            for(var mc_name in drivers_container){
                var mc=drivers_container[mc_name];
                if(mc.driver.hitTest(sh)==true){
                    ep.ship.attachMovie('fillingScreen_damage','damage_taken',ep.ship.getNextHighestDepth());
                    onEnterFrame=undefined;
                    return null;
                }
            }
        }
        onEnterFrame=addShips_testHit;
        var x_step;var y_step;
        function driver_init(mc){
        //asta e aici ca onenterframe de aici poate fi inlocuit tot acest frame
            mc.attachMovie('enemy','driver',mc.getNextHighestDepth());
            com.add_fuel(driver_w,driver_h/2,1,'backFuel',mc);mc.backFuel._x=0-(driver_w/2);mc.backFuel._y=driver_h/2;
            mc._rotation=random(360);
            mc.x_step=Math.sin(mc._rotation*Math.PI/180)*driver_step;
            mc.y_step=0-(Math.cos(mc._rotation*Math.PI/180)*driver_step);
            mc.onEnterFrame=driver_move;
        }
        var rot;
        function driver_move(){
            this.backFuel.fuelOn();
            rot=this._rotation;this._rotation=0;
            ep.rot_0();
            this._x+=this.x_step;
            if(ep.margin_tester(this.driver)>0){
                this._x-=this.x_step;this.x_step*=-1;this._x+=this.x_step;
                change_rotation(this);
            }
            this._y+=this.y_step;
            if(ep.margin_tester(this.driver)>0){
                this._y-=this.y_step;this.y_step*=-1;this._y+=this.y_step;
                change_rotation(this);
            }
            ep.rot_back();
            this._rotation=rot;
        }
        function change_rotation(mc){
            rot=Math.atan2(mc.y_step,mc.x_step)*180/Math.PI+90;
        }
        var diag_step=Math.sin(45*Math.PI/180)*driver_step;var factor;
        function afterHit(){
        //asta e aici dupa ce se incheie damage_taken-ul, fara cod in plus
            for(var mc_name in drivers_container){
                var mc=drivers_container[mc_name];
                factor=int((mc._x-x_start)/(zone_w/2))*2-1;
                mc.x_step=diag_step*factor;
                factor=int((mc._y-y_start)/(zone_h/2))*2-1;
                mc.y_step=diag_step*factor;
                mc._rotation=Math.atan2(mc.y_step,mc.x_step)*180/Math.PI+90;
                mc.onEnterFrame=backToHole;
            }
        }
        function backToHole(){
            this.backFuel.fuelOn();
            ep.rot_0();
            portalTest(this);
            ep.rot_back();
            this._x+=this.x_step;this._y+=this.y_step;
        }
        function portalTest(mc){
            for(var mc_name in portals){
                if(portals[mc_name].hitTest(mc)==true){
                    mc.removeMovieClip();
                    //daca mai sunt nave aicea e return
                    for(var mc_name in drivers_container)return null;
                    //daca a fost ultima vine iar functia generala
                    onEnterFrame=addShips_testHit;
                    return null;
                }
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fillingScreen");
    //fillingScreen_damage
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var radius=%u;var fps=%u",ship_body_sz[0],fps);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var dt_matrix=new flash.geom.Matrix();
        var colors=new Array(0xff0000,0xffFF00);
        //
        var alphaVal=90;
        var alphas=new Array(alphaVal,alphaVal);
        var ratios=new Array(0,255*radius/Stage.width);
        var alpha_decrement=100/fps;
        onEnterFrame=function(){
            alphaVal-=alpha_decrement;
            if(alphaVal>0){
                alphas[0]=alphaVal;alphas[1]=alphaVal;
                clear();
                beginGradientFill('radial',colors,alphas,ratios,dt_matrix);
                com.drawCircle_offset(this,radius,0-radius);
                endFill();
            }else{
                ep.env.afterHit();
                removeMovieClip();
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fillingScreen_damage");

    //
    actionf(buf,"var width=%u;var height=%u",width,height);
    actionf(buf,"var ship_down_y=%u;var ship_down_dist=%u",ship_down_y,ship_down_dist);
    actionf(buf,"var outOff=%u;var default_zone_w=%u",outOff,zone_w);
    actionf(buf,"var view_step=%u",(height/2)/fps);
	////spc._x=0-_x;spc._y=0-ship_down_y;
	action(R"(
		createEmptyMovieClip('env_container',getNextHighestDepth());
        function stars_bg(onX,onY){
            var d=env_container.getNextHighestDepth();
            com.space_env(env_container,'bg'+d,d);
            var mc=env_container['bg'+d];mc._xscale=onX;mc._yscale=onY;
        }
        stars_bg(150,150);stars_bg(-150,-150);stars_bg(-150,150);stars_bg(150,-150);
        var zoneNr=0;
        var prev_env;
        var zone_w;var zone_h;
        var env;var enter_dist;
        var y_view;var current_y_view=0;var frames_to_y_view;var step_to_y_view;
        function add_named_zone(name){
            env=env_container.attachMovie(name,'env'+zoneNr,env_container.getNextHighestDepth());
        }
        function new_zone(){
            prev_env=env;
            enter_dist=ship_down_dist;
            y_view=ship_down_y;
            zone_w=default_zone_w;
            if(zoneNr==0){
                add_named_zone('fullRoad');
                zone_h=fullRoad_h;
                env['up_end']=true;
            }
            else if(zoneNr==1){
                enter_dist=holeTrap_enterDist;
                y_view=height/2;
                add_named_zone('holeTrap');
                zone_h=holeTrap_h;
                env['with_offset']=true;
            }
            else if(zoneNr==2){
                add_curvesRoad();
            }
            else if(zoneNr==3){
                add_curvesRoad();
                //curvesRoad variabile specifice
                env['fasters']=true;
            }
            else if(zoneNr==4){
                add_named_zone('backOrForward');zone_h=backOrForward_h;
                env['up_end']=true;
            }
            else if(zoneNr==5){
                add_redX();
            }
            else if(zoneNr==6){
                add_redX();env['moving_targets']=true;
            }
            else if(zoneNr==7){
                add_curvesRoad();
                //curvesRoad variabile specifice
                env['trap']=true;
            }
            else if(zoneNr==8){
                add_named_zone('rounds');zone_h=rounds_h;
                env['up_end']=true;
            }
            else if(zoneNr==9){
                enter_dist=portals_h/2;
                y_view=height/2;
                add_named_zone('fillingScreen');
                zone_h=portals_h;
            }
            else _root.end_scenario();
            //swap ca sa se vada platforma cand ajunge la cea noua
            env.swapDepths(prev_env);
            env.attachMovie('margins','margins',env.getNextHighestDepth());
            //astea aici, trebuie la obiecte si margini
            env['zone_w']=zone_w;env['zone_h']=zone_h;
            env['x_start']=0-(env['zone_w']/2);env['y_start']=0-(env['zone_h']/2);
            //
            var dist=y_view-current_y_view;
            //frames absolut ca de la 2 la 1 e negativ, si stepul pozitiv sau negativ
            frames_to_y_view=Math.abs(dist/view_step);step_to_y_view=dist/frames_to_y_view;current_y_view=y_view;
            env_container.onEnterFrame=function(){
                if(frames_to_y_view>0){
                    _y+=step_to_y_view;
                    //main_bg._y+=step_to_y_view;
                    frames_to_y_view--;
                }
                else{
                    _y=current_y_view;
                    //main_bg._y=current_y_view;
                    env_container.onEnterFrame=undefined;
                }
            }
            //
            zoneNr++;
        }
        function add_curvesRoad(){add_named_zone('curvesRoad');zone_h=curvesRoad_h;}
        function add_redX(){
            add_named_zone('redX');zone_w=redX_lat;zone_h=redX_lat;enter_dist=redX_lat/2;
        }
        //
        function set_start(){
            attachMovie('ship','ship',getNextHighestDepth());
            env._y=0-spec_dist();
            inter_function=inter_zones;
        }
        function inter_zones(){
            var r1=prev_env.margins.up.getBounds(prev_env);
            var env_enter_y=prev_env._y+r1.yMin-outOff;
            env._y=env_enter_y-env.margins.down._y;
            env._x=prev_env._x;
            //
            prev_env.platform_margin.move_platform_set(env._y+spec_dist());
        }
        //dist
        function spec_dist(){
            if(env.with_offset==true)return 2*enter_dist;
            return env.margins.down._y-enter_dist;
        }
        function env_loaded(){
            if(env.up_end==true){
                var mc=env.attachMovie('platform_margin','platform_margin',env.getNextHighestDepth());
                mc._x=env.margins.up._x+(env.zone_w/2);mc._y=env.margins.up._y;
            }
            inter_function();
        }
        function platform_zone_done(){
            var mc=env.attachMovie('platform_margin','platform_margin',env.getNextHighestDepth());
            mc._x=env.margins.up._x+(env.zone_w/2);mc._y=env.margins.up._y-outOff;
            mc['zone_done']=true;
        }
///////////////////////////////////////////aicea cu 0 ca altfel face hitTest urat cu marginile
        var rot_value;
        function rot_0(){
            rot_value=env_container._rotation;
            env_container._rotation=0;
        }
        function rot_back(){
            env_container._rotation=rot_value;
        }
        //rect
        function rot_getBounds(mc1,mc2){
            rot_0();
            var rect=mc1.getBounds(mc2);
            rot_back();
            return rect;
        }
///////////////////////////////////////////
        function margin_tester(mc){
            var rct_mc=mc.getBounds(this);
            //
            var rct=env.margins.up.getBounds(this);
            if(rct_mc.yMin<rct.yMax)return rct.yMax-rct_mc.yMin;
            rct=env.margins.right.getBounds(this);
            if(rct.xMin<rct_mc.xMax)return rct_mc.xMax-rct.xMin;
            rct=env.margins.down.getBounds(this);
            if(rct.yMin<rct_mc.yMax)return rct_mc.yMax-rct.yMin;
            rct=env.margins.left.getBounds(this);
            if(rct_mc.xMin<rct.xMax)return rct.xMax-rct_mc.xMin;
            return 0;
        }
        _x=width/2;
        //main_bg._x=width/2;
        var inter_function=set_start;
        new_zone();
    )");

    action(R"(
        var testObjects_matrix=new flash.geom.Matrix();
        function stopObject(xPos,yPos,wd,hg){
            var d=env.testObjects.getNextHighestDepth();
            var mc=env.testObjects.createEmptyMovieClip('obj'+d,d);
            mc._x=xPos;mc._y=yPos;
            mc._alpha=50;
            mc.beginBitmapFill(margin_texture,testObjects_matrix,true);
            mc.lineTo(wd,0);mc.lineTo(wd,hg);
            mc.lineTo(0,hg);mc.lineTo(0,0);
            mc.endFill();
        }
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
